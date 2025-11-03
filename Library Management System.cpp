#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <windows.h> // For color & sleep
#include <conio.h>   // For getch()
using namespace std;

// Utility: set console color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Utility: slow printing animation
void printSlow(string text, int speed = 20) {
    for (int i = 0; i < text.length(); i++) {
        cout << text[i];
        Sleep(speed);
    }
}

// Utility: show a beautiful app header
void showHeader() {
    system("cls");
    setColor(11);
    cout << "\n=============================================================\n";
    cout << "    LIBRARY MANAGEMENT SYSTEM\n";
    cout << "=============================================================\n";
    setColor(15);
}

class Library {
private:
    int bookID;
    string title, author;
    bool isIssued;

    // Helper function to check if book ID already exists
    bool bookIdExists(int id) {
        ifstream file("library.txt");
        if (!file) return false;
        
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                stringstream ss(line.substr(0, pos));
                int existingId;
                if (ss >> existingId && existingId == id) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

    // Helper function to parse book data from line
    bool parseBookData(const string& line, int& id, string& t, string& a, bool& issued) {
        size_t pos1 = line.find(',');
        if (pos1 == string::npos) return false;
        
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos2 == string::npos) return false;
        
        size_t pos3 = line.find(',', pos2 + 1);
        if (pos3 == string::npos) return false;

        stringstream ss(line.substr(0, pos1));
        if (!(ss >> id)) return false;
        
        t = line.substr(pos1 + 1, pos2 - pos1 - 1);
        a = line.substr(pos2 + 1, pos3 - pos2 - 1);
        
        string issuedStr = line.substr(pos3 + 1);
        issued = (issuedStr == "1");
        
        return true;
    }

public:
    // Add new book
    void addBook() {
        showHeader();
        cout << "\n[ ADD NEW BOOK ]\n";
        
        cout << "Enter Book ID: ";
        if (!(cin >> bookID)) {
            setColor(12);
            cout << "\nInvalid Book ID! Please enter numbers only.\n";
            setColor(15);
            cin.clear();
            cin.ignore(10000, '\n');
            getch();
            return;
        }
        
        // Check if book ID already exists
        if (bookIdExists(bookID)) {
            setColor(12);
            cout << "\nBook ID already exists! Please use a different ID.\n";
            setColor(15);
            cin.ignore(10000, '\n');
            getch();
            return;
        }
        
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        
        cout << "Enter Author Name: ";
        getline(cin, author);
        
        isIssued = false;
        
        ofstream file("library.txt", ios::app);
        if (!file) {
            setColor(12);
            cout << "\nError: Could not open file for writing!\n";
            setColor(15);
            getch();
            return;
        }
        
        file << bookID << "," << title << "," << author << "," << isIssued << endl;
        file.close();
        
        setColor(10);
        cout << "\nBook Added Successfully!\n";
        setColor(15);
        getch();
    }

    // Display all books
    void displayBooks() {
        showHeader();
        cout << "\n[ ALL BOOKS LIST ]\n";
        
        ifstream file("library.txt");
        if (!file) {
            setColor(12);
            cout << "No books found in library!\n";
            setColor(15);
            getch();
            return;
        }
        
        string line;
        bool hasBooks = false;
        
        cout << left << setw(10) << "ID" << setw(30) << "Title" 
             << setw(25) << "Author" << setw(10) << "Issued" << endl;
        cout << "-------------------------------------------------------------\n";
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            int id;
            string t, a;
            bool issued;
            
            if (parseBookData(line, id, t, a, issued)) {
                hasBooks = true;
                setColor(14);
                cout << left << setw(10) << id << setw(30) << t 
                     << setw(25) << a << setw(10) << (issued ? "Yes" : "No") << endl;
                setColor(15);
            }
        }
        file.close();
        
        if (!hasBooks) {
            setColor(12);
            cout << "No books available in the library!\n";
            setColor(15);
        }
        
