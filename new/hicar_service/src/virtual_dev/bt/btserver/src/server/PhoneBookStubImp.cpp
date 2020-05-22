#include "PhoneBookStubImp.h"
#include <jsoncpp/json/json.h>
#define CONTACTLIST "ContactList"
#define NAME "Name"
#define PHONE_COUNT "PhoneCount"
#define TELPHONE "Phone"
#define PHONENUMBER "PhoneNumber"
#define PHONETYPE "PhoneType"
#define CALLDATE "CallDate"
#define CALLTIME "CallTime"
#define DIRECTION "Direction"
#define MAXPBCOUNT 2000
#define MAXRECORDCOUNT 30
#define FPY "PY"
#include"PinYin_Define.h"
#include"PinYin_StaticArray.h"
using namespace std;
extern int PhoneBookStubImp::invokeDownPbap;
std::shared_ptr< PhoneBookStubImp > PhoneBookStubImp::phoneBookInstance = NULL;
extern pthread_t PhoneBookStubImp::pbsavedata_id;
extern HSAE_CONTACT_INFO PhoneBookStubImp::pbdata;
PhoneBook* PhoneBookStubImp::m_interface = NULL;
extern pthread_mutex_t PhoneBookStubImp::mutex_for_pbdata;
extern pthread_cond_t PhoneBookStubImp::pbcond;

//const char* get_phone_type_str( HSAE_PHONE_TYPE type )
//{
//    switch( type )
//    {
//    case HSAE_PHONE_PREF:
//        return "PREF:";
//    case HSAE_PHONE_WORK:
//        return "WORK:";
//    case HSAE_PHONE_HOME:
//        return "HOME:";
//    case HSAE_PHONE_VOICE:
//        return "VOICE:";
//    case HSAE_PHONE_FAX:
//        return "FAX:";
//    case HSAE_PHONE_MSG:
//        return "MSG:";
//    case HSAE_PHONE_CELL:
//        return "CELL:";
//    case HSAE_PHONE_PAGER:
//        return "PAGER:";
//    case HSAE_PHONE_BBS:
//        return "BBS:";
//    case HSAE_PHONE_MODEM:
//        return "MODEM:";
//    case HSAE_PHONE_CAR:
//        return "CAR:";
//    case HSAE_PHONE_ISDN:
//        return "ISDN:";
//    case HSAE_PHONE_VIDEO:
//        return "VIDEO:";
//    default:
//        return "";
//    }
//}

HSAE_CBK_RET pbap_dl_contact( void *HSAECbkParam, const BT_ADDR ba,
                             const HSAE_CONTACT_INFO contact[],
                             const size_t contact_len,
                             const HSAE_PHONEBOOK_DATA data,
                             HSAE_CMD_RET cmd_ret )
{

    std::shared_ptr< PhoneBookStubImp > m_phoneBook = PhoneBookStubImp::getInstance();
    LOG(BT_LOG_PB,"cmd_ret =%d,contact_len =%d",(int)cmd_ret,(int)contact_len);
    if (contact_len > 0)
    {
        char btAddress[20]={0};// = (char*)malloc( 50 );
        m_phoneBook->chartostr(btAddress,ba.b);
       if (m_phoneBook->invokeDownPbap ==1)
       {
            m_phoneBook->m_bnoticeend =false;
            m_phoneBook->pdata->clearContacts(string(btAddress));
            m_phoneBook->m_bsaved = false;
       }

    }
    pthread_mutex_lock(&(PhoneBookStubImp::mutex_for_pbdata));
    m_phoneBook->contacts_ptr =new ContactArray(contact, contact_len);
    pthread_cond_signal(&(PhoneBookStubImp::pbcond));
    pthread_mutex_unlock(&(PhoneBookStubImp::mutex_for_pbdata));
    return HSAE_CBK_SUCCESS;
}


