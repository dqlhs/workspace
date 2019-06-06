/*
 * =====================================================================================
 *
 *       Filename:  gdbus_server.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/31/2017 10:11:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qiaotsh
 * =====================================================================================
 */
#ifndef __GDBUS_SERVER_H_
#define __GDBUS_SERVER_H_


#define GDBUS_API  __attribute__((visibility("hidden")))

GDBUS_API void GBusAcquired_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data);

GDBUS_API void GBusNameAcquired_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data);

GDBUS_API void GBusNameLost_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data);


#endif

