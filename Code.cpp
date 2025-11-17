#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <iomanip>
using namespace std;

struct Book {
    char isbn[20];
    char title[100];
    char author[50];
    bool isAvailable;
    Book* next;
};

struct User {
    char userId[20];
    char name[50];
    char contact[15];
    User* next;
};

struct Transaction {
    char userId[20];
    char isbn[20];
    char borrowDate[15];
    char returnDate[15];
    bool isReturned;
    Transaction* next;
};

Book* bookHead = nullptr;
User* userHead = nullptr;
Transaction* transactionHead = nullptr;

void getCurrentDate(char* dateStr) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    sprintf(dateStr, "%02d-%02d-%04d", ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year);
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
}

bool containsSubstring(const char* str, const char* substr) {
    char tempStr[100], tempSubstr[100];
    strcpy(tempStr, str);
    strcpy(tempSubstr, substr);
    toLowerCase(tempStr);
    toLowerCase(tempSubstr);
    return strstr(tempStr, tempSubstr) != nullptr;
}

void addBook() {
    Book* newBook = new Book;
    cout << "\nEnter ISBN: ";
    cin.ignore();
    cin.getline(newBook->isbn, 20);
    for (Book* temp = bookHead; temp != nullptr; temp = temp->next) {
        if (strcmp(temp->isbn, newBook->isbn) == 0) {
            cout << "Error: ISBN already exists.\n";
            delete newBook;
            return;
        }
    }
    cout << "Enter Title: ";
    cin.getline(newBook->title, 100);
    cout << "Enter Author: ";
    cin.getline(newBook->author, 50);
    newBook->isAvailable = true;
    newBook->next = bookHead;
    bookHead = newBook;
    cout << "Book added successfully!\n";
}

void swapBookData(Book* a, Book* b) {
    char tempIsbn[20], tempTitle[100], tempAuthor[50];
    bool tempAvailable;
    strcpy(tempIsbn, a->isbn);
    strcpy(tempTitle, a->title);
    strcpy(tempAuthor, a->author);
    tempAvailable = a->isAvailable;
    strcpy(a->isbn, b->isbn);
    strcpy(a->title, b->title);
    strcpy(a->author, b->author);
    a->isAvailable = b->isAvailable;
    strcpy(b->isbn, tempIsbn);
    strcpy(b->title, tempTitle);
    strcpy(b->author, tempAuthor);
    b->isAvailable = tempAvailable;
}

void sortBooksByTitle() {
    if (!bookHead || !bookHead->next) return;
    bool swapped;
    Book* ptr1;
    Book* lptr = nullptr;
    do {
        swapped = false;
        ptr1 = bookHead;
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->title, ptr1->next->title) > 0) {
                swapBookData(ptr1, ptr1->next);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    cout << "Books sorted by Title successfully.\n";
}

void sortBooksByAuthor() {
    if (!bookHead || !bookHead->next) return;
    for (Book* i = bookHead; i != nullptr; i = i->next) {
        Book* minNode = i;
        for (Book* j = i->next; j != nullptr; j = j->next) {
            if (strcmp(j->author, minNode->author) < 0) {
                minNode = j;
            }
        }
        if (minNode != i) {
            swapBookData(i, minNode);
        }
    }
    cout << "Books sorted by Author successfully.\n";
}

void displayAllBooks() {
    if (!bookHead) {
        cout << "\nNo books available.\n";
        return;
    }
    cout << "\nISBN           Title                              Author                  Status\n";
    cout << "-------------------------------------------------------------------------------\n";
    for (Book* temp = bookHead; temp != nullptr; temp = temp->next) {
        cout << left << setw(15) << temp->isbn
             << setw(35) << temp->title
             << setw(25) << temp->author
             << (temp->isAvailable ? "Available" : "Borrowed") << endl;
    }
}

void searchBookByTitle() {
    char searchTitle[100];
    cout << "\nEnter title to search: ";
    cin.ignore();
    cin.getline(searchTitle, 100);
    bool found = false;
    for (Book* temp = bookHead; temp != nullptr; temp = temp->next) {
        if (containsSubstring(temp->title, searchTitle)) {
            cout << "\nISBN: " << temp->isbn << "\nTitle: " << temp->title
                 << "\nAuthor: " << temp->author
                 << "\nStatus: " << (temp->isAvailable ? "Available" : "Borrowed") << "\n";
            found = true;
        }
    }
    if (!found) cout << "No books found.\n";
}

void deleteBook() {
    char isbn[20];
    cout << "\nEnter ISBN to delete: ";
    cin.ignore();
    cin.getline(isbn, 20);
    Book *temp = bookHead, *prev = nullptr;
    while (temp && strcmp(temp->isbn, isbn) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) {
        cout << "Book not found.\n";
        return;
    }
    if (!temp->isAvailable) {
        cout << "Cannot delete. Book is currently borrowed.\n";
        return;
    }
    if (!prev)
        bookHead = temp->next;
    else
        prev->next = temp->next;
    delete temp;
    cout << "Book deleted successfully.\n";
}

