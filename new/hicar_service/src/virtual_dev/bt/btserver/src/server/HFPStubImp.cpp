#include "HFPStubImp.h"
#define HFP_ASYNC
using namespace std;
std::shared_ptr< HFPStubImp > HFPStubImp::phoneInstance = NULL;
CallState HFPStubImp::callstate = CallState::CALL_TERMINATE;
#include "alsa_wav_play.h"
Hfp* HFPStubImp::m_interface = NULL;
gboolean HFPStubImp::m_privateState = false;
string HFPStubImp::PHONE_STATE = "";
#include "stdio.h"
#include "time.h"
extern bool g_stopRingRun;
//add by qianzhong 2017-5-31
HSAE_CBK_RET hf_subscriberinfo(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber)
{
    printf( "The subscriberinfo of the phone %02X:%02X:%02X:%02X:%02X:%02X is (%s) \r\n ",
           ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] , pwzNumber);

    if  (pwzNumber !=NULL)
    {
        if (strlen(pwzNumber)<21)
        {
            if (pwzNumber[0] == '+')
                memcpy(HFPStubImp::phoneInstance->m_localnumer,pwzNumber,strlen(pwzNumber));
            else
             {

                HFPStubImp::phoneInstance->m_localnumer[0]='+';
                memcpy(HFPStubImp::phoneInstance->m_localnumer+1,pwzNumber,strlen(pwzNumber));
             }
        }
    }
     else  
         memset(HFPStubImp::phoneInstance->m_localnumer,20, 0);
    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET handsfree_status( void *nfmCbkParam, const BT_ADDR ba,
                              const HSAE_HANDSFREE_STATE state )
{
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    char *m_data = (char*)malloc( 255 );
    g_phone->chartostr(m_data,(unsigned char*)&ba.b);
    string deviceAddress = string( m_data );
    if( state == HSAE_HF_STATE_DISCONNECTED )
    {

        g_device->BTConnectionStateChanged( deviceAddress, PROFILEID::HANDFREE,
                                            ConnectionState::DISCONNECTION );
    }
    else if( state == HSAE_HF_STATE_INITIALISING )
    {
        g_device->BTConnectionStateChanged( deviceAddress, PROFILEID::HANDFREE,
                                            ConnectionState::CONNECTING );
    }

    else if( state == HSAE_HF_STATE_READY )
    {
        g_device->BTConnectionStateChanged( deviceAddress, PROFILEID::HANDFREE,
                                            ConnectionState::CONNECTION );
    }

    free( m_data );
    m_data = NULL;
    LOG(BT_LOG_HFP," HSAE_HANDSFREE_STATE is %d",(int)state);
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET call_terminated( void *nfmCbkParam, const BT_ADDR ba,
                             const char *pwzNumber, unsigned char idx )
{

    LOG(BT_LOG_HFP," The phone call of %s on %02X:%02X:%02X:%02X:%02X:%02X has been terminated at line %d",pwzNumber, ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5],idx);
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();
    char *m_data = (char*)malloc( 255 );
    g_phone->chartostr(m_data,(unsigned char*)&ba.b);
    string deviceAddress = string( m_data );
 
   // g_phone->callInNumberChanged( string( pwzNumber ),(int)idx);
    if (pwzNumber[0] != '0')
    {
        if (strcmp(pwzNumber,g_phone->m_localnumer) !=0 )
        g_phone->callStateChanged( CallState::CALL_TERMINATE ,(int)idx);
    }
    else
    {
        //notify change  audio
//        hfp_emit_hsaeasr_state_changed(HFPStubImp::m_interface,m_data,false);
//        hfp_set_hsaeasr_state(HFPStubImp::m_interface,false);
//        g_phone->m_localnumer ="";
        memset(g_phone->m_localnumer,20, 0);
    }
    free( m_data );
    m_data = NULL;
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET current_call( void *nfmCbkParam, const BT_ADDR ba,
                          const char *pwzNumber, unsigned char idx )
{
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();

    LOG(BT_LOG_HFP," The phone call of %s on %02X:%02X:%02X:%02X:%02X:%02X has been INCOME_CALL at line %d",pwzNumber, ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5],idx);
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    if (g_device->currentStates.hfpState == ConnectionState::CONNECTION)
    {

        char *m_data = (char*)malloc( 255 );
        g_phone->chartostr(m_data,(unsigned char*)&ba.b);
        printf("pwzNumber len =%d\r\n",strlen(pwzNumber));
         if (pwzNumber[0] != '0')
        {
           // commpare is phones  number
            if (strcmp(pwzNumber,g_phone->m_localnumer) !=0 )
            {
                g_phone->callStateChanged( CallState::INCOME_CALL,(int)idx);
                g_phone->callInNumberChanged( string( pwzNumber ) ,(int)idx);
            }
        }
        else
        {
            //notify change  audio
//            const gchar *arg_addr = ba.b;
//            hfp_emit_hsaeasr_state_changed(HFPStubImp::m_interface,m_data,true);
//            hfp_set_hsaeasr_state(HFPStubImp::m_interface,true);
        }
        free( m_data );
        m_data = NULL;
    }
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET handsfree_ring( void *nfmCbkParam, const BT_ADDR ba )
{

    LOG(BT_LOG_HFP,"   %02X:%02X:%02X:%02X:%02X:%02X ", ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5]);

    return HSAE_CBK_SUCCESS;
}


