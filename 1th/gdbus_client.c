/*
 * =====================================================================================
 *
 *       Filename:  gdbus_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/17/2017 08:28:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qiaotsh 
 *
 * =====================================================================================
 */
#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include "Building.h"
#include "gdbus_client.h"

static gboolean test_status_handler(Test* object,gint status,gpointer user_data)
{
    g_print("signal received %d\n",status);
    return TRUE;
}

static void cb_OwnerNameChangedNotify(GObject *object, GParamSpec *pspec, gpointer userdata)
{
    gchar *pname_owner = NULL;
    pname_owner = g_dbus_proxy_get_name_owner((GDBusProxy*)object);

    if (NULL != pname_owner){
        g_print("DBus service is ready!\n");
        g_free(pname_owner);
    }
    else{
        g_print("DBus service is NOT ready!\n");
        g_free(pname_owner);
    }
}

int main(int argc, char *argv[])
{
    Test *proxy=NULL;
    GError *error = NULL;
    GMainLoop* loop = NULL;

    proxy = test_proxy_new_for_bus_sync (
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_NONE,
            "com.qiaotsh.Building.Test",
            "/com/qiaotsh/Building/Test",
            NULL,
            &error);

    if(0 == proxy)
    {
        g_print("InitDBusCommunication: Failed to create proxy. Reason: %s.\n", error->message);
    }

    g_signal_connect(proxy,"test-status",G_CALLBACK(test_status_handler),NULL);

    char *reply = NULL;
    test_call_set_version_sync (proxy, "hello world", &reply,NULL, &error);
    g_print("sync reply = %s\n",reply);

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_object_unref(proxy);
    return 0;
}