void addUser() {
    User* newUser = new User;
    cout << "\nEnter User ID: ";
    cin.ignore();
    cin.getline(newUser->userId, 20);
    for (User* temp = userHead; temp != nullptr; temp = temp->next) {
        if (strcmp(temp->userId, newUser->userId) == 0) {
            cout << "User ID already exists.\n";
            delete newUser;
            return;
        }
    }
    cout << "Enter Name: ";
    cin.getline(newUser->name, 50);
    cout << "Enter Contact: ";
    cin.getline(newUser->contact, 15);
    newUser->next = userHead;
    userHead = newUser;
    cout << "User added successfully!\n";
}

void displayAllUsers() {
    if (!userHead) {
        cout << "\nNo users registered.\n";
        return;
    }
    cout << "\nUser ID       Name                           Contact\n";
    cout << "------------------------------------------------------\n";
    for (User* temp = userHead; temp != nullptr; temp = temp->next) {
        cout << left << setw(15) << temp->userId
             << setw(30) << temp->name
             << temp->contact << endl;
    }
}

User* searchUserByID(const char* userId) {
    for (User* temp = userHead; temp != nullptr; temp = temp->next)
        if (strcmp(temp->userId, userId) == 0)
            return temp;
    return nullptr;
}

void deleteUser() {
    char userId[20];
    cout << "\nEnter User ID to delete: ";
    cin.ignore();
    cin.getline(userId, 20);
    for (Transaction* t = transactionHead; t != nullptr; t = t->next) {
        if (strcmp(t->userId, userId) == 0 && !t->isReturned) {
            cout << "Cannot delete. User has borrowed books.\n";
            return;
        }
    }
    User *temp = userHead, *prev = nullptr;
    while (temp && strcmp(temp->userId, userId) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) {
        cout << "User not found.\n";
        return;
    }
    if (!prev)
        userHead = temp->next;
    else
        prev->next = temp->next;
    delete temp;
    cout << "User deleted successfully.\n";
}

void borrowBook() {
    char userId[20], isbn[20];
    cout << "\nEnter User ID: ";
    cin.ignore();
    cin.getline(userId, 20);
    User* user = searchUserByID(userId);
    if (!user) {
        cout << "User not found.\n";
        return;
    }
    cout << "Enter Book ISBN: ";
    cin.getline(isbn, 20);
    Book* book = bookHead;
    while (book && strcmp(book->isbn, isbn) != 0) book = book->next;
    if (!book) {
        cout << "Book not found.\n";
        return;
    }
    if (!book->isAvailable) {
        cout << "Book already borrowed.\n";
        return;
    }
    Transaction* newTrans = new Transaction;
    strcpy(newTrans->userId, userId);
    strcpy(newTrans->isbn, isbn);
    getCurrentDate(newTrans->borrowDate);
    strcpy(newTrans->returnDate, "Not Returned");
    newTrans->isReturned = false;
    newTrans->next = transactionHead;
    transactionHead = newTrans;
    book->isAvailable = false;
    cout << "Book borrowed successfully.\n";
}

void returnBook() {
    char userId[20], isbn[20];
    cout << "\nEnter User ID: ";
    cin.ignore();
    cin.getline(userId, 20);
    cout << "Enter Book ISBN: ";
    cin.getline(isbn, 20);
    Transaction* temp = transactionHead;
    while (temp) {
        if (strcmp(temp->userId, userId) == 0 && strcmp(temp->isbn, isbn) == 0 && !temp->isReturned)
            break;
        temp = temp->next;
    }
    if (!temp) {
        cout << "No active borrow record found.\n";
        return;
    }
    getCurrentDate(temp->returnDate);
    temp->isReturned = true;
    Book* book = bookHead;
    while (book && strcmp(book->isbn, isbn) != 0) book = book->next;
    if (book) book->isAvailable = true;
    cout << "Book returned successfully.\n";
}

void displayAllTransactions() {
    if (!transactionHead) {
        cout << "\nNo transactions recorded.\n";
        return;
    }
    cout << "\nUser ID       ISBN           Borrow Date   Return Date   Status\n";
    cout << "-------------------------------------------------------------------\n";
    for (Transaction* t = transactionHead; t != nullptr; t = t->next) {
        cout << left << setw(15) << t->userId
             << setw(15) << t->isbn
             << setw(13) << t->borrowDate
             << setw(13) << t->returnDate
             << (t->isReturned ? "Returned" : "Active") << endl;
    }
}

