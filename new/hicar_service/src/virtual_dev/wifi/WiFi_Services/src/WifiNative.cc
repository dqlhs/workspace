#include <WifiNative.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wpa_ctrl.h>
#include <WifiService.h>
#include <sys/select.h>
#include <common.h>

namespace connectivity {

static WifiNative *gInstance = nullptr;

const string WifiNative::TAG = "WifiNative";

#define EVENT_BUF_SIZE 4096 + 1

#ifndef CONFIG_CTRL_IFACE_DIR
#define CONFIG_CTRL_IFACE_DIR "/var/run/wpa_supplicant"
#endif /* CONFIG_CTRL_IFACE_DIR */

#ifndef CONFIG_HOSTAPD_CTRL_IFACE_DIR
#define CONFIG_HOSTAPD_CTRL_IFACE_DIR "/run/hostapd"
#endif /* CONFIG_HOSTAPD_CTRL_IFACE_DIR */

static const char *ctrl_iface_dir = CONFIG_CTRL_IFACE_DIR;
static const char *ctrl_hostapd_iface_dir = CONFIG_HOSTAPD_CTRL_IFACE_DIR;

static struct wpa_ctrl *ctrl_conn;
static struct wpa_ctrl *mon_conn;
static const char IFNAME[]              = "IFNAME=";
#define IFNAMELEN			(sizeof(IFNAME) - 1)
static const char WPA_EVENT_IGNORE[]    = "CTRL-EVENT-IGNORE ";
#define MIN_RSSI  -100
#define MAX_RSSI  -55
#define RSSI_LEVELS 5
#define TIME_OUT    10
#define RECV_TIMES  5

WifiNative::WifiNative() : mRecvCount(0), mPriority(0), mConnectionInfo(NULL)
{
}

WifiNative::~WifiNative()
{
    if (!mConnectionInfo)
        g_variant_unref(mConnectionInfo);
}

// Not thread-safe
WifiNative *WifiNative::getInstance()
{
	return gInstance ? gInstance :
			(gInstance = new WifiNative());
}

// Not thread-safe
void WifiNative::destroy()
{
    // clear network map
    mNetWorks.clear();
	gInstance = nullptr;
}

int WifiNative::system_check(int result)
{
    if ((-1 != result) &&
            (WIFEXITED(result)) && (!(WEXITSTATUS(result))))
        return 0;
    else
        return -1;
}

int WifiNative::wifi_start_supplicant(void)
{
    int ret = SUCC;
	int i=0;
    // boot wpa_supplicant
    #if 0
    ret = system("wpa_supplicant -B -c/etc/wpa_supplicant.conf -iwlan0 -P /var/run/wpa_supplicant.pid -f /wifilog -t -ddd");
	ret = system("wpa_supplicant -B -c/etc/wpa_supplicant.conf -iwlan0 -ddd -P /var/run/wpa_supplicant.pid -s -t");
	wpa_supplicant -iwlan0 -Dnl80211 -c /etc/wpa_supplicant.conf -dd -B


	ret = system("wpa_supplicant -iwlan0 -Dnl80211 -c /etc/wpa_supplicant.conf -dd  -P /var/run/wpa_supplicant.pid -B -s -t");
		ret = system("wpa_supplicant -iwlan0 -Dnl80211 -c /etc/wpa_supplicant.conf -dd -B -s");
#else
	for(i=0;i<3;i++)
	{
		ret = system("wpa_supplicant -iwlan0 -Dnl80211 -c /etc/wpa_supplicant.conf -dd -B -s -t");
		if (system_check(ret))
		{
			printf("\r\n\r\n %s,start ERROR! ERROR! ,%d,ret:%d BBBBBBBBB \r\n ",__func__,i,ret);
			ret = ERR_START_SUPP_FAILED;
		}
		else{
			ret = SUCC;
			break;
		}
	}
#endif
		
    return ret;
}

int WifiNative::wifi_stop_supplicant(void)
{
    // kill wpa_supplicant
    system("kill `cat /var/run/wpa_supplicant.pid`");

    usleep(500000);

    return SUCC;
}

void WifiNative::wifi_close_connection(void)
{
    if (ctrl_conn == NULL)
        return;

    wpa_ctrl_detach(mon_conn);

    wpa_ctrl_close(ctrl_conn);

    ctrl_conn = NULL;

    if (mon_conn) {
        wpa_ctrl_close(mon_conn);
        mon_conn = NULL;
    }
}

char *WifiNative::wifi_get_default_ifname(void)
{
	char *ifname = NULL;
	struct dirent *dent;

	DIR *dir = opendir(ctrl_iface_dir);

	if (!dir) {
		return NULL;
	}

	while ((dent = readdir(dir))) {
		if (strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0)
			continue;

		ifname = strdup(dent->d_name);
		break;
	}

	closedir(dir);

	return ifname;
}

int WifiNative::wifi_open_connection(const char *ifname)
{
	static char cfile[PATH_MAX];

    snprintf(cfile, sizeof cfile, "%s/%s", ctrl_hostapd_iface_dir,
            ifname);

    printf("[debug]cfile = %s\n", cfile);
	ctrl_conn = wpa_ctrl_open(cfile);

    if (ctrl_conn == NULL)
        return ERR_CONN_SUPP_FAILED;

    mon_conn = wpa_ctrl_open(cfile);

    if (mon_conn == NULL) {
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = NULL;
        
        return ERR_CONN_SUPP_FAILED;
    }

    if (wpa_ctrl_attach(mon_conn) != 0) {
        wpa_ctrl_close(mon_conn);
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = mon_conn = NULL;
        return ERR_CONN_SUPP_FAILED;
    }

    // load networks
    loadNetworks();

    printf("\r\n [debug] okokokok \r\n ");

	return SUCC;
}

int WifiNative::wifi_send_command(const char *cmd, char *reply, size_t *reply_len)
{
    int ret = SUCC;

    if (ctrl_conn == NULL)
        return ERR_SUPP_CMD_FAILED;

    if (cmd == NULL)
        return ERR_SUPP_CMD_FAILED;

    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), reply, reply_len, NULL);

    if (ret < 0 || strncmp(reply, "FAIL", 4) == 0) {
        return ERR_SUPP_CMD_FAILED;
    }

    return ret;
}