        getch();
    }

    // Search book by ID
    void searchBook() {
        showHeader();
        cout << "\n[ SEARCH BOOK ]\n";
        
        int id;
        cout << "Enter Book ID: ";
        if (!(cin >> id)) {
            setColor(12);
            cout << "\nInvalid Book ID! Please enter numbers only.\n";
            setColor(15);
            cin.clear();
            cin.ignore(10000, '\n');
            getch();
            return;
        }
        
        ifstream file("library.txt");
        if (!file) {
            setColor(12);
            cout << "\nNo books found in library!\n";
            setColor(15);
            getch();
            return;
        }
        
        string line;
        bool found = false;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            int bookId;
            string t, a;
            bool issued;
            
            if (parseBookData(line, bookId, t, a, issued) && bookId == id) {
                found = true;
                setColor(10);
                cout << "\nBook Found!\n";
                cout << "ID: " << bookId << endl;
                cout << "Title: " << t << endl;
                cout << "Author: " << a << endl;
                cout << "Issued: " << (issued ? "Yes" : "No") << endl;
                setColor(15);
                break;
            }
        }
        file.close();
        
        if (!found) {
            setColor(12);
            cout << "\nBook Not Found!\n";
            setColor(15);
        }
        getch();
    }

    // Issue book
    void issueBook() {
        showHeader();
        cout << "\n[ ISSUE BOOK ]\n";
        
        int id;
        cout << "Enter Book ID to Issue: ";
        if (!(cin >> id)) {
            setColor(12);
            cout << "\nInvalid Book ID! Please enter numbers only.\n";
            setColor(15);
            cin.clear();
            cin.ignore(10000, '\n');
            getch();
            return;
        }
        
        ifstream file("library.txt");
        if (!file) {
            setColor(12);
            cout << "\nNo books found in library!\n";
            setColor(15);
            getch();
            return;
        }
        
        ofstream temp("temp.txt");
        if (!temp) {
            setColor(12);
            cout << "\nError creating temporary file!\n";
            setColor(15);
            file.close();
            getch();
            return;
        }
        
        string line;
        bool found = false;
        
        while (getline(file, line)) {
            if (line.empty()) {
                temp << endl;
                continue;
            }
            
            int bookId;
            string t, a;
            bool issued;
            
            if (parseBookData(line, bookId, t, a, issued)) {
                if (bookId == id) {
                    found = true;
                    if (issued) {
                        setColor(14);
                        cout << "\nBook is already issued!\n";
                        setColor(15);
                        temp << line << endl; // Keep original status
                    } else {
                        temp << bookId << "," << t << "," << a << ",1" << endl;
                        setColor(10);
                        cout << "\nBook Issued Successfully!\n";
                        setColor(15);
                    }
                } else {
                    temp << line << endl;
                }
            } else {
                temp << line << endl; // Keep corrupted lines as-is
            }
        }
        
        file.close();
        temp.close();
        
        if (found) {
            remove("library.txt");
            rename("temp.txt", "library.txt");
        } else {
            remove("temp.txt"); // Clean up temp file
            setColor(12);
            cout << "\nBook Not Found!\n";
            setColor(15);
        }
        getch();
    }

    // Return book
    void returnBook() {
        showHeader();
        cout << "\n[ RETURN BOOK ]\n";
        
        int id;
        cout << "Enter Book ID to Return: ";
        if (!(cin >> id)) {
            setColor(12);
            cout << "\nInvalid Book ID! Please enter numbers only.\n";
            setColor(15);
            cin.clear();
            cin.ignore(10000, '\n');
            getch();
            return;
        }
        
        ifstream file("library.txt");
        if (!file) {
            setColor(12);
            cout << "\nNo books found in library!\n";
            setColor(15);
            getch();
            return;
        }
        
        ofstream temp("temp.txt");
        if (!temp) {
            setColor(12);
            cout << "\nError creating temporary file!\n";
            setColor(15);
            file.close();
            getch();
            return;
        }
        
        string line;
        bool found = false;
        
        while (getline(file, line)) {
            if (line.empty()) {
                temp << endl;
                continue;
            }
            
            int bookId;
            string t, a;
            bool issued;
            
            if (parseBookData(line, bookId, t, a, issued)) {
                if (bookId == id) {
                    found = true;
                    if (!issued) {
                        setColor(14);
                        cout << "\nBook was not issued!\n";
                        setColor(15);
                        temp << line << endl; // Keep original status
                    } else {
                        temp << bookId << "," << t << "," << a << ",0" << endl;
                        setColor(10);
                        cout << "\nBook Returned Successfully!\n";
                        setColor(15);
                    }
                } else {
                    temp << line << endl;
                }
            } else {
                temp << line << endl; // Keep corrupted lines as-is
            }
        }
        
        file.close();
        temp.close();
        
        if (found) {
            remove("library.txt");
            rename("temp.txt", "library.txt");
        } else {
            remove("temp.txt"); // Clean up temp file
            setColor(12);
            cout << "\nBook Not Found!\n";
            setColor(15);
        }
        getch();
    }

    // Delete a book
    void deleteBook() {
        showHeader();
        cout << "\n[ DELETE BOOK ]\n";
        
        int id;
        cout << "Enter Book ID: ";
        if (!(cin >> id)) {
            setColor(12);
            cout << "\nInvalid Book ID! Please enter numbers only.\n";
            setColor(15);
            cin.clear();
            cin.ignore(10000, '\n');
            getch();
            return;
        }
        
        ifstream file("library.txt");
        if (!file) {
            setColor(12);
            cout << "\nNo books found in library!\n";
            setColor(15);
            getch();
            return;
        }
        
        ofstream temp("temp.txt");
        if (!temp) {
            setColor(12);
            cout << "\nError creating temporary file!\n";
            setColor(15);
            file.close();
            getch();
            return;
        }
        
        string line;
        bool found = false;
        
        while (getline(file, line)) {
            if (line.empty()) {
                temp << endl;
                continue;
            }
            
            int bookId;
            string t, a;
            bool issued;
            
            if (parseBookData(line, bookId, t, a, issued)) {
                if (bookId != id) {
                    temp << line << endl;
                } else {
                    found = true;
                }
            } else {
                temp << line << endl; // Keep corrupted lines as-is
            }
        }
        
        file.close();
        temp.close();
        
        if (found) {
            remove("library.txt");
            rename("temp.txt", "library.txt");
            setColor(10);
            cout << "\nBook Deleted Successfully!\n";
        } else {
            remove("temp.txt"); // Clean up temp file
            setColor(12);
            cout << "\nBook Not Found!\n";
        }
        setColor(15);
        getch();
    }
};

