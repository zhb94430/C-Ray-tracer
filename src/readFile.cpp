//
//  readFile.cpp
//  
//
//  Created by Peter Zhang on 14-9-29.
//
//

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

char* readFile (int argc, char **argv) {
    FILE *filePointer;
    filePointer = fopen(argv[1], "r");
    
    struct stat fileStatus;
    stat(argv[1], &fileStatus);
    
    int fileSize = fileStatus.st_size + 1;
    
    char *buffer = (char *) malloc(sizeof(char) * fileSize);
    
    int index = 0;
    char ch = getc(filePointer);
    
    while (ch != EOF) {
        buffer[index] = ch;
        index++;
        ch = getc(filePointer);
    }
    buffer[index] = '\0';
    
    return buffer;
}