int WifiNative::wifi_recv_event(char *buf, size_t buflen)
{
    size_t nread = buflen - 1;
    int result;
    char *match, *match2;

    result = wpa_ctrl_recv(mon_conn, buf, &nread);

    if (result < 0) {
        printf("wifi_ctrl_recv failed: %s\n", strerror(errno));
        return ERR_RECV_EVENT;
    }

    buf[nread] = '\0';
    /* Check for EOF on the socket */
    if (result == 0 && nread == 0) {
        /* Fabricate an event to pass up */
        printf("Received EOF on supplicant socket\n");
        return ERR_RECV_EVENT;
    }

    if (strncmp(buf, IFNAME, IFNAMELEN) == 0) {
        match = strchr(buf, ' ');
        if (match != NULL) {
            if (match[1] == '<') {
                match2 = strchr(match + 2, '>');
                if (match2 != NULL) {
                    nread -= (match2 - match);
                    memmove(match + 1, match2 + 1, nread - (match - buf) + 1);
                }
            }
        } else {
            return snprintf(buf, buflen, "%s", WPA_EVENT_IGNORE);
        }
    } else if (buf[0] == '<') {
        match = strchr(buf, '>');
        if (match != NULL) {
            nread -= (match + 1 - buf);
            memmove(buf, match + 1, nread + 1);
        }
    } else {
    }

    return nread;
}

int WifiNative::wifi_connect_to_supplicant(void)
{
    //char *ifname = wifi_get_default_ifname();
    const char *ifname = "wlan0";

    printf("[debug]ifname = %s\n",ifname);
    return wifi_open_connection(ifname);

}

void WifiNative::wifi_close_supplicant_connection()
{
    wifi_close_connection();
}

