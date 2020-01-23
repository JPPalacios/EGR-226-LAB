#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

int main()
{
    book book_array[1000];
    char filename[] = "Booklist.txt";
    char title[500];

    int user_input, error_check = 0;
    int numb_books = 0;


    parse_file(filename, book_array);
    //book print_book();

    do{
        do{
            printf("Search by...\n");
            error_check = scanf("%d", &user_input);
            while(getchar() != '\n');
        }while(user_input > 2);

        switch(user_input){
        case 0:
            printf("Title\n");


            do{
                printf("Enter book title: \n");
                error_check = gets(title);
                //while(getchar() != '\n');
            }while(error_check != 0);
            search_title(book_array, numb_books, title);
            break;
        case 1:
            printf("Author Name\n");
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

