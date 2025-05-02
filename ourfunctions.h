///Constants and structs///
#define MAX_COMMANDS 100
#define CMD_LEN 100
#define ARG_LEN 200

typedef struct 
{
    char dos[CMD_LEN];
    char lin[CMD_LEN];
} CommandMap;


/// Functions///

// Show Menu
void show_menu();

// Load Command Map
int load_map(CommandMap map[], const char *filename);

// Map Command
const char* map_command(const char *input, CommandMap map[], int count);

// Show Manual
void show_manual(const char *task_name);