char *WifiNative::wifi_wait_for_event(char *event, int len)
{
    char buf[EVENT_BUF_SIZE];
    wifi_recv_event(buf, sizeof buf);
    strncpy(event, buf, len-1);

    if (findString(buf, WPA_EVENT_CONNECTED)) {
        // update connect state
        mConnectState = CONNECT_STATE_AUTO;
    }

    return event;
}

int WifiNative::wifi_command(const char *command, char *reply, size_t *reply_len)
{
    return wifi_send_command(command, reply, reply_len);
}

vector<string> WifiNative::split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> tokens;

    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }

    return tokens;
}

bool WifiNative::starts_with(string& s1, const string& s2)
{
    bool ret = false;

    if (s2.size() <= s1.size() &&
            s1.compare(0, s2.size(), s2) == 0)
        ret = true;

    if (ret)
        s1 = s1.substr(s2.size());

    return ret;
}

bool WifiNative::findString(const string &src, const string &delim)
{
    const char *show;
    show = strstr(src.c_str(), delim.c_str());

    if (show != NULL)
        return true;
    else
        return false;
}

int WifiNative::frequency_to_channel(int freq)
{
    if (freq == 2484)
        return 14;

    if (freq < 2484)
        return (freq - 2407) / 5;

    return freq/5 - 1000;
}

GVariant* WifiNative::wifi_get_scan_results()
{
    char reply[2048];
    int index = 0;
    char cmd[20];
    auto *builders = g_variant_builder_new(G_VARIANT_TYPE("a(ssiiiasb)"));

    while (1) {
        snprintf(cmd, sizeof(cmd), "BSS %d", index++);

        if (index > 1000)
            break;

        string bss = doStringCommand(cmd);
        if (bss.empty())
            break;

        bool wps = false;
        int  channel = 0;
        string bssid, freq, level, flags;

        char ssid[32];
        GVariantBuilder *g_auth = g_variant_builder_new(G_VARIANT_TYPE("as"));
        vector<string> lines = split(bss, '\n');

        for (auto line : lines) {
            bool isfind = false;
            if (starts_with(line, "flags=")) {
                if (findString(line.c_str(), "WPS")) {
                    wps = true;
                } else {
                    wps = false;
                }

                if (findString(line.c_str(), "WPA2-PSK"))
                {
                    isfind = true;
                    g_variant_builder_add(g_auth, "s", "WPA2-PSK");
                }

                if (findString(line.c_str(), "WPA-PSK"))
                {
                    isfind = true;
                    g_variant_builder_add(g_auth, "s", "WPA-PSK");
                }

                if (findString(line.c_str(), "WEP"))
                {
                    isfind = true;
                    g_variant_builder_add(g_auth, "s", "WEP");
                }

                if (!isfind)
                    g_variant_builder_add(g_auth, "s", "NONE");

            } else if (starts_with(line, "ssid=")) {
                // decode for chinese
                printf_decode((u8 *)ssid, sizeof ssid, line.c_str());
            } else if (starts_with(line, "bssid=")) {
                bssid = line;
            } else if (starts_with(line, "level=")) {
                level = line;
            } else if (starts_with(line, "freq=")) {
                channel = frequency_to_channel(atoi(line.c_str()));
            }
        }

        g_variant_builder_add(builders, "(ssiiiasb)", ssid, bssid.c_str(),
                channel, atoi(level.c_str()), calculateSignalLevel(atoi(level.c_str()), RSSI_LEVELS), g_auth, wps);

        g_variant_builder_unref(g_auth);
    }

    return g_variant_builder_end(builders);
}

int WifiNative::wifi_get_mon_fd(void)
{
    return wpa_ctrl_get_fd(mon_conn);
}

int WifiNative::calculateSignalLevel(int rssi, int numLevels)
{
    if (rssi <= MIN_RSSI) {
        return 0;
    } else if (rssi >= MAX_RSSI) {
        return numLevels - 1;
    } else {
        float inputRange = (MAX_RSSI - MIN_RSSI);
        float outputRange = (numLevels - 1);
        return (int)((float)(rssi - MIN_RSSI) * outputRange / inputRange);
    }
}

