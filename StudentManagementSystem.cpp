#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdio> // For remove() and rename()

using namespace std;

// Class representing a single Student entity
class Student {
private:
    int rollNo;
    string name;
    string course;
    string grade;

public:
    // Default constructor
    Student() : rollNo(0), name(""), course(""), grade("") {}

    // Parameterized constructor
    Student(int r, string n, string c, string g) : rollNo(r), name(n), course(c), grade(g) {}

    // Getters
    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    string getCourse() const { return course; }
    string getGrade() const { return grade; }

    // Setters
    void setName(const string& n) { name = n; }
    void setCourse(const string& c) { course = c; }
    void setGrade(const string& g) { grade = g; }

    // Convert student object to a comma-separated string for file storage
    string toCSV() const {
        return to_string(rollNo) + "," + name + "," + course + "," + grade;
    }

    // Populate student object from a comma-separated string
    bool fromCSV(const string& csvLine) {
        stringstream ss(csvLine);
        string token;
        try {
            if (getline(ss, token, ',')) rollNo = stoi(token); else return false;
            if (!getline(ss, name, ',')) return false;
            if (!getline(ss, course, ',')) return false;
            if (!getline(ss, grade)) return false; // Grade is the last element (no trailing comma)
            return true;
        } catch (...) {
            return false; // Handle any conversion errors (e.g., non-integer roll no)
        }
    }
};

// Main System Class
class StudentManagementSystem {
private:
    const string fileName = "students.txt";

    // Helper function to clear the input buffer to prevent getline skipping issues
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Helper function to check if a roll number already exists
    bool isRollNoExists(int rollNo) {
        ifstream inFile(fileName);
        if (!inFile.is_open()) return false; // If file doesn't exist, roll no doesn't exist

        string line;
        while (getline(inFile, line)) {
            Student s;
            if (s.fromCSV(line) && s.getRollNo() == rollNo) {
                inFile.close();
                return true;
            }
        }
        inFile.close();
        return false;
    }

public:
    // 1. Add a new student record
    void addStudent() {
        int rollNo;
        string name, course, grade;

        cout << "\n--- Add New Student ---\n";
        cout << "Enter Roll Number: ";
        
        // Input validation for Roll Number
        while (!(cin >> rollNo)) {
            cout << "Invalid input. Please enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        if (isRollNoExists(rollNo)) {
            cout << "Error: A student with Roll Number " << rollNo << " already exists!\n";
            return;
        }

        clearInputBuffer(); // Clear buffer before using getline

        cout << "Enter Full Name: ";
        getline(cin, name);
        
        cout << "Enter Course: ";
        getline(cin, course);
        
        cout << "Enter Grade: ";
        getline(cin, grade);

        Student newStudent(rollNo, name, course, grade);

        // Open file in append mode
        ofstream outFile(fileName, ios::app);
        if (outFile.is_open()) {
            outFile << newStudent.toCSV() << "\n";
            outFile.close();
            cout << "Student added successfully!\n";
        } else {
            cout << "Error: Unable to open file for writing.\n";
        }
    }

    // 2. Display all student records
    void displayAllStudents() {
        ifstream inFile(fileName);
        if (!inFile.is_open()) {
            cout << "\nError: No records found or unable to open database.\n";
            return;
        }

        cout << "\n-------------------------------------------------------------------\n";
        cout << left << setw(10) << "Roll No" 
             << setw(25) << "Name" 
             << setw(20) << "Course" 
             << "Grade" << "\n";
        cout << "-------------------------------------------------------------------\n";

        string line;
        bool hasRecords = false;

        while (getline(inFile, line)) {
            Student s;
            if (s.fromCSV(line)) {
                hasRecords = true;
                cout << left << setw(10) << s.getRollNo() 
                     << setw(25) << s.getName() 
                     << setw(20) << s.getCourse() 
                     << s.getGrade() << "\n";
            }
        }
        inFile.close();
        cout << "-------------------------------------------------------------------\n";

        if (!hasRecords) {
            cout << "No student records available.\n";
        }
    }

