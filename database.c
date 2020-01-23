#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#define MAX 1000

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

    return i;
}

book print_book(book book_array[]){
    int i;
    for(i = 0; i < MAX; i++){
    printf("%s", book_array[i].title);
    }

}

void search_title(book book_array[], int numb_books, char *title){
    int i, j = 0;
    char title_array[MAX];

    for(i = 0; i < numb_books; i++){
        strcpy(title_array, book_array[i].title);

        if(strcmp(title, title_array) == 0){
            print_book(book_array[i].title);
            j++;
        }
    }

    if(j == 0){
        printf("No information could be retrieved for this title\n");
        j = 0;
    }
}

/*
void search_author(book book_array[], int numb_books, char *author){
    int i, j = 0;
    char author_array[MAX];

    for(i = 0; i < numb_books; i++){
        strcpy(author_array, book_array[i].author);

        if(strcmp(author, author_array) == 0){
            print_book(book_array[i].author);
            j++;
        }
    }

    if(j == 0){
        printf("No information could be retrieved for this title\n");
        j = 0;
    }
}
*/
