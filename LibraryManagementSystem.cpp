#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Class representing a Library Member
class Member {
private:
    int memberId;
    string name;

public:
    Member(int id, string n) : memberId(id), name(n) {}

    int getId() const { return memberId; }
    string getName() const { return name; }
};

// Class representing a Book
class Book {
private:
    int bookId;
    string title;
    string author;
    bool isIssued;
    int issuedToMemberId;

public:
    Book(int id, string t, string a) 
        : bookId(id), title(t), author(a), isIssued(false), issuedToMemberId(-1) {}

    // Getters
    int getId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getIssueStatus() const { return isIssued; }
    int getIssuedMemberId() const { return issuedToMemberId; }

    // Operations on the book
    void issueBook(int memberId) {
        isIssued = true;
        issuedToMemberId = memberId;
    }

    void returnBook() {
        isIssued = false;
        issuedToMemberId = -1;
    }
};

// Main Library Class to manage collections and operations
class Library {
private:
    vector<Book> books;     // Dynamic array of books
    vector<Member> members; // Dynamic array of members

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Helper function to find a book by ID
    Book* findBookById(int id) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].getId() == id) {
                return &books[i]; // Return memory address of the book
            }
        }
        return nullptr; // Equivalent to NULL in C
    }

    // Helper function to find a member by ID
    bool isValidMember(int id) {
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].getId() == id) {
                return true;
            }
        }
        return false;
    }

public:
    // 1. Add a new book
    void addBook() {
        int id;
        string title, author;

        cout << "\n--- Add New Book ---\n";
        cout << "Enter Book ID: ";
        while (!(cin >> id)) {
            cout << "Invalid input. Enter a numeric ID: ";
            clearInputBuffer();
        }
        
        if (findBookById(id) != nullptr) {
            cout << "Error: Book with ID " << id << " already exists.\n";
            return;
        }

        clearInputBuffer();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);

        // push_back automatically resizes the vector and adds the new object to the end
        books.push_back(Book(id, title, author));
        cout << "Book added successfully!\n";
    }

    // 2. Register a new member
    void addMember() {
        int id;
        string name;

        cout << "\n--- Register New Member ---\n";
        cout << "Enter Member ID: ";
        while (!(cin >> id)) {
            cout << "Invalid input. Enter a numeric ID: ";
            clearInputBuffer();
        }

        if (isValidMember(id)) {
            cout << "Error: Member with ID " << id << " already exists.\n";
            return;
        }

        clearInputBuffer();
        cout << "Enter Member Name: ";
        getline(cin, name);

        members.push_back(Member(id, name));
        cout << "Member registered successfully!\n";
    }

    // 3. Issue a book to a member
    void issueBook() {
        int bookId, memberId;

        cout << "\n--- Issue Book ---\n";
        cout << "Enter Book ID: ";
        cin >> bookId;

        Book* targetBook = findBookById(bookId);

        if (targetBook == nullptr) {
            cout << "Error: Book not found.\n";
            return;
        }

        if (targetBook->getIssueStatus()) {
            cout << "Sorry, this book is already issued to Member ID: " << targetBook->getIssuedMemberId() << ".\n";
            return;
        }

        cout << "Enter Member ID: ";
        cin >> memberId;

        if (!isValidMember(memberId)) {
            cout << "Error: Member ID not found. Please register the member first.\n";
            return;
        }

        targetBook->issueBook(memberId);
        cout << "Book '" << targetBook->getTitle() << "' successfully issued to Member ID " << memberId << ".\n";
    }

    // 4. Return an issued book
    void returnBook() {
        int bookId;

        cout << "\n--- Return Book ---\n";
        cout << "Enter Book ID: ";
        cin >> bookId;

        Book* targetBook = findBookById(bookId);

        if (targetBook == nullptr) {
            cout << "Error: Book not found.\n";
            return;
        }

        if (!targetBook->getIssueStatus()) {
            cout << "This book is not currently issued to anyone.\n";
            return;
        }

        targetBook->returnBook();
        cout << "Book '" << targetBook->getTitle() << "' has been successfully returned.\n";
    }

    // 5. Search for a book by Title or Author
    void searchBook() {
        clearInputBuffer();
        string query;
        cout << "\n--- Search Book ---\n";
        cout << "Enter exact Title or Author to search: ";
        getline(cin, query);

        bool found = false;
        cout << "\nSearch Results:\n";
        cout << "--------------------------------------------------------------\n";
        cout << left << setw(10) << "Book ID" << setw(25) << "Title" << setw(20) << "Author" << "Status\n";
        cout << "--------------------------------------------------------------\n";

        for (size_t i = 0; i < books.size(); i++) {
            // Check if query matches either title or author
            if (books[i].getTitle() == query || books[i].getAuthor() == query) {
                found = true;
                cout << left << setw(10) << books[i].getId() 
                     << setw(25) << books[i].getTitle() 
                     << setw(20) << books[i].getAuthor() 
                     << (books[i].getIssueStatus() ? "Issued" : "Available") << "\n";
            }
        }
        
        if (!found) {
            cout << "No books found matching '" << query << "'.\n";
        }
        cout << "--------------------------------------------------------------\n";
    }

    // 6. Display all books in the library
    void displayAllBooks() {
        if (books.empty()) {
            cout << "\nThe library is currently empty.\n";
            return;
        }

        cout << "\n--- Library Database ---\n";
        cout << "--------------------------------------------------------------\n";
        cout << left << setw(10) << "Book ID" << setw(25) << "Title" << setw(20) << "Author" << "Status\n";
        cout << "--------------------------------------------------------------\n";

        for (size_t i = 0; i < books.size(); i++) {
            cout << left << setw(10) << books[i].getId() 
                 << setw(25) << books[i].getTitle() 
                 << setw(20) << books[i].getAuthor() 
                 << (books[i].getIssueStatus() ? "Issued" : "Available") << "\n";
        }
        cout << "--------------------------------------------------------------\n";
    }
};

// Entry Point
int main() {
    Library lib;
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "        LIBRARY MANAGEMENT SYSTEM        \n";
        cout << "=========================================\n";
        cout << "1. Add New Book\n";
        cout << "2. Register New Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book (by Title/Author)\n";
        cout << "6. Display All Books\n";
        cout << "7. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice (1-7): ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.addMember(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.searchBook(); break;
            case 6: lib.displayAllBooks(); break;
            case 7: cout << "Exiting system. Goodbye!\n"; break;
            default: cout << "Invalid choice. Select between 1 and 7.\n";
        }
    } while (choice != 7);

    return 0;
}