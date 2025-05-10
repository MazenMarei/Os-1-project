///Constants and structs///
#define MAX_COMMANDS 500
#define CMD_LEN 100
#define ARG_LEN 200
#define PATH_LEN 200

#include <stdio.h>

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
const char* map_command(const char *input, CommandMap map[] , int count);

// Is a part cmd?
int isPartCmd(const char *inputPart2, CommandMap map[], int count);

// Change Directory
void changeDirectory(const char *path);

// get parent directory
void getParentDirectory(char *path);

// Show Manual
void show_manual(const char *task_name);


char *runCmd(char *cmd , CommandMap map[MAX_COMMANDS] , int count);