/*Server_test.c*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include "Account.h"
#include "Contacts.h"
#include "Chess.h"
#include "Board.h"

/* constants for different message types */

#define SIGNUP 0
#define LOGIN 1
#define SEND 2
#define REQUEST 3
#define BYE 4
#define SHUTDOWN 5
#define ADDCONTACT 6
#define DELETECONTACT 7
#define ACCOUNTINFO 8
#define CONTACTLIST 9
#define CHESS 10
#define USERONLINE 11
#define BOARDRESET 13

/* #define DEBUG */	/* be verbose */

/*** type definitions ****************************************************/

//typedef void (*ClientHandler)(int DataSocketFD, Message *M, char *IP_Address, LIST *AList);
typedef void (*TimeoutHandler)(void);

/*** global variables ****************************************************/

const char *Program	= NULL; /* program name for descriptive diagnostics */
int Shutdown = 0;		/* keep running until Shutdown == 1 */
int bye = 0;
int client_count = 2;
//int online, online_tag;

/* a message struct to save all the info*/
typedef struct {
    int type;
    char message[256];
	char sender[21];
	char receiver[21];
	char password[9];
	char fname[15];
	char lname[15];
	int chess;
	int status;
	int start;
	int stall;
} Message;

typedef void (*ClientHandler)(int DataSocketFD, Message M, char *IP_Address, int port, LIST *AList);
char ClockBuffer[26] = "";	/* current time in printable format */

/*** global functions ****************************************************/

/*receive the struct from the client*/
int recv_message(int socket, Message *msg) {
    size_t length = sizeof(Message);
    char *ptr = (char*) msg;
    while(length > 0) {
        int num_bytes = recv(socket, ptr, length, 0);
        if(num_bytes < 0) 
            return -1;
        ptr += num_bytes;
        length -= num_bytes;
    }
    msg->type = ntohl(msg->type);
	msg->chess = ntohl(msg->chess);
	msg->status = ntohl(msg->status);
	msg->start = ntohl(msg->start);
	msg->stall = ntohl(msg->stall);

    return 0;
}

int verify_login(Message M, LIST *AList){
	
	ENTRY *current;
	int verified;
	current = AList->First;

	if(AList->length == 0){
		printf("No registered user!\n");
		verified = 4;
	}

	for(int i = 0; i < AList->length; i++)
	{
		printf("%d\nID: %s\nPass: %s\n", i+1, current->account->username, current->account->password);
		//if username is part of the list of usernames then it is valid
		if (!strcmp(M.sender, current->account->username))
		{
			printf("Username in database\n");
		
			//if username and password match then we have a successful login
			if (!strcmp(M.password, current->account->password))
			{
				//send this to the client eventually
				verified = 1; //set flag to 1 since it is verified
				strcpy(M.sender, current->account->username);
				current->account->status = 1;
				
			}

			//otherwise we have incorrect password
			else
			{
				verified = 2; //incorrect password sets flag to 2
				break;
			}
		}
		else
		{
			current = current->Next;
			if(i == AList->length - 1 && current == NULL)
			{
				verified = 3;
			}
		}
	}//end for loop
	return verified;

}
int signup(Message M, LIST *AList){

	assert(AList);
	int valid = 0;
	ENTRY *e, *n;
	ACCOUNT *a;
	e = AList->First;
	while(e)
	{
		n = e->Next;
		if (!strcmp(e->account->username, M.sender))
		{
			valid = 1;
			break;
		}
		else
			e = n;
	}
	if (valid == 0)
	{
		a = CreateAccount(M.sender, M.password, M.fname, M.lname, M.status);
        	e = CreateAccountEntry(a);
        	AppendAccount(AList, a);
        	printf("ID: %s\nPass: %s\n", e->account->username, e->account->password);
	}
	return valid;
		
}

int send_msg(Message M, LIST *AList, char *TempBuf)
{
	/*loop thru the list and save the the message in the receiver account struct*/
	ENTRY *e;
	e = AList -> First;
	assert(AList);
	while(e)
	{
		if(strcmp(e -> account -> username, M.receiver) == 0)
		{
			strcpy(e -> account -> chat, TempBuf);
			return 0;
		}	
		e = e -> Next;
	}
	return -1;
}

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} /* end of FatalError */

