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

book print_book(int book_index, book book_array[]){

    printf("Title: %s| Author: %s| ISBN: %s| Pages: %d| Year Published: %d|\n", book_array[book_index].title, book_array[book_index].author_name, book_array[book_index].ISBN, book_array[book_index].pages, book_array[book_index].year_published);

}

void search_title(book book_array[], int numb_books, char *title){
    int i, j = 0;
    char title_array[MAX];

    for(i = 0; i < numb_books; i++){
        if(strcasecmp(title, book_array[i].title) == 0){
            print_book(i, book_array);
            j++;
        }
    }

    if(j == 0){
        printf("No information could be retrieved for this title\n");
        j = 0;
    }
}

void search_author(book book_array[], int numb_books, char *author){
    int i, j = 0;

    for(i = 0; i < numb_books; i++){
        if(strcasecmp(author, book_array[i].author_name) == 0){
            print_book(i, book_array);
            j++;
        }
    }

    if(j == 0){
        printf("No information could be retrieved for this author\n");
        j = 0;
    }
}

void search_ISBN(book book_array[], int numb_books, char *ISBN){
    int i, j = 0;

    for(i = 0; i < numb_books; i++){
        if(strcasecmp(ISBN, book_array[i].ISBN) == 0){
            print_book(i, book_array);
            j++;
        }
    }

    if(j == 0){
        printf("No information could be retrieved for this ISBN\n");
        j = 0;
    }
}


