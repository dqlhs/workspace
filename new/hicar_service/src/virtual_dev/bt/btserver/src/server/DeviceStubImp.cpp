#include "DeviceStubImp.h"
#include "spp.h"
//add qianzhong 2017-1-16
//end add
#include <jsoncpp/json/json.h>
#include <sstream>
#include <string.h>
#include <map>
#include <stdlib.h>

char* phone_pair_nf_pincode;
BT_ADDR remote_pair_addr;
bool wait_pair_result = false;
bool remote_pairing = false;
bool remote_pair_success = false;

extern BT_ADDR DeviceStubImp::hfp_connect_addr;
extern BT_ADDR DeviceStubImp::hfp_connecting_addr;
extern BT_ADDR DeviceStubImp::avrcp_connect_addr;
extern BT_ADDR DeviceStubImp::a2dp_connect_addr;
HSAE_ANSWERPAIR_STATE pair_state;
//extern unsigned long testcount;
//extern unsigned long GetTickCountTest();
int32_t gpinCodeSize = 0;
int gpincode = 0;
char *g_pincode;
 
 
bool avrcp_registing = false;


#define PAIRLIST "PairList"
#define DEVICENAME "DeviceName"
#define BTADDRESS "BtAddr"
using namespace std;
using namespace com::hsae::bluetooth::util::HsaeTypes;
//int SHOW_LOG = 0;

std::shared_ptr< DeviceStubImp > DeviceStubImp::deviceInstance = NULL;
//add 2017-1-14
Pairs pairLists;

PlayAttribute DeviceStubImp::PLAYATTRIBUTE = { "", "", "", "", 0 };

//add by qianzhong 2017-5-23
Device* DeviceStubImp::interface = NULL;
unsigned long DeviceStubImp::m_dwDialMoment = 0;

//void str_to_char( const char *addr, unsigned char b[BT_ADDR_LEN] )
//{
//    int i;
//    char byteInChar[2];

//    for( i = 0; i < BT_ADDR_LEN; i++ )
//    {
//        byteInChar[0] = toupper( addr[0] );
//        byteInChar[1] = toupper( addr[1] );

//        // first 4 bit
//        if( isalpha( byteInChar[0] ) )
//            b[i] = (byteInChar[0] - 55) * 16;
//        else if( isdigit( byteInChar[0] ) )
//            b[i] = (byteInChar[0] - 48) * 16;

//        // last 4 bit
//        if( isalpha( byteInChar[1] ) )
//            b[i] += (byteInChar[1] - 55);
//        else if( isdigit( byteInChar[1] ) )
//            b[i] += (byteInChar[1] - 48);

//        if( addr[2] != '\0' )
//            addr = &addr[3];
//    }
//}
void str_to_char( const char *addr, unsigned char b[BT_ADDR_LEN] )
{
    int i;
    char byteInChar[2];

    for( i = 0; i < BT_ADDR_LEN; i++ )
    {
        byteInChar[0] = toupper( addr[0] );
        byteInChar[1] = toupper( addr[1] );

        // first 4 bit
        if( isalpha( byteInChar[0] ) )
            b[BT_ADDR_LEN-i-1] = (byteInChar[0] - 55) * 16;
        else if( isdigit( byteInChar[0] ) )
            b[BT_ADDR_LEN-i-1] = (byteInChar[0] - 48) * 16;

        // last 4 bit
        if( isalpha( byteInChar[1] ) )
            b[BT_ADDR_LEN-i-1] += (byteInChar[1] - 55);
        else if( isdigit( byteInChar[1] ) )
            b[BT_ADDR_LEN-i-1] += (byteInChar[1] - 48);

        if( addr[2] != '\0' )
            addr = &addr[3];
    }
}
void print_cmd_error( HSAE_CMD_RET cmd_ret )
{
    if( cmd_ret < HSAE_CMD_FAIL_FIRST || cmd_ret > HSAE_CMD_FAIL_LAST )
        return;
//    else if( cmd_ret == HSAE_CMD_FAIL )
//        printf( "HSAE_CMD_FAIL\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_INVALID_HANDLE )
//        printf( "HSAE_CMD_FAIL_INVALID_HANDLE\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_INVALID_CALLBACK )
//        printf( "HSAE_CMD_FAIL_INVALID_CALLBACK\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_NOT_SUPPORT )
//        printf( "HSAE_CMD_FAIL_NOT_SUPPORT\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_NOT_READY )
//        printf( "HSAE_CMD_FAIL_NOT_READY\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_CANCEL )
//        printf( "HSAE_CMD_FAIL_CANCEL\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_MEMORY_NOT_ENOUGH )
//        printf( "HSAE_CMD_FAIL_MEMORY_NOT_ENOUGH\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_DEPENDENCY )
//        printf( "HSAE_CMD_FAIL_DEPENDENCY\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_STATE )
//        printf( "HSAE_CMD_FAIL_STATE\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_REPEAT )
//        printf( "HSAE_CMD_FAIL_REPEAT\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_IO )
//        printf( "HSAE_CMD_FAIL_IO\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_CUS )
//        printf( "HSAE_CMD_FAIL_CUS\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_PARAMETER )
//        printf( "HSAE_CMD_FAIL_PARAMETER\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_RETRY )
//        printf( "HSAE_CMD_FAIL_RETRY\n" );
//    else if( cmd_ret == HSAE_CMD_FAIL_REMOTE )
//        printf( "HSAE_CMD_FAIL_REMOTE\n" );
    LOG(BT_LOG_DEVICE,"ERROR=%d",cmd_ret);
    return;
}
HSAE_CBK_RET acl_connection_state_changed(void *hsaeCbkParam, const BT_ADDR ba, HSAE_ACL_CONNECTION_STATE newState, unsigned char status)
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    char m_data[50] = {0};
    g_device->chartostr(m_data,(unsigned char*)&ba.b);
    device_emit_aclconnectstate(DeviceStubImp::interface, m_data,(int)newState);

//    bool iSame = true ;
//    for (int i =0; i<6; i++)
//    {
//       if ( ba.b[i] !=  DeviceStubImp::hfp_connect_addr .b[i])
//       {
//           iSame =false;
//           break;
//       }