HSAE_CBK_RET print_dl_status( void *HSAECbkParam, const BT_ADDR ba,
                             const HSAE_DOWNLOAD_STATE state, const char *type )
{
    std::shared_ptr< PhoneBookStubImp > m_phoneBook = PhoneBookStubImp::getInstance();
    printf( "The %s download status of %02X:%02X:%02X:%02X:%02X:%02X is now ",
           type, ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );
    LOG(BT_LOG_PB,"state =%d",(int)state)
    switch( state )
    {
    case HSAE_LS_END:
//        printf( "not downloading.\n" );
       if (PhoneBookStubImp::invokeDownPbap ==1 )
       {
            if (m_phoneBook->m_bnoticeend)
            {

                m_phoneBook->PhoneBookDownLoadStateChanged(PhoneBookDownLoadState::DOWNLOADED );
                PhoneBookStubImp::invokeDownPbap =-1;
            }
        }
       m_phoneBook->m_downloading = false;
        break;
    case HSAE_LS_START:
//        printf( "start downloading.\n" );
//        m_phoneBook->downloaded = false;
        m_phoneBook-> m_downloading = true;
        m_phoneBook->m_bnoticeend =true;
        if (PhoneBookStubImp::invokeDownPbap ==1 )
        {
           m_phoneBook->PhoneBookDownLoadStateChanged( PhoneBookDownLoadState::START_DOWNLOADING );

        }

        break;
    case HSAE_DLS_ING:
//        printf( "downloading.\n" );
       m_phoneBook-> m_downloading = true;
       if (PhoneBookStubImp::invokeDownPbap ==1 )
       {
         m_phoneBook->PhoneBookDownLoadStateChanged(PhoneBookDownLoadState::DOWNLOADING );
       }
        break;
  case HSAE_LS_STOP:
        if (PhoneBookStubImp::invokeDownPbap ==1 )
        {
               m_phoneBook->PhoneBookDownLoadStateChanged( PhoneBookDownLoadState::DOWNFAILD );
        }

        m_phoneBook-> m_bnoticeend = true;
        break;
    default:
//        printf( "in unknown state %d.\n", state );
        break;
    }

    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET pbap_dl_status( void *HSAECbkParam, const BT_ADDR ba,
                            const HSAE_DOWNLOAD_STATE state )
{
    return print_dl_status( HSAECbkParam, ba, state, "PBAP" );
}

PhoneBookStubImp::PhoneBookStubImp()
{
    hsaeCmdSetCbk_PhonebookDownloadStatus( NULL, pbap_dl_status );
    hsaeCmdSetCbk_DownloadContact( NULL, pbap_dl_contact );
    m_bsaved = true;
    m_bnoticeend =true;
    downloadcalled =false;
    m_downloading =false;
    pbsavedata_id = 0;
    invokeDownPbap = -1;
    needdownloadcalllog =false;
    m_cancleddowncallog =false;
    mutex_for_pbdata =PTHREAD_MUTEX_INITIALIZER;
    pbcond = PTHREAD_COND_INITIALIZER;
    pthread_create(&pbsavedata_id, NULL, dealdataFunc, (void *)this);
}

PhoneBookStubImp::~PhoneBookStubImp()
{
  // pthread_mutex_destory(&mutex_for_pbdata);
}
std::shared_ptr< PhoneBookStubImp > PhoneBookStubImp::getInstance()
{
//#if 0
    if( phoneBookInstance == NULL )
    {
        phoneBookInstance = std::make_shared< PhoneBookStubImp >();
    }
//#else
//    static std::shared_ptr< PhoneBookStubImp > phoneBookInstance = std::make_shared< PhoneBookStubImp >();
//#endif


    return phoneBookInstance;
}

void PhoneBookStubImp::on_bus_acquired( GDBusConnection *connection,
                                        const gchar *name, gpointer user_data )
{
    GError **error;

    //创建一个skeleton对象,以注册输出对象
    PhoneBookStubImp::m_interface = phone_book_skeleton_new();

    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-down-load-phone-book", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(downLoadPhoneBook),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-down-load-call-log", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(downLoadCallLog),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-cancel-down-load-phone-book", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(CancelDownLoadPhoneBook),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-searchcontact", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(searchcontact),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-clearcontacts", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(clearcontacts),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-deletecontact", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(deletecontact),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    // deletecallog
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-deletecalllog", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(deletecalllog),//具体的方法处理函数, 注意增加输入参数
                    user_data );
   // clearcallog
    g_signal_connect( PhoneBookStubImp::m_interface,
                      "handle-clearcalllogs", //signal_name, 在.c文件中的_default_init查找
                    G_CALLBACK(clearcalllogs),//具体的方法处理函数, 注意增加输入参数
                    user_data );
    // savecontact
     g_signal_connect( PhoneBookStubImp::m_interface,
                       "handle-savecontacts", //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(savecontacts),//具体的方法处理函数, 注意增加输入参数
                     user_data );

     // getcontacts
      g_signal_connect( PhoneBookStubImp::m_interface,
                        "handle-getcontacts", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(getcontacts),//具体的方法处理函数, 注意增加输入参数
                      user_data );
      // syncdata downloadcallog contacts
       g_signal_connect( PhoneBookStubImp::m_interface,
                         "handle-syncdata", //signal_name, 在.c文件中的_default_init查找
                       G_CALLBACK(syncdata),//具体的方法处理函数, 注意增加输入参数
                       user_data );
    //输出interface到总线上
    if( !g_dbus_interface_skeleton_export(
                    G_DBUS_INTERFACE_SKELETON( PhoneBookStubImp::m_interface ),
                    connection, "/com/hsaeyz/bluetooth/phoneBook", //object_path
                    error ) )
    {
        /* handle error */
    }

}
gboolean PhoneBookStubImp::downLoadPhoneBook( PhoneBook *interface,
                                              GDBusMethodInvocation *invocation,
                                              gint location )
{
    std::shared_ptr< PhoneBookStubImp > m_phoneBook = PhoneBookStubImp::getInstance();
    LOG(BT_LOG_PB,"m_downloading =%d " ,m_phoneBook->m_downloading );
//    pthread_create(&(phoneBookInstance->pbsavedata_id), NULL,PhoneBookStubImp::dealdataFunc,NULL);
//    cout<<" PhoneBookStubImp pbsavedata_id"<<(int)(phoneBookInstance->pbsavedata_id)<<endl;
 #if 0
    if (!m_phoneBook->m_downloading)
    {
        PhoneBookStubImp::invokeDownPbap = 1;
        HSAE_PHONEBOOK_DATA data;
        data.max_count = MAXPBCOUNT;
        hsaeCmdDownloadNoPhotoContactsStart( NULL, &DeviceStubImp::hfp_connect_addr, data );
    }
 #endif
    phone_book_complete_down_load_phone_book( interface, invocation );

    return TRUE;
}
gboolean PhoneBookStubImp::downLoadCallLog( PhoneBook *interface,
                                            GDBusMethodInvocation *invocation,
                                            gint type )
{
    LOG(BT_LOG_PB,"");
    PhoneBookStubImp::invokeDownPbap = 0;
    HSAE_PHONEBOOK_DATA data;
    data.max_count = MAXRECORDCOUNT;
    data.location = HSAE_PHONEBOOK_LOCATION::HSAE_PB_LOC_PHONE;
    data.type = HSAE_PHONEBOOK_TYPE::HSAE_PB_TYPE_COMBINED;
#if 1
    hsaeCmdDownloadNoPhotoPhonebookStart( NULL, &DeviceStubImp::hfp_connect_addr,data );
#endif
    phone_book_complete_down_load_call_log( interface, invocation );
    return TRUE;
}

gboolean PhoneBookStubImp::CancelDownLoadPhoneBook(
                PhoneBook *interface, GDBusMethodInvocation *invocation )
{

    phone_book_complete_cancel_down_load_phone_book( interface, invocation );
    return TRUE;
    //end
}