int WifiNative::select_monitor_fd(char *event, int size, int *sec, int *usec)
{
    int ret = SUCC;
    int ret_sel;
    fd_set fds;
    struct timeval timeout={*sec, *usec};

    int fd = wpa_ctrl_get_fd(mon_conn);
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    ret_sel = select(fd + 1, &fds, NULL, NULL, &timeout);

    *sec = timeout.tv_sec;
    *usec = timeout.tv_usec;

    if (ret_sel < 0) {
        // todo
    } else if (ret_sel == 0) {
        // time out
        printf("select time out!\n");
        ret = ERR_CONN_TIMEOUT;
    } else {
        if (FD_ISSET(fd, &fds)) {
            // recv event
            wifi_recv_event(event, size);

            if (strcmp(event, WPA_EVENT_SCAN_RESULTS) == 0)
                mRecvCount++;
        }
    }

    if (mRecvCount == RECV_TIMES)
	{
		printf(" %s,mRecvCount == RECV_TIMES\n",__func__);
		ret = ERR_CONN_TIMEOUT;
	}
	
    return ret;
}

void WifiNative::loadNetworks(void)
{
    bool done = false;
    char cmd[20];
    int last_id = -1;

    while(!done) {
        snprintf(cmd, sizeof(cmd), "LIST_NETWORKS LAST_ID=%d", last_id);

		cout << __func__ << ", cmd: "<< cmd << endl;
	
        string lists = doStringCommand(cmd);
        printf("list networks reply = %s\n", lists.c_str());

        if (lists.empty())
            break;

        vector<string> lines = split(lists, '\n');

        printf("lines size = %ld\n", lines.size());

        // no list
        if (lines.size() == 1)
            break;

        for (unsigned int i = 1; i < lines.size(); i++) {
            vector<string> result = split(lines[i], '\t');

            last_id = atoi(result[0].c_str());

            // decode for chinese
            char ssid[32];
            char cmd[64];
            printf_decode((u8 *)ssid, sizeof ssid, result[1].c_str());

            snprintf(cmd, sizeof(cmd), "GET_NETWORK %d priority",
                    atoi(result[0].c_str()));

            int priority = doIntCommand(cmd);

            printf("[loadNetworks] netId = %d, priority = %d\n",atoi(result[0].c_str()), priority);
            // insert network default disabled
            mNetWorks.insert(map<string, networkInfo> :: value_type(ssid,
                        {atoi(result[0].c_str()), priority, NETWORK_STATE_DISABLED}));

            // save max priority
            if (priority > mPriority)
                mPriority = priority;

            // no line
            if ((i + 1) == lines.size())
                done = true;

        }
    }
}

bool WifiNative::wifi_check_auto_scan(void)
{
    return mNetWorks.empty();
}

bool WifiNative::setNetworkVariable(int netId, string name, string value)
{
	char cmd[64];

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d %s %s",
            netId, name.c_str(), value.c_str());

    return doBooleanCommand(cmd);
}

bool WifiNative::setNetworkExtra(int netId, string name, string value)
{
	char cmd[64];

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d %s \"%s\"",
            netId, name.c_str(), value.c_str());

    return doBooleanCommand(cmd);
}

bool WifiNative::doBooleanCommand(string cmd)
{
    bool ret = true;
	char reply[8] = { 0, };
	size_t len;

    len = sizeof(reply) - 1;

	wifi_command(cmd.c_str(), reply, &len);

    reply[len] = '\0';

    if (strncmp(reply, "FAIL", 4) == 0)
        ret = false;

    return ret;
}

int WifiNative::doIntCommand(string cmd)
{
	char reply[8] = { 0, };
	size_t len;

    len = sizeof(reply) - 1;

	wifi_command(cmd.c_str(), reply, &len);

    reply[len] = '\0';

    return atoi(reply);
}

string WifiNative::doStringCommand(string cmd)
{
    char reply[2048];
    size_t reply_len;

    reply_len = sizeof(reply) - 1;

    if (wifi_command(cmd.c_str(), reply, &reply_len) < 0)
       return "";

    reply[reply_len] = '\0';

    string result(reply);
    if (result.empty() || starts_with(result, "FAIL"))
        return "";

    return result;
}