//    }
//    printf("acl_connection_state_changed with %s, connected =%d, iSame=%d\n",m_data,newState,iSame);
//    if (newState == HSAE_ACL_CONNECTION_CONNECTED) {
//    {
//        //if now
//          if (iSame == false)
//          {
//              printf("DeviceStubImp::hfp_connect_addr %x:%x:%x:%x:%x:%x\r\n",DeviceStubImp::hfp_connect_addr.b[0],\
//                      DeviceStubImp::hfp_connect_addr.b[1],\
//                      DeviceStubImp::hfp_connect_addr.b[2],\
//                      DeviceStubImp::hfp_connect_addr.b[3],\
//                      DeviceStubImp::hfp_connect_addr.b[4],\
//                      DeviceStubImp::hfp_connect_addr.b[5]
//                      );
//              if ((DeviceStubImp::hfp_connect_addr.b[0] !=0)\
//                  |(DeviceStubImp::hfp_connect_addr.b[1] !=0)\
//                  |(DeviceStubImp::hfp_connect_addr.b[2] !=0)\
//                  |(DeviceStubImp::hfp_connect_addr.b[3] !=0)\
//                  |(DeviceStubImp::hfp_connect_addr.b[4] !=0)\
//                  |(DeviceStubImp::hfp_connect_addr.b[5] !=0))
//             {
//                  printf("====================test \r\n");
//                  hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::hfp_connect_addr, HSAE_PROFILEID_e::NPI_ALL_PROFILE);

//              }
//          }

//     }


//    } else if (newState == HSAE_ACL_CONNECTION_DISCONNECTED){

//    }

}


HSAE_CBK_RET disconnect_reason(void *HSAECbkParam, const BT_ADDR ba, const HSAE_DISCONNECT_REASON reason)
{

    LOG(BT_LOG_DEVICE," reason=%d ",(int)reason);
    switch(reason){
        case HSAE_DISCONNECT_REASON_NOT_FIND:

            break;

        case HSAE_DISCONNECT_REASON_LINK_LOSS:

            break;
        case HSAE_DISCONNECT_REASON_REMOTE_USER:

            break;
        case HSAE_DISCONNECT_REASON_REMOTE_POWER_OFF:

            break;
    }
if (reason ==HSAE_DISCONNECT_REASON_LINK_LOSS )
  {

    // if (interface != nullptr)
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    std::shared_ptr< SettingStubImp > g_set = SettingStubImp::getInstance();
    char m_data[50] = {0};
    g_device->chartostr(m_data,(unsigned char*)&ba.b);
    string autoconnectaddress =g_set->getautoaddr();
    if (m_data != NULL)
    {
       printf("autoconnectaddress =%s ,m_data =%s \r\n",autoconnectaddress.data(),m_data);
        if (autoconnectaddress == string(m_data) )
        {
            printf("g_device->currentStates.hfpState =%d \r\n",g_device->currentStates.hfpState);
            if (g_device->currentStates.hfpState != ConnectionState::CONNECTION)
            {
                device_emit_disconnectreason( DeviceStubImp::interface, m_data,(int)reason);
            }
        }
        else
        {
              LOG(BT_LOG_DEVICE," address is not the same with autoconnect address ,so donnot notice the app ");
        }
    }
    else
    {
          LOG(BT_LOG_DEVICE," address is null ,so donnot notice the app ");
    }

    }
    return HSAE_CBK_SUCCESS;
}
static int test =0;
HSAE_CBK_RET connection_status( void *HSAECbkParam, const BT_ADDR ba,
                               const HSAE_PROFILEID ProfileID,
                               const HSAE_CONNECTION_STATE state )
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    char m_data[255] = {0};
    g_device->chartostr(m_data,(unsigned char*)&ba.b);
    std::string address = string( m_data );
    LOG(BT_LOG_DEVICE,"profile: %d connection_status state value is %d",(int)ProfileID, (int)state);
    ConnectionState tempstate =(ConnectionState)state;
    switch(ProfileID)
    {
       case NPI_HANDFREE:
       { 	  
//    	   int profile;
    	   if (state ==HSAE_CON_CONNECTION)
    	   {	  
               std::shared_ptr< PhoneBookStubImp > g_phonebook = PhoneBookStubImp::getInstance();
               std::shared_ptr< SettingStubImp > g_set = SettingStubImp::getInstance();
               g_phonebook->createContactTable(string( m_data ));
               g_device->currentStates.hfpState = ConnectionState::CONNECTION;
               DeviceStubImp::hfp_connect_addr = ba;
               g_set ->closeAutoConnectThread();
               g_device->currentStates.btAddress = string(m_data);
               g_device->BTConnectionStateChanged( address, PROFILEID::HANDFREE,ConnectionState::CONNECTION );
               DeviceStubImp::hfp_connecting_addr ={0, 0, 0, 0, 0, 0};
              //set autoaddress
           //    hsaeCmdSetScanEnable( NULL, false);
               g_set->setautoconnectmac(m_data);
               g_device->m_cout =0;
               g_device->m_connecttimer.StartTimer();

               printf("hfp isconnected =%d\r\n",g_device->GetTickCount());
    	   }
           else  if (state == HSAE_CON_DISCONNECTION)
    	   {
               // hsaeCmdSetScanEnable( NULL, true);
                g_device->currentStates.hfpState = ConnectionState::DISCONNECTION;
                g_device->currentStates.btAddress  ="";
                g_device->BTConnectionStateChanged( address, PROFILEID::HANDFREE,ConnectionState::DISCONNECTION );
                std::shared_ptr< HFPStubImp > g_hfp = HFPStubImp::getInstance();
                g_hfp->inithfp();
                g_device->m_cout =3;

                DeviceStubImp::hfp_connect_addr = { 0, 0, 0, 0, 0, 0 };
                DeviceStubImp::hfp_connecting_addr ={0, 0, 0, 0, 0, 0};
                g_device->m_disconnecttimer.StartTimer();
    	   }
          else if  (state ==HSAE_CON_CONNECTING)
           {
               g_device->currentStates.hfpState = ConnectionState::CONNECTING;
               DeviceStubImp::hfp_connecting_addr = ba;
               g_device->BTConnectionStateChanged( address, PROFILEID::HANDFREE,ConnectionState::CONNECTING );
           }


    	   break;
       }
       case NPI_AUDIOSINK:
       {
            g_device->currentStates.a2dpState =  (state==0)?true:false;
            if ( g_device->currentStates.a2dpState)
            {
                g_device->m_cout =0;
                g_device->BTConnectionStateChanged( address, PROFILEID::AUDIOSINK, ConnectionState::CONNECTION  );
                DeviceStubImp::a2dp_connect_addr = ba;
            }
            else
            {
               if (g_device->IsCompareArray((unsigned char *)&ba,6,DeviceStubImp::a2dp_connect_addr.b,6))
               {
                   g_device->BTConnectionStateChanged( address, PROFILEID::AUDIOSINK, ConnectionState::DISCONNECTION  );
                   DeviceStubImp::a2dp_connect_addr = {0,0,0,0,0,0};
               }
            }
  

    	   break;
       }
       case NPI_AVRCP:
       {
//            if (g_device->IsCompareArray((unsigned char *)&ba,6,DeviceStubImp::hfp_connect_addr.b,6)||g_device->IsArrayEmpty(DeviceStubImp::hfp_connect_addr.b,6))
//            {

                g_device->currentStates.avrcpState = (state==0)?true:false;
                avrcp_registing = g_device->currentStates.avrcpState;
                if (avrcp_registing)
                {
                    g_device->m_cout =0;
                    hsaeCmdGetCapability( NULL, (BT_ADDR *) &ba );
                }
                if (state == HSAE_CONNECTION_STATE::HSAE_CON_CONNECTION )
                {
                    g_device->BTConnectionStateChanged( address, PROFILEID::AVRCP, ConnectionState::CONNECTION  );
                    hsaeCmdGetNowPlayingAttributes( NULL, (BT_ADDR *) &ba );
                    DeviceStubImp::avrcp_connect_addr = ba;
                }
                else  if (state == HSAE_CONNECTION_STATE::HSAE_CON_DISCONNECTION)
                {
                    if (g_device->IsCompareArray((unsigned char *)&ba,6,DeviceStubImp::avrcp_connect_addr.b,6))
                    {
                        std::shared_ptr< MediaPlayBackStubImp > m_meida = MediaPlayBackStubImp::getInstance();
                        g_device->BTConnectionStateChanged( address, PROFILEID::AVRCP, ConnectionState::DISCONNECTION  );
                        m_meida->TrancStreamStatus(false);
                        DeviceStubImp::avrcp_connect_addr = {0,0,0,0,0,0};
                    }
                }
               else  if (state == HSAE_CONNECTION_STATE::HSAE_CON_CONNECTING)
               {
                     DeviceStubImp::avrcp_connect_addr = ba;
                }

//             }
    	   break;
       }
       case NPI_PBAP:
    	   {
               g_device->currentStates.pbapState = (state==0)?true:false;
//               g_device->BTConnectionStateChanged( address, PROFILEID::PBAP,tempstate);
               if ( g_device->currentStates.pbapState)
                   g_device->BTConnectionStateChanged( address, PROFILEID::PBAP, ConnectionState::CONNECTION  );
               else
                   g_device->BTConnectionStateChanged( address, PROFILEID::PBAP, ConnectionState::DISCONNECTION  );

    		   break;
    	   }
     case NPI_RFCOMM_1:{

        g_device->m_server[0].status = state;

        if (state == HSAE_CON_CONNECTION )
        {
           g_device->sppaddr =  ba;
            g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_1, ConnectionState::CONNECTION  );
        }
        else   if (state == HSAE_CON_DISCONNECTION )
            g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_1, ConnectionState::DISCONNECTION  );
