#ifndef __WIFINATIVE_H__
#define __WIFINATIVE_H__

#include <string>
#include <map>
#include <glibmm.h>
#include <giomm.h>
#include <condition_variable>

namespace connectivity {

    using namespace std;

#define SUCC                   0
#define ERR_START_SUPP_FAILED -1
#define ERR_CONN_SUPP_FAILED  -2
#define ERR_SUPP_CMD_FAILED   -3
#define ERR_PASSWD_FORMAT     -4
#define ERR_CONN_NET_PASSWD_INCORRECT -5
#define ERR_CONN_NET_AP_DISABLE  -6
#define ERR_CONN_TIMEOUT       -7
#define ERR_RECV_EVENT         -8
#define ERR_SAVE_CONFIG        -9
#define ERR_SELECT_NETWORK     -10
#define ERR_INVALID_NETID      -11

#define EVENT_SCAN_RESULTS     0
#define EVENT_CONNECTED        1
#define EVENT_DISCONNECTED     2
#define EVENT_TEMP_DISABLED    3
#define EVENT_WRONG_KEY        4
#define EVENT_CONNECT_FAILED   5

    class WifiNative {

            WifiNative();
            ~WifiNative();

            enum network_status {
                NETWORK_STATE_DISABLED,		// 0
                NETWORK_STATE_ENABLED,	  	// 1
                NETWORK_STATE_TEMP,         // 2
                NETWORK_STATE_CURRENT,		// 3
            };

            enum {
                CONNECT_STATE_DISCONNECT,
                CONNECT_STATE_ACTIVE,
                CONNECT_STATE_AUTO,
            } mConnectState = CONNECT_STATE_DISCONNECT;

            struct networkInfo {
                int netId;
                int priority;
                network_status status;
            };

            void wifi_close_connection(void);
            char * wifi_get_default_ifname(void);
            int wifi_open_connection(const char *);
            int wifi_send_command(const char *cmd, char *reply, size_t *reply_len);
            int wifi_recv_event(char *buf, size_t buflen);
            vector<string> split(const string &s, char delim);
            bool starts_with(string& s1, const string& s2);
            bool findString(const string &src, const string &delim);
            int frequency_to_channel(int freq);
            int system_check(int result);
            void disconnect_network(int id);
            int calculateSignalLevel(int rssi, int numLevels);
            void remove_network(int netId);
            void disable_network(int netId);
            int select_monitor_fd(char *event, int size, int *sec, int *usec);
            bool setNetworkVariable(int netId, string name, string value);
            bool setNetworkExtra(int netId, string name, string value);
            void loadNetworks(void);
            bool doBooleanCommand(string cmd);
            int doIntCommand(string cmd);
            string doStringCommand(string cmd);
            int wait_for_supplicant_event(bool isNew, int netId);
            void enable_networks();
            GVariant* get_connection_info(void);
            string get_gateway(void);
            string signal_poll_info(string kw);
            string get_connection_rssi(void);
            string get_connection_linkspeed(void);
            string get_ssid_from_netid(int netId);

        private:
            static const string TAG;
            int mRecvCount;
            int mPriority;
            string mCurrentSsid;
            map<string, networkInfo> mNetWorks;
            GVariant *mConnectionInfo;

        public:
            static WifiNative *getInstance(void);

            void destroy();

            int wifi_start_supplicant(void);
            int wifi_stop_supplicant(void);
            int wifi_connect_to_supplicant(void);
            void wifi_close_supplicant_connection(void);
            char *wifi_wait_for_event(char *event, int len);
            int wifi_command(const char *command, char *reply, size_t *reply_len);
            int wifi_get_mon_fd(void);
            GVariant* wifi_get_scan_results(void);
            int wifi_connect_network(const string&, const string&);
            int wifi_disconnect_network(bool);
            bool wifi_check_auto_scan(void);
            void notify_dhcp_result(bool isSucc);
            GVariant* wifi_get_connection_info(void);
            GVariant* wifi_get_signal_strength(void);
            GVariant* wifi_get_saved_networks(void);
            int wifi_select_network(int netId);

			/* hostap */
			int wifi_start_hostap(void);
			int wifi_stop_hostap(void);
            int wifi_connect_to_hostap(void);
            void wifi_close_hostap_connection(void);
			int wifi_start_dhcpserver(void);
			int wifi_set_ip_address(void);		

    };

}
#endif
