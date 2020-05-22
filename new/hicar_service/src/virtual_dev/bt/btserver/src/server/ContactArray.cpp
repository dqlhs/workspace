/*
 * ContactArray.cpp
 *

 * See the COPYING file for the terms of usage and distribution.
 */

#include "ContactArray.h"
#include <stdio.h>
#include <string.h>

ContactElement::ContactElement() {

    contact_index.name = NULL;
    contact_index.address = NULL;
    contact_index.phone = NULL;
    contact_index.phone_count = 0;
    contact_index.photo_data = NULL;
    contact_index.call_info.dir = NULL;
    contact_index.call_info.date = NULL;
    contact_index.call_info.time = NULL;
    contact_index.fullname = NULL;

}

ContactElement *ContactElement::Initialize(const HSAE_CONTACT_INFO *contact_content){

    if(contact_content){

        Finalize();
        if(contact_content->name)
            CopyStrFromPtr(&contact_index.name,contact_content->name);
        else
            contact_index.name = NULL;


        if(contact_content->address)
            CopyStrFromPtr(&contact_index.address,contact_content->address);
        else
            contact_index.address = NULL;


        contact_index.phone_count = contact_content->phone_count;
//        printf("contact_index.phone_count =%d\r\n",contact_index.phone_count);
        if(contact_index.phone_count > 0){
            contact_index.phone = new HSAE_PHONE[contact_index.phone_count];

            for(int i=0;i<contact_index.phone_count;i++){

                if(contact_content->phone[i].phone_number)
                    CopyStrFromPtr(&contact_index.phone[i].phone_number,contact_content->phone[i].phone_number);
                else
                    contact_index.phone[i].phone_number = NULL;

                contact_index.phone[i].phone_type_count = contact_content->phone[i].phone_type_count;
//                printf("HSAE_CONTACT_INFO.phone[%d].phone_type_count=%d\r\n",i,contact_index.phone[i].phone_type_count);
                contact_index.phone[i].phone_type = new HSAE_PHONE_TYPE[contact_index.phone[i].phone_type_count];
                memcpy(contact_index.phone[i].phone_type,contact_content->phone[i].phone_type,sizeof(HSAE_PHONE_TYPE)*contact_index.phone[i].phone_type_count);
//                printf("HSAE_CONTACT_INFO.phone[%d].phone_type_count=%d end\r\n",i,contact_index.phone[i].phone_type_count);

            }

        }else{
            contact_index.phone = NULL;
        }
         //  printf("contact_index.phone_count =%d end\r\n",contact_index.phone_count);
        if(contact_content->address)
            CopyStrFromPtr(&contact_index.photo_data,contact_content->photo_data);
        else
            contact_index.photo_data = NULL;

          printf("call_info.dir =%s\r\n",contact_content->call_info.dir);

        if(contact_content->call_info.dir)
            CopyStrFromPtr(&contact_index.call_info.dir,contact_content->call_info.dir);
        else
            contact_index.call_info.dir=NULL;
       // printf("call_info.date =%d\r\n",contact_content->call_info.date);
        if(contact_content->call_info.date)
            CopyStrFromPtr(&contact_index.call_info.date,contact_content->call_info.date);
        else
            contact_index.call_info.date=NULL;
          // printf("call_info.dir =%d\r\n",contact_content->call_info.time);
        if(contact_content->call_info.time)
            CopyStrFromPtr(&contact_index.call_info.time,contact_content->call_info.time);
        else
            contact_index.call_info.time=NULL;
        // printf("call_info.dir =%d\r\n",contact_content->fullname);
        if(contact_content->fullname)
            CopyStrFromPtr(&contact_index.fullname,contact_content->fullname);
        else
            contact_index.fullname=NULL;

    }
//   DisplayEleContents();
    return this;

}

void ContactElement::Finalize(){

    if(contact_index.name){
        delete [] contact_index.name;
    }

    if(contact_index.address){
        delete [] contact_index.address;
    }

    if(contact_index.phone){
        for(int i=0;i<contact_index.phone_count;i++){
            if(contact_index.phone->phone_number)
                delete [] contact_index.phone[i].phone_number;
            if(contact_index.phone->phone_type)
                delete [] contact_index.phone[i].phone_type;
        }
        delete [] contact_index.phone;
    }

    if(contact_index.photo_data){
        delete [] contact_index.photo_data;
    }

    if(contact_index.call_info.dir){
        delete [] contact_index.call_info.dir;
    }

    if(contact_index.call_info.date){
        delete [] contact_index.call_info.date;
    }

    if(contact_index.call_info.time){
        delete [] contact_index.call_info.time;
    }

    if(contact_index.fullname){
        delete [] contact_index.fullname;
    }

    contact_index.name = NULL;
    contact_index.address = NULL;
    contact_index.phone = NULL;
    contact_index.phone_count = 0;
    contact_index.photo_data = NULL;
    contact_index.call_info.dir = NULL;
    contact_index.call_info.date = NULL;
    contact_index.call_info.time = NULL;
    contact_index.fullname = NULL;

    return;

}

ContactElement::ContactElement(const HSAE_CONTACT_INFO *contact_content) {

    contact_index.name = NULL;
    contact_index.address = NULL;
    contact_index.phone = NULL;
    contact_index.phone_count = 0;
    contact_index.photo_data = NULL;
    contact_index.call_info.dir = NULL;
    contact_index.call_info.date = NULL;
    contact_index.call_info.time = NULL;
    contact_index.fullname = NULL;

    Initialize(contact_content);

}

