/* Client_test.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <gtk/gtk.h>
#include <pthread.h> 
//#include "Chess.h"



/* #define DEBUG */
#define	BUFFSIZE 256

/*** global variables ****************************************************/

const char *Program     /* program name for descriptive diagnostics */
        = NULL;
struct sockaddr_in
        ServerAddress;
int ifShutdown = 0;       //to know if the client quit the program
int ifLogin = 0;        //to know if the client has logged in
char ID[21] = "";       //for receiving thread use
/* a message struct to save all the info*/
typedef struct 
{
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

int send_message(int socket, Message *msg) 	
{
    	size_t length = sizeof(Message);
    	char *ptr = (char*) msg;
        msg->type = htonl(msg->type);
		msg->chess = htonl(msg->chess);
		msg->status = htonl(msg->status);
		msg->start = htonl(msg->start);
		msg->stall = htonl(msg->stall);
    	
	while(length > 0) 
	{
        	int num_bytes = send(socket, ptr, length, 0);
        	if(num_bytes < 0)
            		return -1;
        	ptr += num_bytes;
        	length -= num_bytes;
    	}

    return 0;
}

int recv_message(int socket, Message *msg) 
{
    	size_t length = sizeof(Message);
    	char *ptr = (char*) msg;
    	while(length > 0) 
	{
       		int num_bytes = recv(socket, ptr, length, 0);
        	if(num_bytes < 0)
            		return -1;
        	ptr += num_bytes;
        	length -= num_bytes;
    	}
    	msg->type = ntohl(msg->type);

    	return 0;
}

void FatalError(/*const char *Program,*/ const char *ErrorMsg)
{
    	fputs(Program, stderr);
    	fputs(": ", stderr);
    	perror(ErrorMsg);
    	fputs(Program, stderr);
    	fputs(": Exiting!\n", stderr);
	exit(20);
} /* end of FatalError */

//multi-threading to handle receiving messages
void * get_message_thread(void * ServerAddress)
{       
        while(ifShutdown != 1)
        {       
                while(!ifLogin)
                {
                        //printf("Not logged in!\n");
                        sleep(5);
                }
                
                char RecvBuf[256];
                Message M;
                int SocketFD = socket(AF_INET, SOCK_STREAM, 0);
                if (SocketFD < 0)
                {  
                        FatalError("socket creation failed");
                }

                if (connect(SocketFD, (struct sockaddr*)ServerAddress,sizeof(struct sockaddr_in)) < 0)
                {   
                        FatalError("connecting to server failed");
                }

                M.type = 3;
                strcpy(M.sender, ID);
                send_message(SocketFD, &M);
                int n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                RecvBuf[n] = 0;
                if(strcmp(RecvBuf, "") != 0) //only print it out when someone sent something
                {
                        printf("\nReceived message from: %s\n", RecvBuf);
                }
                close(SocketFD);  
                sleep(5);       //wait for 5 sec until the next message  
        }
        pthread_exit(NULL);
}

void Add_callback(GtkWidget *button, gpointer data)
{
	GtkEntry *entry_1 = g_object_get_data(G_OBJECT(button), "entry_1");

        const gchar *entry_text1 = gtk_entry_get_text(GTK_ENTRY(entry_1));

        char RecvBuf[256] = "";
        Message M;
        int n, SocketFD;

        strcpy(M.receiver, entry_text1);

        M.type = 6;
	strcpy(M.sender, ID);

        SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (SocketFD < 0)
        {
                FatalError("socket creation failed");
        }

        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
        {
                FatalError("connecting to server failed");
        }
        send_message(SocketFD, &M);
        n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        RecvBuf[n] = 0;
	printf("%s\n", RecvBuf);

	close(SocketFD);
}

void Message_callback(GtkWidget *button)
{
	GtkEntry *entry_1 = g_object_get_data(G_OBJECT(button), "entry_1");
        GtkEntry *entry_2 = g_object_get_data(G_OBJECT(button), "entry_2");

        const gchar *entry_text1 = gtk_entry_get_text(GTK_ENTRY(entry_1));
        const gchar *entry_text2 = gtk_entry_get_text(GTK_ENTRY(entry_2));

        char RecvBuf[256] = "";
        char SendBuf[256] = "";
	Message M;
        int l, n, SocketFD;


        M.type = 2;
	strcpy(M.sender, ID);
        SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (SocketFD < 0)
        {
                FatalError("socket creation failed");
        }

        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
        {
                FatalError("connecting to server failed");
        }
	strncpy(SendBuf, entry_text2, sizeof(SendBuf)-1);
        l = strlen(SendBuf);

        if (l)
        {
                strcpy(M.message, entry_text2);
                strcpy(M.receiver, entry_text1);

        }

	send_message(SocketFD, &M);
        n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        RecvBuf[n] = 0;

	close(SocketFD);
}
void Messages(GtkWidget *Message)
{
	GtkWidget *window;
        GtkWidget *entry_1, *entry_2, *button, *button2;
        GtkWidget *table, *label1, *label2;

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Send Message");
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(window), 10);

        table = gtk_table_new(2, 2, FALSE);
        gtk_container_add(GTK_CONTAINER(window), table);

        label1 = gtk_label_new("Who would you like to\nsend a message to");
        label2 = gtk_label_new("Message");

        gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
        gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

        entry_1 = gtk_entry_new();
        entry_2 = gtk_entry_new();

        gtk_table_attach(GTK_TABLE(table), entry_1, 1, 2, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
        gtk_table_attach(GTK_TABLE(table), entry_2, 1, 2, 1, 2,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


        gtk_entry_set_max_length(GTK_ENTRY(entry_1), 20);
        g_signal_connect (entry_1, "activate", G_CALLBACK (Message_callback), entry_1);
        gtk_entry_set_max_length(GTK_ENTRY(entry_2), 20);
        g_signal_connect (entry_2, "activate", G_CALLBACK (Message_callback), entry_2);

        gtk_entry_set_text (GTK_ENTRY (entry_1), "");
        gtk_entry_set_text (GTK_ENTRY (entry_2), "");

        gtk_widget_show(entry_1);
        gtk_widget_show(entry_2);


        button = gtk_button_new_with_label("Ok");
        gtk_table_attach(GTK_TABLE(table), button, 0, 1, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        button2 = gtk_button_new_with_label("Cancel");
        gtk_table_attach(GTK_TABLE(table), button2, 1, 2, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        g_object_set_data(G_OBJECT(button), "entry_1", entry_1);
        g_object_set_data(G_OBJECT(button), "entry_2", entry_2);

	g_signal_connect(button, "clicked", G_CALLBACK(Message_callback), NULL);

        g_signal_connect_swapped(button2, "clicked", G_CALLBACK(gtk_widget_destroy), window);

        gtk_widget_show(button);
        gtk_widget_show(button2);

	gtk_widget_show_all(window);

}

void Add_Contact(GtkWidget *Add_contacts)
{
	GtkWidget *window;
        GtkWidget *entry_1, *button, *button2;
        GtkWidget *table, *label1;

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Add Contact");
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(window), 10);

        table = gtk_table_new(1, 1, FALSE);
        gtk_container_add(GTK_CONTAINER(window), table);

        label1 = gtk_label_new("Who would you\nlike to\nadd?");

        gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

        entry_1 = gtk_entry_new();

        gtk_table_attach(GTK_TABLE(table), entry_1, 1, 2, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


        gtk_entry_set_max_length(GTK_ENTRY(entry_1), 20);
        g_signal_connect (entry_1, "activate", G_CALLBACK (Add_callback), entry_1);

        gtk_entry_set_text (GTK_ENTRY (entry_1), "");

        gtk_widget_show(entry_1);

	button = gtk_button_new_with_label("Add");
        gtk_table_attach(GTK_TABLE(table), button, 0, 1, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        button2 = gtk_button_new_with_label("Cancel");
        gtk_table_attach(GTK_TABLE(table), button2, 1, 2, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        g_object_set_data(G_OBJECT(button), "entry_1", entry_1);


        g_signal_connect(button, "clicked", G_CALLBACK(Add_callback), NULL);

        g_signal_connect_swapped(button2, "clicked", G_CALLBACK(gtk_widget_destroy), window);

        gtk_widget_show(button);
        gtk_widget_show(button2);


        gtk_widget_show_all(window);
}

/*Callback for Deleting Contact*/
static void delete_callback(GtkWidget* button, gpointer data)
{
    GtkEntry* entry_1 = g_object_get_data(G_OBJECT(button), "entry_1");

    const gchar* entry_text1 = gtk_entry_get_text(GTK_ENTRY(entry_1));

    char RecvBuf[256] = "";
    Message M;
    int n, SocketFD;

    g_stpcpy(M.receiver, entry_text1);


    M.type = 7;
    strcpy(M.sender, ID);
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {
        FatalError("socket creation failed");
    }

    if (connect(SocketFD, (struct sockaddr*) & ServerAddress, sizeof(ServerAddress)) < 0)
    {
        FatalError("connecting to server failed");
    }
    send_message(SocketFD, &M);
    n = read(SocketFD, RecvBuf, sizeof(RecvBuf) - 1);
    RecvBuf[n] = 0;

    close(SocketFD);
}

void ContactList(GtkWidget* Contact_list)
{
    GtkWidget* Win, * view, * Vbox;
    GtkTextBuffer* buffer;
    GtkTextIter iter;

    Win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(Win), "Contact List");
    gtk_window_set_position(GTK_WINDOW(Win), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(Win), 300, 300);
    gtk_container_set_border_width(GTK_CONTAINER(Win), 10);

    Vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(Win), Vbox);

    view = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(Vbox), view, TRUE, TRUE, 0);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));


    char RecvBuf[256] = "";
    Message M;
    int n, SocketFD;
    M.type = 9;
    strcpy(M.sender, ID);
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {
        FatalError("socket creation failed");
    }
    if (connect(SocketFD, (struct sockaddr*) & ServerAddress, sizeof(ServerAddress)) < 0)
    {
        FatalError("connecting to server failed");
    }

    send_message(SocketFD, &M);
    n = read(SocketFD, RecvBuf, sizeof(RecvBuf) - 1);
    RecvBuf[n] = 0;


    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    gtk_text_buffer_insert(buffer, &iter, RecvBuf, -1);


    //g_signal_connect(Win, "destroy", G_CALLBACK(gtk_main_quit), Win);

    gtk_widget_show_all(Win);

    close(SocketFD);

}

void Delete_Contact(GtkWidget* Remove_contacts)
{
    GtkWidget* window;
    GtkWidget* entry_1, * button, * button2;
    GtkWidget* table, * label1;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Delete Contact");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    table = gtk_table_new(1, 1, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);

    label1 = gtk_label_new("Who would you \nlike to remove?");

    gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
        GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    entry_1 = gtk_entry_new();

    gtk_table_attach(GTK_TABLE(table), entry_1, 1, 2, 0, 1,
        GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


    gtk_entry_set_max_length(GTK_ENTRY(entry_1), 20);
    g_signal_connect(entry_1, "activate", G_CALLBACK(delete_callback), entry_1);;

    gtk_entry_set_text(GTK_ENTRY(entry_1), "");

    gtk_widget_show(entry_1);


    button = gtk_button_new_with_label("Remove");
    gtk_table_attach(GTK_TABLE(table), button, 0, 1, 4, 5,
        GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

    button2 = gtk_button_new_with_label("Cancel");
    gtk_table_attach(GTK_TABLE(table), button2, 1, 2, 4, 5,
        GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

    g_object_set_data(G_OBJECT(button), "entry_1", entry_1);


    g_signal_connect(button, "clicked", G_CALLBACK(delete_callback), NULL);

    g_signal_connect_swapped(button2, "clicked", G_CALLBACK(gtk_widget_destroy), window);

    gtk_widget_show(button);
    gtk_widget_show(button2);


    gtk_widget_show_all(window);
}

void Contacts(GtkWidget *Contact)
{
        GtkWidget *Win;
        GtkWidget *Add_contacts, *Contact_list, *Remove_contacts;
        GtkWidget *vbox;

        Win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(Win), "Contacts");
        gtk_window_set_position(GTK_WINDOW(Win), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(Win), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(Win), 10);

        vbox = gtk_vbox_new(TRUE, 1);
        gtk_container_add(GTK_CONTAINER(Win), vbox);

        Add_contacts = gtk_button_new_with_label("Add Contact");
        Remove_contacts = gtk_button_new_with_label("Remove Contact");
        Contact_list = gtk_button_new_with_label("View Contact List");

        gtk_box_pack_start(GTK_BOX(vbox), Add_contacts, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), Remove_contacts, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), Contact_list, TRUE, TRUE, 0);
        
        g_signal_connect(Add_contacts, "clicked", G_CALLBACK(Add_Contact), NULL);
        g_signal_connect(Remove_contacts, "clicked", G_CALLBACK(Delete_Contact), NULL);
        g_signal_connect(Contact_list, "clicked", G_CALLBACK(ContactList), NULL);
        g_signal_connect(Contact, "clicked", G_CALLBACK(Contacts), NULL);


	gtk_widget_show_all(Win);

}

void LogOff(GtkWidget* Quit)
{
    char RecvBuf[256] = "";
    Message M;
    int n, SocketFD;
    M.type = 4;
    M.status = 0;
    strcpy(M.sender, ID);
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {
        FatalError("socket creation failed");
    }
    if (connect(SocketFD, (struct sockaddr*) & ServerAddress, sizeof(ServerAddress)) < 0)
    {
        FatalError("connecting to server failed");
    }

    send_message(SocketFD, &M);
    n = read(SocketFD, RecvBuf, sizeof(RecvBuf) - 1);
    RecvBuf[n] = 0;

    close(SocketFD);
}

void Account_Info(GtkWidget *AccountInfo)
{
	GtkWidget *Win, *view, *Vbox;
	GtkTextBuffer *buffer;
	GtkTextIter iter;

        Win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(Win), "Account Information");
        gtk_window_set_position(GTK_WINDOW(Win), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(Win), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(Win), 10);

	Vbox = gtk_vbox_new(FALSE, 0);
  	gtk_container_add (GTK_CONTAINER (Win), Vbox);	

	view = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(Vbox), view, TRUE, TRUE, 0);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));


	char RecvBuf[256] = "";
        Message M;
        int n, SocketFD;
        M.type = 8;
	strcpy(M.sender, ID);
        SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (SocketFD < 0)
        {
                FatalError("socket creation failed");
        }
        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
        {
                FatalError("connecting to server failed");
        }

        send_message(SocketFD, &M);
        n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        RecvBuf[n] = 0;
        

	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
	gtk_text_buffer_insert(buffer, &iter, RecvBuf, -1);


        //g_signal_connect(Win, "destroy", G_CALLBACK(gtk_main_quit), Win);

        gtk_widget_show_all(Win);

	close(SocketFD);
	
}

