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
#include<stdio.h>
#include<gio/gio.h>
#include<glib.h>
#include<assert.h>
#include"Building.h"
#include"gdbus_server.h"

static Test* skeleton=NULL;


static gboolean Emit_Test_Status(gconstpointer p)          
{                                                         
    g_print("Emit_Test_Status() is called.\n");            

    gint status = 5;
    test_emit_test_status (skeleton,status);

    return TRUE;                                          
}                                                         

static  gboolean on_handle_set_version(Test* skeleton,
        GDBusMethodInvocation *invocation,
        const gchar *greeting,
        gpointer user_data)
{
    g_print("Method call: %s\n", greeting);

    char* reply = "getSuccess";
    test_complete_set_version (skeleton,invocation,reply);

    return TRUE;
}


void GBusAcquired_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data)
{
    GError *error = NULL;

    printf("GBusAcquired_Callback has been invoked\n");
    printf("GBusAcquired_Callback the name = %s\n",name);
    printf("GBusAcquired_Callback the user_data = %s\n",(char*)user_data);

    skeleton =  test_skeleton_new ();
    g_signal_connect(skeleton,"handle-set-version",G_CALLBACK(on_handle_set_version),NULL);

    g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(skeleton), connection, "/com/qiaotsh/Building/Test", &error);
    if(error != NULL){                                                           
        g_print("Error: Failed to export object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }                                                                             
}

void GBusNameAcquired_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data)
{
    printf("GBusNameAcquired_Callback has been invoked\n");
}

void GBusNameLost_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data)
{
    printf("GBusNameLost_Callback has been invoked\n");
}

int main(int argc,char* argv[])
{
    char* s = "qiaotsh";
    GMainLoop* loop = NULL;

    guint own_id =
        g_bus_own_name (G_BUS_TYPE_SESSION,
                "com.qiaotsh.Building.Test",
                G_BUS_NAME_OWNER_FLAGS_NONE,
                GBusAcquired_Callback,
                GBusNameAcquired_Callback,
                GBusNameLost_Callback,
                (gpointer)s,
                NULL);

    /* * emit my signal1 every 1 second to trigger example communication */
    g_timeout_add(1000, (GSourceFunc)Emit_Test_Status, NULL);              

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_bus_unown_name(own_id);
    return 0;
}