//        else   if (state == HSAE_CON_CONNECTING )
//            g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_1, ConnectionState::CONNECTING  );

        break;
    }
    case NPI_RFCOMM_2:{
          g_device->m_server[1].status = state;
       if (state == HSAE_CON_CONNECTION )
      {
           g_device->sppaddr =  ba;
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_2, ConnectionState::CONNECTION  );
       }
       else   if (state == HSAE_CON_DISCONNECTION )
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_2, ConnectionState::DISCONNECTION  );
//       else   if (state == HSAE_CON_CONNECTING )
//           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_2, ConnectionState::CONNECTING  );
       break;
   }
    case NPI_RFCOMM_3:{
        g_device->m_client[0].status = state;
       if (state == HSAE_CON_CONNECTION )
       {
            g_device->sppaddr =  ba;
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_3, ConnectionState::CONNECTION  );
       }
       else   if (state == HSAE_CON_DISCONNECTION )
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_3, ConnectionState::DISCONNECTION  );
//       else   if (state == HSAE_CON_CONNECTING )
//           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_3, ConnectionState::CONNECTING  );
       break;
   }
    case NPI_RFCOMM_4:{
        g_device->m_client[1].status = state;
       if (state == HSAE_CON_CONNECTION )
       {
           g_device->sppaddr =  ba;
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_4, ConnectionState::CONNECTION  );
       }
       else   if (state == HSAE_CON_DISCONNECTION )
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_4, ConnectionState::DISCONNECTION  );
//       else   if (state == HSAE_CON_CONNECTING )
//           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_4, ConnectionState::CONNECTING  );
       break;
   }
    case NPI_RFCOMM_5:{
       if (state == HSAE_CON_CONNECTION )
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_5, ConnectionState::CONNECTION  );
       else   if (state == HSAE_CON_DISCONNECTION )
           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_5, ConnectionState::DISCONNECTION  );
//       else   if (state == HSAE_CON_CONNECTING )
//           g_device->BTConnectionStateChanged( address, PROFILEID::RFCOMM_5, ConnectionState::CONNECTING  );
       break;
   }
     case NPI_SPP:
          {
               std::shared_ptr< SPPStubImp > g_spp = SPPStubImp::getInstance();
               printf("hsaetest :spp state =%d,ProfileID =%d\r\n",(int)state,(int)ProfileID);
               if (state == HSAE_CON_CONNECTION)
               {
                    g_device->m_currentsppstate = true;

                   g_device->sppaddr =  ba;
                  if (NPI_SPP_IAP == ProfileID)
                        g_spp->setios(true);
                  else
                        g_spp->setios(false);
                   g_spp->createSession();
               }else if (state == HSAE_CON_DISCONNECTION)
               {
                   g_device->m_currentsppstate = false;
                   memset (g_device->sppaddr.b,0,6);
                   g_spp->closeSession();

                       
               }
               break;
          }

       default:break;

    }
     printf("-----------------------------------------------------------\r\n");
