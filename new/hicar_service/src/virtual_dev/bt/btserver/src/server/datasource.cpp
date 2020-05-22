#include "databasedefine.h"
#include "datasource.h"
//#include  <unistd.h>
#include <stdlib.h>
//#include "time.h"
//#define PHONE_DB_PATH "/usr/bin/bluetooth/phone.db"
using namespace std;

 datasource * datasource::sqlInstance = NULL;
 sqlite3* datasource::db= NULL;
 extern int callogsavemax;
 datasource *datasource::getInstance()
{
    if (sqlInstance == NULL)
    {
          sqlInstance = new datasource ();
    }
    return sqlInstance;
}

void datasource::initDB()
{
    int rc =1;
    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
     string path = PHONE_DB_PATH;
     dbPath = path;
     int iResult = 0;
     rc = sqlite3_open(dbPath.data(), &db);
     this->isDBExists = (rc>0?false:true);
     LOG(BT_LOG_SQL,"initDB   table %d = ,iResult=%d,db =%d ",isDBExists,iResult,(int)db);
}

void datasource::saveContacts(std::string addr, Contacts contactlist)
{


}

void datasource::insertContacts(std::string addr, NewContact contact)
{
   // LOG(BT_LOG_SQL,"start");
    string tablename = addrtoname(addr,0);
    if ( tablename.empty())
        return ;
    char sql[500];
    memset(sql,0,500);
    sprintf(sql,"\"%s\",\"%s\",%d,\"%s\",\"%s\"",contact.name.data(),contact.phoneNumber.data(),(int)contact.type,contact.py.data(),contact.fpy.data());
    string sqlcmd=INSERT_TABLE_CONTACT_BOOK(tablename,sql);
    int result = 0;
    result = ExcuteCommand(sqlcmd);
    if(0 == result )
    {


    }
    else
    {

    }
  //  LOG(BT_LOG_SQL,"end %d",result);
}

void datasource::savePairlist(std::string addr, std::string devicename)
{

}


void datasource::deleteContact(std::string addr,string name,string  num)
{
    string tablename = addrtoname(addr,0);
    if ( tablename.empty())
        return ;
    string sql ;
    sql = DELETE_TABLE_CONTACT_BOOK_NAMENUM (tablename,name,num);

    int result = 0;
    result = ExcuteCommand(sql);
#ifndef  USE_DOWNLOADCALLLOG
    updatecallogs(addr,"",num);
#endif
      system("sync");
}

void datasource::clearContacts(std::string addr)
{
    LOG(BT_LOG_SQL,"cmd=%s ",addr.data());
    string tablename = addrtoname(addr,0);
    if ( tablename.empty())
        return ;
    string sql = DELETE_TABLE_CONTACT_BOOK_ALL(tablename);
    int result = 0;
    result = ExcuteCommand(sql);
#ifndef  USE_DOWNLOADCALLLOG
    //update callog
    tablename = addrtoname(addr,1);
    sql = UPDATE_TABLE_CALL_RECORDALL(tablename,"");
    result = ExcuteCommand(sql);
#endif
     system("sync");
}


int datasource::dropAllTable()
{

}

bool datasource::contactTableExist(string qsTableName)
{

    bool ret =false;
    int result = 0;
    string sqlCmd;
    char **dbresult;
    char *zErrMsg = 0;
    int nrow = 0;
    int ncolumn= 0;
    sqlCmd = COUNT_CONTACTTABLE;// "SELECT * FROM " + tableName + ";";
    result = sqlite3_get_table(db,sqlCmd.data(),&dbresult,&nrow,&ncolumn,&zErrMsg);
    if(result != SQLITE_OK)
    {
        nrow = 0;
        printf("zErrMsg =%s",zErrMsg);
    }
    LOG(BT_LOG_SQL,"sqlCmd =%s result =%d,nrow =%d ,ncolumn =%d",sqlCmd.c_str(),result,nrow,ncolumn);
    int row = -1;
    for (int i=0;i<nrow;i++)
    {
        if ( dbresult[ncolumn*(i+1)+1] ==qsTableName)
        {
            ret =true;
            row = i;
            break;
        }
    }
   if (nrow>5)
   {
        string dropname;
        if ((ret)&&(row >0))
           dropname = dbresult[ncolumn+1] ;
        else
            dropname = dbresult[ncolumn+2] ;
         dropContactTable(dropname);
         sqlCmd = DROP_TABLE_CONTACT_BOOK(dropname);
         ExcuteCommand(sqlCmd);//
   }
//   sqlCmd = COUNT_TABLE;// "SELECT * FROM " + tableName + ";";
//   result = sqlite3_get_table(db,sqlCmd.data(),&dbresult,&nrow,&ncolumn,&zErrMsg);
//   sqlite3_free_table(dbresult);
    return ret;

}