template<typename T>
void saveToFile(const char* filename, T* head, void (*writeFunc)(ofstream&, T*)) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening file " << filename << "!\n";
        return;
    }
    for (T* temp = head; temp != nullptr; temp = temp->next) {
        writeFunc(file, temp);
    }
    file.close();
}

void writeBook(ofstream& file, Book* b) {
    file << b->isbn << "|" << b->title << "|" << b->author << "|" << b->isAvailable << "\n";
}

void writeUser(ofstream& file, User* u) {
    file << u->userId << "|" << u->name << "|" << u->contact << "\n";
}

void writeTransaction(ofstream& file, Transaction* t) {
    file << t->userId << "|" << t->isbn << "|" << t->borrowDate << "|" << t->returnDate << "|" << t->isReturned << "\n";
}

void saveAllData() {
    saveToFile("books.txt", bookHead, writeBook);
    saveToFile("users.txt", userHead, writeUser);
    saveToFile("transactions.txt", transactionHead, writeTransaction);
    cout << "All data saved.\n";
}

template<typename T>
void loadFromFile(const char* filename, T** head, void (*readFunc)(char*, T**)) {
    ifstream file(filename);
    if (!file) return;
    char line[256];
    while (file.getline(line, 256)) {
        readFunc(line, head);
    }
    file.close();
}

void readBook(char* line, Book** head) {
    Book* newBook = new Book;
    char* token = strtok(line, "|");
    strcpy(newBook->isbn, token);
    token = strtok(NULL, "|");
    strcpy(newBook->title, token);
    token = strtok(NULL, "|");
    strcpy(newBook->author, token);
    token = strtok(NULL, "|");
    newBook->isAvailable = atoi(token);
    newBook->next = *head;
    *head = newBook;
}

void readUser(char* line, User** head) {
    User* newUser = new User;
    char* token = strtok(line, "|");
    strcpy(newUser->userId, token);
    token = strtok(NULL, "|");
    strcpy(newUser->name, token);
    token = strtok(NULL, "|");
    strcpy(newUser->contact, token);
    newUser->next = *head;
    *head = newUser;
}

void readTransaction(char* line, Transaction** head) {
    Transaction* newTrans = new Transaction;
    char* token = strtok(line, "|");
    strcpy(newTrans->userId, token);
    token = strtok(NULL, "|");
    strcpy(newTrans->isbn, token);
    token = strtok(NULL, "|");
    strcpy(newTrans->borrowDate, token);
    token = strtok(NULL, "|");
    strcpy(newTrans->returnDate, token);
    token = strtok(NULL, "|");
    newTrans->isReturned = atoi(token);
    newTrans->next = *head;
    *head = newTrans;
}

void loadAllData() {
    loadFromFile("books.txt", &bookHead, readBook);
    loadFromFile("users.txt", &userHead, readUser);
    loadFromFile("transactions.txt", &transactionHead, readTransaction);
}

void bookMenu() {
    int choice;
    do {
        cout << "\n--- Book Menu ---\n"
             << "1. Add Book\n"
             << "2. Display All Books\n"
             << "3. Search Book by Title\n"
             << "4. Delete Book\n"
             << "5. Sort Books by Title (Bubble Sort)\n"
             << "6. Sort Books by Author (Selection Sort)\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3: searchBookByTitle(); break;
            case 4: deleteBook(); break;
            case 5: sortBooksByTitle(); break;
            case 6: sortBooksByAuthor(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void userMenu() {
    int choice;
    do {
        cout << "\n--- User Menu ---\n1. Add User\n2. Display All Users\n3. Delete User\n0. Back\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: addUser(); break;
            case 2: displayAllUsers(); break;
            case 3: deleteUser(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void transactionMenu() {
    int choice;
    do {
        cout << "\n--- Transactions ---\n1. Borrow Book\n2. Return Book\n3. Display All Transactions\n0. Back\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: borrowBook(); break;
            case 2: returnBook(); break;
            case 3: displayAllTransactions(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    loadAllData();
    int choice;
    cout << "=== Library Management System ===\n";
    do {
        cout << "\n1. Book Management\n2. User Management\n3. Transactions\n4. Save Data\n0. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: bookMenu(); break;
            case 2: userMenu(); break;
            case 3: transactionMenu(); break;
            case 4: saveAllData(); break;
            case 0:
                saveAllData();
                cout << "Exiting. Thanks for using the system!\n";
                break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    while (bookHead) {
        Book* temp = bookHead;
        bookHead = bookHead->next;
        delete temp;
    }
    while (userHead) {
        User* temp = userHead;
        userHead = userHead->next;
        delete temp;
    }
    while (transactionHead) {
        Transaction* temp = transactionHead;
        transactionHead = transactionHead->next;
        delete temp;
    }
    return 0;
}