void PhoneBookStubImp::PhoneBookDownLoadStateChanged(
                const PhoneBookDownLoadState pbapDownLoadState )
{
//  fireBtPbapDownLoadStateEvent(pbapDownLoadState);
    if( PhoneBookStubImp::m_interface != NULL )
    {
        phone_book_emit_pbap_down_load_state( PhoneBookStubImp::m_interface,
                                              (gint)pbapDownLoadState );

    }

}
void PhoneBookStubImp::Btnoticepadata(int kind)
{
    LOG(BT_LOG_PB,"kind =%d ",kind);
    if (kind ==0)
    {
        BT_ADDR ba;
        memcpy(ba.b, DeviceStubImp::hfp_connect_addr.b, 6);
        char btAddress[20]={0};// = (char*)malloc( 50 );

        PhoneBookStubImp::getInstance()->chartostr(btAddress,ba.b);
        m_bsaved = false;

#ifndef  USE_DOWNLOADCALLLOG
        updatepbdata(string(btAddress));
#else
        Contacts contactlist;
        pdata->getcontacts(string(btAddress),contactlist);
        BtPdownLoadPhoneBookChanged(contactlist);
#endif
        PhoneBookDownLoadStateChanged(PhoneBookDownLoadState::DOWNLOADED );
        PhoneBookStubImp::invokeDownPbap =-1;
        m_bnoticeend = true;
      //   cout<<"--------------------needdownloadcalllog------"<<(int)needdownloadcalllog<<endl;
        if (needdownloadcalllog)
        {
            std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();

            needdownloadcalllog =false;
             cout<<"--------------------currentStates.phoneNumber------"<<(int)g_device->currentStates.phoneNumber.empty()<<"downloadcalled"<<(int)downloadcalled<<endl;
            if (downloadcalled)
            {

                if ( g_device->currentStates.phoneNumber.empty())
                {
                    downloadcalllogs(1);
                }
            }
            else
            {
                  downloadcalllogs(30);
            }

        }
        //
    }
    else if (kind ==1)
    {
#ifdef USE_DOWNLOADCALLLOG
//     if ( downloadcalled)
//        return;
//      BtDownLoadCallLogChanged(pbdatalist);
#endif

    }
}
void PhoneBookStubImp::addcalllog(std::string addr,NewContact contact)
{
     LOG(BT_LOG_PB,"");
     BtCallLogChanged(contact.name,contact.phoneNumber,contact.call_info.dir,\
                      contact.call_info.date,contact.call_info.time);
     pdata->addcalllog(addr,contact);
}

void PhoneBookStubImp::addcontact(std::string addr,string name, string number, int kind)
{
//    LOG(BT_LOG_PB,"");


//     BT_ADDR ba;
//     memcpy(ba.b, DeviceStubImp::hfp_connect_addr.b, 6);

//    char btAddress[20]={0};// = (char*)malloc( 50 );
//    sprintf( btAddress, "%02X:%02X:%02X:%02X:%02X:%02X", ba.b[0], ba.b[1], ba.b[2],
//             ba.b[3], ba.b[4], ba.b[5] );


    NewContact contact;
     string tempname ="";
    if (name.length()>0)
       tempname = handleSpecialCharacter(name);
    contact.name = tempname;
    contact.type = (PhoneType)kind;
    contact.phoneNumber = number;
    // get fpy
    string fpyletterts ;
    string fullletters ;
    getFirstLettersFromChinese(name,fullletters,fpyletterts);
    contact.fpy = fpyletterts;
    contact.py =fullletters;
    pdata->insertContacts(addr,contact);
//    contactlist.insert(number,name);
}

