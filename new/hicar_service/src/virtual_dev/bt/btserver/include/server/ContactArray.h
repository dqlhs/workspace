/*
 * ContactArray.h
 *
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _CONTACT_ARRAY_H
#define _CONTACT_ARRAY_H

#include "HsaeTypes.h"



class  ContactElement {

public:
	ContactElement();
	ContactElement(const HSAE_CONTACT_INFO *contact_content);
	~ContactElement();
	ContactElement *Initialize(const HSAE_CONTACT_INFO *contact_content);
	void Finalize();
	HSAE_CONTACT_INFO *GetStructor();
	void DisplayEleContents();

private:
	bool CopyStrFromPtr(char **desc,const char *src);
	HSAE_CONTACT_INFO contact_index;
};

typedef struct ConEleLink
{	ContactElement *contact_array_ptr;
	struct ConEleLink *next;
}CONTACTLINK;


class  ContactArray {

public:
	ContactArray(const HSAE_CONTACT_INFO *contact_content,unsigned int size);
	~ContactArray();
	HSAE_CONTACT_INFO *GetAt(unsigned int pos);
	bool Append(const HSAE_CONTACT_INFO *contact_content);
	bool Remove();
	bool RemoveAll();
	unsigned int GetCount() const;
	void DisplayArrayContents();
	
private:
	CONTACTLINK *array_head;
	unsigned int count;
};


#endif 
