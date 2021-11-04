#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "Contacts.h"

typedef struct {
	
    char username[21];         
	char password[9];           
	char chat[256];   
	char fname[15];
	char lname[15];
	int status;
	int chess;
	int turn;
	char player_board[8][8];
	CLIST *CList;
	 
} ACCOUNT;

typedef struct AccountEntry ENTRY;

typedef struct AccountList LIST;

struct AccountEntry {
	ACCOUNT *account;
	ENTRY *Next;
	ENTRY *Prev;
	LIST *List;
};

struct AccountList {
	ENTRY *First;
	ENTRY *Last;
	int length;
};

/* Appending contact to user list of contacts */
void AppendContact(ACCOUNT *a, CONTACT *contact);

/* Removing contact from list */
void RemoveContact(ACCOUNT *a, char *Name);

/* New ContactList */
void NewContactList(CLIST *l, CONTACT *c);

/* Print Board to User */
void PrintBoard(char *SendBuf, char board[8][8]);

/* Shows users account information */
void PrintAccountInfo(ACCOUNT *account, char *SendBuf);

/* Shows user contact list */
void PrintContactList(ACCOUNT *account, char *SendBuf, LIST* AList);

ACCOUNT *CreateAccount(char *username, char *password, char *fname, char *lname, int status);

void DeleteAccount(ACCOUNT *account);

/* Create a new account entry */
ENTRY *CreateAccountEntry(ACCOUNT *account);

/* Delete an account entry */
void DeleteAccountEntry(ENTRY *entry);

/* Create a new account list */
LIST *CreateAccountList(void);

/* Delete an account list */
void DeleteAccountList(LIST *list);

/* Insert an account to the image list at the end */
void AppendAccount(LIST *list, ACCOUNT *account);

#endif