bool datasource::calllogTableExist(string qsTableName)
{
    bool ret =false;
    int result = 0;
    string sqlCmd;
    char **dbresult;
    char *zErrMsg = 0;
    int nrow = 0;
    int ncolumn= 0;
    sqlCmd = COUNT_CALLOGTABLE;// "SELECT * FROM " + tableName + ";";
    result = sqlite3_get_table(db,sqlCmd.data(),&dbresult,&nrow,&ncolumn,&zErrMsg);
    if(result != SQLITE_OK)
    {
        nrow = 0;
        printf("zErrMsg =%s",zErrMsg);
    }
    LOG(BT_LOG_SQL,"sqlCmd =%s result =%d,nrow =%d ,ncolumn =%d",sqlCmd.c_str(),result,nrow,ncolumn);
    int row =-1;
    for (int i=0;i<nrow;i++)
    {
        if ( dbresult[ncolumn*(i+1)+1] ==qsTableName)
        {
            ret =true;
            row = i;
            break;
        }
    }
   if (nrow>5)
   {
      string dropname;
       if ((ret)&&(row >0))
          dropname = dbresult[ncolumn+1] ;
       else
           dropname = dbresult[ncolumn+2] ;
        dropContactTable(dropname);
        sqlCmd = DROP_TABLE_CONTACT_BOOK(dropname);
        ExcuteCommand(sqlCmd);//
   }
//   sqlCmd = COUNT_TABLE;// "SELECT * FROM " + tableName + ";";
//   result = sqlite3_get_table(db,sqlCmd.data(),&dbresult,&nrow,&ncolumn,&zErrMsg);
//   sqlite3_free_table(dbresult);
    return ret;
}

int datasource::createContactTable(string qsTableName)
{
    int ret =0;
    if (!contactTableExist(qsTableName))
    {

        int iResult = 0;
        string sqlCmd;
        sqlCmd =CREATE_TABLE_CONTACT_BOOK(qsTableName);
        LOG(BT_LOG_SQL,"sqlCmd1 =%s ",sqlCmd.data());
        iResult = ExcuteCommand(sqlCmd);//
        if(iResult != 0)
        {
            return iResult;
        }
        ret =1;
    }
    return ret;
}

int datasource::dropContactTable(string qsTableName)
{
     string tablename = addrtoname(qsTableName,0);
     if ( tablename.empty())
         return -1;
     int iResult = 0;
     string sqlCmd;
      sqlCmd =DROP_TABLE_CONTACT_BOOK(tablename);
      LOG(BT_LOG_SQL,"sqlCmd1 =%s ",sqlCmd.data());
     iResult = ExcuteCommand(sqlCmd);//
     if(iResult != 0)
     {
         return iResult;
     }
     system("sync");
}

int datasource::dropAllContactTable()
{

}

void datasource::searchcontacts(string addr, string filter, Contacts &contactlist, int kind)
{
    string tablename =addrtoname(addr,0);
     if ( tablename.empty())
        return ;
    string sql;
//        cout<<"------kind-------"<<kind;
    switch (kind) {
    case 0:
        sql = SELECT_TABLE_CONTACT_BOOK(tablename);
        break;
    case 1:
        sql = SELECT_TABLE_CONTACT_BOOK_FILTER_FPY(tablename,filter);
        break;

    case 2:
         sql = SELECT_TABLE_CONTACT_BOOK_FILTER_NUMBER (tablename,filter);
         break;
    case 3:
         sql = SELECT_TABLE_CONTACT_BOOK_FILTER_PINYIN (tablename,filter);
         break;
    default:
        break;
    }
   querycontacts(sql,contactlist);
}




