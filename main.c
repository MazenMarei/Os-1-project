#include <gtk/gtk.h>
#include <string.h>
#include "src/main.h"
typedef struct {
    GApplication *app;
    GtkWidget *window;
    GtkBuilder *builder;
    GtkCssProvider *css_provider;
    GtkWidget *cmdText;
    GtkWidget *cmdInput;
    char currentDir[PATH_LEN];
    CommandMap map[MAX_COMMANDS];
    int count;
} AppData;

// Load CSS from file
gboolean load_css_from_file(GtkCssProvider *provider, const gchar *file_path) {
    GError *error = NULL;
    
    
    // load CSS file
    gtk_css_provider_load_from_path(provider, file_path);
    
    if (error) {
        g_printerr("Error loading CSS file: %s\n", error->message);
        g_error_free(error);
        return FALSE;
    }
    
    return TRUE;
}

// handle when enters an input
void handleCmdInputs( GtkEditable* self, AppData* data) {
    const char *homeDir = getenv("HOME");

    const char *cmdInput = gtk_entry_buffer_get_text( gtk_entry_get_buffer(GTK_ENTRY(data->cmdInput)));
    if (cmdInput == NULL || strlen(cmdInput) == 0) return;
    char tempCmd[500];
    strcpy(tempCmd , cmdInput);

    GtkWidget *shellScreen = GTK_WIDGET(gtk_builder_get_object(data->builder, "cmdText"));
    GtkTextBuffer *oldShellBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(shellScreen));

    GtkTextIter  end;
    gtk_text_buffer_get_end_iter(oldShellBuffer, &end);

    char FinallInpt[PATH_LEN * 3]; 

    getcwd(data->currentDir, sizeof(data->currentDir));

    /// replace the home dir with ~
    if(strcmp(data->currentDir , homeDir) > 1) {
        GRegex *regex = g_regex_new(homeDir, 0, 0, NULL);  
        
        gchar *result = g_regex_replace(regex, data->currentDir , -1, 0, "~", 0, NULL);
        strcpy(data->currentDir  , result);

        g_regex_unref(regex);
        g_free(result);
    }


    GtkTextTagTable *tagTable = gtk_text_buffer_get_tag_table(oldShellBuffer);
    GtkTextTag *promptTag = gtk_text_tag_table_lookup(tagTable, "prompt");
    GtkTextTag *promptTag2 = gtk_text_tag_table_lookup(tagTable, "prompt2");


    if (!promptTag) {
        promptTag = gtk_text_buffer_create_tag(oldShellBuffer, "prompt", "foreground", "#26a168", "weight", 800,  NULL);
    }
    if(!promptTag2) {
        promptTag2 = gtk_text_buffer_create_tag(oldShellBuffer, "prompt2", "foreground", "#B1A98F",  NULL); 

    }

    snprintf(FinallInpt, sizeof(FinallInpt), "%s>", data->currentDir);
    gtk_text_buffer_insert_with_tags_by_name(oldShellBuffer, &end,FinallInpt, -1, "prompt", NULL);

    char * cmd = runCmd(tempCmd , data->map , data-> count);

    snprintf(FinallInpt, sizeof(FinallInpt), " %s\n", cmdInput);

    gtk_text_buffer_insert_with_tags_by_name(oldShellBuffer, &end, FinallInpt, -1, "prompt2", NULL);

    snprintf(FinallInpt, sizeof(FinallInpt), "%s", cmd );


    gtk_text_buffer_insert(oldShellBuffer, &end, FinallInpt , -1);



    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(data->cmdInput)), "", -1);
    // params ( textview , iter , margin from 0 to 0.5 , bool , x , y)
    gtk_text_view_scroll_to_iter (GTK_TEXT_VIEW(shellScreen),&end,0,TRUE,0, 0);
    

    
}

// Load Ui From file while activate
void activate(GApplication * app , gpointer user_data)  {
    // Initialize app data
    AppData *data = (AppData*)user_data;

    // Load UI from file
    data->builder   = gtk_builder_new_from_file ("./assets/ui/main.ui");
    data->window    = GTK_WIDGET (gtk_builder_get_object (data->builder, "win"));
    // Load CSS from file
    data->css_provider = gtk_css_provider_new();

    if(!data->builder) {
        g_print("Failed to load the ui file from /assets/ui/main.ui");
    }
    // check if the window init succ
    if(!data->window) {
        g_print("Failed to get the window from the ui file");
    }

    // load  CSS file
    if (!load_css_from_file(data->css_provider, "./assets/css/style.css")) {
        g_printerr("Failed to load CSS file from  /assets/css/style.css\n");
    }

    // Apply the CSS to the default display
    gtk_style_context_add_provider_for_display( gdk_display_get_default(),
        GTK_STYLE_PROVIDER(data->css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    


    // set the app
    gtk_window_set_application (GTK_WINDOW (data->window), GTK_APPLICATION (app));

    gtk_window_present (GTK_WINDOW(data->window ));

    // init cmdText
    data->cmdText = GTK_WIDGET(gtk_builder_get_object(data->builder, "cmdText"));
    // init cmdInput
    data->cmdInput = GTK_WIDGET(gtk_builder_get_object(data->builder, "cmdInput"));
    GtkWidget *EnterBtn = GTK_WIDGET(gtk_builder_get_object(data->builder, "EnterBtn"));
    g_signal_connect((data->cmdInput) ,"activate" , G_CALLBACK(handleCmdInputs) ,data);
    g_signal_connect((EnterBtn) ,"clicked" , G_CALLBACK(handleCmdInputs) , data);



}






int main (int argc, char *argv[]) {


    // init AppData struct 
    AppData app_data = { NULL, NULL, NULL, NULL  , 0};

    /// init the commands map
    app_data.count = load_map(app_data.map, "./src/command_map.txt");


    // init the app
    GtkApplication * app = gtk_application_new ("dev.racoon", 0);

    g_signal_connect(app , "activate" , G_CALLBACK(activate),  &app_data);


    int stat = g_application_run (G_APPLICATION (app), argc, argv);
    



    if(app_data.builder) {
        // free builder
        g_object_unref(app_data.builder );
    }
    // Free app
    g_object_unref (app);

    return stat;
    
}