#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include "../headers/main.h"
#include "../headers/tree.h"

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

void logAction(char* action) {
    FILE *fp = fopen("bin/log", "a");
    if(fp) {
        fprintf(fp, action);
        fprintf(fp, "\n");
        fclose(fp);
    } else {
        printf("Error while logging command: %s", strerror(errno));
    }
}

char* currentPosition(char* cwd, int size) {
    if(getcwd(cwd, size) != 0) {
        return cwd;
    } else {
        printf("Error while retrieving cwd: %s", strerror(errno));
        return "Unknown";
    }
}

void changeDir(char *path) {
    chdir(path);
}

void buildTree(char *command) {
    char *token;
    node *root;
    
    token = strtok(command, " ");
    //root = createRoot(token);
    while(token != NULL) {
        printf("%s|", token);
        logAction(token);
        token = strtok(NULL, " ");
    }
}

void interactiveMode() {
    char *command;
    char path[8192];
    
    printf("Enter a command: \n");
    while(strcmp(command, "exit") != 0) {
        currentPosition(path, 8192);
        printf("\n%s > ", path);
        command = inputString(stdin, 10);
        buildTree(command);
    }

    if(strcmp(command, "exit") == 0) {
        exit(0);
    } else {
        exit(42);
    }
}

void checkMode(int sizeCommand, char *commands[]) {
    if(sizeCommand == 1) {
        printf("Activate interactive mode\n");
        interactiveMode();
    } else if(strcmp(commands[1], "-c") == 0) {
        printf("Executing command\n");
        buildTree(commands[2]);
    } else {
        printf("Bad argument error");
    }
}

int main(int argc, char** argv){
    checkMode(argc, argv);
    printf("\n");
}
