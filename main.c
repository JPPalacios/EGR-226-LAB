#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#define MAX 1000

int main()
{
    book book_array[MAX];
    char filename[] = "Booklist.txt";
    char title[MAX];
    char author[MAX];

    int user_input = 3, error_check = 0;
    int numb_books = 0;

    numb_books = parse_file(filename, book_array);

    do{
        do{                                         // prompts the user to select their choice of input
            printf("Search by...\n[0] - Title\n[1] - Author\n[2] - ISBN\n");
            error_check = scanf("%d", &user_input);
            while(getchar() != '\n');               // error checks for user input
        }while(user_input > 2 || error_check != 1); // cannot select greater than

        switch(user_input){
        case 0:
            printf("Title\n");
            printf("Enter book title: \n");
            fflush(stdin);
            gets(title);
            search_title(book_array, numb_books, &title);
            break;
        case 1:
            printf("Author Name\n");
            printf("Enter Author Name: \n");
            fflush(stdin);
            gets(author);

//            search_author(book_array, numb_books, &author);
            break;
        case 2:
            printf("ISBN\n");
            break;
        default:
            break;
        }

        printf("Press 0 to exit\n");
        error_check = scanf("%d", &user_input);
        while(getchar() != '\n');

    }while(user_input != 0);

    return 0;
}