//     free( m_data );
//     m_data = NULL;
     LOG(BT_LOG_DEVICE,"end");
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET print_device( void *HSAECbkParam, const HSAE_DEVICE *pDeviceList,
                          const int len )
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    LOG(BT_LOG_DEVICE,"paired_lists len =%d",len);
    Pair pair;
    if (pairLists.size()>0)
    {
          vector<Pair>().swap(pairLists);
    }
    pairLists.clear();
    char *m_data = (char*)malloc( 255 );
    int i;
    for( i = 0; i < len; i++ )
    {
        g_device->chartostr(m_data,(unsigned char *)&(pDeviceList[i].btAddr.b));
         LOG(BT_LOG_DEVICE,"paired_lists name = %s,btAddress =%s",pDeviceList[i].szName,m_data);
        pair.btAddress = string( m_data );
        if (pDeviceList[i].szName !="")
              pair.btDeviceName = string( pDeviceList[i].szName );
        else
            pair.btDeviceName ="";
        pairLists.push_back( pair );
    }
    if (len >0)
        g_device->BTPairListChanged( pairLists );
    free( m_data );
    m_data = NULL;
    LOG(BT_LOG_DEVICE,"paired_lists len =%d end",len);
    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET paired_lists( void *HSAECbkParam, const HSAE_PAIRED *pPairedList,
                          const int len )
{
    LOG(BT_LOG_DEVICE,"paired_lists %d",len);
    return print_device( HSAECbkParam, pPairedList, len );
}
HSAE_CBK_RET nf_diagnosticMessage(void *HSAECbkParam, HSAE_DIAG_CODE code)
{
   char dev_name[PATH_MAX_LEN]={0};
    switch(code)
    {
    case HSAE_NOERR:
        printf("[BtServer][ERROR]:NFBT_NOERR\n");
        break;
    case HSAE_DBG_SYS_A2DP_AUDIO_ERROR:
    case HSAE_DBG_SYS_RING_AUDIO_ERROR:
    case HSAE_DBG_SYS_SCO_AUDIO_ERROR:
        printf("[BtServer][ERROR]:System Audio can not open(%d).\n", code);
        break;
    case HSAE_DBG_SYS_SCO_MIC_ERROR:
        printf("[BtServer][ERROR]:System Microphone can not open(%d).\n", code);
        break;
    case HSAE_DBG_SYS_A2DP_AUDIO_ACTIVE:
        hsaeCmdGetA2DPDeviceName(NULL, dev_name, PATH_MAX_LEN);
        printf("[BtServer][ERROR]:System Mdeia sound \"%s\" active\n", dev_name);
        break;
    case HSAE_DBG_SYS_A2DP_AUDIO_RELEASE:
        hsaeCmdGetA2DPDeviceName(NULL, dev_name, PATH_MAX_LEN);
        printf("[BtServer][ERROR]:System Mdeia sound \"%s\" release\n", dev_name);
        break;
    case HSAE_DBG_SYS_RING_AUDIO_ACTIVE:
        hsaeCmdGetRingDeviceName(NULL, dev_name, PATH_MAX_LEN);
        printf("[BtServer][ERROR]:System Ring sound \"%s\" active\n", dev_name);
        break;
    case HSAE_DBG_SYS_RING_AUDIO_RELEASE:
//        hsaeCmdGetRingDeviceName(HSAEHandle, dev_name, PATH_MAX_LEN);
        printf("[BtServer][ERROR]:System Ring sound \"%s\" release\n", dev_name);
        break;
    case HSAE_DBG_SYS_SCO_AUDIO_ACTIVE:
        printf("[BtServer][ERROR]:System Phone sound resource active\n");
        break;
    case HSAE_DBG_SYS_SCO_AUDIO_RELEASE:
        printf("[BtServer][ERROR]:System Phone sound resource release\n");
        break;
    case HSAE_DBG_SYS_SCO_MIC_ACTIVE:
        printf("[BtServer][ERROR]:System Phone Microphone resource active\n");
        break;
    case HSAE_DBG_SYS_SCO_MIC_RELEASE:
        printf("[BtServer][ERROR]:System Phone Microphone resource release\n");
        break;
    case HSAE_ERR_BT_HW_EXCEPTION:
        printf("[BtServer][ERROR]:Bluetooth adapter exception. Restart adapter\n");
        hsaeCmdSetPower( NULL, HSAE_POWERON );
        break;
    case HSAE_ERR_SYS_AUDIO_DRIVER_ERROR:
        printf("[BtServer][ERROR]:Bluetooth adapter exception. Restart adapter\n");

        break;

    default:
        printf("Unknown code\n");
        break;
    }

    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET response_pair( void *HSAECbkParam, const int pincode,
                           const HSAE_ANSWERPAIR_STATE state, const BT_ADDR ba,
                           const bool success )
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
//    char *m_data = (char*)malloc( 255 );
//    string m_btAddress;
//    sprintf( m_data, "%02X:%02X:%02X:%02X:%02X:%02X", ba.b[0], ba.b[1], ba.b[2],
//             ba.b[3], ba.b[4], ba.b[5] );
//    m_btAddress = string( m_data );
    int i;
    if( state != HSAE_ANSWERPAIR_RESULT )
    {
        remote_pairing = true;
        gpincode = pincode;
        for( i = 0; i < BT_ADDR_LEN; i++ )
            remote_pair_addr.b[i] = ba.b[i];
         LOG(BT_LOG_DEVICE,"Remote device %02X:%02X:%02X:%02X:%02    string localName = gDevice->localName();X:%02X send a pair request",ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );
        if( state == HSAE_ANSWERPAIR_CONFIRM )
        {

            phone_pair_nf_pincode = (char*)malloc( 255 );
            sprintf( phone_pair_nf_pincode, "%0u", pincode );
            LOG(BT_LOG_DEVICE,"To confirm request pin code %0u is same as displayed on remote device,phone_pair_nf_pincode %s",pincode ,phone_pair_nf_pincode);

            g_device->phoneConnnectNF();
        }
        pair_state = state;
    }
    else
    {
        if( success == true )
        {

            //send broadcast
            LOG(BT_LOG_DEVICE,"Pairf rom successful%02X:%02X:%02X:%02X:%02X:%02X ",ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );

            int profile;
            hsaeCmdSupportProfilelist( NULL, (BT_ADDR*) &ba, &profile );
            printf( "[hsae-bluetooth]profile value is:%d\n", profile );
            print_cmd_error( hsaeCmdGetPairedList( NULL ) );
//            usleep( 1000 * 100 );
//            hsaeCmdConnectRemoteDevice( NULL, &remote_pair_addr, NPI_ALL_AUDIO );
            remote_pair_success = true;
        }
        else
        {

            LOG(BT_LOG_DEVICE,"Pairf rom fail%02X:%02X:%02X:%02X:%02X:%02X ",ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );
            remote_pair_success = false;
        }
        remote_pairing = false;
        wait_pair_result = false;
    }
    free( phone_pair_nf_pincode );
    phone_pair_nf_pincode = NULL;
//    free( m_data );
//    m_data = NULL;
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET scan_status( void *HSAECbkParam, const bool scanning,
                         const HSAE_DEVICE device )
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    char *m_data = (char*)malloc( 255 );
    LOG(BT_LOG_DEVICE,"scanning %d ",(int)scanning );
    if( scanning )
    {
        if( device.szName == NULL )
        {

        }
        else
        {
            g_device->chartostr(m_data,(unsigned char*)&(device.btAddr.b));
            g_device->BTSearchChanged(string(device.szName),string(m_data));
            free(m_data);
            m_data = NULL;

        }
    }
    else
    {
//        printf( "Scanning terminated.\n\n" );
    }
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET hf_battery_status_changed(void *HSAECbkParam, const BT_ADDR ba,
        const HSAE_HANDSFREE_BATT_STATUS status)
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    LOG(BT_LOG_DEVICE,"status %d ",(int)status );
    g_device->BTBatteryChanged((int)status);

    return HSAE_CBK_SUCCESS;
}