int PhoneBookStubImp::clearsqltables()
{
    pdata =datasource::getInstance();
    pdata->clearalltables();
}
void PhoneBookStubImp::getFirstLettersFromChinese(std::string str, std::string& py,std::string &firstpy)
{
    char inbuf[1024]={0};
    unsigned short outbuf[1024]={0};
    int outlen=0;
    char first_letter[30]={0};
    string full_letter ="";
    strcpy(inbuf,str.c_str());
    char cTune;
    bool ret= utf8toucs2((unsigned char*)inbuf,outbuf,&outlen);
//    if (ret ==false)
//    {
//         firstpy="{";
//         py =firstpy;
//         printf("is not unicoude\r\n");
//    }
//    else
//    {
      int nLetterCount = outlen ;
      for(int i=0;i<nLetterCount;i++)
      {
          if(i > 29)
              break;
          char fullword[12];
          memset(fullword, 0, sizeof(fullword));
          unsigned short nCurCode = outbuf[i];

          if(nCurCode < 0x80)
          {
              if(nCurCode >= 'a' && nCurCode <= 'z')
              {
                  *(first_letter + i) = (char)nCurCode;
                  *fullword= (char)nCurCode;
              }
              else if(nCurCode >= 'A' && nCurCode <= 'Z')
              {
                  *(first_letter + i) = (char)(nCurCode + 'a' - 'A');
                  *fullword= (char)(nCurCode + 'a' - 'A');
              }
              else if(nCurCode >= '0' && nCurCode <= '9')
              {
                  *(first_letter + i) = char(nCurCode);
                  if (0 == i)
                    *fullword = NONE_LETTER;
                   else
                    *fullword = char(nCurCode);
              }
              else if(nCurCode == ' ')
              {
                  *(first_letter + i) = ' ';
              }
              else
              {
                  *(first_letter + i) = NONE_LETTER;
                  *fullword = NONE_LETTER;
              }
          }
          else
          {
              if (nCurCode >= UCS2_CHARACTER_START && nCurCode <= UCS2_CHARACTER_END)
              {
                  char pyInfo[12];
                  memset(pyInfo, 0, sizeof(pyInfo));
                  cTune = CHARACTER_TONE_NO;
                  if (-1 != getUcs2PinYinInfo(nCurCode, pyInfo))
                  {
                      *(first_letter + i) = pyInfo[0];

                      memcpy(fullword,pyInfo,strlen(pyInfo));

                  }
                  else
                  {
                      *(first_letter + i) = NONE_LETTER;
                      *fullword = NONE_LETTER;
                  }


              }
              else if(0x3000 == nCurCode) //space cn
              {
                  *(first_letter + i) = ' ';
              }
              else
              {
                  *(first_letter + i) = NONE_LETTER;
                  *fullword= NONE_LETTER;
              }
          }
          trimspace(fullword);
          full_letter += string(fullword);
      }
      trimspace(first_letter);

      firstpy = first_letter;
      if (full_letter.size()>0)
     {
          if (full_letter[0] == NONE_LETTER)
              py = NONE_LETTER;
          else
              py  = full_letter;
      }
      else
      {
           py = NONE_LETTER;
           firstpy = NONE_LETTER;
      }
//    }
}
bool PhoneBookStubImp::utf8toucs2(const unsigned char* utf8_code, unsigned short* ucs2_code,int*len)
{
     unsigned short temp1, temp2;
     bool is_unrecognized = FALSE;
     unsigned char* in = (unsigned char*)utf8_code;
      int outlen =0;
     if(!utf8_code || !ucs2_code)
     {

         return is_unrecognized;
     }


     while(*in != 0)
     {
      //1字节 0xxxxxxx
      //0x80=1000,0000，判断最高位是否为0，如果为0，那么是ASCII字符
      //不需要处理，直接拷贝即可

      if(0x00 == (*in & 0x80))
      {
       /* 1 byte UTF-8 Charater.*/
       *ucs2_code = *in;
       is_unrecognized = TRUE;

       in += 1;
      }
      //2字节 110xxxxx 10xxxxxx
      //0xe0=1110,0000
      //0xc0=1100,0000
      else if(0xc0 == (*in & 0xe0) && 0x80 == (*(in + 1) & 0xc0))
      {
       /* 2 bytes UTF-8 Charater.*/
       //0x1f=0001,1111，获得第一个字节的后5位
       temp1 = (unsigned short)(*in & 0x1f);
       //左移6位
       temp1 <<= 6;

       //0x3f=0011,1111，获得第二个字节的后6位
       //加上上面的5位一共有11位
       temp1 |= (unsigned short)(*(in + 1) & 0x3f);

       *ucs2_code = temp1;

       is_unrecognized = TRUE;

       in += 2;
      }
      //3字节 1110xxxx 10xxxxxx 10xxxxxx
      //中文要进入这一个分支
      else if( 0xe0 == (*in & 0xf0) && 0x80 == (*(in +1) & 0xc0) &&0x80 == (*(in + 2) & 0xc0) )
      {
       /* 3bytes UTF-8 Charater.*/
       //0x0f=0000,1111
       //取出第一个字节的低4位
       temp1 = (unsigned short)(*in & 0x0f);
       temp1 <<= 12;

       //0x3f=0011,1111
       //取得第二个字节的低6位
       temp2 = (unsigned short)(*(in+1) & 0x3F);
       temp2 <<= 6;

       //取得第三个字节的低6位，最后组成16位
       temp1 = temp1 | temp2 | (unsigned short)(*(in+2) & 0x3F);

       *ucs2_code = temp1;

       //移动到下一个字符
       in += 3;
       is_unrecognized = TRUE;
      }
      else
      {
       /* unrecognize byte. */
       *ucs2_code = 0x22e0;
       is_unrecognized = FALSE;

       //直接退出循环
       break;
      }
      outlen++;
      ucs2_code += 1;
     }

     *len = outlen;

     return is_unrecognized;

}
int PhoneBookStubImp::getUcs2PinYinInfo(unsigned short ucsCode, char *pyInfo)
{
    if (ucsCode < UCS2_CHARACTER_START || ucsCode > UCS2_CHARACTER_END)
    {
        return -1;
    }

    unsigned short offset = ucsCode - UCS2_CHARACTER_START;
    unsigned short index = ucs2Table[offset];

    if(0 == index)
    {
        return -1;
    }

    unsigned short smIndex = index/1000;
    unsigned short ymIndex = (index/10)%100;
    char tuneIndex = (char)(index%10) + '0';

    if (smIndex > 0 && smIndex < PINYIN_SHENGMU_NUM_MAX)
    {
        memcpy(pyInfo, smTable[smIndex], strlen(smTable[smIndex]));
    }
    if (ymIndex > 0 && ymIndex < PINYIN_YUNMU_NUM_MAX)
    {
        memcpy(pyInfo + strlen(pyInfo), ymTable[ymIndex], strlen(ymTable[ymIndex]));
    }
    if (0 == strlen(pyInfo))
    {
        return -1;
    }
    memcpy(pyInfo + strlen(pyInfo), &tuneIndex, 1);

    return 0;
}
void PhoneBookStubImp::trimspace(char *pSrc)
      {
          if(NULL == pSrc)
          {
              return;
          }

          char *p = pSrc;
          char *pTemp = pSrc;
          int nLen = strlen(pSrc);

          int i;
          for(i=0;i<nLen;i++)
          {
              char c = *(p + i);
              if( !isspace(c))
              {
                  *pTemp = *(p + i);
                  pTemp++;
              }
          }
          *pTemp = '\0';

          return;
}

string PhoneBookStubImp::addrtoname(string soucename,int kind)
{
    string str = "";// "table_CB_";
    if (soucename.length() == 17)
   {
         if (kind ==0)
         str = "table_CB_"+soucename.substr(0,2)+soucename.substr(3,2)\
                 +soucename.substr(6,2)+soucename.substr(9,2)\
                 +soucename.substr(12,2)+soucename.substr(15,2);
        else
         str ="table_CR_"+soucename.substr(0,2)+soucename.substr(3,2)\
                    +soucename.substr(6,2)+soucename.substr(9,2)\
                    +soucename.substr(12,2)+soucename.substr(15,2);
    }
    return str;
}

string PhoneBookStubImp::handleSpecialCharacter(string soucename)
{
    string str ="";
    for (int i =0 ;i < soucename.length();i++)
    {
        if (soucename.at(i) =='"')
        {
          str += "\"\"";
        }
        else
          str += soucename.at(i);
    }
    return str;
}

