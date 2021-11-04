/* Contacts.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Contacts.h"

/* add new contact */
CONTACT *AddContact(char *Name)
{
	CONTACT *c = (CONTACT *)malloc(sizeof(CONTACT));
	
	if (c == NULL)
	{
		return NULL;
	}
	
	strcpy(c->Name, Name);
	return c;
}

/* Delete Contact */
void DeleteContact(CONTACT *c)
{
	assert(c);
	free(c);
		
	/* Remove Contact from list */
}

/* Create contact entry */
CENTRY *CreateContactEntry(CONTACT *c)
{
	CENTRY *e;
	e = malloc(sizeof(CENTRY));
	if (e == NULL)
	{
		return NULL;
	}
	e->CList = NULL;
	e->CNext = NULL;
	e->CPrev = NULL;
	e->Contact = c;

	return e;
}

/* Delete Contact Entry */
void DeleteContactEntry(CENTRY *e)
{
	assert(e);
	if (e->Contact != NULL)
	{
		DeleteContact(e->Contact);
		e->Contact = NULL;
	}
	free(e);
}

/* Create a new account list */
CLIST *CreateContactList(void)
{
    	CLIST * l;
    	l = malloc(sizeof(CLIST));
    	if (l == NULL) {
                return NULL;
        }
    	l->Length = 0;
    	l->CFirst = NULL;
    	l->CLast = NULL;

    	return l;
}

/* Delete Contact List */
void DeleteContactList(CLIST *l)
{
	CENTRY *e, *n;
	assert(l);
	e = l->CFirst;
	while(e)
	{
		n = e->CNext;
		DeleteContactEntry(e);
		e = n;
	}
	free(l);
}