int WifiNative::wifi_connect_network(const string &ssid, const string &psk)
{
    int ret = SUCC;
    int netId;
	char cmd[64];

    //printf("[connect test]ssid = %s , psk = %s \n", ssid.c_str(),psk.c_str());

    // disable last connect succ network
    if (mNetWorks.count(mCurrentSsid)) {
        mNetWorks[mCurrentSsid].status = NETWORK_STATE_DISABLED;
    }

    // set connect state active
    mConnectState = CONNECT_STATE_ACTIVE;
    // save current ssid
    mCurrentSsid = ssid;

    if (mNetWorks.count(ssid)) {
        printf("[connect test]ssid exist!\n");

        // enable network
        snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d",
                mNetWorks[ssid].netId);

        doBooleanCommand(cmd);

        if (psk.empty()) {
            // NONE
            setNetworkVariable(mNetWorks[ssid].netId, "key_mgmt", "NONE");
        } else {
            setNetworkVariable(mNetWorks[ssid].netId, "key_mgmt", "WPA-PSK");

            // password format error
            if (!setNetworkExtra(mNetWorks[ssid].netId, "psk", psk)) {
                return ERR_PASSWD_FORMAT;
            }
        }

        // select network
        snprintf(cmd, sizeof(cmd), "SELECT_NETWORK %d",
                mNetWorks[ssid].netId);

        doBooleanCommand(cmd);

        // wait event
        ret = wait_for_supplicant_event(false, mNetWorks[ssid].netId);
    } else {
        printf("[connect test]ssid not exist!\n");

        // add network
        netId = doIntCommand("ADD_NETWORK");

        if (psk.empty()) {
            // NONE
            
			cout << __func__ << " psk.empty ,"<< " key_mgmt , NONE " << endl;
            setNetworkVariable(netId, "key_mgmt", "NONE");
        } else {
            setNetworkVariable(netId, "key_mgmt", "WPA-PSK");

            // password format error
            if (!setNetworkExtra(netId, "psk", psk)) {
				
				cout << __func__ << " psk.error ,"<< " key_mgmt , WPA-PSK  " << ssid.c_str() << " key :" <<psk.c_str()<<endl;
                remove_network(netId);
                return ERR_PASSWD_FORMAT;
            }
        }
		
		cout << __func__ << " psk."<< " key_mgmt , WPA-PSK  " << ssid.c_str() << " key :" <<psk.c_str()<<endl;

        // set ssid
        setNetworkExtra(netId, "ssid", ssid);

        // select network
        snprintf(cmd, sizeof(cmd), "SELECT_NETWORK %d", netId);

        doBooleanCommand(cmd);

        // wait event
        ret = wait_for_supplicant_event(true, netId);
    }

    return ret;
}

int WifiNative::wait_for_supplicant_event(bool isNew, int netId)
{
    int ret = SUCC;
    char event[4096];
    // clear recv count
    mRecvCount = 0;
    // init interval
    int sec = TIME_OUT;
    int usec = 0;

    for(;;) {

        printf("interval sec = %d, usec = %d\n", sec, usec);
        ret = select_monitor_fd(event, sizeof event, &sec, &usec);

        if (ret == ERR_CONN_TIMEOUT) {
            // new connect need to remove
            if (isNew)
                remove_network(netId);
            else
                disable_network(netId);

            break;
        }

        if (findString(event, WPA_EVENT_CONNECTED)) {
            printf("[connect test]connected\n");
            // insert temp network
            if (isNew) {
                mNetWorks.insert(map<string, networkInfo> :: value_type(mCurrentSsid,
                            {netId, 0, NETWORK_STATE_TEMP}));
            }

            break;
        }

        if (findString(event, WPA_EVENT_DISCONNECTED)) {
            // connect error
            printf("%s,[connect test]disconnected\n",__func__);
            printf("%s,[connect test]event = %s\n",__func__,event);

            // revert interval
            sec = TIME_OUT;
            usec = 0;

            for(;;) {
                ret = select_monitor_fd(event, sizeof event, &sec, &usec);

                if (ret == ERR_CONN_TIMEOUT) {
                    // new connect
					printf("ERR_CONN_TIMEOUT interval sec = %d, usec = %d\n", sec, usec);
                    if (isNew)
                        remove_network(netId);
                    else
                        disable_network(netId);

                    break;
                }

                // wrong password
                if (findString(event, "reason=WRONG_KEY")) {
                    ret = ERR_CONN_NET_PASSWD_INCORRECT;
					printf(" reason=WRONG_KEY interval sec = %d, usec = %d\n", sec, usec);
                    // new connect
                    if (isNew)
                        remove_network(netId);
                    else
                        disable_network(netId);

                    break;
                }
            }
            break;
        }

    }
	printf("%s end wait BBBBB \r\n ",__func__);

    return ret;
}

