#ifndef _PHONEBOOKSTUBIMP_H_
#define _PHONEBOOKSTUBIMP_H_
#include "PhoneBook/phonebook.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string.h>
#include <stdio.h>
#include "ContactArray.h"
#include "HsaeTypes.h"
//YANG_HU_DEV
#include "DeviceStubImp.h"
#include "datasource.h"
#include <pthread.h>
#include <signal.h>
using namespace std;
using namespace com::hsae::bluetooth::util::HsaeTypes;
//struct HSAE_CONTACT_INFO
//{
////     HSAE_CONTACT_INFO contact[];
//     HSAE_CONTACT_INFO *contact;
//     size_t contact_len;
//     BT_ADDR ba;
//};
class PhoneBookStubImp{//: public PhoneBookStubDefault {
public:
    PhoneBookStubImp();
    ~PhoneBookStubImp();
    static std::shared_ptr<PhoneBookStubImp> phoneBookInstance;
    static std::shared_ptr<PhoneBookStubImp> getInstance();
    static PhoneBook* m_interface;
    void setpbdata(const Contacts contactlist);
    string getnamebynum(string addr ,string num);
    void initpbdata( );
    bool updatepbdata(string addr);
    void chartostr(char *strmac, const unsigned char *mac);
    datasource * pdata;
    bool createContactTable(string tablename);
//method
    static gboolean downLoadPhoneBook(PhoneBook *interface,GDBusMethodInvocation *invocation,gint location);
    static gboolean downLoadCallLog(PhoneBook *interface,GDBusMethodInvocation *invocation,gint type);
    static gboolean CancelDownLoadPhoneBook(PhoneBook *interface,GDBusMethodInvocation *invocation);

    static gboolean clearcontacts(PhoneBook *interface,GDBusMethodInvocation *invocation, const gchar *arg_addr);
    static gboolean deletecontact(PhoneBook *interface,GDBusMethodInvocation *invocation,const gchar *arg_addr, const gchar *arg_name, const gchar *arg_num);
    static gboolean searchcontact(PhoneBook *interface,GDBusMethodInvocation *invocation,const gchar *arg_addr,const gchar *arg_sql, gint arg_type);
//method
    static gboolean syncdata(PhoneBook *interface,GDBusMethodInvocation *invocation, const gchar *arg_addr,gint arg_type);

   // static gboolean deletecalllog(PhoneBook *interface,GDBusMethodInvocation *invocation,gint type);
    static gboolean savecontacts(PhoneBook *interface,GDBusMethodInvocation *invocation,const gchar *arg_addr);
    static gboolean clearcalllogs(PhoneBook *interface,GDBusMethodInvocation *invocation, const gchar *arg_addr);
    static gboolean deletecalllog(PhoneBook *interface,GDBusMethodInvocation *invocation,const gchar *arg_addr, const gchar *arg_datatime);
    static gboolean getcontacts(PhoneBook *interface,GDBusMethodInvocation *invocation,const gchar *arg_addr);

    //signal
    void PhoneBookDownLoadStateChanged(const PhoneBookDownLoadState pbapDownLoadState);
    void BtPdownLoadPhoneBookChanged(const Contacts contactlist);
#ifndef USE_DOWNLOADCALLLOG
    void BtDownLoadCallLogChanged(const Contacts contactlist);
#else
      void BtDownLoadCallLogChanged(const Calllogs contactlist);
#endif
    static void on_bus_acquired (GDBusConnection *connection,const gchar *name,gpointer user_data);
    /**
     * ---------------------------------------------innner logic------------------------------------------
     *
     */
    void Btnoticepadata(int kind);//0 contacts 1callog 2 clear all
    void BtCallLogChanged(string name,string number,string kind,string datatime,string talktime);
    void addcalllog(std::string addr,NewContact contact);
    void addcontact(std::string addr,string name,string number,int kind);
    static int invokeDownPbap;
    bool needdownloadcalllog;

    int clearsqltables();
    bool m_downloading;
    bool m_callogdownloading;
//    bool downloaded;
    bool m_bsaved;
    bool m_bnoticeend;
    bool m_cancleddowncallog;
    static void* dealdataFunc(void* args);
    static pthread_mutex_t mutex_for_pbdata;
    static pthread_cond_t pbcond;
    static HSAE_CONTACT_INFO pbdata;
//    static pthread_t pbsavedata_id;
    static pthread_t pbsavedata_id;
    ContactArray *contacts_ptr;
    int strtype(string str);
    void downloadcalllogs(int count);
     bool downloadcalled;
     void deletecontacttable(string address);
     void cancledownload();
private:
    Contacts pbdatalist;


private:
    void getFirstLettersFromChinese(std::string str, std::string& py,std::string& firstpy);
    int getUcs2PinYinInfo(unsigned short ucsCode, char *pyInfo );
    bool utf8toucs2(const unsigned char* utf8_code, unsigned short* ucs2_code,int *len)  ;
    void trimspace(char *pSrc);
    string pbapmac;
    string addrtoname(string soucename,int kind);
    string handleSpecialCharacter(string soucename);

};
//}
//}
//}
//}

#endif