void *PhoneBookStubImp::dealdataFunc(void *args)
{
//    PhoneBookStubImp *m_phoneBook =(PhoneBookStubImp *)args;
    std::shared_ptr< PhoneBookStubImp > m_phoneBook = PhoneBookStubImp::getInstance();
    size_t contact_len;
    bool isCallLog = false;
    BT_ADDR ba;
    ContactArray *tempcontacts_ptr =NULL;
    while(1)
    {
        // HSAE_CONTACT_INFO  contact[1500];
       pthread_mutex_lock(&(PhoneBookStubImp::mutex_for_pbdata));
       pthread_cond_wait( &PhoneBookStubImp::pbcond, &(PhoneBookStubImp::mutex_for_pbdata));
       pthread_mutex_unlock(&(PhoneBookStubImp::mutex_for_pbdata));

       BT_ADDR ba;
       memcpy(ba.b, DeviceStubImp::hfp_connect_addr.b, 6);
       char btAddress[20]={0};// = (char*)malloc( 50 );

       m_phoneBook->chartostr(btAddress,ba.b);
       Calllogs callogdatalist;
       string name="";
       string number="";
       string kind="";
       string datatime="";
       string talktime="";
       for(size_t i = 0; i < m_phoneBook->contacts_ptr->GetCount(); i++ )
        {
            Contact m_contact;
            HSAE_CONTACT_INFO *example_info = (m_phoneBook->contacts_ptr)->GetAt(i);
          if ((example_info->fullname !=NULL)&&(example_info->name !=NULL))
          {
              if ( strlen(example_info->fullname) >= strlen(example_info->name))
               {
                   if ( example_info->fullname !=NULL)
                    m_contact.name = string( example_info->fullname );
                   else {
                    m_contact.name ="";
                    }
               }
               else
               {
                   if ( example_info->name !=NULL)
                    m_contact.name = string( example_info->name );
                   else {
                    m_contact.name ="";
                    }
               }
          }else if (example_info->name == NULL)
          {
              m_contact.name ="";
          }
            name = m_contact.name;

            m_contact.phone_count =  example_info->phone_count;
            size_t j;
            m_contact.phone.clear();
           if ((example_info->call_info.dir != NULL)&&(example_info->call_info.date != NULL)&&(example_info->call_info.time != NULL))
           {
                if( strcasecmp( example_info->call_info.dir, "\0" ) != 0 ||
                    strcasecmp(example_info->call_info.date, "\0" ) != 0 ||
                    strcasecmp(example_info->call_info.time, "\0" ) != 0
                    )
                {
                    NewContact callogcontact;
                    callogcontact.name = m_contact.name;
                    isCallLog = true;
                    if(strcasecmp( example_info->call_info.dir, "\0" ) == 0)
                    {
                        m_contact.call_info.dir = "";
                    }
                    else
                    {
                        m_contact.call_info.dir = example_info->call_info.dir;
                    }

                    if(strcasecmp( example_info->call_info.date, "\0" ) == 0)
                    {
                        m_contact.call_info.date = "";
                    }
                    else
                    {
                        string buf = example_info->call_info.date;
                        if (buf.length()>=8)
                        m_contact.call_info.date = buf.substr(0,4)+"/"+ buf.substr(4,2)+"/" +buf.substr(6,2);
                    }

                    if(strcasecmp( example_info->call_info.time, "\0" ) == 0)
                    {
                        m_contact.call_info.time = "";

                    }
                    else
                    {
                        string buf= example_info->call_info.time;
                        if (buf.length()>=6)
                        m_contact.call_info.time = buf.substr(0,2)+":"+ buf.substr(2,2)+":" +buf.substr(4,2);
                    }
                    kind = m_contact.call_info.dir;
                    datatime = m_contact.call_info.date;
                    talktime = m_contact.call_info.time ;
                }
                else
                {
                    isCallLog = false;
                }
            }
            for( j = 0; j <  example_info->phone_count; j++ )
            {
                Phone phone;

                size_t k;
//                printf("k =%d \r\n",k);
                for( k = 0; k < example_info->phone[j].phone_type_count; k++ )
                {

                    phone.phoneType = (PhoneType)example_info->phone[j].phone_type[k];
                }

                if  (example_info->phone[j].phone_number !=NULL)
                    phone.phoneNumber = string( example_info->phone[j].phone_number );
                else
                    phone.phoneNumber = "";
                number = phone.phoneNumber;
                if( phone.phoneNumber.empty() )
                {

                    m_contact.name.clear();
                }
                else
                {
                    m_contact.phone.push_back( phone );
                }

                if (m_phoneBook->pbapmac.empty())
                {
                    printf("--------------downabort i =%d",i);
                    i= 5000;
                    PhoneBookStubImp::invokeDownPbap = -1;
                    //drop
                    m_phoneBook->m_bsaved = true;
                    m_phoneBook->pdata->dropContactTable(string(btAddress));
                    break;
                }
                // if not calllog add contacts
                if (PhoneBookStubImp::invokeDownPbap ==1 )
                    m_phoneBook->addcontact(string(btAddress),m_contact.name,phone.phoneNumber,(int)phone.phoneType);
                else
                {
                    callogdatalist.push_back(m_contact);
                }
            }


        }
       delete m_phoneBook->contacts_ptr;
       printf("dealdata finished---------------PhoneBookStubImp::invokeDownPba=%d---------\r\n",PhoneBookStubImp::invokeDownPbap);
        if( PhoneBookStubImp::invokeDownPbap == 1 )
        {
            m_phoneBook->Btnoticepadata(0);

        }
#ifdef USE_DOWNLOADCALLLOG
        else  if( PhoneBookStubImp::invokeDownPbap == 0 )
        {
            if (callogdatalist.size()<31)
            {
                m_phoneBook->BtDownLoadCallLogChanged(callogdatalist);
                m_phoneBook->downloadcalled =true;
            }
            PhoneBookStubImp::invokeDownPbap =-1;
        }else if( PhoneBookStubImp::invokeDownPbap == 2 )
        {
            printf("kind =%s",kind.data());
            m_phoneBook->BtCallLogChanged(name,number,kind,datatime,talktime);
            PhoneBookStubImp::invokeDownPbap =-1;
        }
#endif
        printf( "Contact number2: %d\n\n", (int)contact_len );
    }

    cout<<"PhoneBookStubImp::dealdataFunc end "<<endl;
}

