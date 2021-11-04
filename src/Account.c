/* Account.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "Account.h"
#include "Contacts.h"

//double linked list to save accounts in the server
ACCOUNT *CreateAccount(char *username, char *password, char *fname, char *lname, int status)
{
    ACCOUNT *account = (ACCOUNT *)malloc(sizeof(ACCOUNT));
	if (account == NULL) {
		return NULL;
	}
    CLIST *CList = NULL;  
    if (CList == NULL)
    {
	    CList = CreateContactList();
    }
    strcpy(account -> username, username);
    strcpy(account -> password, password);
    strcpy(account -> fname , fname);
    strcpy(account -> lname, lname);
    account->chess = 0;
    char board[8][8] = {

						
							{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
							{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},			
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
							{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
							{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},


						};
    memcpy(&(account -> player_board), &board, sizeof(board));
    account->CList = CList;
    account->status = status;
    strcpy(account -> chat, "");
    

    return account;
}

/* Print Board to User */
void PrintBoard(char *SendBuf, char board[8][8])
{
 	int j = 8;
	char buffer[3];
	char piece[2];

	strcat(SendBuf,"\n    A   B   C   D   E   F   G   H\n");
	for (int i = 0; i < 8; i++)
	{
		strcat(SendBuf, "  +---+---+---+---+---+---+---+---+\n ");
		sprintf(buffer, "%d", j);
		strcat(SendBuf, buffer);
		for (int k = 0; k < 8; k++)
		{
           		if (k == 7)
			{
				strcat(SendBuf, "| ");
				sprintf(piece, "%c |", board[i][k]); 
				strcat(SendBuf, piece);
			//	strcat(SendBuf, " |");
		
			}
			else
			{
				strcat(SendBuf, "| ");
				sprintf(piece, "%c ", board[i][k]);
				strcat(SendBuf, piece);
			//	strcat(SendBuf, " ");
			}
		}
		strcat(SendBuf, buffer);
		strcat(SendBuf, "\n");
		j--;	
	}
	strcat(SendBuf, "  +---+---+---+---+---+---+---+---+\n");
	strcat(SendBuf, "    A   B   C   D   E   F   G   H\n\n");
	printf("%s", SendBuf);
}


/* Shows users account information */
void PrintAccountInfo(ACCOUNT *account, char *SendBuf)
{
	assert(account);
	strcat(SendBuf, "     ~ Account Information ~ \n");
	strcat(SendBuf, "Username: ");
	strcat(SendBuf, account->username);
	strcat(SendBuf, "\nPassword: ");
	strcat(SendBuf,  account->password);
	strcat(SendBuf, "\nFirst Name: ");
	strcat(SendBuf, account->fname);
	strcat(SendBuf, "\nLast Name: ");
	strcat(SendBuf, account->lname);
	strcat(SendBuf, "\n\n");
}

/* Shows user contact list */
void PrintContactList(ACCOUNT *account, char *SendBuf, LIST* AList)
{
	assert(account);
	CENTRY *e, *n;
	e = account->CList->CFirst;
	strcat(SendBuf, "~~~ Contact List ~~~ \n============================================\n");
   	//printf("\nContact List\n");                       
	while (e)
	{	
		n = e->CNext;
        strcat(SendBuf, "- ");
		strcat(SendBuf, e->Contact->Name);
        	//printf("User: %s\n", e->Contact->Name);
        
        	ENTRY* f;
        	f = AList->First;
        	assert(AList);
        	while (f)
        	{
            		if (strcmp(f->account->username, e->Contact->Name) == 0)
            		{
                		//printf("Status: %d\n\n", f->account->status);
                		if (f->account->status == 1)
                		{
                    	    strcat(SendBuf, " (Online)");
                		}
                        else if (f->account->status == 0)
                        {
                            strcat(SendBuf, " (Offline)");
                        }
                		break;
            		}
            		f = f->Next;
        	}
		strcat(SendBuf, "\n");
		e = n;
	}
	strcat(SendBuf, "\n");		
}

void DeleteAccount(ACCOUNT *account)
{
    assert(account);
    free(account);
}


/* Create a new account entry */
ENTRY *CreateAccountEntry(ACCOUNT *account)
{
    ENTRY *e; 
    e = malloc(sizeof(ENTRY));
    if (e == NULL) {
		return NULL;
	}
    e -> List = NULL;
    e -> Next = NULL;
    e -> Prev = NULL;
    e -> account = account;

    return e;
}

/* Delete an account entry */
void DeleteAccountEntry(ENTRY *entry)
{   
    assert(entry);
    if(entry -> account != NULL)
    {
        DeleteAccount(entry -> account);
        entry -> account = NULL;
    }
    free(entry);
}

/* Create a new account list */
LIST *CreateAccountList(void)
{
    LIST * l;
    l = malloc(sizeof(LIST));
    if (l == NULL) {
		return NULL;
	}
    l -> length = 0;
    l -> First = NULL;
    l -> Last = NULL;

    return l;
}

/* Delete an account list */
void DeleteAccountList(LIST *list)
{
    ENTRY *e, *n;
    assert(list);
    e = list -> First;
    while(e)
    {
        n = e -> Next;
        DeleteAccountEntry(e);
        e = n;
    }
    free(list);

}

/* Insert an account to the image list at the end */
void AppendAccount(LIST *list, ACCOUNT *account)
{
    ENTRY *e = NULL;
    assert(list);
    assert(account);
    e = CreateAccountEntry(account);
    if (list -> Last)
    {
        e -> List = list;
        e -> Next = NULL;
        e -> Prev = list -> Last;
        list -> Last -> Next = e;
        list -> Last = e;
    }
    else
    {
        e -> List = list;
        e -> Next = NULL;
        e -> Prev = NULL;
        list -> First = e;
        list -> Last = e;
    }
    list -> length++;
    
}

/* Appending contact to user list of contacts */
void AppendContact(ACCOUNT *a, CONTACT *contact)
{
        CENTRY *e = NULL;
        assert(a);
        assert(contact);
        e = CreateContactEntry(contact);
        if (a->CList->CLast)
        {
                e->CList = a->CList;
                e->CNext = NULL;
                e->CPrev = a->CList->CLast;
                a->CList->CLast->CNext = e;
                a->CList->CLast = e;
        }
        else
        {
                e->CList = a->CList;
                e->CNext = NULL;
                e->CPrev = NULL;
                a->CList->CFirst = e;
                a->CList->CLast = e;
        }
        a->CList->Length++;
}

/* New ContactList */
void NewContactList(CLIST *l, CONTACT *c)
{
        CENTRY *e = NULL;
        assert(l);
        assert(c);
        e = CreateContactEntry(c);
        if (l->CLast)
        {
                e->CList = l;
                e->CNext = NULL;
                e->CPrev = l->CLast;
                l->CLast->CNext = e;
                l->CLast = e;
        }
        else
        {
                e->CList = l;
                e->CNext = NULL;
                e->CPrev = NULL;
                l->CFirst = e;
                l->CLast = e;
        }
        l->Length++;
}


/* Removing contact from list */
void RemoveContact(ACCOUNT *a, char *Name)
{
        assert(a);
        CENTRY *e, *n;
        CLIST *list = CreateContactList();
        e = a->CList->CFirst;
        while (e)
        {
                n = e->CNext;
                if (strcmp(e->Contact->Name, Name) != 0)
                {
                        NewContactList(list, e->Contact);
                }
                e = n;
        }
	DeleteContactList(a->CList);
	a->CList = list;
}


