#include <string.h>
#include "partitions/partition.h"
#include "hfsp/hfsp.h"

void show_parts(){
    part parts[200];
    uint64_t size = get_parts(parts, 200);
    printf("\033[1m\033[36m\n");
    printf(" MAJ | MIN | SIZE(Mb) | NAME\n");
    printf("\033[0m");
    printf(" --- | --- | -------- | -----\n");
    for (int i = 0; i < size; ++i) {
        printf(" %3d | %3d | %8lu | %s\n", parts[i].maj, parts[i].min, parts[i].size_mb, parts[i].name);
    }
    printf("\n");
}

void work_with_fs(){
	printf("\nPut path:\n");
	char path[50];
	fgets(path, 50, stdin);
	path[strlen(path)-1] = '\0';
	hfsplus *fs = read_hfsplus(strtok(path, " "));
	if (ERR == ERR_RESERVE) {
	    printf("\033[0;31m");
	    puts("Error while reading volume header: wrong reserved space");
	    printf("\033[0m");
	    return ;
	}
	if (ERR == ERR_WRONG_STRUCTURE) {
	    printf("\033[0;31m");
	    puts("Error while reading fs: file system haven't hfs+ structure");
	    printf("\033[0m");
	    return ;
	}
	
        printf("\033[0;32m");
        printf(">");
        puts(fs->pwd);
        printf("\033[0m");
        
        char temp[50];
        fgets(temp, 50, stdin);
        uint32_t read = strlen(temp);
        temp[read - 1] = '\0';
        char *command = strtok(temp, " ");
        char *buffer[100];
        while (strcmp(command, "exit") != 0) {
            if (strcmp(command, "ls") == 0) {
                int *is_file = malloc(sizeof(int));
                uint32_t cnt = ls(fs, (char **) buffer, is_file);
                printf("\033[0;33m");
                printf("\n(type) | name\n");
                printf("-----  | ----    \n");
                for (int i = 0; i < cnt; ++i) {
                    if(*is_file == 0){
                        printf("\033[0;34m");
                        printf("(d)    | ");                       
                    }else if(*is_file == 1){
                        printf("\033[0;36m");
                        printf("(f)    | ");
                    }
                    puts(buffer[i]);
                    printf("\033[0m");
                }
            } else if (strcmp(command, "cd") == 0) {
                char *pwd = cd(fs, strtok(NULL, " "));
                if (pwd == NULL) {
                    if (ERR == ERR_CANT_FIND) {
                        printf("\033[0;31m");
                        puts("WARNING during execution of command 'cd': No such file or directory");
                        printf("\033[0m");
                    } else if (ERR == ERR_FIND_FILE) {
                        printf("\033[0;31m");
                        puts("WARNING during execution of command 'cd': It is a file");
                        printf("\033[0m");
                    }
                } else {
                    printf("\033[0;32m");
                    printf(">");
                    puts(pwd);
                    printf("\033[0m");
                }
            } else if (strcmp(command, "pwd") == 0) {
                printf("\033[0;32m");
                printf(">");
                puts(pwd(fs));
                printf("\033[0m");
            } else if (strcmp(command, "help") == 0) {
                printf("\033[0;33m");
                puts("ls\n"
                     "cd [directory]\n"
                     "back\n"
                     "pwd\n"
                     "copy [directory | file] [target directory]\n"
                     "exit");
                printf("\033[0m");
                
            } else if (strcmp(command, "back") == 0) {
                char *pwd = back(fs);
                if (pwd == NULL && ERR == ERR_ON_ROOT) {
                    printf("\033[0;31m");
                    puts("WARNING during execution of command 'back': It is root!");
                    printf("\033[0m");
                } else {
                    printf("\033[0;32m");
                    printf(">");
                    puts(pwd);
                    printf("\033[0m");
                }
            } else if (strcmp(command, "copy") == 0) {
                char *filename = strtok(NULL, " ");
                char *destination = strtok(NULL, " ");
                printf("\033[0;33m");
                printf("copying from %s" , filename);
                printf(" to %s...\n" , destination);
                printf("\033[0m");
                uint32_t res = copy(filename, destination, fs);
                if (res < 0) {
                    printf("\033[0;31m");
                    puts("WARNING during execution of command 'copy'");
                    printf("\033[0m");
                }else{
                    printf("\033[0;32m");
                    puts("DONE!");
                    printf("\033[0m");
                }
            } else {
                printf("\033[0;31m");
                puts("Wrong command");
                printf("\033[0m");
            }
            
            fgets(temp, 50, stdin);
            read = strlen(temp);
            temp[read - 1] = '\0';
            command = strtok(temp, " ");
        }
}
int main(int argc, char **argv) {
    if(argc < 2){
        printf("\nWELCOME!\n");
        printf("\033[0;33m");
        printf("---to show partitions start with '--parts' arg\n");
        printf("---to work with filesystem start with '--fs' arg\n");
        printf("\033[0m\n");
        return 0;
    }
    if (strcmp(argv[1], "--parts") == 0) {
	show_parts();
    } else if(strcmp(argv[1], "--fs") == 0) {
    	work_with_fs();
    }else{
        printf("\033[0;31m");
        puts("Wrong arg");
        printf("\033[0m");
    }
}
