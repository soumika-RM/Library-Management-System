#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Book {
    int id;
    char bookName[50];
    char authorName[50];
    char date[12];
} book;

struct Student {
    int id;
    char studentName[50];
    char studentClass[50];
    int studentRoll;
    char issuedBookName[50];
    char date[12];
} student;

FILE *filePointer;

void addBook();
void displayBooks();
void removeBook();
void issueBook();
void displayIssuedBooks();

int main() {
    int choice;

    while (1) {
        system("cls");
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Display Issued Books\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                exit(0);

            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                removeBook();
                break;

            case 4:
                issueBook();
                break;

            case 5:
                displayIssuedBooks();
                break;

            default:
                printf("Invalid Choice...\n\n");
        }
        printf("Press Any Key To Continue...");
        getch();
    }

    return 0;
}

void addBook() {
    char currentDate[12];
    time_t currentTime = time(NULL);
    struct tm timeStruct = *localtime(&currentTime);
    sprintf(currentDate, "%02d/%02d/%d", timeStruct.tm_mday, timeStruct.tm_mon + 1, timeStruct.tm_year + 1900);
    strcpy(book.date, currentDate);

    filePointer = fopen("books.txt", "a");  // Use "a" to append to the file

    printf("Enter book id: ");
    scanf("%d", &book.id);

    printf("Enter book name: ");
    fflush(stdin);
    gets(book.bookName);

    printf("Enter author name: ");
    fflush(stdin);
    gets(book.authorName);

    fprintf(filePointer, "%-10d %-30s %-20s %s\n", book.id, book.bookName, book.authorName, book.date);

    fclose(filePointer);

    printf("Book Added Successfully\n");
}

void displayBooks() {
    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book id", "Book Name", "Author", "Date");

    filePointer = fopen("books.txt", "rb");
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(filePointer, "%d %[^'\n'] %[^'\n'] %[^\n]", &book.id, book.bookName, book.authorName, book.date) == 4) {
        printf("%-10d %-30s %-20s %s\n", book.id, book.bookName, book.authorName, book.date);
    }

    fclose(filePointer);
}

void removeBook() {
    int bookId, found = 0;
    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &bookId);

    FILE *tempFile;

    filePointer = fopen("books.txt", "rb");
    tempFile = fopen("temp.txt", "wb");

    while (fread(&book, sizeof(book), 1, filePointer) == 1) {
        if (bookId == book.id) {
            found = 1;
        } else {
            fwrite(&book, sizeof(book), 1, tempFile);
        }
    }

    if (found == 1) {
        printf("\n\nDeleted Successfully.\n");
    } else {
        printf("\n\nRecord Not Found!\n");
    }

    fclose(filePointer);
    fclose(tempFile);

    remove("books.txt");
    rename("temp.txt", "books.txt");
}

void issueBook() {
    char currentDate[12];
    time_t currentTime = time(NULL);
    struct tm timeStruct = *localtime(&currentTime);
    sprintf(currentDate, "%02d/%02d/%d", timeStruct.tm_mday, timeStruct.tm_mon + 1, timeStruct.tm_year + 1900);
    strcpy(student.date, currentDate);

    int found = 0;

    system("cls");
    printf("<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d", &student.id);

    filePointer = fopen("books.txt", "rb");
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&book, sizeof(book), 1, filePointer) == 1) {
        if (book.id == student.id) {
            strcpy(student.issuedBookName, book.bookName);
            found = 1;
            break;
        }
    }

    fclose(filePointer);

    if (found == 0) {
        printf("No book found with this id\n");
        printf("Please try again...\n\n");
        return;
    }

    filePointer = fopen("issuedBooks.txt", "ab");

    printf("Enter Student Name: ");
    fflush(stdin);
    gets(student.studentName);

    printf("Enter Student Class: ");
    fflush(stdin);
    gets(student.studentClass);

    printf("Enter Student Roll: ");
    scanf("%d", &student.studentRoll);

    printf("Book Issued Successfully\n\n");

    fwrite(&student, sizeof(student), 1, filePointer);
    fclose(filePointer);
}

void displayIssuedBooks() {
    system("cls");
    printf("<== Book Issue List ==>\n\n");

    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "S.id", "Name", "Class", "Roll", "Book Name", "Date");

    filePointer = fopen("issuedBooks.txt", "rb");
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&student, sizeof(student), 1, filePointer) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", student.id, student.studentName, student.studentClass, student.studentRoll, student.issuedBookName, student.date);
    }

    fclose(filePointer);
}