void WifiNative::disable_network(int netId)
{
	char cmd[64];

    snprintf(cmd, sizeof(cmd), "DISABLE_NETWORK %d", netId);
    doBooleanCommand(cmd);
}

void WifiNative::remove_network(int netId)
{
	char cmd[64];
	printf(" %s,last ",__func__);
    snprintf(cmd, sizeof(cmd), "REMOVE_NETWORK %d", netId);
    doBooleanCommand(cmd);
}

int WifiNative::wifi_disconnect_network(bool isPassive /*passive disconnect*/)
{
    int ret = SUCC;
    char event[4096];

    // disconnect network
    doBooleanCommand("DISCONNECT");

    int sec = TIME_OUT;
    int usec = 0;

    if (!isPassive)
        for(;;) {
            ret = select_monitor_fd(event, sizeof event, &sec, &usec);

            if (ret == ERR_CONN_TIMEOUT) {
                break;
            }

            if (findString(event, WPA_EVENT_DISCONNECTED)) {
                printf("%s,[disconnect test]disconnected\n",__func__);
                break;
            }
        }

    return ret;
}

void WifiNative::enable_networks()
{
    for (auto it = begin(mNetWorks); it != end(mNetWorks); ++it) {
        if (it->second.status == NETWORK_STATE_DISABLED)
        {
            char cmd[64];
            // enable network
            snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d",
                    it->second.netId);

            doBooleanCommand(cmd);
        }
    }
}

void WifiNative::notify_dhcp_result(bool isSucc)
{
    switch(mConnectState) {
        case CONNECT_STATE_DISCONNECT:
            break;
        case CONNECT_STATE_ACTIVE:
            {
                if (isSucc) {
                    // set priority
                    setNetworkVariable(mNetWorks[mCurrentSsid].netId, "priority",
                            to_string(++mPriority));

                    // get connection info
                    mConnectionInfo = get_connection_info();

                    // update priority
                    mNetWorks[mCurrentSsid].priority = mPriority;

                    // update state
                    mNetWorks[mCurrentSsid].status = NETWORK_STATE_CURRENT;

                    // enable other networks
                    enable_networks();

                    // save config
                    doBooleanCommand("SAVE_CONFIG");
                } else {
                    // disconnect network
                    wifi_disconnect_network(false);

                    if (mNetWorks[mCurrentSsid].status == NETWORK_STATE_TEMP)
                    {
                        printf("temp remove network\n");
                        // remove network
                        remove_network(mNetWorks[mCurrentSsid].netId);
                        // remove network from mNetworks
                        mNetWorks.erase(mCurrentSsid);
                    }
                }
            }
            break;
        case CONNECT_STATE_AUTO:
            {
                if (isSucc) {
                    // get connection info
                    mConnectionInfo = get_connection_info();
                    printf("current ssid = %s\n", mCurrentSsid.c_str());

                    // update state
                    mNetWorks[mCurrentSsid].status = NETWORK_STATE_CURRENT;
                } else {
                    // disconnect network
                    wifi_disconnect_network(false);
                }
            }
            break;
        default:
            break;
    }

}