    // 3. Search for a specific student by Roll Number
    void searchStudent() {
        int rollNo;
        cout << "\n--- Search Student ---\n";
        cout << "Enter Roll Number to search: ";
        
        while (!(cin >> rollNo)) {
            cout << "Invalid input. Please enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        ifstream inFile(fileName);
        if (!inFile.is_open()) {
            cout << "Error: No database found.\n";
            return;
        }

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            Student s;
            if (s.fromCSV(line) && s.getRollNo() == rollNo) {
                cout << "\n--- Student Found ---\n";
                cout << "Roll No : " << s.getRollNo() << "\n";
                cout << "Name    : " << s.getName() << "\n";
                cout << "Course  : " << s.getCourse() << "\n";
                cout << "Grade   : " << s.getGrade() << "\n";
                found = true;
                break; // Stop searching once found
            }
        }
        inFile.close();

        if (!found) {
            cout << "Student with Roll Number " << rollNo << " not found.\n";
        }
    }

    // 4. Update an existing student's details
    void updateStudent() {
        int rollNo;
        cout << "\n--- Update Student Record ---\n";
        cout << "Enter Roll Number to update: ";
        
        while (!(cin >> rollNo)) {
            cout << "Invalid input. Please enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        ifstream inFile(fileName);
        if (!inFile.is_open()) {
            cout << "Error: No database found.\n";
            return;
        }

        string tempFileName = "temp.txt";
        ofstream outFile(tempFileName);

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            Student s;
            if (s.fromCSV(line)) {
                if (s.getRollNo() == rollNo) {
                    found = true;
                    clearInputBuffer();
                    
                    cout << "Student found! Enter new details.\n";
                    string newName, newCourse, newGrade;
                    
                    cout << "Enter New Name (leave blank to keep current: " << s.getName() << "): ";
                    getline(cin, newName);
                    if (!newName.empty()) s.setName(newName);
                    
                    cout << "Enter New Course (leave blank to keep current: " << s.getCourse() << "): ";
                    getline(cin, newCourse);
                    if (!newCourse.empty()) s.setCourse(newCourse);

                    cout << "Enter New Grade (leave blank to keep current: " << s.getGrade() << "): ";
                    getline(cin, newGrade);
                    if (!newGrade.empty()) s.setGrade(newGrade);

                    outFile << s.toCSV() << "\n"; // Write updated record
                    cout << "Student record updated successfully.\n";
                } else {
                    outFile << line << "\n"; // Write existing record unchanged
                }
            }
        }
        
        inFile.close();
        outFile.close();

        // Replace old file with the updated temp file
        if (found) {
            remove(fileName.c_str());
            rename(tempFileName.c_str(), fileName.c_str());
        } else {
            remove(tempFileName.c_str()); // Cleanup temp file if not found
            cout << "Student with Roll Number " << rollNo << " not found.\n";
        }
    }

    // 5. Delete a student record
    void deleteStudent() {
        int rollNo;
        cout << "\n--- Delete Student Record ---\n";
        cout << "Enter Roll Number to delete: ";
        
        while (!(cin >> rollNo)) {
            cout << "Invalid input. Please enter a numeric Roll Number: ";
            clearInputBuffer();
        }

        ifstream inFile(fileName);
        if (!inFile.is_open()) {
            cout << "Error: No database found.\n";
            return;
        }

        string tempFileName = "temp.txt";
        ofstream outFile(tempFileName);

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            Student s;
            if (s.fromCSV(line)) {
                if (s.getRollNo() == rollNo) {
                    found = true;
                    // Simply skip writing this record to the temp file to "delete" it
                } else {
                    outFile << line << "\n";
                }
            }
        }
        
        inFile.close();
        outFile.close();

        if (found) {
            remove(fileName.c_str());
            rename(tempFileName.c_str(), fileName.c_str());
            cout << "Student record deleted successfully.\n";
        } else {
            remove(tempFileName.c_str()); // Cleanup
            cout << "Student with Roll Number " << rollNo << " not found.\n";
        }
    }
};

// Entry point of the application
int main() {
    StudentManagementSystem sms;
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "       STUDENT MANAGEMENT SYSTEM         \n";
        cout << "=========================================\n";
        cout << "1. Add New Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice (1-6): ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 6.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                sms.addStudent();
                break;
            case 2:
                sms.displayAllStudents();
                break;
            case 3:
                sms.searchStudent();
                break;
            case 4:
                sms.updateStudent();
                break;
            case 5:
                sms.deleteStudent();
                break;
            case 6:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please select a valid option from the menu.\n";
        }
    } while (choice != 6);

    return 0;
}