string datasource::querynamebynum(string addr, string number)
{
//    string tablename =addrtoname(addr,0);
//    string sql;
//    sql = SELECT_TABLE_CONTACT_BOOK_FILTER_NAME_NUMBER(tablename,number);
//    char* errmsg;
//    char **dbResult; //是 char ** 类型，两个*号
//    int nRow, nColumn;
    string name ="";
//    sqlite3_get_table(db, sql.data(), &dbResult, &nRow, &nColumn, &errmsg);
//    if (nRow>0)
//    {
//        name = std::string(dbResult[nColumn]);
//        cout<<"datasource::querynamebynum"<<name<<endl;
//    }
//     sqlite3_free_table(dbResult);

    if (contactlists.size()>0)
     name = contactlists[number];
     LOG(BT_LOG_SQL,"number =%s name =%s",number.data(),name.data());
     return name;
}

void datasource::addcalllog(std::string addr, NewContact contact)
{
    LOG(BT_LOG_SQL,"start");
    string tablename =addrtoname(addr,1);
    if ( tablename.empty())
        return;
    string sqlcmd;
     int result ;
    int calllognum = getdatacount(tablename);
    cout<<"datasource::addcalllog"<<callogsavemax<<endl;
    if (calllognum > callogsavemax)
    {
        // delete the first one

        sqlcmd = DELETE_TABLE_CALL_RECORD_EARLST(tablename);
        result = ExcuteCommand(sqlcmd);
    }

    char sql[500];
    memset(sql,0,500);
    sprintf(sql,"\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",contact.name.data(),contact.phoneNumber.data(),\
            contact.call_info.dir.data(),contact.call_info.date.data(),contact.call_info.time.data());
    sqlcmd = INSERT_TABLE_CALL_RECORD(tablename,sql);
     result = ExcuteCommand(sqlcmd);
    if(0 == result )
    {


    }
    else
    {

    }
  LOG(BT_LOG_SQL,"end %d",result);
}

int datasource::createCallRecordTable(string qsTableName)
{
    int ret =0;
    if (!calllogTableExist(qsTableName))
    {

        int iResult = 0;
        string sqlCmd;
        sqlCmd =CREATE_TABLE_CALL_RECORD(qsTableName);
        LOG(BT_LOG_SQL,"sqlCmd1 =%s ",sqlCmd.data());
        iResult = ExcuteCommand(sqlCmd);//
        if(iResult != 0)
        {
            return iResult;
        }
        ret =1;
    }
    return ret;
}

int datasource::dropCallRecordTable(string qsTableName)
{
    string tablename =addrtoname(qsTableName,1);
    if ( tablename.empty())
        return -1;
    int iResult = 0;
    string sqlCmd;
     sqlCmd =DROP_TABLE_CALL_RECORD(tablename);
     LOG(BT_LOG_SQL,"sqlCmd1 =%s ",sqlCmd.data());
    iResult = ExcuteCommand(sqlCmd);//
    if(iResult != 0)
    {
        return iResult;
    }
}

int datasource::dropAllCallRecordTable()
{

}

int datasource::deleteCallRecord(std::string addr, string datetime)
{
     // LOG(BT_LOG_SQL,"sqlCmd1 =%s ",datetime.data());
#ifndef  USE_DOWNLOADCALLLOG
       string tablename = addrtoname(addr,1);
       if ( tablename.empty())
           return -1;
       string sql ;
       sql =DELETE_TABLE_CALL_RECORD_ID(tablename,datetime);
       int result = 0;
       result = ExcuteCommand(sql);
#endif

}

int datasource::clearCallRecords(std::string addr)
{
#ifndef  USE_DOWNLOADCALLLOG
    string tablename =addrtoname(addr,1);
     if ( tablename.empty())
        return -1;
    string sql = DELETE_TABLE_CALL_RECORD_ALL(tablename);
    int result = 0;
    result = ExcuteCommand(sql);
#endif
}

