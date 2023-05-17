/*  test machine: atlas.cselabs.umn.edu (ssh)
*   group number: G19
*   name: Timothy Anderson, Luc Rubin, Hank Hanson
*   x500: and09985, rubin206, hans7186 */

#include "utils.h"
// pthread.h included in header.h

// Feel free to add any functions or global variables

/* File operations */
void writeLineToFile(char *filepath, char *line) {
    int fd = open(filepath, O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (fd < 0){
        printf("ERROR: Cannot open the file %s\n", filepath);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    int ret = write(fd, line, strlen(line));
    if(ret < 0){
        printf("ERROR: Cannot write to file %s\n", filepath);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
}

FILE * getFilePointer(char *inputFileName) {
    return fopen(inputFileName, "r");
}

ssize_t getLineFromFile(FILE *fp, char *line, size_t len) {
    memset(line, '\0', len);
    return getline(&line, &len, fp);
}

void _removeOutputDir(){
    pid_t pid = fork();
    if(pid == 0){
        char *argv[] = {"rm", "-rf", "output", NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else{
        wait(NULL);
    }
}

void _createOutputDir(){
    mkdir("output", ACCESSPERMS);
}

void bookeepingCode(){
    _removeOutputDir();
    sleep(1);
    _createOutputDir();
}

/* other functions */
void insert(char* line, int lineNum) { 
    /* Inserts a new element at the tail end of the list with the data passed in */
    struct Packet* new = (struct Packet*) malloc(sizeof(struct Packet));
    
    new->data = strdup(line); // Duplicated the string to a new memory location
    new->next = NULL; // Initial next points to null
    new->line = lineNum;
    
    if (head && tail) {
        tail->next = new;
        tail = new;
    }
    else {
        head = tail = new;
    }
}

struct Packet* extract() {
    /* Returns the head element of the list and removes it from the list */
    struct Packet* ret = head;
    if (head) head = head->next;
    if (!head) tail = NULL;
    return ret;
}