void Chess_callback(GtkWidget *button)
{
	GtkEntry* entry_1 = g_object_get_data(G_OBJECT(button), "entry_1");

    	const gchar* entry_text1 = gtk_entry_get_text(GTK_ENTRY(entry_1));



	char RecvBuf[5000] = "";
        Message M;
        int n, SocketFD, l;
	char choice[10];
	int chess_response = 0;
	int exit = 0;
	char temp_send[256];
	int choice_num = 0;
	char SendBuf[256];
    

	system("clear");	
	SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (SocketFD < 0)
        {  FatalError("socket creation failed");
        }
        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
    	{   FatalError("connecting to server failed");
        }
//		printf("Who would you like to play chess with: ");
//		scanf("%s", receiver);
  //      getchar();
//        printf("receiver is:  %s\n", receiver);

		/* Check for person they wanna play is a friend online */
		M.type = 11; // Gets the Contact list and see if they are online
		strcpy(M.sender, ID);
		strcpy(M.receiver, entry_text1);
		send_message(SocketFD, &M);
        n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        RecvBuf[n] = 0;
		char temp[50];
					
        printf("%s", RecvBuf);
        close(SocketFD);
		strcpy(temp, entry_text1);
		strcat(temp, " is online, connecting...\n\n");
				
		//M.stall = 1;
		if (!strcmp(RecvBuf, temp))
		{
			chess_response = 1;
			M.start = 1;
			do{	
			/* Display current board */							
				M.type = 10;
				if (M.start == 1)
				{
					SocketFD = socket(AF_INET, SOCK_STREAM, 0);
 					if (SocketFD < 0)
                    {  FatalError("socket creation failed");
                        //return 0;
                    }
                    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
                    {   FatalError("connecting to server failed");
                        //return 0;
                    }
					send_message(SocketFD, &M);
					n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                    RecvBuf[n] = 0;
                    printf("%s", RecvBuf);
                    close(SocketFD);
					M.start = 0;
				}
						//if (M.stall == 0)
							//{
					printf("1. Make a move\n");
					printf("2. Exit Game\n");
					printf("Selection: ");
						
					fgets(choice, 10, stdin);

            		if (isdigit(choice[0]) && strlen(choice) == 2)
                		choice_num = atoi(choice);
            		else
                		choice_num = 0;

					if (choice_num == 1)
					{
						printf("Make your move(example format: A2 to A4): ");
						//exit = chess();
						fgets(SendBuf, sizeof(SendBuf), stdin);
						l = strlen(SendBuf);
						strcpy(temp_send, SendBuf);

						if (SendBuf[l-1] == '\n')
						{
							SendBuf[--l] = 0;
						}

						if(strcmp(temp_send, "exit")){
							exit = 3;
						}

						if (l)
						{
							SocketFD = socket(AF_INET, SOCK_STREAM, 0);
							if (SocketFD < 0)
							{	  
								FatalError("socket creation failed");
							}
							if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
								sizeof(ServerAddress)) < 0)
							{  
								FatalError("connecting to server failed");
							}
							strcpy(M.sender, ID);
							strcpy(M.message, SendBuf);
							strcpy(M.receiver, entry_text1);
							M.type = 10;
							if(send_message(SocketFD, &M)== 0)
								printf("Sending message '%s'...\n", M.message);
							else
								printf("ERROR\n");
							if (n < 0)
							{
								FatalError("writing to socket failed");
							}
							n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
							if (n < 0)
							{	
								FatalError("reading from socket failed");
								//Program = Username;
							}
							RecvBuf[n] = 0;
							printf("%s: Received response: %s\n", ID, RecvBuf);
							if(!strcmp(RecvBuf, "Move Successful"))
							{
								//printf("Move Successful\n");
								exit = 0;
							}
							else if(!strcmp(RecvBuf, "Incorrect Format"))
							{
								printf("Please try again\n");
								exit = 0;
							}
							else if(!strcmp(RecvBuf, "Illegal Move"))
							{
								printf("Please try again\n");
								exit = 0;
							}
							else if(!strcmp(RecvBuf, "Checkmate"))
							{
								exit = 3;
							}
							else if(!strcmp(RecvBuf, "Stalemate"))
							{
								exit = 3;
							}
						}
						close(SocketFD);
							//}
						printf("%s\n", RecvBuf);
                                               	//	while(strcmp(RecvBuf, "")){

                                                //	}

                        if(choice_num == 2)
                            exit = 3;
                        if (choice_num != 1 && choice_num != 2)
                            printf("Invalid Selection!\n");
						if (choice_num != 2)
							exit = 0;
					}
					SocketFD = socket(AF_INET, SOCK_STREAM, 0);	   
					if (connect(SocketFD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
                    {
                        FatalError("connecting to server failed");
                    }
             
       				M.type = 12;
					send_message(SocketFD, &M);
                    n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                 	RecvBuf[n] = 0;
					printf("Waiting for other player\n\n");
							
					while (!strcmp(RecvBuf, "Other Player's turn"))
					{	

						close(SocketFD);
						SocketFD = socket(AF_INET, SOCK_STREAM, 0);
						if (connect(SocketFD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
                        {
                            FatalError("connecting to server failed");
                        }

						send_message(SocketFD, &M);
						n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
						//ecvBuf[n] = 0;	
						if (!strcmp(RecvBuf, "Your turn"))
						{	RecvBuf[n] = 0;
							break;	
						}	
						//printf("Wait...\n\n");
						//printf("%s\n", RecvBuf);
						sleep(1);
					}
					close(SocketFD);
					M.start = 1;

			}while(exit != 3);//end of do while	
			//reset the board after client exiting
			M.type = 13;
			strcpy(M.sender, ID);
			strcpy(M.receiver, entry_text1);
			SocketFD = socket(AF_INET, SOCK_STREAM, 0);
			if (SocketFD < 0)
			{  
				FatalError("socket creation failed");
			}
			if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
			{   
				FatalError("connecting to server failed");
			}
			send_message(SocketFD, &M);
			n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
			RecvBuf[n] = 0;
			printf("\n%s\n", RecvBuf);
			close(SocketFD);
		}

//	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
//	gtk_text_buffer_insert(buffer, &iter, "Please check the terminal to play chess.", -1);


        //g_signal_connect(Win, "destroy", G_CALLBACK(gtk_main_quit), Win);


	close(SocketFD);
	
}

void play_chess(GtkWidget *Chess)
{
	GtkWidget *window;
        GtkWidget *entry_1, *button, *button2;
        GtkWidget *table, *label1;

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Log In");
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(window), 10);

        table = gtk_table_new(1, 1, FALSE);
        gtk_container_add(GTK_CONTAINER(window), table);

        label1 = gtk_label_new("Who would you like\nto play against?");

        gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

        entry_1 = gtk_entry_new();

        gtk_table_attach(GTK_TABLE(table), entry_1, 1, 2, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


        gtk_entry_set_max_length(GTK_ENTRY(entry_1), 20);
        g_signal_connect (entry_1, "activate", G_CALLBACK (Chess_callback), entry_1);

        gtk_entry_set_text (GTK_ENTRY (entry_1), "");

        gtk_widget_show(entry_1);


        button = gtk_button_new_with_label("Ok");
        gtk_table_attach(GTK_TABLE(table), button, 0, 1, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        button2 = gtk_button_new_with_label("Cancel");
        gtk_table_attach(GTK_TABLE(table), button2, 1, 2, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        g_object_set_data(G_OBJECT(button), "entry_1", entry_1);

	g_signal_connect(button, "clicked", G_CALLBACK(Chess_callback), NULL);

        g_signal_connect_swapped(button2, "clicked", G_CALLBACK(gtk_widget_destroy), window);

        gtk_widget_show(button);
        gtk_widget_show(button2);


        gtk_widget_show_all(window);
}
	

/*callback for registering*/
static void enter_callback(GtkWidget *button)
{
    	GtkEntry *entry1 = g_object_get_data(G_OBJECT(button), "entry1");
	GtkEntry *entry2 = g_object_get_data(G_OBJECT(button), "entry2");
	GtkEntry *entry3 = g_object_get_data(G_OBJECT(button), "entry3");
	GtkEntry *entry4 = g_object_get_data(G_OBJECT(button), "entry4");
	
	const gchar *entry_text1 = gtk_entry_get_text(GTK_ENTRY(entry1));
	const gchar *entry_text2 = gtk_entry_get_text(GTK_ENTRY(entry2));
	const gchar *entry_text3 = gtk_entry_get_text(GTK_ENTRY(entry3));
	const gchar *entry_text4 = gtk_entry_get_text(GTK_ENTRY(entry4));

	char RecvBuf[256] = "";

	Message M;
	int n, SocketFD;

        
	printf("First Name: %s\n", entry_text1);
	g_stpcpy(M.fname, entry_text1);
	printf("Last Name: %s\n", entry_text2);
        g_stpcpy(M.lname, entry_text2);
	printf("Username: %s\n", entry_text3);
        g_stpcpy(M.sender, entry_text3);
	printf("Password: %s\n", entry_text4);
        g_stpcpy(M.password, entry_text4);

	M.type = 0;

	SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (SocketFD < 0)
        {  
		FatalError("socket creation failed");
        }

        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
        {   
		FatalError("connecting to server failed");
        }
        
	send_message(SocketFD, &M);
        n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        RecvBuf[n] = 0;
        printf("Received response from server: %s\n", RecvBuf);

        close(SocketFD);
			
	
}

/*callback for logging in*/
static void enter_callback2(GtkWidget *button, gpointer data)
{
	GtkEntry *entry_1 = g_object_get_data(G_OBJECT(button), "entry_1");        
	GtkEntry *entry_2 = g_object_get_data(G_OBJECT(button), "entry_2");

	const gchar *entry_text1 = gtk_entry_get_text(GTK_ENTRY(entry_1));
	const gchar *entry_text2 = gtk_entry_get_text(GTK_ENTRY(entry_2));

	char RecvBuf[256] = "";
	Message M;
	int n, SocketFD;

        printf("Username: %s\n", entry_text1);
	g_stpcpy(M.sender, entry_text1);
	printf("Password: %s\n", entry_text2);
	g_stpcpy(M.password, entry_text2);

	M.type = 1;

	SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (SocketFD < 0)
        {
        	FatalError("socket creation failed");
        }

        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) < 0)
        {
                FatalError("connecting to server failed");
        }
        send_message(SocketFD, &M);
        n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        RecvBuf[n] = 0;
        printf("Received response from server: %s\n", RecvBuf);
        if(!strcmp(RecvBuf, "Successful login!\n\n"))
        {
        	ifLogin = 1;
                strcpy(ID, entry_text1);
	}
	/*If successful log in*/
	if(ifLogin == 1)
	{
		GtkWidget *win;
		GtkWidget *box;
		GtkWidget *AccountInfo, *Contact, *Chess, *Message, *Quit;

		win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(win), "Welcome");
	        gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	        gtk_window_set_default_size(GTK_WINDOW(win), 300, 300);
	        gtk_container_set_border_width (GTK_CONTAINER(win), 10);

		box = gtk_vbox_new(TRUE, 1);
		gtk_container_add(GTK_CONTAINER(win), box);

		AccountInfo = gtk_button_new_with_label("Account Information");
		Contact = gtk_button_new_with_label("Contacts");
		Message = gtk_button_new_with_label("Messages");
		Chess = gtk_button_new_with_label("Play Chess");
		Quit = gtk_button_new_with_label("Log Out");		

		gtk_box_pack_start(GTK_BOX(box), AccountInfo, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(box), Contact, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(box), Message, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(box), Chess, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(box), Quit, TRUE, TRUE, 0);

		g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), win);		
		g_signal_connect(Quit, "clicked", G_CALLBACK (LogOff), NULL);
		g_signal_connect_swapped (Quit, "clicked", G_CALLBACK (gtk_widget_destroy), win);
		g_signal_connect_swapped (Message, "clicked", G_CALLBACK(Messages), NULL);
		g_signal_connect(Contact, "clicked", G_CALLBACK(Contacts), NULL);
		g_signal_connect(AccountInfo, "clicked", G_CALLBACK(Account_Info), NULL);
		g_signal_connect(Chess, "clicked", G_CALLBACK(play_chess), NULL);
		gtk_widget_show_all(win);
	}
        
	close(SocketFD);


}