int MakeServerSocket(uint16_t PortNo)		/* create a socket on this server */
{
    int ServSocketFD; /*socket file descriptor*/
    struct sockaddr_in ServSocketName;

    /* create the socket */
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0)
    {   
		FatalError("service socket creation failed");
    }
    /* bind the socket to this server */
    ServSocketName.sin_family = AF_INET;
    ServSocketName.sin_port = htons(PortNo);
    ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ServSocketFD, (struct sockaddr*)&ServSocketName, sizeof(ServSocketName)) < 0)
    {   
		FatalError("binding the server to a socket failed");
    }
    /* start listening to this socket */
    if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
    {   
		FatalError("listening on socket failed");
    }
    return ServSocketFD;
} /* end of MakeServerSocket */

/*dont need in our case i think*/
void PrintCurrentTime(void)	/*  print/update the current real time */
{
    time_t CurrentTime; /* seconds since 1970 (see 'man 2 time') */
    char   *TimeString;	/* printable time string (see 'man ctime') */
    char   Wheel,
	   *WheelChars = "|/-\\";
    static int WheelIndex = 0;

    CurrentTime = time(NULL);	/* get current real time (in seconds) */
    TimeString = ctime(&CurrentTime);	/* convert to printable format */
    WheelIndex = (WheelIndex+1) % 4;
    Wheel = WheelChars[WheelIndex];
    fflush(stdout);
} /* end of PrintCurrentTime */

