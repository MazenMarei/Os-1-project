///Constants and structs///
#define MAX_COMMANDS 100
#define CMD_LEN 100
#define ARG_LEN 200
#define PATH_LEN 200

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

// split into commands and arguments
void splitIntoCommandsAndArguments(char* input, char *cmd, char *args, int *doArgsExist, CommandMap map[], int count);

// run command
void runCommand(const char *linux_cmd, int doArgsExist, char *args, char *cmd);

// Is a part cmd?
int isPartCmd(const char *inputPart2, CommandMap map[], int count);

// Change Directory
void changeDirectory(const char *path);

// get parent directory
void getParentDirectory(char *path);

// Show Manual
void show_manual();
