typedef struct{
        char title[255];
        char author_name[50];
        char ISBN[10];
        int pages;
        int year_published;
    }book;

int parse_file(char filename[], book book_array[]);
//void print_book(book);

book search_title(book book_array[], int numb_books, char title);

