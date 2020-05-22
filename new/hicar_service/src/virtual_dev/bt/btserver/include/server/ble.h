#ifndef BLE_H
#define BLE_H
#include "../src-gen/ble/hsaeble.h"
#include "HsaeTypes.h"
using namespace com::hsae::bluetooth::util::HsaeTypes;
using namespace std;
#include"ctimer.h"

class BleImp
{
public:
    BleImp();
    static std::shared_ptr<BleImp> bleInstance;
    static std::shared_ptr<BleImp> getInstance();
    bool initble();
    void broadcastdata( );
    void setintervals(int interval);
    void GATTServerDisableAdv();
    void GATTServerEnableAdv();
    static void timeout();
    int mcount;
    int m_interval;
     CTimer m_createtimer;
    ~BleImp();
    bool s_GATTServer_Adv;
    static gboolean startBleAdvertise( Hsaeble *object,GDBusMethodInvocation *invocation,GVariant *arg_data,gint arg_datalen);
    static gboolean stopBleAdvertise(Hsaeble *object,GDBusMethodInvocation *invocation);
    static void on_bus_acquired (GDBusConnection *connection,const gchar *name,gpointer user_data);
    static Hsaeble*  m_interface;
    int adv_min_interval;
    int adv_max_interval;
    HSAE_GATT_Advertise_Info_t advertise;
};



#endif // BLE_H