int PhoneBookStubImp::strtype(string str)
{

    int type=1;
     if (str =="")
         return type;
   //  cout<<"PhoneBookStubImp::strtype start "<<(int)str[0]<<endl;
    if ((int)str[0] > 0x2F && (int)str[0] < 0x3A)
    {
        type =2;
    }
    else   if ((int)str[0] > 0x7F)
    {
        type =3;
    }
    return type;

}

void PhoneBookStubImp::downloadcalllogs(int count)
{
   #if 0
    LOG(BT_LOG_PB,"count =%d,invokeDownPbap =%d,m_cancleddowncallog =%d",count,invokeDownPbap,m_cancleddowncallog);
    if (m_cancleddowncallog == true)
    {
        return;
    }
    if (invokeDownPbap ==1)
    {
        needdownloadcalllog =true;
        return;
    }
    if ( count == MAXRECORDCOUNT )
        invokeDownPbap = 0;
    else
        invokeDownPbap =2;
    HSAE_PHONEBOOK_DATA data;
    data.max_count = count;
    data.location = HSAE_PHONEBOOK_LOCATION::HSAE_PB_LOC_PHONE;
    data.type = HSAE_PHONEBOOK_TYPE::HSAE_PB_TYPE_COMBINED;
    hsaeCmdDownloadNoPhotoPhonebookStart( NULL, &DeviceStubImp::hfp_connect_addr,data );
 #endif
}

void PhoneBookStubImp::deletecontacttable(string address)
{
   LOG(BT_LOG_PB,"address =%s",address.data());
//   string tablename = addrtoname(address,0);
//   if (tablename != "")
//   {
       pdata = datasource::getInstance();;
       pdata->dropContactTable(address);
       //   }
}

void PhoneBookStubImp::cancledownload()
{
    LOG(BT_LOG_PB,"START m_bsaved  %d",m_bsaved);
    HSAE_DOWNLOAD_STATE state;
    HSAE_PHONEBOOK_DATA data;
    hsaeCmdDownloadPhonebookState(NULL, &DeviceStubImp::hfp_connect_addr,&state,&data);
      printf("[BtServer][PB]:downloading= %d-\r\n",state);
    if ((state ==HSAE_LS_START )||(state ==HSAE_DLS_ING))
    {
        printf("[BtServer][PB]:downloading--------\r\n");
        hsaeCmdDownloadPhonebookStop( NULL, &DeviceStubImp::hfp_connect_addr );
    }
    m_cancleddowncallog = true;
    pbapmac ="";
    if (!m_bsaved)
     {
         //drop contact table
         BT_ADDR ba;
         memcpy(ba.b, DeviceStubImp::hfp_connect_addr.b, 6);
         char btAddress[20]={0};// = (char*)malloc( 50 );
         PhoneBookStubImp::getInstance()->chartostr(btAddress,ba.b);
         pdata->dropContactTable(string (btAddress));
         m_bsaved = true;

     }


}
void PhoneBookStubImp::BtCallLogChanged(string name, string number, string kind, string datatime,string talktime)
{
    if( PhoneBookStubImp::m_interface != NULL )
    {
        if ( downloadcalled) // notice ui  add calllog
            phone_book_emit_new_calllog_changed( PhoneBookStubImp::m_interface,
                                          name.c_str(),number.c_str(),kind.c_str(),datatime.c_str(),talktime.c_str());
 #ifndef USED_SQL
       else //insert callog
        {
            NewContact m_contact;
            m_contact.name = name;
            m_contact.phoneNumber = number;

            m_contact.call_info.dir = kind;
            m_contact.call_info.date = datatime;
            m_contact.call_info.time = talktime;
            pbdatalist.push_back(m_contact);
        }
#endif
    }
}
bool PhoneBookStubImp::createContactTable(string tablename)
{

     bool ret =false;
     m_cancleddowncallog = false;
     pdata =datasource::getInstance();
     pbapmac = tablename;
     string pbtablename= addrtoname(tablename ,0);
     if ( pbtablename =="")
         return ret;
     int result =pdata->createContactTable(pbtablename);
#ifndef  USE_DOWNLOADCALLLOG
     string crtablename = addrtoname(tablename ,1);
     result = pdata->createCallRecordTable(crtablename);
#endif
     return ret;
}
void PhoneBookStubImp::setpbdata(const Contacts contactlist)
{
    if (pbdatalist.size()>0)
    {
          vector<NewContact>().swap(pbdatalist);

    }
    pbdatalist.assign(contactlist.begin(), contactlist.end());

}

string PhoneBookStubImp::getnamebynum(string addr,string num)
{
   string name="";
//   if (downloaded)
//   {
       pdata =datasource::getInstance();
       name = pdata->querynamebynum(addr,num);
//   }
    return name;
}
void PhoneBookStubImp::initpbdata()
{
    // cancel pbbook

    HSAE_DOWNLOAD_STATE state;
    HSAE_PHONEBOOK_DATA data;
    hsaeCmdDownloadPhonebookState(NULL, &DeviceStubImp::hfp_connect_addr,&state,&data);

    printf("[BtServer][PB]:downloading= %d-\r\n",state);
    if ((state ==HSAE_LS_START )||(state ==HSAE_DLS_ING))
    {
      //  printf("[BtServer][PB]:downloading--------\r\n");
        hsaeCmdDownloadPhonebookStop( NULL, &DeviceStubImp::hfp_connect_addr );
    }
    m_cancleddowncallog = true;
    downloadcalled =false;
    if (pbdatalist.size()>0)
    {
          vector<NewContact>().swap(pbdatalist);
    }
    pbapmac ="";
    printf("PhoneBookStubImp::initpbdata( %d",m_bsaved);
    if (!m_bsaved)
    {
        //drop contact table
        BT_ADDR ba;
        memcpy(ba.b, DeviceStubImp::hfp_connect_addr.b, 6);
        char btAddress[20]={0};// = (char*)malloc( 50 );
        PhoneBookStubImp::getInstance()->chartostr(btAddress,ba.b);
        pdata->dropContactTable(string (btAddress));
        m_bsaved =true;

    }

    m_bnoticeend =true;

    needdownloadcalllog =false;
    invokeDownPbap = -1;
  //  pdata->clearcontacts();
    pdata =datasource::getInstance();;
    pdata->clearmapcontacts();
//    phoneBookInstance->pdata->clearcontacts();
//    phoneBookInstance->pdata->clearmapcontacts();
}

