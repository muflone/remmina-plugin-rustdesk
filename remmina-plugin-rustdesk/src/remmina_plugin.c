/*
 *     Project: Remmina Plugin RUSTDESK
 * Description: Remmina protocol plugin to launch a Rustdesk connection.
 *      Author: Fabio Castelli (Muflone) <muflone@muflone.com>
 *   Copyright: 2022 Fabio Castelli (Muflone)
 *     License: GPL-2+
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of ERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugin_config.h"
#include <remmina/remmina_plugin.h>

typedef struct {
  GtkTextView *text_view;
  GtkTextBuffer *text_buffer;
  GPid pid;
} RemminaPluginData;

static RemminaPluginService *remmina_plugin_service = NULL;

/* Initialize plugin */
static void remmina_plugin_rustdesk_init(RemminaProtocolWidget *gp) {
  TRACE_CALL(__func__);
  RemminaPluginData *gpdata;
  remmina_plugin_service->log_printf("[%s] Plugin init\n", PLUGIN_NAME);
  /* Instance log window widgets */
  gpdata = g_new0(RemminaPluginData, 1);
  gpdata->text_view = GTK_TEXT_VIEW(gtk_text_view_new());
  gtk_text_view_set_editable(gpdata->text_view, FALSE);
  gtk_container_add(GTK_CONTAINER(gp), GTK_WIDGET(gpdata->text_view));
  gpdata->text_buffer = gtk_text_view_get_buffer(gpdata->text_view);
  gtk_text_buffer_set_text(gpdata->text_buffer, PLUGIN_DESCRIPTION, -1);
  gtk_widget_show(GTK_WIDGET(gpdata->text_view));
  /* Save reference to plugin data */
  g_object_set_data_full(G_OBJECT(gp), "plugin-data", gpdata, g_free);
}

/* Open connection */
static gboolean remmina_plugin_rustdesk_open_connection(RemminaProtocolWidget *gp) {
  TRACE_CALL(__func__);
  RemminaFile *remminafile;
  gboolean ret;
  GError *error = NULL;
  gchar *argv[50];  // Contains all the arguments included the password
  gchar *argv_debug[50]; // Contains all the arguments, excluding the password
  gchar *command_line; // The whole command line obtained from argv_debug
  gint argc;
  gint i;
  gchar *option_str;
  RemminaPluginData *gpdata;

  #define GET_PLUGIN_STRING(value) \
    g_strdup(remmina_plugin_service->file_get_string(remminafile, value))
  #define GET_PLUGIN_BOOLEAN(value) \
    remmina_plugin_service->file_get_int(remminafile, value, FALSE)
  #define ADD_ARGUMENT(name, value) { \
      argv[argc] = g_strdup(name); \
      argv_debug[argc] = g_strdup(name); \
      argc++; \
      if (value != NULL) { \
        argv[argc] = value; \
        argv_debug[argc++] = g_strdup(g_strcmp0(name, "-p") != 0 ? value : "XXXXX"); \
      } \
    }

  remmina_plugin_service->log_printf("[%s] Plugin open connection\n", PLUGIN_NAME);
  remminafile = remmina_plugin_service->protocol_plugin_get_file(gp);

  gpdata = (RemminaPluginData*) g_object_get_data(G_OBJECT(gp), "plugin-data");

  argc = 0;
  // Main executable name
  ADD_ARGUMENT("rustdesk", NULL);

  // Server address
  option_str = GET_PLUGIN_STRING("server");
  ADD_ARGUMENT("--connect", option_str);
  // End of the arguments list
  ADD_ARGUMENT(NULL, NULL);
  // Retrieve the whole command line
  command_line = g_strjoinv(g_strdup(" "), (gchar **)&argv_debug[0]);
  remmina_plugin_service->log_printf("[RUSTDESK] starting %s\n", command_line);
  g_free(command_line);
  // Execute the external process rdesktop
  ret = g_spawn_async(NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &gpdata->pid, &error);
  remmina_plugin_service->log_printf("[RUSTDESK] started rustdesk with GPid %d\n", &gpdata->pid);
  // Free the arguments list
  for (i = 0; i < argc; i++) {
    g_free(argv_debug[i]);
    g_free(argv[i]);
  }
  // Show error message
  if (!ret) {
    remmina_plugin_service->protocol_plugin_set_error(gp, "%s", error->message);
  }
  remmina_plugin_service->protocol_plugin_signal_connection_opened(gp);
  return TRUE;
}

/* Close connection */
static gboolean remmina_plugin_rustdesk_close_connection(RemminaProtocolWidget *gp) {
  TRACE_CALL(__func__);
  remmina_plugin_service->log_printf("[%s] Plugin close connection\n", PLUGIN_NAME);
  remmina_plugin_service->protocol_plugin_signal_connection_closed(gp);
  return FALSE;
}

/* Array of RemminaProtocolSetting for basic settings.
 * Each item is composed by:
 * a) RemminaProtocolSettingType for setting type
 * b) Setting name
 * c) Setting description
 * d) Compact disposition
 * e) Values for REMMINA_PROTOCOL_SETTING_TYPE_SELECT or REMMINA_PROTOCOL_SETTING_TYPE_COMBO
 * f) Setting tooltip
 */
static const RemminaProtocolSetting remmina_plugin_rustdesk_basic_settings[] = {
  { REMMINA_PROTOCOL_SETTING_TYPE_SERVER, "server", NULL, FALSE, NULL, NULL },
  { REMMINA_PROTOCOL_SETTING_TYPE_END, NULL, NULL, FALSE, NULL, NULL }
};

/* Protocol plugin definition and features */
static RemminaProtocolPlugin remmina_plugin = {
  REMMINA_PLUGIN_TYPE_PROTOCOL,                 // Type
  PLUGIN_NAME,                                  // Name
  PLUGIN_DESCRIPTION,                           // Description
  GETTEXT_PACKAGE,                              // Translation domain
  PLUGIN_VERSION,                               // Version number
  PLUGIN_APPICON,                               // Icon for normal connection
  PLUGIN_APPICON,                               // Icon for SSH connection
  remmina_plugin_rustdesk_basic_settings,       // Array for basic settings
  NULL,                                         // Array for advanced settings
  REMMINA_PROTOCOL_SSH_SETTING_NONE,            // SSH settings type
  NULL,                                         // Array for available features
  remmina_plugin_rustdesk_init,                 // Plugin initialization
  remmina_plugin_rustdesk_open_connection,      // Plugin open connection
  remmina_plugin_rustdesk_close_connection,     // Plugin close connection
  NULL,                                         // Query for available features
  NULL,                                         // Call a feature
  NULL,                                         // Send a keystroke
  NULL                                          // Screenshot support
};

G_MODULE_EXPORT gboolean remmina_plugin_entry(RemminaPluginService *service) {
  TRACE_CALL(__func__);
  remmina_plugin_service = service;

  if (!service->register_plugin((RemminaPlugin *) &remmina_plugin)) {
    return FALSE;
  }
  return TRUE;
}
