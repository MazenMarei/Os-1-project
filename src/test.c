#include "main.c"


int main()
{
   char root[PATH_LEN];
//    strcpy(root, "~/Desktop/gui learn/shellApp/src/OS1-Project/root");
   
//    changeDirectory(root);
   
    CommandMap map[MAX_COMMANDS];
    int count = load_map(map, "./command_map.txt");
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
                  char *inputPart1 = strtok(input, " ");
                  char *inputPart2 = strtok(NULL, "");
                  
                  //printf("%s\n",inputPart1);
                  //printf("%s\n",inputPart2);
                  
   
                  char cmd[CMD_LEN]  ;
                  char args[ARG_LEN] ;
                  int doArgsExist = 0;
                  
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
                          
                          //printf("%s\n",inputPart2_1);
                          //printf("%s\n",inputPart2_2);
                          
                          if(isPartCmd(inputPart2_1, map, count))
                          {
                              strcpy(args, inputPart2_2);
                              snprintf(cmd, CMD_LEN, "%s %s", inputPart1,inputPart2_1);
                              doArgsExist = 1;
                              
                              //printf("%s ",args);
                              //printf("%s\n",cmd);
                          }
                          else
                          {
                              strcpy(args, inputPart2_2);
                              snprintf(args, ARG_LEN, "%s %s", inputPart2_1, inputPart2_2);
                              snprintf(cmd, CMD_LEN, "%s", inputPart1);
                              doArgsExist = 1;
                              
                              //printf("%s\n",cmd);
                              //printf("%s\n",args);
                             
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