void SignUp_window(GtkWidget *widget, gpointer data)
{
        GtkWidget *newWindow;
        GtkWidget *entry1, *entry2, *entry3, *entry4, *button, *button2;
        GtkWidget *table, *label1, *label2, *label3, *label4;
        gint tmp_pos1, tmp_pos2, tmp_pos3, tmp_pos4;

        newWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(newWindow), "Register");
        gtk_window_set_position(GTK_WINDOW(newWindow), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(newWindow), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(newWindow), 10);

        table = gtk_table_new(4, 2, FALSE);
        gtk_container_add(GTK_CONTAINER(newWindow), table);

        label1 = gtk_label_new("First Name");
	label2 = gtk_label_new("Last Name");
	label3 = gtk_label_new("Username");
        label4 = gtk_label_new("Password");


        gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
        	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
        gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,
        	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), label3, 0, 1, 2, 3, 
      		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), label4, 0, 1, 3, 4, 
      		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


        entry1 = gtk_entry_new();
        entry2 = gtk_entry_new();
	entry3 = gtk_entry_new();
	entry4 = gtk_entry_new();

	gtk_table_attach(GTK_TABLE(table), entry1, 1, 2, 0, 1,
        	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
        gtk_table_attach(GTK_TABLE(table), entry2, 1, 2, 1, 2,
        	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), entry3, 1, 2, 2, 3, 
      		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), entry4, 1, 2, 3, 4, 
      		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

        gtk_entry_set_max_length(GTK_ENTRY(entry1), 20);
        g_signal_connect (entry1, "activate", G_CALLBACK (enter_callback), entry1);
        gtk_entry_set_max_length(GTK_ENTRY(entry2), 20);
        g_signal_connect (entry2, "activate", G_CALLBACK (enter_callback), entry2);
	gtk_entry_set_max_length(GTK_ENTRY(entry1), 20);
        g_signal_connect (entry3, "activate", G_CALLBACK (enter_callback), entry3);
        gtk_entry_set_max_length(GTK_ENTRY(entry2), 20);
        g_signal_connect (entry4, "activate", G_CALLBACK (enter_callback), entry4);
        
	gtk_entry_set_text (GTK_ENTRY (entry1), "");
        tmp_pos1 = GTK_ENTRY (entry1)->text_length;

        gtk_entry_set_text (GTK_ENTRY (entry2), "");
        tmp_pos2 = GTK_ENTRY (entry2)->text_length;
	
	gtk_entry_set_text (GTK_ENTRY (entry3), "");
        tmp_pos3 = GTK_ENTRY (entry3)->text_length;

	gtk_entry_set_text (GTK_ENTRY (entry4), "");
        tmp_pos4 = GTK_ENTRY (entry4)->text_length;

        gtk_widget_show(entry1);
        gtk_widget_show(entry2);
	gtk_widget_show(entry3);
        gtk_widget_show(entry4);

        button = gtk_button_new_with_label("Ok");
	gtk_table_attach(GTK_TABLE(table), button, 0, 1, 4, 5,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

	button2 = gtk_button_new_with_label("Cancel");
        gtk_table_attach(GTK_TABLE(table), button2, 1, 2, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

	g_object_set_data(G_OBJECT(button), "entry1", entry1);
	g_object_set_data(G_OBJECT(button), "entry2", entry2);
	g_object_set_data(G_OBJECT(button), "entry3", entry3);
	g_object_set_data(G_OBJECT(button), "entry4", entry4);
       	
	g_signal_connect(button, "clicked", G_CALLBACK(enter_callback), NULL);
	g_signal_connect_swapped (button2, "clicked", G_CALLBACK (gtk_widget_destroy), newWindow);
 
	gtk_widget_show(button);
	gtk_widget_show(button2);


        gtk_widget_show_all(newWindow);
}


void LogIn_window(GtkWidget *widget, gpointer Window)
{
	GtkWidget *window;
	GtkWidget *entry_1, *entry_2, *button, *button2;
	GtkWidget *table, *label1, *label2;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Log In");
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
        gtk_container_set_border_width (GTK_CONTAINER(window), 10);

	table = gtk_table_new(2, 2, FALSE);
        gtk_container_add(GTK_CONTAINER(window), table);

        label1 = gtk_label_new("Username");
        label2 = gtk_label_new("Password");

	gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
        gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	entry_1 = gtk_entry_new();
        entry_2 = gtk_entry_new();

	gtk_table_attach(GTK_TABLE(table), entry_1, 1, 2, 0, 1,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
        gtk_table_attach(GTK_TABLE(table), entry_2, 1, 2, 1, 2,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


	gtk_entry_set_max_length(GTK_ENTRY(entry_1), 20);
        g_signal_connect (entry_1, "activate", G_CALLBACK (enter_callback2), entry_1);
        gtk_entry_set_max_length(GTK_ENTRY(entry_2), 20);
        g_signal_connect (entry_2, "activate", G_CALLBACK (enter_callback2), entry_2);

	gtk_entry_set_text (GTK_ENTRY (entry_1), "");
	gtk_entry_set_text (GTK_ENTRY (entry_2), "");

	gtk_widget_show(entry_1);
        gtk_widget_show(entry_2);


	button = gtk_button_new_with_label("Ok");
        gtk_table_attach(GTK_TABLE(table), button, 0, 1, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

        button2 = gtk_button_new_with_label("Cancel");
        gtk_table_attach(GTK_TABLE(table), button2, 1, 2, 4, 5,
                GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 50, 50);

	g_object_set_data(G_OBJECT(button), "entry_1", entry_1);
        g_object_set_data(G_OBJECT(button), "entry_2", entry_2);


	g_signal_connect(button, "clicked", G_CALLBACK(enter_callback2), NULL);

	g_signal_connect_swapped(button2, "clicked", G_CALLBACK(gtk_widget_destroy), window);

	gtk_widget_show(button);
        gtk_widget_show(button2);


	gtk_widget_show_all(window);


}



int main(int argc, char *argv[])
{
    	struct hostent
		*Server;	/* server host */

	int Port_number;

	Program = argv[0];

	Port_number = atoi(argv[2]);
      	
	/* Establishing Connection to server */
        printf("Client: Starting...\n");
        if (argc < 3)
        {
                printf("Please enter hostname and port number in command line\n\n");
                return 0;
        }

        Server = gethostbyname(argv[1]);
        if (Server == NULL)
        {
                fprintf(stderr, "Client: no such host named '%s'\n", argv[1]);
                return 0;
        }

        if (Port_number <= 2000)
        {
                printf("Client: invalid port number %d, should be greater 2000\n", Port_number);
                return 0;
        }  
      
        ServerAddress.sin_family = AF_INET;
        ServerAddress.sin_port = htons(Port_number);
 	ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];

        system("clear");
        
        pthread_t thread;
        int error;
        struct sockaddr_in *ptr;
        ptr = &ServerAddress;

        
        error = pthread_create(&thread, NULL, get_message_thread, (void *)ptr);

        if (error)
        {
                printf("An error occured: %d", error);
        }


	
             
/* ------------------------------------- GTK --------------------------------*/

        GtkWidget *Window;
        GtkWidget *VBox, *SignUpButton, *LogInButton;
        GtkWidget *HButtons, *QuitButton;
	gtk_init(&argc, &argv);

        /* create the main, top level window */
        Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    	/* give it the title of this program */
    	gtk_window_set_title(GTK_WINDOW(Window), Program);

    	/* center the window */
    	gtk_window_set_position(GTK_WINDOW(Window), GTK_WIN_POS_CENTER);

    	/* set the window's default size */
    	gtk_window_set_default_size(GTK_WINDOW(Window), 200, 280);
    	gtk_container_set_border_width (GTK_CONTAINER(Window), 10);

    	/* map the destroy signal of the window to gtk_main_quit;
 	*  *      * when the window is about to be destroyed, we get a notification and
 	*   *           * stop the main GTK+ loop by returning 0 */
    	g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    	/* overall vertical arrangement in the window */
    	VBox = gtk_vbox_new(TRUE, 10);
    	gtk_container_add(GTK_CONTAINER(Window), VBox);

    	/* on the top, put a button to request the current time from the server */
    	SignUpButton = gtk_button_new_with_label("Register");
    	gtk_container_add(GTK_CONTAINER(VBox), SignUpButton);

    	/* in the middle, a frame with the display of the time */
    	LogInButton = gtk_button_new_with_label("Log In");
    	gtk_container_add(GTK_CONTAINER(VBox), LogInButton);

    	/* on the bottom, two buttons to quit client and shutdown server */
    	HButtons = gtk_hbutton_box_new();
   	gtk_container_add(GTK_CONTAINER(VBox), HButtons);
   	QuitButton = gtk_button_new_with_label("Quit Client");
    	gtk_container_add(GTK_CONTAINER(HButtons), QuitButton);
    	/*ShutdownButton = gtk_button_new_with_label("Shutdown Server");
    	gtk_container_add(GTK_CONTAINER(HButtons), ShutdownButton);*/

    	/* make sure that everything becomes visible */
    	gtk_widget_show_all(Window);

	g_signal_connect(SignUpButton, "clicked", G_CALLBACK(SignUp_window), Window);
	g_signal_connect(LogInButton, "clicked", G_CALLBACK(LogIn_window), Window);	
	/* connect quit button with function terminating this client */
    	/* (note the 'swapped' call; try without to see the effect) */
    	g_signal_connect_swapped(QuitButton, "clicked", G_CALLBACK(gtk_widget_destroy), Window);

	/* connect shutdown button with function terminating server and client */
    	/*g_signal_connect(ShutdownButton, "clicked",
                        G_CALLBACK(ShutdownServer), NULL);*/
    	/* start the main loop, handle all registered events */
   	gtk_main();



	return 0;
}



/* EOF Client_test.c */