/*use siminalr structure to just send message back and forth*/
void ProcessRequest(int DataSocketFD, Message M, char *IP_Address, int port, LIST *AList)		/* process a time request by a client */
{
    int  l, n;
	int sign_up;
    char SendBuf[5000];	/* message buffer for sending a response */
	char TempBuf[256];
	int bye = 0;
	int verified;
	int send_status;

	n = recv_message(DataSocketFD, &M); /*receive the function from client and save it in the struct*/
    if (n < 0) 
    {  
		FatalError("reading from data socket failed");
    }
    /*RecvBuf[n] = 0;*/
	#ifdef DEBUG
    printf("%s: Received message: %s\n", Program, RecvBuf);
	#endif
	
	strcpy(SendBuf, "");
/*client writing bye*/
    if (M.type == BYE)
    {   
		ENTRY* e = AList->First;
		ENTRY* n;

		while (e)
		{
			n = e->Next;
			if (!strcmp(e->account->username, M.sender))
			{
				e->account->status = 0;
				break;
			}
			e = n;
		}

		bye = 1;
		printf("User Logging Out: %s\n", M.sender);
		M.status = 0;
		strncpy(SendBuf, "Logging out of server...\n", sizeof(SendBuf)-1);
		SendBuf[sizeof(SendBuf)-1] = 0;
    }
	/*client writing shutdown*/
    else if (M.type == SHUTDOWN)
    {   
		Shutdown = 1;
		strncpy(SendBuf, "server shutdown", sizeof(SendBuf)-1);
		SendBuf[sizeof(SendBuf)-1] = 0;
    }
	/*sign up*/
	else if(M.type == SIGNUP)
	{
		sign_up = signup(M, AList);
		if(sign_up == 0){
			printf("%s: Signup Successful!\n\n", M.sender);
			strncpy(SendBuf, "Signup Successful!\n\n", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
		} 
		else 
		{	printf("%s: Signup Failed!\n\n", M.sender);
			strncpy(SendBuf, "Signup Failed", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
		}
	}
	/*login*/
	else if(M.type == LOGIN)
	{
		verified = verify_login(M, AList);
		if(verified == 1){
			printf("%s: Successful login!\n\n", M.sender);
			strncpy(SendBuf, "Successful login!\n\n", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
			//printf("%s Status: %d\n", M.sender, M.status);
		}
		else if(verified == 2){
			printf("%s: Incorrect password!\n\n", M.sender);
			strncpy(SendBuf, "Incorrect password!\n\n", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
		}
		else if(verified == 3){
			printf("%s: NO username in database!\n\n", M.sender);
			strncpy(SendBuf, "NO username in database!\n\n", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
		}
		else if (verified == 4){
			strcpy(SendBuf, "No registered user!\n");
		}
	}
	/*client sending message*/
	else if (M.type == SEND)
	{
		/*concatenate the sender and the message*/
		strcat(M.sender, ": ");
		strncpy(TempBuf, M.sender, sizeof(TempBuf)-1);
		TempBuf[sizeof(TempBuf)-1] = 0;
		strncat(TempBuf, M.message, sizeof(TempBuf)-1-strlen(TempBuf));
		printf("message is %s\n\n", TempBuf);
		
		send_status = send_msg(M, AList, TempBuf);
		if(send_status < 0)
		{
			printf("Sending failed\n\n");
			strncpy(SendBuf, "Sending failed\n\n", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
		}
		else
		{
			printf("Message sent\n\n");
			strncpy(SendBuf, "Message sent\n\n", sizeof(SendBuf)-1);
			SendBuf[sizeof(SendBuf)-1] = 0;
		}
	}
	/*request message*/
	else if (M.type == REQUEST)
	{
		/*loop thru the list and find the message*/
		ENTRY *e;
		e = AList -> First;
		assert(AList);
		while(e)
		{
			if(strcmp(e -> account -> username, M.sender) == 0)
			{
				if(!strcmp(e -> account -> chat, ""))
				{
					strcpy(SendBuf, "");
				}
				else
				{
					strcpy(SendBuf, e -> account -> chat);
					strcpy(e -> account -> chat, "");
				}
				
			}	
			e = e -> Next;
		}
	}

	/* ADDING Contact */
	else if (M.type == ADDCONTACT)
	{
		ENTRY *e = AList->First;
		ENTRY *n;
		CENTRY *contact, *next;
		int validC = 1;
		int validU = 1;
	
		/* Checks if contact they like to add exists in server */
		while (e)
		{
			n = e->Next;
			if (!strcmp(e->account->username, M.receiver))
			{
				validU = 1;
				break;
			}
			e = n;
			validU = 0;
		}
		
		if (validU == 0)
                {
                        printf("%s: %s does not exist!\n\n", M.sender, M.receiver);
                        strcat(SendBuf, M.receiver);
                        strcat(SendBuf, " does not exist!\n\n");
                }
		else
		{
			e = AList->First;

			while (e)
			{
				n = e->Next;
				if (!strcmp(e->account->username, M.sender))
				{
					contact = e->account->CList->CFirst;
					while (contact)
					{
						next = contact->CNext;
						if (!strcmp(contact->Contact->Name, M.receiver))
						{
							printf("%s: Contact %s already exists\n\n", M.sender, M.receiver);
							strcat(SendBuf, "Contact ");
							strcat(SendBuf, M.receiver);
							strcat(SendBuf, " already exists\n\n");
							validC = 0;
							break;
						}
						contact = next;
					}
				
					if (validC == 1)
					{	
						CONTACT *c = AddContact(M.receiver);
						AppendContact(e->account, c);
						printf("%s: Contact %s successfully added\n\n", M.sender, M.receiver);
						strcat(SendBuf, "Contact ");
						strcat(SendBuf, M.receiver);
						strcat(SendBuf, " successfully added!\n\n");
						break;
					} 
				}
				else
				{
					e = n;
				}
				if (validC == 0)
					break;
			}
		}	
	}//end of AddContact

	else if(M.type == CHESS){
		/*concatenate the sender and the message*/

		int colorOption = 0;
		char temp_move[9];
		char temp_move2[9];
		int result = 0;
		int PlayerNum = 0;
		char board[8][8];
		char current_position[3];
		char new_position[3];

		char reset_board[8][8] = {

						
							{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
							{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},			
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
							{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},


						};

			
			if (M.start == 0)
			{
				ENTRY *e;
				e = AList -> First;
				assert(AList);
				while(e)
				{
					if(strcmp(e -> account -> username, M.sender) == 0)
					{
						if(e -> account -> chess == 0)
						{	
							e -> account -> chess = 1;	//means this player is white
						}
					}
					else if(strcmp(e -> account -> username, M.receiver) == 0)
					{
						if(e -> account -> chess == 0)
						{
							e -> account -> chess = 2;	//means this player is black
						}

					}

					e = e -> Next;
				}
				e = AList -> First;
				assert(AList);
				while(e)
				{
					if(strcmp(e -> account -> username, M.sender) == 0)
					{
						for(int x = 0; x < 8; x++)
						{
							for(int y = 0; y < 8; y++)
							{
								board[x][y] = e -> account -> player_board[x][y];
							}
						}
						if(e -> account -> chess == 1)
						{
							PlayerNum = 1;
							colorOption = 1;
						}
						if(e -> account -> chess == 2)
						{
							PlayerNum = 2;
							colorOption = 2;
						}
					}
					e = e -> Next;
				}
				//put the message into temp
				strcpy(temp_move, M.message);
				strcpy(temp_move2, temp_move);
				printf("the move is: %s", temp_move);
				//flags arent set in chess yet
				//pass color and movement;

				result = chess(colorOption, temp_move, board, PlayerNum);
				if(result == 0)
				{
					strcpy(SendBuf, "Move Successful");

					ENTRY *s, *r;
                                	s = AList->First;
                                	r = AList->First;
                                	while(s)
                                	{
                                        	if (!strcmp(s->account->username, M.sender))
                                        	{
							s->account->turn = 0;
							break;
						}
						s = s->Next;
					}
                                        while(r)
                                        {
                                        	if (!strcmp(r->account->username, M.receiver))
                                                {
                                                	r->account->turn = 1;     
							break;          	                                  
                                         	}
						r = r->Next;
                                        }

					sscanf(temp_move2, "%s to %s\n\n", current_position, new_position);

					PrintBoard(SendBuf, board);
					e = AList -> First;
					assert(AList);
					while(e)
					{
						if(strcmp(e -> account -> username, M.receiver) == 0)
						{
							for(int x = 0; x < 8; x++)
							{
								for(int y = 0; y < 8; y++)
								{
									e -> account -> player_board[x][y] = board[x][y];
								}
							}
						}
						e = e -> Next;
					}
				}
				else if(result == 1)
				{
					strcpy(SendBuf, "Incorrect Format");
				}
				else if(result == 2)
				{
					strcpy(SendBuf, "Illegal Move");
				}
				else if(result == 3)
				{
					strcpy(SendBuf, "Checkmate");
					sscanf(temp_move2, "%s to %s\n\n", current_position, new_position);			
					e = AList -> First;
					assert(AList);
					while(e)
					{
						if(strcmp(e -> account -> username, M.receiver) == 0)
						{
							for(int x = 0; x < 8; x++)
							{
								for(int y = 0; y < 8; y++)
								{
									e -> account -> player_board[x][y] = board[x][y];
								}
							}
						}
						else if(strcmp(e -> account -> username, M.sender) == 0)
						{
							for(int x = 0; x < 8; x++)
							{
								for(int y = 0; y < 8; y++)
								{
									e -> account -> player_board[x][y] = reset_board[x][y];
								}
							}
							e -> account -> chess = 0;
						}
						e = e -> Next;
					}
				}
				else if(result == 4)
				{
					strcpy(SendBuf, "Stalemate");
					sscanf(temp_move2, "%s to %s\n\n", current_position, new_position);				
					e = AList -> First;
					assert(AList);
					while(e)
					{
						if(strcmp(e -> account -> username, M.receiver) == 0)
						{
							for(int x = 0; x < 8; x++)
							{
								for(int y = 0; y < 8; y++)
								{
									e -> account -> player_board[x][y] = board[x][y];
								}
							}
						}
						else if(strcmp(e -> account -> username, M.sender) == 0)
						{
							for(int x = 0; x < 8; x++)
							{
								for(int y = 0; y < 8; y++)
								{
									e -> account -> player_board[x][y] = reset_board[x][y];
								}
							}
							e -> account -> chess = 0;
						}
						e = e -> Next;
					}
				}
			}
			if (M.start == 1)
			{
				ENTRY *e, *r;
				e = AList->First;
				r = AList->First;
				while(e)
				{
					if (!strcmp(e->account->username, M.sender))
					{
						while(r)
						{
							if (!strcmp(r->account->username, M.receiver))
							{
								if ((r->account->player_board == e->account->player_board))
								{	
									break;
								}
								else
								{
									PrintBoard(SendBuf, e->account->player_board);
									break;
								}
				
							}
							r = r->Next;
						}
						break;
					}
					e = e->Next;
				}
			}
		//receive the color and pass to chess
		//print out board and colors in terminal and start game
		//receive the moves from the client
		//parse the string and update the board
		//send the updates board to the client
	}
	
	else if (M.type == 12) // stall
	{
		ENTRY *s;
                s = AList->First;
                                  
                while(s)
                {
                	if (!strcmp(s->account->username, M.sender))
                       	{
                               	if (s->account->turn == 0)
					strcpy(SendBuf, "Other Player's turn");
				else
					strcpy(SendBuf, "Your turn");       
				break;
                        }
                        s = s->Next;
               	}
	
	}
	else if (M.type == BOARDRESET)
	{
		//reset board inside the struct if client exits
		char reset_board[8][8] = {

						
							{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
							{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},			
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
							{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},


						};
		
		ENTRY *e;
		e = AList -> First;
		assert(AList);
		while(e)
		{
			if(strcmp(e -> account -> username, M.receiver) == 0)
			{
				for(int x = 0; x < 8; x++)
				{
					for(int y = 0; y < 8; y++)
					{
						e -> account -> player_board[x][y] = reset_board[x][y];
					}
				}
				e -> account -> chess = 0;
			}
			else if(strcmp(e -> account -> username, M.sender) == 0)
			{
				for(int x = 0; x < 8; x++)
				{
					for(int y = 0; y < 8; y++)
					{
						e -> account -> player_board[x][y] = reset_board[x][y];
					}
				}
				e -> account -> chess = 0;
			}
			e = e -> Next;
		}
		strcpy(SendBuf, "Game Over");
	}
	else if (M.type == DELETECONTACT)
	{
		ENTRY *e = AList->First;
       		ENTRY *n;
        	CENTRY *contact, *next;
		int valid = 0;
                
		while (e)
                {
                	n = e->Next;
                        if (!strcmp(e->account->username, M.sender))
                        {
                        	contact = e->account->CList->CFirst;
                                while (contact)
                                {
                                	next = contact->CNext;
                                     	if (!strcmp(contact->Contact->Name, M.receiver))
                                        {
						RemoveContact(e->account, M.receiver);
                                        	printf("%s: Deleting contact %s\n\n", M.sender, M.receiver);
                                                strcat(SendBuf, "Deleting contact ");
                                                strcat(SendBuf, M.receiver);
                                                strcat(SendBuf, "\n\n");
						valid = 1;
                                                break;
                                        }
                                        contact = next;
                                }

                                if (valid == 0)
                                {
                                        printf("%s: Contact %s does not exist!\n\n", M.sender, M.receiver);
                                        strcat(SendBuf, "Contact ");
                                        strcat(SendBuf, M.receiver);
                                        strcat(SendBuf, " does not exist!\n\n");
                                        break;
                                }
				else
					break;
                        }
                      	e = n;
        }

	}
	else if (M.type == ACCOUNTINFO)
	{ 
		ENTRY *e;
                e = AList -> First;
                assert(AList);
                while(e)
                {
                        if(strcmp(e -> account -> username, M.sender) == 0)
                        {
                                PrintAccountInfo(e->account, SendBuf);
				break;
                        }
                        e = e -> Next;
                }
	
	}
	else if (M.type == CONTACTLIST)
	{
                ENTRY *e;
                e = AList -> First;
                assert(AList);
                while(e)
                {
                        if(strcmp(e -> account -> username, M.sender) == 0)
                        {
                                PrintContactList(e->account, SendBuf, AList);
                                break;
                        }
                        e = e -> Next;
                }

	}
	else if (M.type == USERONLINE)
	{
		ENTRY *e;
		e = AList->First;
		assert(AList);
		int CFlag = 0;

		while(e)
		{
			if (!strcmp(e->account->username, M.sender))
			{
				CENTRY *c = e->account->CList->CFirst;
				
				while(c)
				{
					if (!strcmp(c->Contact->Name, M.receiver))
					{
						CFlag = 1;
						break;
					}
					c = c->CNext;	
				}
				break;
			}
			e = e->Next;
		}
		
		if (CFlag == 1)
		{
			ENTRY *s = AList->First;
			
			while (s)
			{
				if (!strcmp(M.receiver, s->account->username))
				{
					if(s->account->status == 1)
					{
						printf("%s: %s is online, connecting....\n\n", M.sender, M.receiver);
						strcat(SendBuf, M.receiver);
						strcat(SendBuf, " is online, connecting...\n\n");
						break;
					}
					else
					{
						printf("%s: %s is not online\n\n", M.sender, M.receiver);
                                                strcat(SendBuf, M.receiver);
                                                strcat(SendBuf, " is not online\n\n");
                                                break;
					}
				}
				s = s->Next;
			}
		}
		else
		{
			printf("%s: %s not in contact list\n\n", M.sender, M.receiver);
			strcat(SendBuf, M.receiver);
			strcat(SendBuf, " not in contact list\n\n");
		}
	} 
    	l = strlen(SendBuf);
	
    l = strlen(SendBuf);

    n = write(DataSocketFD, SendBuf, l);


    if (n < 0)
    {   
	FatalError("writing to data socket failed");
    }
	
} /* end of ProcessRequest */

void ServerMainLoop(		/* simple server main loop */
	int ServSocketFD,		/* server socket to wait on */
	ClientHandler HandleClient,	/* client handler to call */
	TimeoutHandler HandleTimeout,
	int Timeout, Message M, LIST *AList)			/* timeout in micro seconds */
{
    int DataSocketFD;	/* socket for a new client */
    socklen_t ClientLen;
    struct sockaddr_in
	ClientAddress;	/* client address we connect with */
    fd_set ActiveFDs;	/* socket file descriptors to select from */
    fd_set ReadFDs;	/* socket file descriptors ready to read from */
	//fd_set clientFDs;
    struct timeval TimeVal;
    int res, i;
	char IP_Address[16];
	int port;

    FD_ZERO(&ActiveFDs);		/* set of active sockets */
    FD_SET(ServSocketFD, &ActiveFDs);	/* server socket is active */
    while(!Shutdown)
    {   
		ReadFDs = ActiveFDs; /*because select is destructive so we need a temp*/

		TimeVal.tv_sec  = Timeout / 1000000;	/* seconds */
		TimeVal.tv_usec = Timeout % 1000000;	/* microseconds */

		/* block until input arrives on active sockets or until timeout */
		/*first is size, second is reading, third is writing, fourth is errors, 
		last is timeout*/
		res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);

		if (res < 0)
		{   
			FatalError("wait for input or timeout (select) failed");
		}
		if (res == 0)	/* timeout occurred */
		{
			#ifdef DEBUG
	    	printf("%s: Handling timeout...\n", Program);
			#endif
	    	HandleTimeout();
		}
		else		/* some FDs have data ready to read */
		{   
			for(i=0; i<FD_SETSIZE; i++) /*go through all FD values*/
	    	{   
				if (FD_ISSET(i, &ReadFDs)) /*check if it is set*/
				{   
					if (i == ServSocketFD) /*new connection we can get*/
		    		{	/* connection request on server socket */
						#ifdef DEBUG
						printf("%s: Accepting new client %d...\n", Program, i);
						#endif
						ClientLen = sizeof(ClientAddress);
						DataSocketFD = accept(ServSocketFD, (struct sockaddr*)&ClientAddress, &ClientLen);
						if (DataSocketFD < 0)
						{   
							FatalError("data socket creation (accept) failed");
						}
						#ifdef DEBUG
						printf("%s: Client %d connected from %s:%hu.\n",
						Program, i,
						inet_ntoa(ClientAddress.sin_addr), ntohs(ClientAddress.sin_port));
						#endif
						/*add socket to set of sockets we're watching which is ActiveFDs*/
						FD_SET(DataSocketFD, &ActiveFDs); 
		    		}
		    		else
		    		{   /* active communication with a client */
						#ifdef DEBUG
						printf("%s: Dealing with client %d...\n", Program, i);
						#endif

						strcpy(IP_Address, inet_ntoa(ClientAddress.sin_addr));
						port = ntohl(ClientAddress.sin_port);

						//handle the connection
						HandleClient(i, M, IP_Address, port, AList);

						//#ifdef DEBUG
						//printf("%s: Closing client %d connection.\n", Program, i);
						//#endif
						close(i);
						//remove from set of connections we are watching
						FD_CLR(i, &ActiveFDs);
		    		}
				}	
	    	}
		}
    }
} /* end of ServerMainLoop */

/*** main function *******************************************************/

int main(int argc, char *argv[])
{
    int ServSocketFD;	/* socket file descriptor for service */
    int PortNo;		/* port number */

	LIST *AList = NULL;
	Message M;
	//if the list is empty make it createaccount list
	if(AList == NULL){
		AList = CreateAccountList();
	}
	

    Program = argv[0];	/* publish program name (for diagnostics) */
	#ifdef DEBUG
    printf("%s: Starting...\n", Program);
	#endif
    if (argc < 2)
    {   
		fprintf(stderr, "Usage: %s port\n", Program);
		exit(10);
    }
    PortNo = atoi(argv[1]);	/* get the port number */
    if (PortNo <= 2000)
    {   
		fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
        exit(10);
    }
	#ifdef DEBUG
    printf("%s: Creating the server socket...\n", Program);
	#endif
    ServSocketFD = MakeServerSocket(PortNo);
    printf("%s: Waiting for messages %d...\n", Program, PortNo);
    ServerMainLoop(ServSocketFD, ProcessRequest, PrintCurrentTime, 250000, M, AList);
	DeleteAccountList(AList); //delete the account list after the server is shut down
   // printf("\n%s: Shutting down.\n",ram);
    close(ServSocketFD);
    return 0;
}

/* EOF Server_test.c */