bool PhoneBookStubImp::updatepbdata(string addr)
{
    bool dataexits =false;
    Contacts contactlist;
    LOG(BT_LOG_PB,"START");
    pdata->getcontacts(addr,contactlist);
    phoneBookInstance->BtPdownLoadPhoneBookChanged(contactlist);
    if (contactlist.size()>0)
        dataexits =true;
#ifndef  USE_DOWNLOADCALLLOG
    vector<NewContact>().swap(contactlist);
    phoneBookInstance->pdata->getcallogs(addr,contactlist);
    phoneBookInstance->BtDownLoadCallLogChanged(contactlist);
#else
//      std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
//    if (g_device->currentStates.callState == CallState::CALL_TERMINATE)
//        phoneBookInstance->downloadcalllogs(MAXRECORDCOUNT);
#endif
    return dataexits;
}

void PhoneBookStubImp::chartostr(char *strmac, const unsigned char *mac)
{
    sprintf( strmac, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3],mac[2], mac[1], mac[0] );
}

void PhoneBookStubImp::BtPdownLoadPhoneBookChanged( const Contacts contactlist )
{

    LOG(BT_LOG_PB,"START");
    Json::Value root;
    Json::Value contact_arry;
    Json::Value contact_item;
    Json::Value phone_array;
    int j=0;
    string lastname ="";
    for( int i = 0; i < contactlist.size(); i++ )
    {
          Json::Value phone_item;
          phone_item.clear();
          if (NULL != contactlist[i].name.c_str() )
          {
              if (lastname !=contactlist[i].name)
              {
                    lastname = contactlist[i].name;
                    if (i !=0)
                   {
                        contact_item[TELPHONE] = phone_array;
                        contact_arry.append( contact_item );
                        contact_item.clear();
                        phone_array.clear();
                   }
              }

             contact_item[NAME] = contactlist[i].name;
             contact_item[FPY]  = contactlist[i].fpy;
         }
          else
          {
             // if contact_item
              lastname ="";
              if (i !=0)
             {
                  contact_item[TELPHONE] = phone_array;
                  contact_arry.append( contact_item );
                  contact_item.clear();
                  phone_array.clear();
             }
          }
        if( contactlist[i].phoneNumber != "" )
        {

            phone_item[PHONENUMBER] = contactlist[i].phoneNumber;
            phone_item[PHONETYPE] = (PhoneType)contactlist[i].type;
            phone_array.append( phone_item );


        }
    }
    if ( contactlist.size()>0)
    {
        contact_item[TELPHONE] = phone_array;
        contact_arry.append( contact_item );
    }
    root[CONTACTLIST] = contact_arry;
    contact_arry.clear();
    Json::FastWriter writer;
    string str_contactlist = writer.write( root );

    if( PhoneBookStubImp::m_interface != NULL )
    {

        phone_book_emit_phone_book_changed( PhoneBookStubImp::m_interface,
                                            str_contactlist.c_str() );
    }
     LOG(BT_LOG_PB,"END");
}
#ifndef USE_DOWNLOADCALLLOG
void PhoneBookStubImp::BtDownLoadCallLogChanged( const Contacts contactlist )
{
    LOG(BT_LOG_PB,"start ");

    Json::Value root;
    Json::Value contact_arry;
    for( int i = 0; i < contactlist.size(); i++ )
    {
        Json::Value phone_item;
        Json::Value contact_item;
        Json::Value phone_array;
        if( NULL != contactlist[i].name.c_str() )
        {
            contact_item[NAME] = contactlist[i].name;
        }
        if( NULL != contactlist[i].phoneNumber.c_str() )
        {
            phone_item[PHONENUMBER] = contactlist[i].phoneNumber;
#ifndef USE_DOWNLOADCALLLOG
            phone_item[PHONETYPE] = PhoneType::NONE;
#else
            phone_item[PhoneType] = contactlist[i].type;
#endif
        }

        contact_item[DIRECTION]  = contactlist[i].call_info.dir;
        phone_array.append( phone_item );
        contact_item[TELPHONE] = phone_array;
        contact_item[CALLDATE] = contactlist[i].call_info.date;
        contact_item[CALLTIME] = contactlist[i].call_info.time;
        contact_arry.append( contact_item );

    }
    root[CONTACTLIST] = contact_arry;
    contact_arry.clear();
    Json::FastWriter writer;
    string str_contactlist = writer.write( root );

    downloadcalled =true;
    if( PhoneBookStubImp::m_interface != NULL )
    {
        phone_book_emit_call_log_changed( PhoneBookStubImp::m_interface,
                                          str_contactlist.c_str() );
    }
     LOG(BT_LOG_PB,"end ");
}
#else
void PhoneBookStubImp::BtDownLoadCallLogChanged( const Calllogs contactlist )
{
    Json::Value root;
    Json::Value contact_arry;
    Json::Value contact_item;
    Json::Value phone_array;
    Json::Value phone_item;
    Json::Value phone_item_type;
    Json::Value call_date;
    Json::Value direction;
    Json::Value call_time;
    for( int i = 0; i < contactlist.size(); i++ )
    {
        if( NULL != contactlist[i].name.c_str() )
        {
            contact_item[NAME] = contactlist[i].name;
        }
        if( contactlist[i].phone.size() > 0 )
        {
            for( int j = 0; j < contactlist[i].phone.size(); j++ )
            {
                if( NULL != contactlist[i].phone[j].phoneNumber.c_str() )
                {
                    phone_item[PHONENUMBER] = contactlist[i].phone[j]
                                    .phoneNumber;
                }
                if( NULL != contactlist[i].phone[j].phoneType )
                {
                    phone_item[PHONETYPE] = (int)contactlist[i].phone[j]
                                    .phoneType;
                }
                phone_array.append( phone_item );
                phone_item.clear();
            }
            contact_item[TELPHONE] = phone_array;
            phone_array.clear();
        }
        //add by qianzhong 2017-6-13
        contact_item[DIRECTION] = contactlist[i].call_info.dir;
        contact_item[CALLDATE] = contactlist[i].call_info.date;
        contact_item[CALLTIME] = contactlist[i].call_info.time;
//        contact_arry[TELPHONE] = phone_array;
        contact_arry.append( contact_item );
        contact_item.clear();
    }
    root[CONTACTLIST] = contact_arry;
    contact_arry.clear();
    Json::FastWriter writer;
    string str_contactlist = writer.write( root );
    if( PhoneBookStubImp::m_interface != NULL )
    {
        phone_book_emit_call_log_changed( PhoneBookStubImp::m_interface,
                                          str_contactlist.c_str() );
    }
}
#endif