GVariant* WifiNative::get_connection_info(void)
{
    string status = doStringCommand("STATUS");

    if (status.empty())
        return NULL;

    string bssid, freq, auth, ip, gw, rssi, linkSpeed;
    char ssid[32];
    int  channel;

    vector<string> lines = split(status, '\n');

    for (auto line : lines) {
        if (starts_with(line, "wpa_state=")) {
            // check wpa state
            if (line != "COMPLETED") {
                return NULL;
            }
        } else if (starts_with(line, "ssid=")) {
            // decode for chinese
            printf_decode((u8 *)ssid, sizeof ssid, line.c_str());
            // update current ssid
            mCurrentSsid = ssid;
        } else if (starts_with(line, "bssid=")) {
            bssid = line;
        } else if (starts_with(line, "freq=")) {
            freq = line;
            channel = frequency_to_channel(atoi(line.c_str()));
        } else if (starts_with(line, "key_mgmt=")) {
            auth = line;
        } else if (starts_with(line, "ip_address=")) {
            ip = line;
        }

    }

    rssi = get_connection_rssi();

    linkSpeed = get_connection_linkspeed();

    gw = get_gateway();

    return g_variant_new("(ssiisiiiss)", ssid, bssid.c_str(), channel, atoi(freq.c_str()),
            auth.c_str(), atoi(rssi.c_str()), calculateSignalLevel(atoi(rssi.c_str()), RSSI_LEVELS),
            atoi(linkSpeed.c_str()), ip.c_str(), gw.c_str());
}

GVariant* WifiNative::wifi_get_connection_info(void)
{
    g_variant_ref(mConnectionInfo);
    return mConnectionInfo;
}

string WifiNative::get_gateway()
{
    FILE *fp;
    char line[32];
    string result;

    fp = popen("ip route list match 0/0 | grep wlan0 | awk '{print $3}'", "r");

    if (!fp)
        return "";

    while (fgets(line, 32, fp))
    {
        // extract '\n'
        strtok(line, "\n");
        result += line;
    }

    pclose(fp);

    return result;
}

string WifiNative::signal_poll_info(string kw)
{
    string result;

    string info = doStringCommand("SIGNAL_POLL");

    if (info.empty())
        return "";

    vector<string> lines = split(info, '\n');

    for (auto line : lines) {
        if (starts_with(line, kw)) {
            result = line;
            break;
        }
    }

    return result;
}

string WifiNative::get_connection_rssi()
{
    return signal_poll_info("RSSI=");
}

string WifiNative::get_connection_linkspeed(void)
{
    return signal_poll_info("LINKSPEED=");
}

GVariant* WifiNative::wifi_get_signal_strength(void)
{
    string rssi = get_connection_rssi();

    return g_variant_new("(ii)", atoi(rssi.c_str()),
            calculateSignalLevel(atoi(rssi.c_str()), RSSI_LEVELS));
}

GVariant* WifiNative::wifi_get_saved_networks(void)
{
    auto builders = g_variant_builder_new(G_VARIANT_TYPE("a(issibb)"));

    for (auto it = begin(mNetWorks); it != end(mNetWorks); ++it)
    {
        g_variant_builder_add(builders, "(issibb)", it->second.netId, it->first.c_str(),
                ""/*psk is null*/, it->second.priority, true/*all enabled*/, false);

    }

    return g_variant_builder_end(builders);
}

string WifiNative::get_ssid_from_netid(int netId)
{
    string ssid;

    for (auto it = begin(mNetWorks); it != end(mNetWorks); ++it) {
        if (it->second.netId == netId) {
            ssid = it->first;
            break;
        }
    }

    return ssid;
}