ContactElement::~ContactElement()
{
    Finalize();
}

HSAE_CONTACT_INFO *ContactElement::GetStructor()
{
    return  &contact_index;
}

bool ContactElement::CopyStrFromPtr(char **desc,const char *src)
{
//   printf("CopyStrFromPtr before =%s\r\n",src);
    if(src){
        size_t str_length = strlen(src);
        *desc = new char[str_length+1];
        strcpy(*desc,src);
//        printf("CopyStrFromPtr after =%s\r\n",*desc);
        return true;
    }else{
//         printf("CopyStrFromPtr error\r\n");
        desc = NULL;
        return false;
    }



}

void ContactElement::DisplayEleContents()
{
    if(contact_index.name)
        printf("HSAE_CONTACT_INFO.name=%s\r\n",contact_index.name);
    if(contact_index.address)
    printf("HSAE_CONTACT_INFO.address=%s\r\n",contact_index.address);

    printf("HSAE_CONTACT_INFO.phone_count=%d\r\n",contact_index.phone_count);
    for(int i=0;i<contact_index.phone_count;i++){
        printf("HSAE_CONTACT_INFO.phone[%d]->phone_number=%s\r\n",i,contact_index.phone[i].phone_number);
        printf("HSAE_CONTACT_INFO.phone[%d].phone_type_count=%d\r\n",i,contact_index.phone[i].phone_type_count);
        for(int j=0;j<contact_index.phone[i].phone_type_count;j++)
            printf("HSAE_CONTACT_INFO.phone[%d]->phone_type[%d]=%d\r\n",i,j,contact_index.phone[i].phone_type[j]);
    }

    if(contact_index.photo_data)
        printf("HSAE_CONTACT_INFO.photo_data=%s\r\n",contact_index.photo_data);
    if(contact_index.call_info.dir)
        printf("HSAE_CONTACT_INFO.call_info.dir=%s\r\n",contact_index.call_info.dir);
    if(contact_index.call_info.date)
        printf("HSAE_CONTACT_INFO.call_info.date=%s\r\n",contact_index.call_info.date);
    if(contact_index.call_info.time)
        printf("HSAE_CONTACT_INFO.call_info.time=%s\r\n",contact_index.call_info.time);

    if(contact_index.fullname)
        printf("HSAE_CONTACT_INFO.fullname=%s\r\n",contact_index.fullname);
    return;
}

ContactArray::ContactArray(const HSAE_CONTACT_INFO *contact_content,unsigned int size)
{
    CONTACTLINK *array_temp,*array_prev;
    count = 0;
    array_prev = NULL;

    for(int i=0;i<size;i++){

        array_temp = new CONTACTLINK;
        array_temp->contact_array_ptr = new ContactElement((contact_content+i));

        array_temp->next = NULL;
        if(!array_prev){
            array_head = array_temp;
        }
        else{
            array_prev->next = array_temp;
        }
        array_prev = array_temp;
        count++;

    }


}

ContactArray::~ContactArray()
{
    RemoveAll();
}

HSAE_CONTACT_INFO *ContactArray::GetAt(unsigned int pos)
{
    CONTACTLINK *array_temp;
    unsigned int temp_count;

    temp_count = pos;
    array_temp = array_head;
    while(temp_count){
        if(!array_temp)
            break;

        array_temp = array_temp->next;
        temp_count--;
    }

    if(array_temp)
        return array_temp->contact_array_ptr->GetStructor();
    else
        return NULL;

}

bool ContactArray::Append(const HSAE_CONTACT_INFO *contact_content)
{
//	CONTACTLINK *array_temp,*new_ele;
//	new_ele = new CONTACTLINK;
//	new_ele->contact_array_ptr = new ContactElement(contact_content);
//	new_ele->next =NULL;
//	array_temp = array_head;
//	if(!array_temp){
//		array_head = new_ele;
//	}else{
//		while(array_temp->next){
//			array_temp = array_temp->next;
//		}
//		array_temp->next = new_ele;
//	}
//	count++;

    CONTACTLINK *array_temp,*array_prev;
    if (array_head ==NULL)
        array_prev =NULL;
    array_temp = new CONTACTLINK;

    array_temp->contact_array_ptr = new ContactElement(contact_content);
    array_temp->next = NULL;
    if(!array_prev){
        array_head = array_temp;
    }
    else{
        array_prev->next = array_temp;
    }
    array_prev = array_temp;
    count++;
    return true;
}

bool ContactArray::Remove()
{
    CONTACTLINK *array_temp;

    array_temp = array_head;
    if(array_head){
        if(!array_head->next){
            delete array_head->contact_array_ptr;
            delete array_head;
            array_head  = NULL;
        }else{
            while(array_temp->next->next){
                array_temp = array_temp->next;
            }
            delete array_temp->next->contact_array_ptr;
            delete array_temp->next;
            array_temp->next  = NULL;
        }
        count--;
    }

    return true;
}

bool ContactArray::RemoveAll()
{
    while(count){
        Remove();

    }
    return true;
}

unsigned int ContactArray::GetCount() const
{
    return count;
}

void ContactArray::DisplayArrayContents(){

    CONTACTLINK *array_temp;
    unsigned int temp_count=0;
    array_temp = array_head;
    while(array_temp){
        printf("HSAE_CONTACT_INFO Array Elements[%d]---------------------->\r\n",temp_count);
        array_temp->contact_array_ptr->DisplayEleContents();
        array_temp = array_temp->next;
        temp_count++;
    }
    return;
}