gboolean PhoneBookStubImp::clearcontacts(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr)
{
   // Contacts contactlist;
    LOG(BT_LOG_PB,"cmd=%s ",arg_addr);
    phoneBookInstance->pdata->clearContacts(string(arg_addr));
  //  phoneBookInstance->pdata->dropContactTable(string(arg_addr));

//
#ifndef  USE_DOWNLOADCALLLOG
    phoneBookInstance->updatepbdata(string(arg_addr));
#else
    Contacts contactlist;
   // cout<<"PhoneBookStubImp::updatepbdata"<<pdata<<endl;
    phoneBookInstance->pdata->getcontacts(string(arg_addr),contactlist);
    phoneBookInstance->BtPdownLoadPhoneBookChanged(contactlist);
#endif
    phone_book_complete_clearcontacts( interface, invocation );
     return TRUE;
}

gboolean PhoneBookStubImp::deletecontact(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr,const gchar *arg_name, const gchar *arg_num)
{
   // Contacts contactlist;
    phoneBookInstance->pdata->deleteContact(string(arg_addr),string(arg_name),string(arg_num));
#ifndef  USE_DOWNLOADCALLLOG
    phoneBookInstance->updatepbdata(string(arg_addr));
#else
    Contacts contactlist;

    phoneBookInstance->pdata->getcontacts(string(arg_addr),contactlist);
    phoneBookInstance->BtPdownLoadPhoneBookChanged(contactlist);
#endif
//    phoneBookInstance->pdata->getcallogs(string(arg_addr),contactlist);
//    phoneBookInstance->BtDownLoadCallLogChanged(contactlist);
//    system("sync");
    phone_book_complete_deletecontact( interface, invocation );
     return TRUE;
}

gboolean PhoneBookStubImp::searchcontact(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr, const gchar *arg_sql, gint arg_type)
{
   Contacts contactlist;
   string str="";
   int type =0;
   if (arg_type > 0)
   {
         if (arg_sql !=NULL)
        {

           str = string(arg_sql);
           type  = phoneBookInstance->strtype(str);
       }
   }
   string address = "";
   if (arg_addr !=NULL)
       address = string(arg_addr);
   phoneBookInstance->pdata->searchcontacts(address,str,contactlist,type);
   phoneBookInstance->BtPdownLoadPhoneBookChanged(contactlist);
   phone_book_complete_searchcontact( interface, invocation );
   return TRUE;
}

gboolean PhoneBookStubImp::syncdata(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr, gint arg_type)
{
   LOG(BT_LOG_PB,"kind = %d",arg_type);
   switch (arg_type)
   {
       case 0:
           phoneBookInstance->pdata->clearContacts(string(arg_addr));
         //  doSyncStart(&DeviceStubImp::hfp_connect_addr);
           break;
       case 1:
           phoneBookInstance->pdata->clearContacts(string(arg_addr));
        //   doSyncStart(&DeviceStubImp::hfp_connect_addr);
           break;
       case 2:
        //   doSync_CallLog(&DeviceStubImp::hfp_connect_addr);
           break;


   }
   phone_book_complete_syncdata(interface, invocation);
   return TRUE;
}

gboolean PhoneBookStubImp::savecontacts(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr)
{

    gboolean  result = FALSE;
    phoneBookInstance->m_bsaved =true;
    phone_book_complete_savecontacts(interface, invocation,result);
    return TRUE;
}

gboolean PhoneBookStubImp::clearcalllogs(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr)
{
#ifndef  USE_DOWNLOADCALLLOG
    phoneBookInstance->pdata->clearCallRecords(string(arg_addr));
    Contacts contactlist;
    phoneBookInstance->pdata->getcallogs(string(arg_addr),contactlist);
    phoneBookInstance->BtDownLoadCallLogChanged(contactlist);
    phone_book_complete_clearcalllogs(interface, invocation);
#endif
}

gboolean PhoneBookStubImp::deletecalllog(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr, const gchar *arg_datatime)
{
 #ifndef  USE_DOWNLOADCALLLOG
    phoneBookInstance->pdata->deleteCallRecord(string(arg_addr),string(arg_datatime));
    Contacts contactlist;
    phoneBookInstance->pdata->getcallogs(string(arg_addr),contactlist);
    phoneBookInstance->BtDownLoadCallLogChanged(contactlist);
    phone_book_complete_deletecalllog(interface, invocation);
#endif
    return TRUE;
}


gboolean PhoneBookStubImp::getcontacts(PhoneBook *interface, GDBusMethodInvocation *invocation, const gchar *arg_addr)
{
    bool  dataexits = phoneBookInstance->updatepbdata(string(arg_addr));
    phone_book_complete_getcontacts(m_interface, invocation,dataexits);
    return TRUE;
}
