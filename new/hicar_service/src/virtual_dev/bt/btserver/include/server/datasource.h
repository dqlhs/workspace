#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <sqlite3.h>
#include <sstream>
#include "HsaeTypes.h"
#include <map>
using namespace std;
using namespace com::hsae::bluetooth::util::HsaeTypes;

class datasource
{
public:
    static  datasource * sqlInstance;
    datasource();
     ~datasource();
    static datasource *  getInstance();
    void initDB();
    void saveContacts(std::string addr, Contacts contactlist); //save all conntacts
    void insertContacts(std::string addr,NewContact contact); //insert new contact
    void savePairlist(std::string addr,std::string devicename);
    void deleteContact(std::string addr,string name,string  num);
    void clearContacts(std::string addr);
    int  dropContactTable(string qsTableName); //delete all contacts
    int  dropAllTable(void);                   //delete all contacts table
    bool contactTableExist(string qsTableName); //
    bool calllogTableExist(string qsTableName); //
    int createContactTable(string qsTableName);
    int dropAllContactTable(void);
    void searchcontacts(string addr,string filter, Contacts &contactlist,int kind); //kind 0  all ;1  query by number;2  by fpy or number
    string  querynamebynum(string addr,string number); //query name by  number
    void addcalllog(std::string addr,NewContact contact);
    int createCallRecordTable(string qsTableName);
    int dropCallRecordTable(string qsTableName);
    int dropAllCallRecordTable(void);
    int deleteCallRecord(std::string addr,string datetime);//
    int clearCallRecords(std::string addr );//
    void getcallogs(std::string addr ,Contacts &contactlist);
    void updatecallogs(std::string addr,string name,string num );
    void clearalltables();
    void getcontacts(string addr, Contacts &contactlist);
    void clearmapcontacts();

 private:
     static int sqlCmdResultCallBack(void *data, int argc, char **argv, char **azColName);
     bool isDBExists;
     static sqlite3* db;
     std::string dbPath;
     typedef  int (*DataBaseCallback)(void *data, int argc, char **argv, char **azColName);
     int ExcuteCommand(string & cmd,DataBaseCallback callback = sqlCmdResultCallBack,void *data = NULL);
     stringstream mTables;
     string addrtoname(string soucename,int kind);
     void querycontacts(string sql,Contacts &contactlist);
     int getdatacount(string tablename);
      map<string,string> contactlists;

};

#endif // DATASOURCE_H
