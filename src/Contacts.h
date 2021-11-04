#ifndef CONTACTS_H
#define CONTACTS_H


typedef struct ContactEntry CENTRY; 
typedef struct Contact CONTACT;
typedef struct ContactList CLIST;

struct Contact
{
	int messages;
	int GameRequests;
	int ChessMoves;
	char Name[21];
	int status;
};

struct ContactEntry
{
	CLIST *CList;
	CENTRY *CNext;
	CENTRY *CPrev;
	CONTACT *Contact;
};

struct ContactList
{
	int Length;
	CENTRY *CFirst;
	CENTRY *CLast;
};

/* add new contact */
CONTACT *AddContact(char *Name);

/* Delete Contact */
void DeleteContact(CONTACT *c);

/* Create contact entry */
CENTRY *CreateContactEntry(CONTACT *c);

/* Delete Contact Entry */
void DeleteContactEntry(CENTRY *e);

/* Create a new account list */
CLIST *CreateContactList(void);

/* Delete Contact List */
void DeleteContactList(CLIST *l);

#endif