DeviceStubImp::DeviceStubImp()
{
    m_cout = 0;
    position = { "", "" };
    status = { PlayState::PAUSED, position };
    hfp_connect_addr = { 0, 0, 0, 0, 0, 0 };
    hfp_connecting_addr = { 0, 0, 0, 0, 0, 0 };
    avrcp_connect_addr = { 0, 0, 0, 0, 0, 0 };
    a2dp_connect_addr = { 0, 0, 0, 0, 0, 0 };
    memset (sppaddr.b,0,6);
    hsaeCmdSetScanEnableTime( NULL, 20 );

    hsaeCmdSetCbk_InquiryStatus( NULL, scan_status );
    hsaeCmdSetCbk_ConnectionStatus( NULL, connection_status );
    hsaeCmdSetCbk_GetPairedList( NULL, paired_lists );
    hsaeCmdSetCbk_RemotePairRequest( NULL, response_pair );
    hsaeCmdSetCbk_HandsfreeBattStatusChanged( NULL, hf_battery_status_changed );
    hsaeCmdSetCbk_DisconnectReason( NULL, disconnect_reason );
    hsaeCmdSetCbk_DiagnosticMessage(NULL,nf_diagnosticMessage);
    hsaeCmdSetCbk_ACLConnectionStateChanged(NULL, acl_connection_state_changed);
    m_property.address = "00:00:00:00:00:00";
    m_property.name = "help";
    m_property.powered = false;
    localname = "help";
    localaddess = "00:00:00:00:00:00";
    currentStates = { "", ConnectionState::DISCONNECTION, false, false, false, CallState::CALL_TERMINATE, true,status, "" };
    m_connecttimer.SetTimer(1,0,timeout);
    m_disconnecttimer.SetTimer(2,0,disconnecttimeout);
    m_currentsppstate = false;

}


DeviceStubImp::~DeviceStubImp()
{

}
void DeviceStubImp::on_bus_acquired( GDBusConnection *connection,
                                     const gchar *name, gpointer user_data )
{
    //add by qianzhong 2017-5-12
    GError **error;
    interface = device_skeleton_new();
    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect( interface, "handle-start-scan", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK (deviceInquiryStart),//具体的方法处理函数, 注意增加输入参数
                      user_data );

    g_signal_connect( interface, "handle-stop-scan", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK (deviceInquiryStop),//具体的方法处理函数, 注意增加输入参数
                      user_data );

    g_signal_connect( interface, "handle-accpect-pair",
                      G_CALLBACK (accpectPair), user_data );

    g_signal_connect( interface, "handle-refuse-pair", G_CALLBACK (rejectPair),
                      user_data );

    g_signal_connect( interface, "handle-delete-pair-device",
                      G_CALLBACK (deletePaired), user_data );

    g_signal_connect( interface, "handle-connect-device", G_CALLBACK (connect),
                      user_data );

    g_signal_connect( interface, "handle-dis-connect-device",
                      G_CALLBACK (disConnect), user_data );

    g_signal_connect( interface, "handle-get-paired-list",
                      G_CALLBACK (getPairedList), user_data );

    g_signal_connect( interface, "handle-delete-pair-device",
                      G_CALLBACK (deletePaired), user_data );

//    g_signal_connect( interface, "handle-set-auto-connect",
//                      G_CALLBACK (setAutoConnection), user_data );

    g_signal_connect( interface, "handle-get-local-properties",
                      G_CALLBACK (getLocalProperties), user_data );

    g_signal_connect( interface, "handle_dis_connect_current_device",
                          G_CALLBACK (disConnectCurrentProfiles), user_data );

    g_signal_connect( interface, "handle-get-current-states",
                             G_CALLBACK (getCurrentStates), user_data );
    //set property
    device_set_bt_service_state( DeviceStubImp::interface, false );

    device_set_bt_service_state( DeviceStubImp::interface, false );
    device_set_pair_state( DeviceStubImp::interface, false );
    device_set_scan_state( DeviceStubImp::interface, false );
    device_set_power_state( DeviceStubImp::interface, false );
    set_set_power_state (SettingStubImp::m_interface, false);

    //输出interface到总线上
    if( !g_dbus_interface_skeleton_export(
                    G_DBUS_INTERFACE_SKELETON( DeviceStubImp::interface ),
                    connection, "/com/hsaeyz/bluetooth/device", //object_path
                    error ) )
    {
        /* handle error */
    }

}
std::shared_ptr< DeviceStubImp > DeviceStubImp::getInstance()
{
    if( deviceInstance == NULL )
    {
        deviceInstance = std::make_shared< DeviceStubImp >();
//        deviceInstance->initProxy();
    }
    return deviceInstance;
}
//method

gboolean DeviceStubImp::accpectPair( Device *object,
                                     GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_DEVICE,"");
    bool ret=false;
    HSAE_CMD_RET ret1 = hsaeCmdAnswerPair( NULL, &remote_pair_addr, 6, phone_pair_nf_pincode );
    ret = (ret1== HSAE_CMD_SUCCESS)?true:false;
    device_complete_accpect_pair( object, invocation );
    return TRUE;

}
gboolean DeviceStubImp::rejectPair( Device *object,
                                    GDBusMethodInvocation *invocation )
{
     LOG(BT_LOG_DEVICE,"");
     bool ret=false;
      HSAE_CMD_RET ret1 = hsaeCmdRefusePair( NULL, &remote_pair_addr);
     device_complete_refuse_pair( object, invocation );
    ret = (ret1== HSAE_CMD_SUCCESS)?true:false;
    return TRUE;

}
void DeviceStubImp::initProxy()
{

}

