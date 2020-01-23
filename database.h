typedef struct{
        char title[255];
        char author_name[50];
        char ISBN[10];
        int pages;
        int year_published;
    }book;

int parse_file(char filename[], book book_array[]);

book print_book(int book_number, book book_array[]);

void search_title(book book_array[], int numb_books, char *title);
void search_author(book book_array[], int numb_books, char *author);
void search_ISBN(book book_array[], int numb_books, char *ISBN);

