#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "ourfunctions.h"


int main()
{
   if (chdir("root") != 0) 
   {
      perror("chdir failed");
      return 1;
   }
   
   
    CommandMap map[MAX_COMMANDS];
    int count = load_map(map, "../command_map.txt");
    if (count == 0) 
      return 1;

    char choice[CMD_LEN];
    char input[CMD_LEN];
    char full_command[CMD_LEN + ARG_LEN];
    
        while (1) 
        {
          show_menu();
          fgets(choice, sizeof(choice), stdin);
          choice[strcspn(choice, "\n")] = 0;

          if (strcmp(choice, "1") == 0) 
          {
              printf("Enter DOS command: ");
              fgets(input, sizeof(input), stdin);
              input[strcspn(input, "\n")] = 0;

              // Split command and arguments
              char *cmd = strtok(input, " ");
              char *args = strtok(NULL, "");

              const char *linux_cmd = map_command(cmd, map, count);
              if (linux_cmd) 
              {
                  if (args)
                      snprintf(full_command, sizeof(full_command), "%s %s", linux_cmd, args);
                  else
                      snprintf(full_command, sizeof(full_command), "%s", linux_cmd);

                  system(full_command);
              }
              else 
              {
                  printf("Unrecognized DOS command: %s\n", cmd);
              }

          }
          else if (strcmp(choice, "2") == 0) 
          {
              printf("Enter task name (e.g., task4): ");
              fgets(input, sizeof(input), stdin);
              input[strcspn(input, "\n")] = 0;
              show_manual(input);

          }
          else if (strcmp(choice, "3") == 0)
          {
              printf("Exiting shell...\n");
              break;
          }
          else
          {
              printf("Invalid option. Please try again.\n");
          }
    }
   return 0;
}




/// Function Definitions///

// Show Menu
void show_menu()
{
    printf("\n=== DOS Shell Simulation ===\n");
    printf("1. Enter DOS command\n");
    printf("2. Show manual (e.g., man task4)\n");
    printf("3. Exit\n\n");
    printf("Choose an option: ");
}

// Load Command Map
int load_map(CommandMap map[], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open command_map.txt");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%[^=]=%s\n", map[count].dos, map[count].lin) == 2) {
        count++;
    }
    fclose(file);
    return count;
}

// Map Command
const char* map_command(const char *input, CommandMap map[], int count)
{
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(input, map[i].dos) == 0)
        {
            return map[i].lin;
        }
    }
    return NULL;
}

// Show Manual
void show_manual(const char *task_name)
{
    char filename[CMD_LEN + 10];
    snprintf(filename, sizeof(filename), "manual/%s.txt", task_name);
    
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Manual page not found for %s\n", task_name);
        return;
    }

    printf("\n--- Manual for %s ---\n", task_name);
    char line[200];
    while (fgets(line, sizeof(line), f))
    {
        printf("%s", line);
    }
    fclose(f);
    printf("\n------------------------\n");
}