HSAE_CBK_RET call_hold( void *nfmCbkParam, const BT_ADDR ba,
                       const char *pwzNumber, unsigned char idx )
{
    LOG(BT_LOG_HFP,"  ");
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();

    //set current status
     if (pwzNumber[0] != '0')
    {
      if (strcmp(pwzNumber,g_phone->m_localnumer) !=0 )
        g_phone->callStateChanged( CallState::CALL_HOLD ,(int )idx);
    }
    else
    {
        //notify change  audio
//        hfp_emit_hsaeasr_state_changed(HFPStubImp::m_interface,m_data,false);

//        hfp_set_hsaeasr_state(HFPStubImp::m_interface,false);
    }

    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET call_active( void *nfmCbkParam, const BT_ADDR ba,
                         const char *pwzNumber, unsigned char idx )
{
    LOG(BT_LOG_HFP,"  %d, numlen =%d\r\n",idx,strlen(pwzNumber));
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    if (g_device->currentStates.hfpState == ConnectionState::CONNECTION)
    {
        std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();
        char *m_data = (char*)malloc( 255 );
         g_phone->chartostr(m_data,(unsigned char*)&ba.b);


        if (pwzNumber[0] != '0')
        {   
          printf("call active =%d\r\n",strcmp(pwzNumber,g_phone->m_localnumer));
          printf("call pwzNumber  =%s, m_localnumer =%s\r\n",pwzNumber ,g_phone->m_localnumer);
          if (strcmp(pwzNumber,g_phone->m_localnumer) !=0 )
         {
              g_phone->callStateChanged( CallState::CALL_ACTIVE ,(int)idx);
              g_phone->callInNumberChanged( string(pwzNumber),(int)idx);
          }

        }
        else
        {
            //notify change  audio      
//            hfp_emit_hsaeasr_state_changed(HFPStubImp::m_interface,m_data,true);
//            hfp_set_hsaeasr_state(HFPStubImp::m_interface,true);
        }

        free( m_data );
        m_data = NULL;
    }
     printf("3333\r\n");
    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET mpty_call_hold( void *nfmCbkParam, const BT_ADDR ba,
                            const HSAE_HF_MPTY_INFO *pMptyInfo,
                            const unsigned char mpty_line_count )
{
    LOG(BT_LOG_HFP," mpty_line_count=%d ",mpty_line_count);

    unsigned char i;
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();
  //  g_phone->callStateChanged( CallState::MPTY_CALL_HOLD,(int)mpty_line_count);

    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET mpty_call_active( void *nfmCbkParam, const BT_ADDR ba,
                              const HSAE_HF_MPTY_INFO *pMptyInfo,
                              const unsigned char mpty_line_count )
{
     LOG(BT_LOG_HFP," mpty_line_count=%d ",mpty_line_count);
    char *m_data = (char*)malloc( 255 );


    unsigned char i;
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();  
  //  g_phone->callStateChanged( CallState::MPTY_CALL_ACTIVE ,(int)mpty_line_count);
    free( m_data );
    m_data = NULL;
    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET outgoing_call( void *nfmCbkParam, const BT_ADDR ba,
                           const char *pwzNumber, unsigned char idx )
{
    LOG(BT_LOG_HFP," pwzNumber=%s ",pwzNumber);
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    if (g_device->currentStates.hfpState == ConnectionState::CONNECTION)
    {
        std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();

        char *m_data = (char*)malloc( 255 );
        g_phone->chartostr(m_data,(unsigned char*)&ba.b);
       if (pwzNumber[0] != '0')
       {
        if (strcmp(pwzNumber,g_phone->m_localnumer) !=0 )
          {
             g_phone->callStateChanged( CallState::OUTGONING_CALL ,(int)idx);
             g_phone->callInNumberChanged( string(pwzNumber),(int)idx);
          }
       }
       else
       {
           //notify change  audio
       }
        free( m_data );
        m_data = NULL;
    }
    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET hf_sco_status( void *nfmCbkParam, const BT_ADDR ba, const bool on )
{
    std::shared_ptr< HFPStubImp > g_phone = HFPStubImp::getInstance();
    g_phone->scoStateChanged( on );
    LOG(BT_LOG_HFP," on=%d ",(int)on);
    return HSAE_CBK_SUCCESS;
}


HSAE_CBK_RET hf_signal_strength( void *nfmCbkParam, const BT_ADDR ba,
                                const HSAE_HANDSFREEP_SIGNASL_STRENGTH strength )
{
//    printf( "The signal strength of %02X:%02X:%02X:%02X:%02X:%02X is now level ",
//            ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );

//    switch( strength )
//    {
//    case SIGNASL_STRENGTH_LEVEL_0:
//        printf( "0.\n\n" );
//        break;
//    case SIGNASL_STRENGTH_LEVEL_1:
//        printf( "1.\n\n" );
//        break;
//    case SIGNASL_STRENGTH_LEVEL_2:
//        printf( "2.\n\n" );
//        break;
//    case SIGNASL_STRENGTH_LEVEL_3:
//        printf( "3.\n\n" );
//        break;
//    case SIGNASL_STRENGTH_LEVEL_4:
//        printf( "4.\n\n" );
//        break;
//    case SIGNASL_STRENGTH_LEVEL_5:
//        printf( "5.\n\n" );
//        break;
//    default:
//        printf( "(error).\n\n" );
//        break;
//    }
     LOG(BT_LOG_HFP,"SIGNASL_STRENGTH_LEVEL =%d ",(int)strength);
    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET print_hf_command_response( void *nfmCbkParam, const BT_ADDR ba,
                                       const HSAE_HANDSFREEP_AT_COMMAND command,
                                       const char *rsp )
{

      LOG(BT_LOG_HFP,"command =%d ",(int)command);
//    switch( command )
//    {
//    case HSAE_HF_ANSWER:
//        printf( "\"answer call\".\n\n" );
//        break;
//    case HSAE_HF_DIAL:
//        printf( "\"dial call\".\n\n" );
//        break;
//    case HSAE_HF_DTMF:
//        printf( "\"send DTMF\".\n\n" );
//        break;
//    case HSAE_HF_REDIAL:
//        printf( "\"re-dial call\".\n\n" );
//        break;
//    case HSAE_HF_HANGUP:
//        printf( "\"hang up call\".\n\n" );
//        break;
//    case HSAE_HF_HOLD:
//        printf( "\"hold call\".\n\n" );
//        break;
//    case HSAE_HF_SPK_VOL:
//        printf( "\"set speaker volume\".\n\n" );
//        break;
//    case HSAE_HF_MIC_VOL:
//        printf( "\"set microphone volume\".\n\n" );
//        break;
//    case HSAE_HF_XAPL:
//        printf( "\"Apple custom AT commands\".\n\n" );
//        break;
//    case HSAE_HF_VR:
//        printf( "\"Voice Recognition\".\n\n" );
//        break;
//    case HSAE_HF_APLEFM:
//        printf( "\"Apple Siri Eye Free Mode\".\n\n" );
//        break;
//    case HSAE_HS_ANSWER:
//        printf( "\"answer\\hang up call by headset\".\n\n" );
//        break;
//    case HSAE_HS_HANGUP:
//        printf( "\"answer\\hang up call by headset\".\n\n" );
//        break;
//    case HSAE_HF_GET_CURRECT_CALL_REJECT:
//        printf( "\"get current call fail by handfree\".\n\n" );
//        break;
//    default:
//        printf( "(unknown).\n\n" );
//        break;
//    }

    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET hf_command_OK( void *nfmCbkParam, const BT_ADDR ba,
                           const HSAE_HANDSFREEP_AT_COMMAND command )
{
    return print_hf_command_response( nfmCbkParam, ba, command, "succeed" );
}

HSAE_CBK_RET hf_command_error( void *nfmCbkParam, const BT_ADDR ba,
                              const HSAE_HANDSFREEP_AT_COMMAND command )
{
    return print_hf_command_response( nfmCbkParam, ba, command, "failed" );
}
//end add

std::shared_ptr< HFPStubImp > HFPStubImp::getInstance()
{
    if( phoneInstance == NULL )
    {
        phoneInstance = std::make_shared< HFPStubImp >();
    }
    return phoneInstance;
}
HFPStubImp::HFPStubImp()
{
//    m_localnumer = "";
    memset(m_localnumer,20, 0);
    m_needdownload = true;
    m_playRingTong = true;
    ringing =false;
    m_dwDialMoment = 0;
    playringtone_thread_id =0;
    threeway =false;
    m_downloadtimer.SetTimer(1,0,timeout);
    //REGISTER CALLBACK
    hsaeCmdSetCbk_HandsfreeStatus( NULL, handsfree_status );
    hsaeCmdSetCbk_HandsfreeCurrentCall( NULL, current_call );
    hsaeCmdSetCbk_HandsfreeRing( NULL, handsfree_ring );
    hsaeCmdSetCbk_HandsfreeCallTerminated( NULL, call_terminated );
    hsaeCmdSetCbk_HandsfreeCallHold( NULL, call_hold );
    hsaeCmdSetCbk_HandsfreeCallActive( NULL, call_active );
    hsaeCmdSetCbk_HandsfreeMptyCallHold( NULL, mpty_call_hold );
    hsaeCmdSetCbk_HandsfreeMptyCallActive( NULL, mpty_call_active );
    hsaeCmdSetCbk_HandsfreeOutgoingCall( NULL, outgoing_call );
    hsaeCmdSetCbk_HandsfreeScoStatus( NULL, hf_sco_status );
    hsaeCmdSetCbk_HandsfreeSignalStrength( NULL, hf_signal_strength );
    hsaeCmdSetCbk_HandsfreeSubscriberInfo( NULL,  hf_subscriberinfo);

}
void HFPStubImp::inithfp()
{
    m_dwDialMoment = 0;
    m_playRingTong =true;
    ringing =false;
    std::shared_ptr< PhoneBookStubImp > g_phonebook = PhoneBookStubImp::getInstance();
    g_phonebook->initpbdata();
}
HFPStubImp::~HFPStubImp()
{
}
void HFPStubImp::play(bool state)
{
   LOG(BT_LOG_HFP,"state = %d",state);
   if (state)
   {
       ringing =true;
       string path =LOCAL_FILE_RINGTONE;
       startAlsaPlaying((char*)path.c_str(),true);
       if ( playringtone_thread_id == 0)
       {
            g_bRun =true;
            int ret_thr= pthread_create(&playringtone_thread_id, NULL, playringFunc, NULL);
            if (ret_thr!=0)
            {
                 //  printf("[bt ringtone] Start ring play tone fail");
            }
             else
            {
              //  printf("[bt ringtone] Start ring tone play success");
                pthread_detach(playringtone_thread_id);
            }
       }

   }
   else
   {
      //stop
//       if (ringing)
//       {
           g_bRun =false;
           setringtone(true);
//           while ( playringtone_thread_id != 0) {
//               cout<<"-----------HFPStubImp::stopRingTone-------"<<playringtone_thread_id<<endl;
//               usleep(1000*100);
//           }
//           stopAlsaPlaying();
           printf("[bt ringtone] stop ring tone play success");
            ringing =false;
//       }
   }
}

void HFPStubImp::on_bus_acquired( GDBusConnection *connection,
                                  const gchar *name, gpointer user_data )
{
    GError **error;

    //创建一个skeleton对象,以注册输出对象
    HFPStubImp::m_interface = hfp_skeleton_new();

    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect( HFPStubImp::m_interface, "handle-answer-call", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(answerCall),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-reject-call", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(rejectCall),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-terminate-call", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(terminateCall),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-hold-call", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(holdCall),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-send-dtmf", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(dialDTMF),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-dail-call", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(dailCall),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-audio-transfer", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(audioTransfer),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-set-call-auto-action", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(setCallAutoAction),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-play-ring-tone", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(playRingTone),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle-stop-ring-tone", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(stopRingTone),//具体的方法处理函数, 注意增加输入参数
                      user_data );
    g_signal_connect( HFPStubImp::m_interface, "handle_mute_mic", //signal_name, 在.c文件中的_default_init查找
                      G_CALLBACK(mutemic),//具体的方法处理函数, 注意增加输入参数
                      user_data );

    hfp_set_private_state( HFPStubImp::m_interface, m_privateState );

    //输出interface到总线上
    if( !g_dbus_interface_skeleton_export(
                    G_DBUS_INTERFACE_SKELETON( HFPStubImp::m_interface ),
                    connection, "/com/hsaeyz/bluetooth/HFP", //object_path
                    error ) )
    {
        /* handle error */
    }
}

gboolean HFPStubImp::answerCall( Hfp *interface,
                                 GDBusMethodInvocation *invocation )
{
     LOG(BT_LOG_HFP,"");
   //  play(false);
    hsaeCmdAnswerCall( NULL, &DeviceStubImp::hfp_connect_addr );

    hfp_complete_answer_call( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::rejectCall( Hfp *interface,
                                 GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_HFP,"");
    hsaeCmdRejectCall( NULL, &DeviceStubImp::hfp_connect_addr );
    hfp_complete_reject_call( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::terminateCall( Hfp *interface,
                                    GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_HFP,"");
 //   play(false);
    hsaeCmdTerminateCall( NULL, &DeviceStubImp::hfp_connect_addr );
    hfp_complete_terminate_call( interface, invocation );
    return TRUE;
}
void HFPStubImp::rejectandterminate()
{

}
gboolean HFPStubImp::holdCall( Hfp *interface,
                               GDBusMethodInvocation *invocation, gint action )
{
    LOG(BT_LOG_HFP,"");
    unsigned int specified_call_number = 0;
    hsaeCmdHoldCall( NULL, &DeviceStubImp::hfp_connect_addr,
                    CALL_HELD_HOLD_CALLS, specified_call_number );
    hfp_complete_hold_call( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::dialDTMF( Hfp *interface,
                               GDBusMethodInvocation *invocation,
                               const gchar* dtmf )
{
    LOG(BT_LOG_HFP,"");
    char *m_phoneNumber = const_cast< char* >( dtmf );
    hsaeCmdSendDTMF( NULL, &DeviceStubImp::hfp_connect_addr, m_phoneNumber );
    hfp_complete_send_dtmf( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::dailCall( Hfp *interface,
                               GDBusMethodInvocation *invocation,
                               const gchar* number )
{
    LOG(BT_LOG_HFP,"");
    char *m_phoneNumber = const_cast< char* >( number );
    hsaeCmdDialCall( NULL, &DeviceStubImp::hfp_connect_addr, m_phoneNumber );
    hfp_complete_dail_call( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::audioTransfer( Hfp *interface,
                                    GDBusMethodInvocation *invocation,
                                    gboolean iActive )
{
    LOG(BT_LOG_HFP,"%d",(int)iActive);
    hsaeCmdAudioTransfer( NULL, &DeviceStubImp::hfp_connect_addr, iActive );
    hfp_complete_audio_transfer( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::setCallAutoAction( Hfp *interface,
                                        GDBusMethodInvocation *invocation,
                                        gint action )
{
    LOG(BT_LOG_HFP,"%d",action);
//    hsaeCmdMicrophoneMute( NULL, &DeviceStubImp::hfp_connect_addr, iActive );
    hfp_complete_set_call_auto_action( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::playRingTone( Hfp *interface,
                                   GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_HFP,"play");

    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    string path =LOCAL_FILE_RINGTONE;
   if (g_device->currentStates.callState == CallState::INCOME_CALL)
   {
       shared_ptr<HFPStubImp> p_phone = HFPStubImp::getInstance();
       if (p_phone->playringtone_thread_id == 0)
        {
            startAlsaPlaying((char*)path.c_str(),true);
            p_phone->g_bRun =true;
            int ret_thr= pthread_create(&p_phone->playringtone_thread_id, NULL, playringFunc, NULL);
             if (ret_thr!=0)
             {
                    printf("[bt ringtone] Start ring tone fail");
             }
              else
             {
                 printf("[bt ringtone] Start ring tone success");
                 pthread_detach(p_phone->playringtone_thread_id);
             }
        }
    }
    hfp_complete_play_ring_tone( interface, invocation );
    return TRUE;
}
gboolean HFPStubImp::stopRingTone( Hfp *interface,
                                   GDBusMethodInvocation *invocation )
{
    LOG(BT_LOG_HFP,"");
    shared_ptr<HFPStubImp> p_phone = HFPStubImp::getInstance();
    p_phone->g_bRun =false;
    setringtone(true);
//    while (p_phone->playringtone_thread_id!=0) {
//       cout<<"-----------HFPStubImp::stopRingTone-------"<<p_phone->playringtone_thread_id<<endl;
//       usleep(1000*100);
//    }
//    stopAlsaPlaying();

    hfp_complete_stop_ring_tone( interface, invocation );
    return TRUE;
}

gboolean HFPStubImp::mutemic(Hfp *interface, GDBusMethodInvocation *invocation, gboolean iActive)
{
    LOG(BT_LOG_HFP,"");
    hsaeCmdMicrophoneMute( NULL, &DeviceStubImp::hfp_connect_addr, iActive );
    hfp_complete_mute_mic( interface, invocation );
     return TRUE;
}
//broadcast

void HFPStubImp::callInNumberChanged( std::string phoneNumber ,int index)
{
     LOG(BT_LOG_HFP,"phoneNumber =%s",phoneNumber.data());
     std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
     if (phoneNumber != g_device->currentStates.phoneNumber)
     {
          g_device->currentStates.phoneNumber = phoneNumber;
          std::shared_ptr< PhoneBookStubImp > g_phonebook = PhoneBookStubImp::getInstance();
          string name = g_phonebook->getnamebynum( g_device->currentStates.btAddress ,phoneNumber);
          g_device->currentStates.phonename = name;
          LOG(BT_LOG_HFP,"name =%s",name.data());
          hfp_emit_call_number_changed( HFPStubImp::m_interface,name.c_str(), phoneNumber.c_str(),index-1);
  #ifdef USED_SQL
        if (index ==1)
         {

            call1.num = phoneNumber;
            call1.name = name;
        }
        else if (index ==2)
        {
            call2.num = phoneNumber;
            call2.name = name;
        }

  #endif
     }
}
//void HFPStubImp::callStateChanged( CallState callState ,int index)
//{
//    LOG(BT_LOG_HFP,"callState =%d",(int)callState);
//    DeviceStubImp::currentStates.callState = callState;
//    gint m_callState = (gint)callState;
//    hfp_emit_call_state_changed( HFPStubImp::m_interface, m_callState );
//}
void HFPStubImp::callStateChanged(CallState callState,int index )
{
    LOG(BT_LOG_HFP,"callState =%d, index =%d,call status =%s",(int)callState,index,call1.dir.data());
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
     std::shared_ptr< PhoneBookStubImp > g_phonebook = PhoneBookStubImp::getInstance();
    gint m_callState = (gint)callState;
    if (index ==2)
    {
        if (callState != CallState::CALL_TERMINATE)
        {
           // g_device->currentStates.callState = callState;
            threeway =true;
        }
        else
        {

        if (threeway)
        {

            g_device->currentStates.callState = callState;
            hfp_emit_call_state_changed( HFPStubImp::m_interface, m_callState,1 );
            threeway = false;
            g_device->currentStates.phoneNumber = "";
            g_device->currentStates.phonename = "";
 #ifndef USE_DOWNLOADCALLLOG
            BT_ADDR rdbtaddr1 ;
            memcpy(rdbtaddr1.b, DeviceStubImp::hfp_connect_addr.b, 6);
            char btAddress1[20]={0};// = (char*)malloc( 50 );
            g_device->chartostr(btAddress1,(unsigned char*)&rdbtaddr1.b);
            NewContact contact1;
            contact1.name = call2.name;
            contact1.phoneNumber = call2.num;
            contact1.call_info.date =call2.startdate;
            if (!call2.active )
                contact1.call_info.time ="0";
            else
            {
                contact1.call_info.time =gettalktime(call2.starttime);
            }

            if ((call2.dir == "missed")&&(call2.active))
            {
                call2.dir = "received";
            }
              contact1.call_info.dir = call2.dir;
            std::shared_ptr< PhoneBookStubImp > g_book = PhoneBookStubImp::getInstance();
            g_book->addcalllog(string(btAddress1),contact1);
            call2.active =false;
            call2.dir ="unknown";

#else
            if (g_phonebook->downloadcalled)
            {

                if ( !g_device->currentStates.phoneNumber.empty())
                m_downloadtimer.StartTimer();
            }
            else
            {
                 m_downloadtimer.StartTimer();
            }
#endif
            return;
          }
        }
    }
   printf("----------callState = %d,g_device->currentStates.callState=%d",callState,g_device->currentStates.callState);
   if (callState!= g_device->currentStates.callState)
    {

        g_device->currentStates.callState = callState;
       if (callState == CallState::CALL_TERMINATE)
        {

               play(false);
//               cout<<"=-----------g_phonebook->downloadcalled-============="<<(int)g_phonebook->downloadcalled<<endl;
//               if (g_phonebook->downloadcalled)
//               {
//                   if ( g_device->currentStates.phoneNumber !="")
//                      g_phonebook->downloadcalllogs(1);
//               }
//               else
//               {
//                  g_phonebook->downloadcalllogs(30);
//               }
                if (g_phonebook->downloadcalled)
                {

                    if ( !g_device->currentStates.phoneNumber.empty())
                    m_downloadtimer.StartTimer();
                }
                else
                {
                     m_downloadtimer.StartTimer();
                }
               g_device->currentStates.phoneNumber = "";
               g_device->currentStates.phonename = "";
#ifndef  USE_DOWNLOADCALLLOG
               //addcallog
               BT_ADDR rdbtaddr ;
               memcpy(rdbtaddr.b, DeviceStubImp::hfp_connect_addr.b, 6);
               char btAddress[20]={0};// = (char*)malloc( 50 );
               g_device->chartostr(btAddress,(unsigned char*)&rdbtaddr.b);
               NewContact contact;
               contact.name = call1.name;
               contact.phoneNumber = call1.num;
               contact.call_info.date =call1.startdate;
               if (!call1.active )
                   contact.call_info.time ="0";
               else
               {
                   contact.call_info.time =gettalktime(call1.starttime);
               }
           //    cout<<"ddddddddddddddddddddddddddddddddddddddddddddddddd"<<(int)call1.active<<endl;
               if ((call1.dir == "missed")&&(call1.active))
               {
                   call1.dir = "received";

               }
               contact.call_info.dir = call1.dir;
               std::shared_ptr< PhoneBookStubImp > g_book = PhoneBookStubImp::getInstance();
               g_book->addcalllog(string(btAddress),contact);
               call1.dir == "unknown";
 #endif                //  std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance(); stop rine


       }

       else  if (callState ==CallState::OUTGONING_CALL){
  #ifndef  USE_DOWNLOADCALLLOG
             time_t timep;
             time(&timep);  /*得到time_t类型的UTC时间*/
           if ( index == 1 )
           {
                 call1.startdate = getlocaltime();
                 call1.active = false;

                 call1.starttime = timep;//GetTickCount();
              //   printf("-------------%d------------",call1.starttime);
                 call1.dir = "dialed";
           }else  if ( index == 2)
           {
               call2.active = false;
               call2.startdate = getlocaltime();
               call2.starttime = timep;//GetTickCount();
               call2.dir = "dialed";
           }
#endif
       }
       else  if (callState ==CallState::INCOME_CALL){
#ifndef  USE_DOWNLOADCALLLOG
           time_t timep;
           time(&timep);
           if ( index == 1 )
           {
               call1.active = false;
               call1.startdate = getlocaltime();
               call1.starttime =  timep;//GetTickCount();
               call1.dir = "missed";
           }else  if ( index == 2)
           {
               call2.active = false;
               call2.starttime =  timep;//GetTickCount();
               call2.startdate = getlocaltime();
               call2.dir = "missed";
           }
 #endif
       }

       else  if (callState ==CallState::CALL_ACTIVE){
           play(false);
 #ifndef  USE_DOWNLOADCALLLOG
           time_t timep;
           time(&timep);
           if ( index == 1 )
           {
               //  call1.startdate = getlocaltime();
                 call1.active = true;
                 if (call1.dir == "unknown")
                      call1.startdate = getlocaltime();//
                 call1.starttime = timep;// GetTickCount();
           }else  if ( index == 2)
           {

               if (call2.dir == "unknown")
                    call2.startdate = getlocaltime();
               call2.starttime = timep;// GetTickCount();
               call2.active = true;
           }
         //  std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance(); stop rine

#endif
      }

     hfp_emit_call_state_changed( HFPStubImp::m_interface, m_callState,index-1 );
   }
}

void HFPStubImp::chartostr(char *strmac, unsigned char *mac)
{
   sprintf( strmac,"%02X:%02X:%02X:%02X:%02X:%02X", mac[5],mac[4],  mac[3], mac[2],mac[1], mac[0]);
}
void HFPStubImp::scoStateChanged( const bool socState )
{
    hfp_emit_private_state_changed(HFPStubImp::m_interface, !socState);
    hfp_set_private_state(HFPStubImp::m_interface,!socState);
}
/*
 * -----------------------------------------inner method-------------------------------------------------------------
 */
//void HFPStubImp::HfpConnect( char* path )
//{
//    std::shared_ptr< IniDbus > p_inidus = IniDbus::getInstance();
//    DBusMessage *msg;
//    DBusMessageIter iter;
//    msg = dbus_message_new_method_call( DBUS_NAME, path,
//    DBUS_INTERFACE_HANDSFREEGATEWAY,
//                                        "Connect" );
//    if( msg == NULL )
//    {
//        LOG( SHOW_LOG, "create msg error" );
//        return;
//    }
//#if 0
//    msg = sdb->ExcuteMethodA(msg);
//
//    LOG(SHOW_LOG, "check return msg ");
//    DEBUG_CHECK_MSG(msg);
//
//    dbus_message_unref(msg);
//#else
//    dbus_bool_t ret = p_inidus->sdb->ExecuteMethodWithReplyAsync(
//                    msg, (DBusPendingCallNotifyFunction)HfpConnectResult, NULL,
//                    NULL );
//
//    if( !ret )
//    {
//        LOG( SHOW_LOG, "ExecuteMethod failed!" );
//    }
//#endif
//}
//void HFPStubImp::HfpConnectResult( DBusPendingCall *call, void *user_data )
//{
//    LOG( SHOW_LOG, "" );
//    DBusMessage *msg = NULL;
//    msg = dbus_pending_call_steal_reply( call );
//    if( CheckErrorMessage( msg ) )
//    {
//        LOG( SHOW_LOG, "Connect return error" );
//        //add by qianzhong
//        //incoke callback method
//#ifdef ENABLE_AUTO_CONNECT
////      Proxy::singleton()->sendSignal_auto_conenction_message(MSG_HFP_CONNECT_FAIL, "");
//#endif
//    }
//    else
//    {
//
//        LOG( SHOW_LOG, "Connect return normal" );
//    }
//    dbus_message_unref( msg );
//    dbus_pending_call_cancel( call );
//}
//void HFPStubImp::HfpDisConnect( char* path )
//{
//    std::shared_ptr< IniDbus > p_inidus = IniDbus::getInstance();
//    DBusMessage *msg;
//    msg = dbus_message_new_method_call( DBUS_NAME, path,
//    DBUS_INTERFACE_HANDSFREEGATEWAY,
//                                        "Disconnect" );
//    if( msg == NULL )
//    {
//        LOG( SHOW_LOG, "create msg error " );
//        return;
//    }
//#if 0
//    msg = sdb->ExcuteMethodA(msg);
//
//    LOG(SHOW_LOG, "check return msg ");
//    DEBUG_CHECK_MSG(msg);
//
//    dbus_message_unref(msg);
//#else
//    dbus_bool_t ret = p_inidus->sdb->ExecuteMethodWithReplyAsync(
//                    msg, (DBusPendingCallNotifyFunction)HfpDisconnectResult,
//                    NULL,
//                    NULL );
//    if( !ret )
//    {
//        LOG( SHOW_LOG, "ExecuteMethod failed!" );
//    }
//#endif
//}
//void HFPStubImp::HfpDisconnectResult( DBusPendingCall *call, void *user_data )
//{
//    LOG( SHOW_LOG, "" );
//    DBusMessage *msg = NULL;
//    msg = dbus_pending_call_steal_reply( call );
//    if( CheckErrorMessage( msg ) )
//    {
//        LOG( SHOW_LOG, "Disconnect fail" );
//    }
//    //invoke callback method....
//    dbus_message_unref( msg );
//    dbus_pending_call_cancel( call );
//}
//void HFPStubImp::AudioDisconnect( char *path )
//{
//    DBusMessage *msg;
//    DBusMessageIter iter;
//    std::shared_ptr< IniDbus > p_inidus = IniDbus::getInstance();
//    msg = dbus_message_new_method_call( DBUS_NAME, path,
//    DBUS_INTERFACE_HANDSFREEGATEWAY,
//                                        "AudioDisconnect" );
//
//    if( msg == NULL )
//    {
//        LOG( SHOW_LOG, "create msg error" );
//        return;
//    }
//
//#if 0
//    msg = sdb->ExcuteMethodA(msg);
//
//    LOG(SHOW_LOG, "check return msg ");
//    DEBUG_CHECK_MSG(msg);
//
//    dbus_message_unref(msg);
//#endif
//    dbus_bool_t ret = p_inidus->sdb->ExecuteMethodWithoutReply( msg );
//    if( !ret )
//    {
////        if( gScoStateCallBack != NULL )
////        {
////            gScoStateCallBack( true );
////        }
//        LOG( SHOW_LOG, "ExecuteMethod failed!" );
//    }
//    else
//    {
////        if( gScoStateCallBack != NULL )
////        {
////            gScoStateCallBack( false );
////        }
//    }
//
//}

//void HFPStubImp::AudioConnect( char *path )
//{
//    DBusMessage *msg;
//    DBusMessageIter iter;
//    std::shared_ptr< IniDbus > p_inidus = IniDbus::getInstance();
//    msg = dbus_message_new_method_call( DBUS_NAME, path,
//    DBUS_INTERFACE_HANDSFREEGATEWAY,
//                                        "AudioConnect" );
//    if( msg == NULL )
//    {
//        LOG( SHOW_LOG, "create msg error" );
//        return;
//    }
//#if 0
//    msg = sdb->ExcuteMethodA(msg);
//
//    LOG(SHOW_LOG, "check return msg ");
//    DEBUG_CHECK_MSG(msg);
//
//    dbus_message_unref(msg);
//#endif
//    dbus_bool_t ret = p_inidus->sdb->ExecuteMethodWithoutReply( msg );
//    if( !ret )
//    {
////        if( gScoStateCallBack != NULL )
////        {
////            gScoStateCallBack( false );
////        }
//        LOG( SHOW_LOG, "ExecuteMethod failed!" );
//    }
//    else
//    {
////        if( gScoStateCallBack != NULL )
////        {
////            gScoStateCallBack( true );
////        }
//    }
//}
//add by qianzhong 2017-4-20
unsigned long HFPStubImp::GetTickCount()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    unsigned long temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return temp;
}

void HFPStubImp::timeout()
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
    LOG(BT_LOG_HFP," hfpState =%d ,callstate =%d",g_device->currentStates.hfpState,g_device->currentStates.callState);
    if (g_device->currentStates.hfpState != ConnectionState::DISCONNECTION)
    {
        std::shared_ptr< PhoneBookStubImp > g_phonebook = PhoneBookStubImp::getInstance();

        if (g_phonebook->downloadcalled)
        {
//           if ( !g_device->currentStates.phoneNumber.empty())
              g_phonebook->downloadcalllogs(1);
        }
        else
        {
           g_phonebook->downloadcalllogs(30);
        }
    }
    shared_ptr<HFPStubImp> p_phone = HFPStubImp::getInstance();
    p_phone->m_downloadtimer.StopTimer();
}
void *HFPStubImp::playringFunc(void *args)
{
   shared_ptr<HFPStubImp> p_phone = HFPStubImp::getInstance();
   bool ret;

   while(p_phone->g_bRun)
   {
        ret = play_audio();
        if (ret ==false)
            break;

   }

    cout<<"------------HFPStubImp::closeringtone------------"<<endl;
    p_phone->g_bRun =false;
    stopAlsaPlaying();//stop ringe
    p_phone->playringtone_thread_id = 0;
}

string HFPStubImp::getlocaltime()
{
    char buf[50]={0};
    string curdata="";
    time_t t;
    struct tm * a;
    time(&t);
    a=localtime(&t);
    sprintf(buf,"%4d-%02d-%02d %02d:%02d:%02d",1900+a->tm_year,1+a->tm_mon,a->tm_mday,a->tm_hour,a->tm_min,a->tm_sec);
    curdata = string (buf) ;
    if (curdata != "")
    LOG(BT_LOG_HFP,"%s",curdata.data());
    return curdata;
}

string HFPStubImp::gettalktime(time_t starttime)
{

    LOG(BT_LOG_HFP," %d",starttime);
   // printf("-----------%d-----",GetTickCount());
    string totaltime="0";
     time_t timep;
    time(&timep);
    char buf[50]={0};
    time_t total =  timep - starttime;
    if (total>=3600)
       sprintf(buf, "%02d:%02d:%02d",total/3600,total%3600/60,total%3600%60);
    else
       sprintf(buf, "%02d:%02d",total/60,total%60);
    totaltime = string(buf);
    return buf;

}


