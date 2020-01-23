#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

 int parse_file(char filename[], book book_array[]){
    int i;
    char booklist[720];

    FILE *fpin = fopen(filename, "r");

    if(fpin == NULL){
        printf("ERROR: FILE NOT FOUND");
        exit(-1);
    }else{
        printf("SUCCESS\n");
    }

    while(fgets(booklist, 720, fpin) != NULL){
        char *token = strtok(booklist, ",");            // "." - gives list of all stuff

        if(strcmp(token,"N/A")){
            //printf("%s\n", token);
            strcpy(book_array[i].title, token);
        }
        token = strtok(NULL, ",\n");

        if(strcmp(token,"N/A")){
            strcpy(book_array[i].author_name, token);
        }
        token = strtok(NULL, ",\n");

        if(strcmp(token,"N/A")){
            strcpy(book_array[i].ISBN, token);
        }
        token = strtok(NULL, ",\n");

        if(strcmp(token, "N/A")){
            book_array[i].pages = atoi(token);
        }else{
            book_array[i].pages = -1;
        }

        token = strtok(NULL, ",\n");
        if(strcmp(token, "N/A")){
            book_array[i].year_published = atoi(token);
        }else{
            book_array[i].year_published = -1;
        }
        i++;
    }
    fclose(fpin);

    return 0;
}

book search_title(book book_array[], int numb_books, char title){
    int i;

    printf("BOOOOK");
}

