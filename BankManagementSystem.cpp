#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdio>

using namespace std;

// Class representing a single Bank Account
class BankAccount {
private:
    int accountNumber;
    string accountHolderName;
    string pin; // Basic security
    double balance;

public:
    // Default constructor
    BankAccount() : accountNumber(0), accountHolderName(""), pin(""), balance(0.0) {}

    // Parameterized constructor
    BankAccount(int accNo, string name, string p, double bal) 
        : accountNumber(accNo), accountHolderName(name), pin(p), balance(bal) {}

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getName() const { return accountHolderName; }
    double getBalance() const { return balance; }
    
    // Authenticate PIN
    bool verifyPin(const string& enteredPin) const { return pin == enteredPin; }

    // Banking Operations
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Successfully deposited $" << fixed << setprecision(2) << amount << ".\n";
        } else {
            cout << "Invalid deposit amount.\n";
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Successfully withdrew $" << fixed << setprecision(2) << amount << ".\n";
            return true;
        } else if (amount > balance) {
            cout << "Insufficient funds. Current balance is $" << fixed << setprecision(2) << balance << ".\n";
            return false;
        } else {
            cout << "Invalid withdrawal amount.\n";
            return false;
        }
    }

    // Convert object to CSV string for file storage
    string toCSV() const {
        // Format: AccountNumber,Name,PIN,Balance
        ostringstream oss;
        oss << accountNumber << "," << accountHolderName << "," << pin << "," << fixed << setprecision(2) << balance;
        return oss.str();
    }

    // Populate object from CSV string
    bool fromCSV(const string& csvLine) {
        stringstream ss(csvLine);
        string token;
        try {
            if (getline(ss, token, ',')) accountNumber = stoi(token); else return false;
            if (!getline(ss, accountHolderName, ',')) return false;
            if (!getline(ss, pin, ',')) return false;
            if (getline(ss, token)) balance = stod(token); else return false;
            return true;
        } catch (...) {
            return false;
        }
    }
};

// Main System Class
class BankSystem {
private:
    const string fileName = "bank_database.txt";

    // Clears standard input buffer to prevent infinite loops on bad input
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Generates a new account number by finding the highest existing one
    int generateAccountNumber() {
        ifstream inFile(fileName);
        int maxAccNo = 1000; // Starting account number
        string line;
        
        if (inFile.is_open()) {
            while (getline(inFile, line)) {
                BankAccount acc;
                if (acc.fromCSV(line) && acc.getAccountNumber() > maxAccNo) {
                    maxAccNo = acc.getAccountNumber();
                }
            }
            inFile.close();
        }
        return maxAccNo + 1;
    }

    // Updates a specific account's record in the file
    void updateAccountFile(const BankAccount& updatedAccount) {
        ifstream inFile(fileName);
        ofstream outFile("temp.txt");
        string line;
        bool updated = false;

        if (inFile.is_open() && outFile.is_open()) {
            while (getline(inFile, line)) {
                BankAccount acc;
                if (acc.fromCSV(line)) {
                    if (acc.getAccountNumber() == updatedAccount.getAccountNumber()) {
                        outFile << updatedAccount.toCSV() << "\n";
                        updated = true;
                    } else {
                        outFile << line << "\n";
                    }
                }
            }
            inFile.close();
            outFile.close();

            if (updated) {
                remove(fileName.c_str());
                rename("temp.txt", fileName.c_str());
            } else {
                remove("temp.txt");
            }
        }
    }

public:
    void createAccount() {
        string name, pin;
        double initialDeposit;

        cout << "\n--- Open New Bank Account ---\n";
        clearInputBuffer(); // Clear any leftover newline characters
        
        cout << "Enter Full Name: ";
        getline(cin, name);
        
        cout << "Set a 4-Digit Security PIN: ";
        getline(cin, pin);

        cout << "Enter Initial Deposit Amount: $";
        while (!(cin >> initialDeposit) || initialDeposit < 0) {
            cout << "Invalid amount. Please enter a positive number: $";
            clearInputBuffer();
        }

        int newAccNo = generateAccountNumber();
        BankAccount newAccount(newAccNo, name, pin, initialDeposit);

        ofstream outFile(fileName, ios::app);
        if (outFile.is_open()) {
            outFile << newAccount.toCSV() << "\n";
            outFile.close();
            cout << "\nAccount Created Successfully!\n";
            cout << "Your Account Number is: " << newAccNo << "\n";
            cout << "Please remember this number and your PIN for future logins.\n";
        } else {
            cout << "Database error: Could not save account.\n";
        }
    }

    void loginMenu() {
        int accNo;
        string enteredPin;

        cout << "\n--- Account Login ---\n";
        cout << "Enter Account Number: ";
        while (!(cin >> accNo)) {
            cout << "Invalid input. Enter numeric Account Number: ";
            clearInputBuffer();
        }
        
        clearInputBuffer();
        cout << "Enter PIN: ";
        getline(cin, enteredPin);

        ifstream inFile(fileName);
        string line;
        bool found = false;
        BankAccount currentAccount;

        if (inFile.is_open()) {
            while (getline(inFile, line)) {
                if (currentAccount.fromCSV(line) && currentAccount.getAccountNumber() == accNo) {
                    found = true;
                    break;
                }
            }
            inFile.close();
        }

        if (found && currentAccount.verifyPin(enteredPin)) {
            accountOperations(currentAccount);
        } else {
            cout << "\nAccess Denied. Invalid Account Number or PIN.\n";
        }
    }

    // Sub-menu for logged-in users
    void accountOperations(BankAccount& acc) {
        int choice;
        double amount;

        do {
            cout << "\n=========================================\n";
            cout << " Welcome, " << acc.getName() << "\n";
            cout << " Account No: " << acc.getAccountNumber() << "\n";
            cout << "=========================================\n";
            cout << "1. Check Balance\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Logout\n";
            cout << "=========================================\n";
            cout << "Select Operation (1-4): ";
            
            if (!(cin >> choice)) {
                clearInputBuffer();
                continue;
            }

            switch (choice) {
                case 1:
                    cout << "\nYour Current Balance is: $" << fixed << setprecision(2) << acc.getBalance() << "\n";
                    break;
                case 2:
                    cout << "\nEnter deposit amount: $";
                    if (cin >> amount) {
                        acc.deposit(amount);
                        updateAccountFile(acc); // Save changes immediately
                    } else {
                        clearInputBuffer();
                    }
                    break;
                case 3:
                    cout << "\nEnter withdrawal amount: $";
                    if (cin >> amount) {
                        if (acc.withdraw(amount)) {
                            updateAccountFile(acc); // Save changes immediately
                        }
                    } else {
                        clearInputBuffer();
                    }
                    break;
                case 4:
                    cout << "\nLogging out. Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 4);
    }
};

int main() {
    BankSystem bank;
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "         BANK MANAGEMENT SYSTEM          \n";
        cout << "=========================================\n";
        cout << "1. Open New Account\n";
        cout << "2. Login to Account\n";
        cout << "3. Exit System\n";
        cout << "=========================================\n";
        cout << "Enter your choice (1-3): ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2:
                bank.loginMenu();
                break;
            case 3:
                cout << "Thank you for using our banking system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Select 1, 2, or 3.\n";
        }
    } while (choice != 3);

    return 0;
}