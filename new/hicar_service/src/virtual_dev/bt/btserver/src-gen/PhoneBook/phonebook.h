/*
 * Generated by gdbus-codegen 2.40.2. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef __PHONEBOOK_H__
#define __PHONEBOOK_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for com.hsaeyz.bluetooth.phoneBook */

#define TYPE_PHONE_BOOK (phone_book_get_type ())
#define PHONE_BOOK(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_PHONE_BOOK, PhoneBook))
#define IS_PHONE_BOOK(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_PHONE_BOOK))
#define PHONE_BOOK_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_PHONE_BOOK, PhoneBookIface))

struct _PhoneBook;
typedef struct _PhoneBook PhoneBook;
typedef struct _PhoneBookIface PhoneBookIface;

struct _PhoneBookIface
{
  GTypeInterface parent_iface;


  gboolean (*handle_cancel_down_load_phone_book) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_clearcalllogs) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr);

  gboolean (*handle_clearcontacts) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr);

  gboolean (*handle_deletecalllog) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr,
    const gchar *arg_datatime);

  gboolean (*handle_deletecontact) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr,
    const gchar *arg_name,
    const gchar *arg_num);

  gboolean (*handle_down_load_call_log) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    gint arg_type);

  gboolean (*handle_down_load_phone_book) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    gint arg_location);

  gboolean (*handle_getcontacts) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr);

  gboolean (*handle_savecontacts) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr);

  gboolean (*handle_searchcontact) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr,
    const gchar *arg_sql,
    gint arg_type);

  gboolean (*handle_syncdata) (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_addr,
    gint arg_type);

  void (*call_log_changed) (
    PhoneBook *object,
    const gchar *arg_callLogList);

  void (*new_calllog_changed) (
    PhoneBook *object,
    const gchar *arg_name,
    const gchar *arg_number,
    const gchar *arg_type,
    const gchar *arg_datatime,
    const gchar *arg_talktime);

  void (*pbap_down_load_state) (
    PhoneBook *object,
    gint arg_pbapDownLoadState);

  void (*phone_book_changed) (
    PhoneBook *object,
    const gchar *arg_contactlist);

};

