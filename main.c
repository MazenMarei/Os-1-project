#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "ourfunctions.h"


int main()
{
   char root[PATH_LEN];
   strcpy(root, "/home/omar-ayman/OS1-Project/root");
   
   changeDirectory(root);
   
    CommandMap map[MAX_COMMANDS];
    int count = load_map(map, "../command_map.txt");
    if (count == 0) 
      return 1;

    char choice[CMD_LEN];
    char input[CMD_LEN];
    char full_command[CMD_LEN + ARG_LEN];
    
        while (1) 
        {
            system("clear");
            show_menu();
            
            fgets(choice, sizeof(choice), stdin);
            choice[strcspn(choice, "\n")] = 0;

            if (strcmp(choice, "1") == 0) 
            {
                system("clear");
                while(1)
                {
                  printf("DOSComnand$ ");
                  
                  fgets(input, sizeof(input), stdin);
                  input[strcspn(input, "\n")] = 0;
                  
                  if(strcmp(input, "eee") == 0)
                  {
                    system("clear");
                    break;
                  }

                  // Split command and arguments
                  char cmd[CMD_LEN]  ;
                  char args[ARG_LEN] ;
                  int doArgsExist = 0;
                  
                  char *inputPart1 = strtok(input, " ");
                  char *inputPart2 = strtok(NULL, "");
                  
                  if(inputPart2)
                  {
                      int spaceInInputSecondPartExists = 0;
                      char *p = inputPart2;
                      if (inputPart2)
                      {
                          while (*p)
                          {
                              if (*p == ' ')
                              {
                                  spaceInInputSecondPartExists = 1;
                                  break;
                              }
                              p++;
                          }
                      }
                      
                      char *inputPart2_1 = NULL;
                      char *inputPart2_2 = NULL;
                      if(spaceInInputSecondPartExists == 1)
                      {
                          inputPart2_1 = strtok(inputPart2, " ");
                          inputPart2_2 = strtok(NULL, "");
                          
                          if(isPartCmd(inputPart2_1, map, count))
                          {
                              strcpy(args, inputPart2_2);
                              snprintf(cmd, CMD_LEN, "%s %s", inputPart1,inputPart2_1);
                              doArgsExist = 1;
                          }
                          else
                          {
                              strcpy(cmd, inputPart1);
                              snprintf(args, ARG_LEN, "%s %s", inputPart2_1, inputPart2_2);
                              doArgsExist = 1;
                          }

                      }
                      else
                      {
                          if (isPartCmd(inputPart2, map, count))
                          {
                              snprintf(cmd, CMD_LEN, "%s %s", inputPart1,inputPart2);
                          }
                          else
                          {
                              snprintf(cmd, CMD_LEN, "%s", inputPart1);
                              strcpy(args, inputPart2);
                              doArgsExist = 1;
                          }
                      }
                  }
                  else
                  {
                      strcpy(cmd, inputPart1);
                  }

                  const char *linux_cmd = map_command(cmd, map, count);
                  if (linux_cmd) 
                  {
                      if(strcmp(linux_cmd, "cd") == 0 || strcmp(linux_cmd, "cd ..") == 0)
                      {
                          if(strcmp(linux_cmd, "cd") == 0)
                             changeDirectory(args);
                          else
                          {
                             char cwd[PATH_LEN];
                             getcwd(cwd, sizeof(cwd));
                             getParentDirectory(cwd);
                             changeDirectory(cwd);
                          } 
                      }
                      else
                      {
                          if (doArgsExist)
                            snprintf(full_command, sizeof(full_command), "%s %s", linux_cmd, args);
                          else
                            snprintf(full_command, sizeof(full_command), "%s", linux_cmd);

                           system(full_command);
                      }

                  }
                  else 
                  {
                      printf("Unrecognized DOS command: %s\n", cmd);
                  }
                  
                }
              
            }
            else if (strcmp(choice, "2") == 0) 
            {
                system("clear");
                while(1)
                {
                  printf("Enter task name (e.g., task4): ");
                  fgets(input, sizeof(input), stdin);
                  input[strcspn(input, "\n")] = 0;
                  
                  if(strcmp(input, "eee") == 0)
                  {
                    system("clear");
                    break;
                  }
                  
                  show_manual(input);
                }
                
            }
            else if (strcmp(choice, "3") == 0)
            {
                system("clear");
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
    printf("To view this menu again after choosing one of these, type the command eee\n\n");
    printf("Choose an option: ");
}


// Load Command Map
int load_map(CommandMap map[], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Cannot open command_map.txt");
        return 0;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        char *dos = strtok(line, "=");
        char *lin = strtok(NULL, "");  // Get rest of line after '='

        if (dos && lin)
        {
            strncpy(map[count].dos, dos, CMD_LEN);
            strncpy(map[count].lin, lin, CMD_LEN);
            count++;
        }
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


// Is second part a cmd?
int isPartCmd(const char *inputPart, CommandMap map[], int count)
{
      for(int i = 0; i < count; i++)
      {
          if(strstr(map[i].dos, inputPart))
          {
              return 1;
          }
          
      }
    
    return 0;

}

// Change directory
void changeDirectory(const char *path)
{
     if (chdir(path) != 0) 
     {
        perror("chdir failed");
        exit(1);
     }
}

// get parent directory
void getParentDirectory(char *path)
{
    char *last_slash = strrchr(path, '/');
    if(last_slash != NULL)
    {
        *last_slash = '\0';
    }
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