int main() {
    Library lib;
    int choice;
    
    // Fancy loading animation
    setColor(11);
    printSlow("Loading Library Management System", 50);
    for (int i = 0; i < 3; i++) {
        cout << ".";
        Sleep(500);
    }
    setColor(15);
    
    do {
        showHeader();
        cout << "\n1. Add New Book";
        cout << "\n2. Display All Books";
        cout << "\n3. Search Book";
        cout << "\n4. Issue Book";
        cout << "\n5. Return Book";
        cout << "\n6. Delete Book";
        cout << "\n7. Exit";
        cout << "\n\nEnter your choice: ";
        
        if (!(cin >> choice)) {
            setColor(12);
            cout << "\nInvalid input! Please enter a number (1-7).\n";
            setColor(15);
            cin.clear();
            cin.ignore(10000, '\n');
            Sleep(1000);
            continue;
        }
        
        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.displayBooks(); break;
            case 3: lib.searchBook(); break;
            case 4: lib.issueBook(); break;
            case 5: lib.returnBook(); break;
            case 6: lib.deleteBook(); break;
            case 7: 
                setColor(10);
                cout << "\nThanks for using Library Management System!\n";
                setColor(15);
                break;
            default:
                setColor(12);
                cout << "\nInvalid Choice! Please choose between 1-7.\n";
                setColor(15);
                Sleep(1000);
        }
    } while (choice != 7);
    
    return 0;
}