//
gboolean DeviceStubImp::deletePaired( Device *object,
                                      GDBusMethodInvocation *invocation,
                                      const gchar *arg_btAddress )
{
    bool ret=false;
    string btAddress = string( arg_btAddress );
    LOG(BT_LOG_DEVICE,"btAddress =%s",btAddress.data());
    BT_ADDR m_btAddress;
    size_t i;
    for( i = 0; i < 6; i++ )
    {
        m_btAddress.b[i] = 0;
    }
    str_to_char( btAddress.data(), m_btAddress.b );
    std::shared_ptr< SettingStubImp > g_set = SettingStubImp::getInstance();
     if (g_set->getautoaddr() == btAddress)
     {
         //cleardefault
         char *buf= NULL;
         g_set->setautoconnectmac(buf);
     }

    HSAE_CMD_RET ret1 = hsaeCmdDeletePairDevice( NULL, &m_btAddress );
    ret = (ret1== HSAE_CMD_SUCCESS)?true:false;
    std::shared_ptr< PhoneBookStubImp > g_pbbook = PhoneBookStubImp::getInstance();
    g_pbbook->deletecontacttable(btAddress);
    device_complete_delete_pair_device( object, invocation );
    return TRUE;

}
gboolean DeviceStubImp::connect( Device *object,
                                 GDBusMethodInvocation *invocation,
                                 const gchar *arg_btAddress,
                                 gint arg_profileId )
{
     unsigned long dialmoment = GetTickCount() - m_dwDialMoment;
     std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
     std::shared_ptr< SettingStubImp > g_set = SettingStubImp::getInstance();
     if (arg_profileId == 0)
           g_set ->closeAutoConnectThread();
    LOG(BT_LOG_DEVICE,"g_device->currentStates.hfpState =%d,dialmoment=%d",(int)g_device->currentStates.hfpState,dialmoment);
    if((dialmoment > 3000) ||(arg_profileId) >0)//如果避免用户在界面上频繁输入连接命令
    {
        // stop connect
        BT_ADDR m_btAddress;
        size_t i;
        for( i = 0; i < 6; i++ )
        {
            m_btAddress.b[i] = 0;
        }
        HSAE_CMD_RET ret;
        str_to_char( arg_btAddress, m_btAddress.b );
        if (g_device->currentStates.hfpState == ConnectionState::DISCONNECTION )
        {
// if avrcp is connect disconnect all profile by lhm 2018/2/25 add
           if (!g_device->IsArrayEmpty(DeviceStubImp::avrcp_connect_addr.b,6))
           {

               ret = hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::avrcp_connect_addr,  NPI_ALL_PROFILE );
               printf("avrcp is connected,but hfp is disconnect so we disconnect avrcp ");
           }
            else
           {
               int manu_id = 0;
               hsaeCmdGetRemoteDeviceManufacturerID(NULL,&m_btAddress,&manu_id);
               ret = hsaeCmdConnectRemoteDevice( NULL, &m_btAddress,(HSAE_PROFILEID_e)arg_profileId );

           }
            LOG(BT_LOG_DEVICE,"connect btAddress =%s,ret =%d,arg_profileId =%d\r\n",arg_btAddress,(int)ret,arg_profileId);
        }
        else  if (g_device->currentStates.hfpState == ConnectionState::CONNECTION )
        {
            if (arg_profileId == 0)
                 ret = hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::hfp_connect_addr,  NPI_ALL_PROFILE );
            else
            {
//                usleep(1000*1000);
                ret = hsaeCmdConnectRemoteDevice( NULL, &m_btAddress,(HSAE_PROFILEID_e)arg_profileId );
            }

            LOG(BT_LOG_DEVICE,"connect btAddress =%s,ret =%d,arg_profileId =%d\r\n",arg_btAddress,(int)ret,arg_profileId);

       }
        else if(g_device->currentStates.hfpState == ConnectionState::CONNECTING)
        {
            ret = hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::hfp_connecting_addr,  NPI_ALL_PROFILE );
            LOG(BT_LOG_DEVICE,"disconnect btAddress =%02X:%02X,ret =%d",DeviceStubImp::hfp_connecting_addr.b[0],\
                                                                         DeviceStubImp::hfp_connecting_addr.b[1],(int)ret);
        }

	    m_dwDialMoment = GetTickCount();
	  
    }

    device_complete_connect_device( object, invocation );
    return TRUE;

}
gboolean DeviceStubImp::deviceInquiryStart( Device *object,
                                            GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_DEVICE," ");
    hsaeCmdInquiryStart(NULL);
    device_complete_start_scan( object, invocation );
    return TRUE;
}
gboolean DeviceStubImp::deviceInquiryStop( Device *object,
                                           GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_DEVICE," ");
    hsaeCmdInquiryStop(NULL);
    device_complete_stop_scan( object, invocation );
    return TRUE;

}
//??
gboolean DeviceStubImp::disConnect( Device *object,
                                    GDBusMethodInvocation *invocation,
                                    const gchar *arg_btAddress,
                                    gint arg_profileId )
{
    unsigned long dialmoment = GetTickCount() - m_dwDialMoment;
    LOG(BT_LOG_DEVICE," arg_profileId =%d ,dialmoment = %d",arg_profileId,dialmoment);
    BT_ADDR m_btAddress;
    if( dialmoment > 3000 )
    {
        string disAddress;
        if (arg_btAddress != NULL)
         {

            disAddress = string( arg_btAddress );
            if (disAddress == "error")
            {
                m_btAddress =DeviceStubImp::getInstance()->sppaddr;

            }
            else
              str_to_char( disAddress.data(), m_btAddress.b );
        }
        else
        {
            disAddress = DeviceStubImp::getInstance()->currentStates.btAddress;
            size_t i;
            for( i = 0; i < 6; i++ )
            {
                m_btAddress.b[i] = 0;
            }
             str_to_char( disAddress.data(), m_btAddress.b );
        }

       //
        HSAE_CMD_RET ret;
        HSAE_PROFILEID_e  id  =NPI_ALL_RFCOMM;
        if (arg_profileId == 0)
        {
            ret  = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, HSAE_PROFILEID_e::NPI_ALL_PROFILE);
            printf("show %02x:%02x\r\n",m_btAddress.b[0],m_btAddress.b[1]);
        }
        else if (arg_profileId == NPI_RFCOMM_1)//disconnect chanle 1
        {
             id  =NPI_RFCOMM_1;
             printf(" m_server[0].status %d,m_client[0].status %d\r\n", \
                    DeviceStubImp::getInstance()->m_server[0].status,\
                     DeviceStubImp::getInstance()->m_client[0].status);
            if (DeviceStubImp::getInstance()->m_server[0].status ==HSAE_CON_CONNECTION )
            {
                ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_1 );
                id = NPI_RFCOMM_1;
            }
            if (DeviceStubImp::getInstance()->m_client[0].status ==HSAE_CON_CONNECTION )
            {
                ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_3 );
                id = NPI_RFCOMM_3;
            }

        }
        else if (arg_profileId == NPI_RFCOMM_2)//disconnect chanle 2
        {
            id  =NPI_RFCOMM_2;
            printf(" m_server[1].status %d,m_client[1].status %d\r\n",\
                   DeviceStubImp::getInstance()->m_server[1].status,\
                    DeviceStubImp::getInstance()->m_client[1].status);
            if (DeviceStubImp::getInstance()->m_server[1].status ==HSAE_CON_CONNECTION )
             {
                   id = NPI_RFCOMM_2;
                   ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_2 );
             }
            if (DeviceStubImp::getInstance()->m_client[1].status ==HSAE_CON_CONNECTION )
            {
                  id = NPI_RFCOMM_4;
                  ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_4 );
            }
        }
       else if (arg_profileId == NPI_ALL_RFCOMM)//disconnect chanle 1
        {
            ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_ALL_RFCOMM );
            ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_1 );
            ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_2 );
            ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_3 );
            ret = hsaeCmdDisconnectRemoteDevice( NULL, &m_btAddress, NPI_RFCOMM_4 );

        }
        std::shared_ptr< PhoneBookStubImp > g_phonebook = PhoneBookStubImp::getInstance();
        g_phonebook->cancledownload();
        LOG(BT_LOG_DEVICE,"btAddress =%s,ret =%d,profileid =%d\r\n",arg_btAddress,(int)ret,id);
        // delete
	     m_dwDialMoment = GetTickCount();
      
    }

    device_complete_dis_connect_device( object, invocation );
    return TRUE;
}
gboolean DeviceStubImp::getPairedList( Device *object,
                                       GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_DEVICE," ");