void datasource::getcallogs(std::string addr,Contacts &contactlist)
{
    string tablename =addrtoname(addr,1);
     if ( tablename.empty())
        return;
    string sql = SELECT_TABLE_CALL_RECORD(tablename);

    char* errmsg;
    char **dbResult; //是 char ** 类型，两个*号
    int nRow, nColumn;
     LOG(BT_LOG_SQL," 1 =%s ",sql.data());
     sqlite3_get_table(db, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
     LOG(BT_LOG_SQL," 3 =%s row =%d nColumn =%d",sql.data(),nRow,nColumn);
    for(int i=0;i<nRow;i++)
    {
        NewContact m_contact;
        m_contact.name = std::string(dbResult[nColumn*(i+1)+1]);
        m_contact.phoneNumber = std::string(dbResult[nColumn*(i+1)+2]);
        m_contact.call_info.dir = std::string(dbResult[nColumn*(i+1)+3]);
        m_contact.call_info.date = std::string(dbResult[nColumn*(i+1)+4]);
        m_contact.call_info.time = std::string(dbResult[nColumn*(i+1)+5]);
        contactlist.push_back(m_contact);

    }

     sqlite3_free_table(dbResult);
}
// updatecallog
void datasource::updatecallogs(std::string addr,string name,string num )
{
   string tablename =addrtoname(addr,1);
    if ( tablename.empty())
       return;
   string sql ;//
   sql = UPDATE_TABLE_CALL_RECORD(tablename,name,num);
   int result = 0;
   result = ExcuteCommand(sql);
   LOG(BT_LOG_SQL,"result =%d  ",result );
}

void datasource::clearalltables()
{
    //
    if ( isDBExists)
      sqlite3_close(db); //关闭数据库
    //delete db
    int ret = remove(PHONE_DB_PATH);
    LOG(BT_LOG_SQL,"result =%d  ",ret );
    initDB();
}

void datasource::getcontacts(string addr, Contacts &contactlist)
{
    string tablename =addrtoname(addr,0);
    if ( tablename.empty())
        return;
    string sql = SELECT_TABLE_CONTACT_BOOK(tablename);
    char* errmsg;
    char **dbResult; //是 char ** 类型，两个*号
    int nRow, nColumn;
    LOG(BT_LOG_SQL," 1 =%s ",sql.data());
    sqlite3_get_table(db, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
    LOG(BT_LOG_SQL," 3 =%s row =%d nColumn =%d",sql.data(),nRow,nColumn);
    map<string,string> updatecontactlist;
    for(int i=0;i<nRow;i++)
    {
        NewContact m_contact;
        m_contact.name = std::string(dbResult[nColumn*(i+1)+1]);
        m_contact.phoneNumber = std::string(dbResult[nColumn*(i+1)+2]);
        m_contact.type = (PhoneType)(atoi(dbResult[nColumn*(i+1)+3]));
        m_contact.py = std::string(dbResult[nColumn*(i+1)+4]);
        m_contact.fpy= std::string(dbResult[nColumn*(i+1)+5]);
        contactlist.push_back(m_contact);
        updatecontactlist[m_contact.phoneNumber] = m_contact.name;//.insert(m_contact.phoneNumber,m_contact.name);
    }
    sqlite3_free_table(dbResult);
 #ifndef  USE_DOWNLOADCALLLOG
    if (contactlist.size()==0)
    {
        //updatecallog
        tablename =addrtoname(addr,1);
        sql = UPDATE_TABLE_CALL_RECORDALL(tablename,"");
        int  result = ExcuteCommand(sql);
    }
    else
    {
        //update calllog
        tablename =addrtoname(addr,1);
        sql = SELECT_TABLE_CALL_RECORDNUM(tablename);
        sqlite3_get_table(db, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
        LOG(BT_LOG_SQL," 5 =%s row =%d nColumn =%d",sql.data(),nRow,nColumn);
        for(int j=0;j<nRow;j++)
        {
           string number = std::string(dbResult[nColumn*(j+1)]);
           string name  = updatecontactlist[number];
           updatecallogs(addr,name,number);
        }
        sqlite3_free_table(dbResult);
    }
#endif
    contactlists.swap(updatecontactlist);
}

void datasource::clearmapcontacts()
{
   if (contactlists.size() > 0)
    contactlists.clear();
}

int datasource::ExcuteCommand(string &cmd, datasource::DataBaseCallback callback, void *data)
{
     char *zErrMsg = 0;
     int result;
      result = sqlite3_exec(db, cmd.c_str(), callback, data, &zErrMsg);
      LOG(BT_LOG_SQL,"result =%d,cmd=%s ",result,cmd.data());
      if( result != SQLITE_OK )
      {

          fprintf( stderr , " SQL error : %s\n " , zErrMsg);
          sqlite3_free(zErrMsg);
      }
//      LOG(BT_LOG_SQL,"  =%d ",result);
      return result;
}

string datasource::addrtoname(string soucename,int kind)
{
    string str ="";// "table_CB_";
    if (soucename.length()==17)
   {

        if (kind ==0)
       {
            str = "table_CB_"+soucename.substr(0,2)+soucename.substr(3,2)\
                 +soucename.substr(6,2)+soucename.substr(9,2)\
                 +soucename.substr(12,2)+soucename.substr(15,2);
       }
       else

       {     str ="table_CR_"+soucename.substr(0,2)+soucename.substr(3,2)\
                    +soucename.substr(6,2)+soucename.substr(9,2)\
                    +soucename.substr(12,2)+soucename.substr(15,2);
       }
    }
   return str;
}

void datasource::querycontacts(string sql, Contacts &contactlist)
{

    char* errmsg;
    char **dbResult; //是 char ** 类型，两个*号
    int nRow, nColumn;
  //  LOG(BT_LOG_SQL," 1 =%s ",sql.data());
    sqlite3_get_table(db, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
    LOG(BT_LOG_SQL,"  =%s row =%d nColumn =%d",sql.data(),nRow,nColumn);
    for(int i=0;i<nRow;i++)
    {
        NewContact m_contact;
        m_contact.name = std::string(dbResult[nColumn*(i+1)+1]);
        m_contact.phoneNumber = std::string(dbResult[nColumn*(i+1)+2]);
        m_contact.type = (PhoneType)(atoi(dbResult[nColumn*(i+1)+3]));
        m_contact.py = std::string(dbResult[nColumn*(i+1)+4]);
        m_contact.fpy= std::string(dbResult[nColumn*(i+1)+5]);
        contactlist.push_back(m_contact);
    }
    sqlite3_free_table(dbResult);
}

int datasource::getdatacount(string tablename)
{
    char* errmsg;
    char **dbResult; //是 char ** 类型，两个*号
    int nRow, nColumn;
    int count =0;
    string sql =COUNT_DATA(tablename) ;
    LOG(BT_LOG_SQL," 1 =%s ",sql.data());
    sqlite3_get_table(db, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
    LOG(BT_LOG_SQL," 3 =%s row =%d nColumn =%d mcount =%s",sql.data(),nRow,nColumn,dbResult[1]);
    // dbResult[0]
    count = atoi(dbResult[1]);
    sqlite3_free_table(dbResult);
    return count;
}

int datasource::sqlCmdResultCallBack(void *data, int argc, char **argv, char **azColName)
{
     int i;

      if(data != NULL)
      {

           LOG(BT_LOG_SQL,"  =%s ",(const char*)data);
      }


      for(i=0; i<argc; i++)
      {
        //  DEBUG("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
           LOG(BT_LOG_SQL,"  %s = %s ",azColName[i], argv[i] ? argv[i] : "NULL");
      }

      return 0;
}

datasource::datasource()
{
    isDBExists =false;
    dbPath="";
    //initDB(PHONE_DB_PATH);
     initDB();
//     createContactTable("43:39:00:07:00:00");
}

datasource::~datasource()
{
    cout <<"datasource::~datasource()"<<endl;
    if ( isDBExists)
      sqlite3_close(db); //关闭数据库
}
