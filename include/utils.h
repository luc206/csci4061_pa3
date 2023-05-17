#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define chunkSize 1024
#define acctsNum 1000

struct Packet {
    struct Packet* next;
    char* data;
    int line;
};

/* shared array definition */
extern double balance[acctsNum];

/* shared head and tail value for linked link */
extern struct Packet* head;
extern struct Packet* tail;

/* shared locks and semaphore */
extern sem_t sem_mutex;
extern sem_t sem_cond;
extern sem_t sem_bound;
extern pthread_mutex_t acctsLock[acctsNum];

/* shared values */
extern int consumer_num;
extern int bound;

/* shared log file pointer */
extern FILE *log_fp;

/* file I/O */
/**
 * Get a pointer to a opened file based on the file name
 * @param *inputFileName  the file path
 * @return a file pointer pointing to the file
 */
FILE * getFilePointer(char *inputFileName);

/**
 * Read an entire line from a file
 * @param  *fp    the file to be read
 * @param  *line  contain the line content
 * @param  len    the size of the line
 * @return the number of character reads (including the newline \n, but not including terminator)
           -1 when reaching the end of file or error occurs
 */
ssize_t getLineFromFile(FILE *fp, char *line, size_t len);

/**
 * Open a file, and write a line to the file
 * @param *filepath  the file path
 * @param *line      the line content
 */
void writeLineToFile(char *filepath, char *line);

/* directory */
void bookeepingCode();

/* other function declaration */
void insert(char* line, int lineNum);
void insertEOF();
struct Packet* extract();


#endif