//    hsaeCmdGetPairedList( NULL );
     std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    if (pairLists.size()>0)
            g_device->BTPairListChanged(pairLists);
    else
    {
        hsaeCmdGetPairedList( NULL);
    }
    device_complete_get_paired_list( object, invocation );
    return TRUE;

}
void DeviceStubImp::BTLocalPropertiesChanged( LocalProperty property )
{
    LOG(BT_LOG_DEVICE," ");
    gchar* arg_address = (gchar*)property.address.c_str();
    gchar* arg_localName = (gchar*)property.name.c_str();
    gint arg_powerState = (int)property.powered;
    m_property.powered = device_get_power_state(DeviceStubImp::interface);
    m_property.address = arg_address;
    m_property.name = arg_localName;
    device_set_local_address(DeviceStubImp::interface,arg_address);
    device_set_local_name(DeviceStubImp::interface,arg_localName);
    device_emit_local_property_changed( DeviceStubImp::interface, arg_address,
                                        arg_localName, arg_powerState );
}
void DeviceStubImp::phoneConnnectNF()
{
    LOG(BT_LOG_DEVICE," currentStates.hfpState =%d",currentStates.hfpState);
    if (currentStates.hfpState != ConnectionState::CONNECTION)
    {
        hsaeCmdAnswerPair( NULL, &remote_pair_addr, 6, phone_pair_nf_pincode );
    }
    else
    {
        hsaeCmdRefusePair( NULL, &remote_pair_addr );
    }
}

void DeviceStubImp::clearpairlist()
{
     LOG(BT_LOG_DEVICE," ");
     for (int i =0;i<pairLists.size();i++)
     {
         //deletedevice
           BT_ADDR m_btAddress;
           size_t j;
           for( j = 0; j < 6; j++ )
           {
               m_btAddress.b[j] = 0;
           }
           str_to_char( pairLists[i].btAddress.data(), m_btAddress.b );
           HSAE_CMD_RET ret1 = hsaeCmdDeletePairDevice( NULL, & m_btAddress );
           LOG(BT_LOG_DEVICE,"ret1 =%d ",ret1);
     }
   if (pairLists.size()>0)
   {
         vector<Pair>().swap(pairLists);
   }

}

//??
gboolean DeviceStubImp::getLocalProperties( Device *object,
                                            GDBusMethodInvocation *invocation )
{
     LOG(BT_LOG_DEVICE," ");
#if 1
    std::shared_ptr< DeviceStubImp > pDevice = DeviceStubImp::getInstance();
    LocalProperty property;
    property.name  = pDevice->localname;
    property.address = pDevice->localaddess;
    //set current states
    HSAE_POWER HSAEPower;
    hsaeCmdGetPower( NULL, &HSAEPower );
    if( HSAEPower )
    {
        property.powered = true;
    }
    else
    {
        property.powered = false;
    }
    pDevice->BTLocalPropertiesChanged( property );
   if (pairLists.size()==0)
   {
       hsaeCmdGetPairedList( NULL);
   }
#endif
    device_complete_get_local_properties( object, invocation );
    return TRUE;
}
//gboolean DeviceStubImp::setAutoConnection( Device *object,
//                                           GDBusMethodInvocation *invocation,
//                                           gint arg_retry )
//{
//    LOG(BT_LOG_DEVICE," ");
//    hsaeCmdSetAutoConnectionEnable( NULL, (int)arg_retry );
//    device_complete_set_auto_connect( object, invocation );
//    return TRUE;
//}

gboolean DeviceStubImp::getCurrentStates( Device *object,
                                          GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_DEVICE," ");
    std::shared_ptr< DeviceStubImp > pDevice = DeviceStubImp::getInstance();
    pDevice->BTCurrentStateChanged( pDevice->currentStates );
    device_complete_get_current_states( object, invocation );
    return TRUE;
}
gboolean DeviceStubImp::startScan( Device *object,
                                   GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_DEVICE," ");
    hsaeCmdInquiryStart(NULL);
    device_complete_start_scan( object, invocation );
    return TRUE;
}
gboolean DeviceStubImp::stopScan(Device *object,
								 GDBusMethodInvocation *invocation)
{
    LOG(BT_LOG_DEVICE," ");
    hsaeCmdInquiryStop(NULL);
	device_complete_stop_scan(object, invocation);
	return TRUE;
}
//add by qianzhong 2017-7-4
gboolean DeviceStubImp::disConnectCurrentProfiles(Device *object,
		 GDBusMethodInvocation *invocation)
{

    LOG(BT_LOG_DEVICE," ");
    hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::hfp_connect_addr,
	                                      HSAE_PROFILEID_e::NPI_ALL_AUDIO );
	 device_complete_dis_connect_current_device(object,invocation);
	 return TRUE;
}


void DeviceStubImp::getID3Info()
{
    LOG(BT_LOG_DEVICE," ");
    std::shared_ptr< MediaPlayBackStubImp > pMusic = MediaPlayBackStubImp::getInstance();
    pMusic->TrancNowPlayAttributes( PLAYATTRIBUTE );
}