int WifiNative::wifi_select_network(int netId)
{
    int ret = SUCC;
    char cmd[64];

    // disable last connect succ network
    if (mNetWorks.count(mCurrentSsid)) {
        mNetWorks[mCurrentSsid].status = NETWORK_STATE_DISABLED;
    }

    // set current ssid
    mCurrentSsid = get_ssid_from_netid(netId);
    // set connect state active
    mConnectState = CONNECT_STATE_ACTIVE;

    // enable network
    snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d", netId);

    if (!doBooleanCommand(cmd))
        return ERR_INVALID_NETID;

    // select network
    snprintf(cmd, sizeof(cmd), "SELECT_NETWORK %d", netId);

    if (!doBooleanCommand(cmd))
        return ERR_INVALID_NETID;

    // wait event
    ret = wait_for_supplicant_event(false, netId);

    return ret;
}
/* hostap interface
 *
 *
*/
int WifiNative::wifi_connect_to_hostap(void)
{
    //char *ifname = wifi_get_default_ifname();
	char reply[8] = { 0, };
	size_t len;
    
    const char *ifname = "wlan0";
    
    printf("[debug]cfile = 1111\n");

    printf("[debug]ifname = %s\n",ifname);

    wifi_open_connection(ifname);

    /* send mesg to hostapd */

    return 0;
}

void WifiNative::wifi_close_hostap_connection()
{
    wifi_close_connection();
}

int WifiNative::wifi_set_ip_address(void)
{
    char stringcmd[30];
    FILE *fp = NULL;   //notice
    const char  *filePath = "/etc/ip";
    int filesize;
    char buf[20];

#if 1
    fp  = fopen(filePath,"at+");
    if(fp ==  NULL){
        cout << "\r\n AAAAAAAAAAAAAAA error" << endl;
        return -1;
    }

    fseek(fp,0L,SEEK_END);
    filesize=ftell(fp);

    fseek(fp,0L,SEEK_SET);
    fread(buf,filesize,1,fp);
    buf[filesize] = '\0';
    cout <<  "buf = " << buf << endl;
    cout << "\r\n " << " AAAAAAAAA filesize: "<< filesize << " buf " << buf << endl;

#else
     char buf[] = "192.168.53.2";
#endif
    int ret = SUCC;
	int i=0;

    sprintf(stringcmd,"ifconfig wlan0 %s",buf);
    cout << " \r\n " << " BBBBBBBBBBBBBB set ip : " << stringcmd <<endl;

	for(i;i<1;i++){
        ret=system(stringcmd);
		if(system_check(ret)){
			printf("\r\n\r\n %s,wifi_set_ip_address ERROR! ERROR! ,%d,ret:%d BBBBBBBBB \r\n ",__func__,i,ret);
			ret = ERR_START_SUPP_FAILED;
		}else{
			ret = SUCC;
			break;
		}
	}
	return ret;
}
int WifiNative::wifi_start_dhcpserver(void)
{
	int ret = SUCC;
	int i=0;

    if(fopen("/run/dnsmasq.pid","r") != NULL){
        system("kill `cat /run/dnsmasq.pid`");

    }

    printf("\r %s,start dhcp server\n",__func__);
    system("dnsmasq -x /var/run/dnsmasq.wlan0.pid -l /var/run/dnsmasq-wlan0.leases -i wlan0 -5 --except-interface=lo --dhcp-range=192.168.49.2,192.168.49.20,12h --dhcp-option=3,192.168.49.1 --bind-interfaces");

	return SUCC;
}
int WifiNative::wifi_stop_hostap(void)
{
    // kill wpa_supplicant
    if(fopen("/var/run/hostapd.pid","r") != NULL)
    {
        system("kill `cat /var/run/hostapd.pid`");

        usleep(500000);
    }

    return SUCC;
}

int WifiNative::wifi_start_hostap(void){
    int ret = SUCC;
	int i=0;
	
    // wifi_stop_hostap();
    
	printf("\r %s,start hostap \n",__func__);
    
	for(i=0;i<1;i++)
	{
		ret = system("hostapd -B /etc/hostap.conf -dd  -P /var/run/hostapd.pid ");
		if (system_check(ret))
		{
			printf("\r\n\r\n %s,start ERROR! ERROR! ,%d,ret:%d BBBBBBBBB \r\n ",__func__,i,ret);
			ret = ERR_START_SUPP_FAILED;
		}
		else{
			ret = SUCC;
			break;
		}
	}
    usleep(50000);

    return ret;
}


}