GType phone_book_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *phone_book_interface_info (void);
guint phone_book_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void phone_book_complete_down_load_phone_book (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_down_load_call_log (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_cancel_down_load_phone_book (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_searchcontact (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_deletecontact (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_clearcontacts (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_savecontacts (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    gboolean result);

void phone_book_complete_getcontacts (
    PhoneBook *object,
    GDBusMethodInvocation *invocation,
    gboolean dataexits);

void phone_book_complete_syncdata (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_deletecalllog (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);

void phone_book_complete_clearcalllogs (
    PhoneBook *object,
    GDBusMethodInvocation *invocation);



/* D-Bus signal emissions functions: */
void phone_book_emit_pbap_down_load_state (
    PhoneBook *object,
    gint arg_pbapDownLoadState);

void phone_book_emit_phone_book_changed (
    PhoneBook *object,
    const gchar *arg_contactlist);

void phone_book_emit_new_calllog_changed (
    PhoneBook *object,
    const gchar *arg_name,
    const gchar *arg_number,
    const gchar *arg_type,
    const gchar *arg_datatime,
    const gchar *arg_talktime);

void phone_book_emit_call_log_changed (
    PhoneBook *object,
    const gchar *arg_callLogList);



/* D-Bus method calls: */
void phone_book_call_down_load_phone_book (
    PhoneBook *proxy,
    gint arg_location,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_down_load_phone_book_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_down_load_phone_book_sync (
    PhoneBook *proxy,
    gint arg_location,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_down_load_call_log (
    PhoneBook *proxy,
    gint arg_type,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_down_load_call_log_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_down_load_call_log_sync (
    PhoneBook *proxy,
    gint arg_type,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_cancel_down_load_phone_book (
    PhoneBook *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_cancel_down_load_phone_book_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_cancel_down_load_phone_book_sync (
    PhoneBook *proxy,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_searchcontact (
    PhoneBook *proxy,
    const gchar *arg_addr,
    const gchar *arg_sql,
    gint arg_type,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_searchcontact_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_searchcontact_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    const gchar *arg_sql,
    gint arg_type,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_deletecontact (
    PhoneBook *proxy,
    const gchar *arg_addr,
    const gchar *arg_name,
    const gchar *arg_num,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_deletecontact_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_deletecontact_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    const gchar *arg_name,
    const gchar *arg_num,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_clearcontacts (
    PhoneBook *proxy,
    const gchar *arg_addr,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_clearcontacts_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_clearcontacts_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_savecontacts (
    PhoneBook *proxy,
    const gchar *arg_addr,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_savecontacts_finish (
    PhoneBook *proxy,
    gboolean *out_result,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_savecontacts_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    gboolean *out_result,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_getcontacts (
    PhoneBook *proxy,
    const gchar *arg_addr,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_getcontacts_finish (
    PhoneBook *proxy,
    gboolean *out_dataexits,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_getcontacts_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    gboolean *out_dataexits,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_syncdata (
    PhoneBook *proxy,
    const gchar *arg_addr,
    gint arg_type,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_syncdata_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_syncdata_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    gint arg_type,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_deletecalllog (
    PhoneBook *proxy,
    const gchar *arg_addr,
    const gchar *arg_datatime,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_deletecalllog_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_deletecalllog_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    const gchar *arg_datatime,
    GCancellable *cancellable,
    GError **error);

void phone_book_call_clearcalllogs (
    PhoneBook *proxy,
    const gchar *arg_addr,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean phone_book_call_clearcalllogs_finish (
    PhoneBook *proxy,
    GAsyncResult *res,
    GError **error);

gboolean phone_book_call_clearcalllogs_sync (
    PhoneBook *proxy,
    const gchar *arg_addr,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define TYPE_PHONE_BOOK_PROXY (phone_book_proxy_get_type ())
#define PHONE_BOOK_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_PHONE_BOOK_PROXY, PhoneBookProxy))
#define PHONE_BOOK_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_PHONE_BOOK_PROXY, PhoneBookProxyClass))
#define PHONE_BOOK_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_PHONE_BOOK_PROXY, PhoneBookProxyClass))
#define IS_PHONE_BOOK_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_PHONE_BOOK_PROXY))
#define IS_PHONE_BOOK_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_PHONE_BOOK_PROXY))

typedef struct _PhoneBookProxy PhoneBookProxy;
typedef struct _PhoneBookProxyClass PhoneBookProxyClass;
typedef struct _PhoneBookProxyPrivate PhoneBookProxyPrivate;

struct _PhoneBookProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  PhoneBookProxyPrivate *priv;
};

struct _PhoneBookProxyClass
{
  GDBusProxyClass parent_class;
};

GType phone_book_proxy_get_type (void) G_GNUC_CONST;

void phone_book_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
PhoneBook *phone_book_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
PhoneBook *phone_book_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void phone_book_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
PhoneBook *phone_book_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
PhoneBook *phone_book_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_PHONE_BOOK_SKELETON (phone_book_skeleton_get_type ())
#define PHONE_BOOK_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_PHONE_BOOK_SKELETON, PhoneBookSkeleton))
#define PHONE_BOOK_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_PHONE_BOOK_SKELETON, PhoneBookSkeletonClass))
#define PHONE_BOOK_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_PHONE_BOOK_SKELETON, PhoneBookSkeletonClass))
#define IS_PHONE_BOOK_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_PHONE_BOOK_SKELETON))
#define IS_PHONE_BOOK_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_PHONE_BOOK_SKELETON))

typedef struct _PhoneBookSkeleton PhoneBookSkeleton;
typedef struct _PhoneBookSkeletonClass PhoneBookSkeletonClass;
typedef struct _PhoneBookSkeletonPrivate PhoneBookSkeletonPrivate;

struct _PhoneBookSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  PhoneBookSkeletonPrivate *priv;
};

struct _PhoneBookSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType phone_book_skeleton_get_type (void) G_GNUC_CONST;

PhoneBook *phone_book_skeleton_new (void);


G_END_DECLS

#endif /* __PHONEBOOK_H__ */