//brodcast
//void DeviceStubImp::BTSerachChanged( std::string pba, std::string deviceName )
//{
//
//}
void DeviceStubImp::BTPairListChanged( Pairs pairList )
{
    //
    LOG(BT_LOG_DEVICE," ");
    Json::Value root;
    Json::Value pair_list;
    Json::Value pair_item;
    for( int i = 0; i < pairList.size(); i++ )
    {
        if( NULL == pairList[i].btDeviceName.c_str() || pairList[i].btDeviceName
                        .empty() )
        {
            pair_item[DEVICENAME] = "";
        }
        else
        {
            pair_item[DEVICENAME] = pairList[i].btDeviceName;
        }
        if( NULL != pairList[i].btAddress.c_str() && !pairList[i].btAddress
                        .empty() )
        {
            pair_item[BTADDRESS] = pairList[i].btAddress;
        }
        pair_list.append( pair_item );
    }
    root[PAIRLIST] = pair_list;
    Json::FastWriter writer;
    string pairlist = writer.write( root );
    device_emit_pair_list_changed( DeviceStubImp::interface, pairlist.c_str() );
}
void DeviceStubImp::BTConnectionStateChanged( string address,
                                              PROFILEID profileId,
                                              ConnectionState state )
{
    LOG(BT_LOG_DEVICE," profileId =%d,state =%d",profileId,state);
    gchar *arg_address = (gchar*)address.c_str();
    gint arg_profileId = (gint)profileId;
    gint arg_connectState = (gint)state;
    device_emit_connect_state_changed( DeviceStubImp::interface, arg_address,
                                       arg_profileId, arg_connectState );

}

void DeviceStubImp::BTCurrentStateChanged( CurrentStates currentStates )
{
    LOG(BT_LOG_DEVICE," ");
    gchar *arg_btAddress = (gchar*)currentStates.btAddress.c_str();
    gint arg_hfpConnnectState = (gint)currentStates.hfpState;
    gboolean arg_pbapConnnectState = currentStates.pbapState;
    gboolean arg_avrcpConnnectState = currentStates.avrcpState;
    gboolean arg_a2cpConnnectState = currentStates.a2dpState;
    gint arg_callState = (gint)currentStates.callState;
    gchar* arg_phoneNumber = (gchar*)currentStates.phoneNumber.c_str();
    gboolean arg_audioState = currentStates.audioState;
    gint arg_playState = (gint)currentStates.playStatus.playState;
    gchar *arg_songLength = (gchar*)currentStates.playStatus.positon.songLength
                    .c_str();
    gchar *arg_playPosition = (gchar*)currentStates.playStatus.positon
                    .songPosition.c_str();

    device_emit_current_state_changed( DeviceStubImp::interface, arg_btAddress,
                                       arg_hfpConnnectState,
                                       arg_pbapConnnectState,
                                       arg_avrcpConnnectState,
                                       arg_a2cpConnnectState, arg_callState,
                                       arg_phoneNumber, arg_audioState,
                                       arg_playState, arg_songLength,
                                       arg_playPosition );

}
void DeviceStubImp::BTSearchChanged(string name,string addr)
{
    LOG(BT_LOG_DEVICE," ");
    device_emit_search_device_changed(DeviceStubImp::interface,
                                      name.c_str(),
                                      addr.c_str());
}
void DeviceStubImp::BTBatteryChanged(int battery)
{
    LOG(BT_LOG_DEVICE," ");
    device_emit_battery_changed(DeviceStubImp::interface,battery);
}

void DeviceStubImp::chartostr(char *strmac, unsigned char *mac)
{
     //lhm20200414
    sprintf( strmac, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3],mac[2], mac[1], mac[0] );
}

bool DeviceStubImp::IsArrayEmpty(unsigned char *buf,int len)
{

    int ret =true;
    for (int i =0;i<len;i++)
    {
        if (buf[i] != 0)
        {
            ret =false;
            break;
        }
    }
    return ret;
}

bool DeviceStubImp::IsCompareArray(unsigned char *buf1,int len1,unsigned char *buf2,int len2)
{
    bool ret =true;
    if  (len1 == len2)
    {
        for (int i = 0 ; i < len1 ; i++ )
        {
            if (buf1[i] != buf2[i])
            {
                ret =false;
                break;
            }
        }

    }
    else
       ret =false;

     return ret;
}


unsigned long DeviceStubImp::GetTickCount()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    unsigned long temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return temp;
}

void DeviceStubImp::timeout()
{

    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    LOG(BT_LOG_DEVICE," hfpState =%d ,callstate =%d",g_device->currentStates.hfpState,g_device->currentStates.callState);
    g_device->m_cout++;
    if (g_device->currentStates.hfpState != ConnectionState::DISCONNECTION)
    {
        if (g_device->m_cout > 2)
        {
            g_device->m_cout =0;
            if ( g_device->currentStates.hfpState == ConnectionState::CONNECTION)
            {
                //downcallog
               std::shared_ptr< SPPStubImp > g_spp = SPPStubImp::getInstance();
               HSAE_CMD_RET ret;
               if (g_device->m_currentsppstate == false)
               {
                   int manu_id = 0;
                   hsaeCmdGetRemoteDeviceManufacturerID(NULL,&g_device->hfp_connect_addr,&manu_id);
//                   printf("manu_id =%d \r\n",manu_id);
//                   if (manu_id ==1 )
//                        ret=  hsaeCmdConnectRemoteDevice( NULL, &g_device->hfp_connect_addr,NPI_SPP_IAP );
//                    else
//                   {
//                        ret = hsaeCmdConnectRemoteDevice( NULL, &g_device->hfp_connect_addr,NPI_SPP );
//                        printf("-----create connect spp =%d\r\n-------",ret);
//                   }
               }

               std::shared_ptr< PhoneBookStubImp > g_book = PhoneBookStubImp::getInstance();

               if ( g_device->currentStates.callState == CallState::CALL_TERMINATE)
               {
//                   g_book->downloadcalllogs(30);
               }
            }
             g_device->m_connecttimer.StopTimer();
        }
    }
    else
    {
        g_device->m_cout =0;
        g_device->m_connecttimer.StopTimer();
    }

}

void DeviceStubImp::disconnecttimeout()
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    if(g_device->currentStates.hfpState == ConnectionState::DISCONNECTION)
    {
       if (strlen((char *)(g_device->sppaddr.b))>0)
       {

            hsaeCmdDisconnectRemoteDevice( NULL, &g_device->sppaddr,NPI_ALL_PROFILE );
//            printf("spp is not disconnect ,so disconnect spp,g_device->m_currentspp =%d\r\n",(int)g_device->m_currentspp);
       }
    }
    g_device->m_disconnecttimer.StopTimer();
}
//void DeviceStubImp::phoneConnnectNF()
//{
//    hsaeCmdAnswerPair( NULL, &remote_pair_addr, 6, phone_pair_nf_pincode );
//}


