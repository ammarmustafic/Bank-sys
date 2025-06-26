#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>
#include <random>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

struct Account {
    std::string accountNumber;
    double balance = 0.0;
};

struct User {
    std::string username;
    size_t hashedPassword;
    std::string email;
    Account account;
};

class BankSystem {
public:
    BankSystem(const std::string& dataFile) 
        : m_dataFile(dataFile), m_rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
        loadData();
    }

    ~BankSystem() {
        saveData();
    }

    void run() {
        while (m_isRunning) {
            displayMainMenu();
            int choice = getUserChoice();
            switch (choice) {
                case 1: handleRegistration(); break;
                case 2: handleLogin(); break;
                case 3: m_isRunning = false; break;
                default:
                    std::cout << "\nInvalid selection. Please try again." << std::endl;
                    sleep(1000);
                    break;
            }
        }
        std::cout << "\nSystem shutting down. Goodbye." << std::endl;
    }

private:
    std::unordered_map<std::string, User> m_users;
    std::string m_dataFile;
    std::string m_currentUser;
    bool m_isRunning = true;
    std::mt19937 m_rng;

    void displayMainMenu() {
        clearScreen();
        std::cout << "===== UNICREDIT BANK Mainframe v3.0 =====" << std::endl;
        std::cout << "1. Register New Account" << std::endl;
        std::cout << "2. Login to Existing Account" << std::endl;
        std::cout << "3. Exit System" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Enter your selection: ";
    }

    void displayUserMenu() {
        clearScreen();
        std::cout << "===== Welcome, Operator " << m_currentUser << " =====" << std::endl;
        std::cout << "Account: " << m_users[m_currentUser].account.accountNumber << std::endl;
        std::cout << std::fixed << std::setprecision(2) << "Balance: $" << m_users[m_currentUser].account.balance << std::endl;
        std::cout << "---------------------------------" << std::endl;
        std::cout << "1. Transfer Funds" << std::endl;
        std::cout << "2. Deposit Funds" << std::endl;
        std::cout << "3. Withdraw Funds" << std::endl;
        std::cout << "4. View Transaction Log" << std::endl;
        std::cout << "5. Logout" << std::endl;
        std::cout << "=================================" << std::endl;
        std::cout << "Enter your selection: ";
    }

    int getUserChoice() {
        int choice;
        std::cin >> choice;
        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }

    void handleRegistration() {
        clearScreen();
        std::cout << "--- New Account Registration ---" << std::endl;
        User newUser;
        
        std::cout << "Enter username: ";
        std::getline(std::cin, newUser.username);

        if (m_users.count(newUser.username)) {
            std::cout << "Username already exists." << std::endl;
            sleep(1500);
            return;
        }

        std::cout << "Enter email: ";
        std::getline(std::cin, newUser.email);

        std::cout << "Enter password: ";
        std::string password;
        std::getline(std::cin, password);
        newUser.hashedPassword = std::hash<std::string>{}(password);
        
        newUser.account.accountNumber = generateAccountNumber();
        newUser.account.balance = 100.0; 

        m_users[newUser.username] = newUser;
        std::cout << "\nRegistration successful! Your account number is " << newUser.account.accountNumber << std::endl;
        std::cout << "An initial deposit of $100.00 has been made." << std::endl;
        sleep(3000);
    }

    void handleLogin() {
        clearScreen();
        std::cout << "--- Secure Login ---" << std::endl;
        std::cout << "Enter username: ";
        std::string username;
        std::getline(std::cin, username);

        std::cout << "Enter password: ";
        std::string password;
        std::getline(std::cin, password);

        if (m_users.count(username) && m_users[username].hashedPassword == std::hash<std::string>{}(password)) {
            m_currentUser = username;
            std::cout << "\nLogin successful. Authenticating..." << std::endl;
            sleep(1500);
            userSession();
        } else {
            std::cout << "\nAuthentication failed. Invalid credentials." << std::endl;
            sleep(2000);
        }
    }

    void userSession() {
        bool loggedIn = true;
        while(loggedIn) {
            displayUserMenu();
            int choice = getUserChoice();
            switch(choice) {
                case 1: handleTransfer(); break;
                case 2: handleDeposit(); break;
                case 3: handleWithdrawal(); break;
                case 4: viewTransactions(); break;
                case 5: loggedIn = false; break;
                default:
                    std::cout << "\nInvalid selection." << std::endl;
                    sleep(1000);
                    break;
            }
        }
        m_currentUser.clear();
        std::cout << "\nSession terminated. Logging out..." << std::endl;
        sleep(1500);
    }

    void handleTransfer() {
        std::cout << "Enter recipient username: ";
        std::string recipient;
        std::getline(std::cin, recipient);

        if(!m_users.count(recipient) || recipient == m_currentUser) {
            std::cout << "Invalid recipient." << std::endl;
            sleep(1500);
            return;
        }

        std::cout << "Enter amount to transfer: $";
        double amount;
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        if (amount <= 0 || amount > m_users[m_currentUser].account.balance) {
            std::cout << "Invalid amount or insufficient funds." << std::endl;
            sleep(1500);
            return;
        }
        
        m_users[m_currentUser].account.balance -= amount;
        m_users[recipient].account.balance += amount;
        
        logTransaction(m_currentUser, recipient, amount);
        
        std::cout << "Transfer successful." << std::endl;
        std::cout << "\nPress Enter to return to the menu...";
        std::cin.get();
    }

    void handleDeposit() {
        std::cout << "Enter amount to deposit: $";
        double amount;
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(amount <= 0) {
            std::cout << "Invalid amount." << std::endl;
            sleep(1500);
            return;
        }

        m_users[m_currentUser].account.balance += amount;
        logTransaction(m_currentUser, "DEPOSIT", amount);
        std::cout << "Deposit successful." << std::endl;
        std::cout << "\nPress Enter to return to the menu...";
        std::cin.get();
    }

    void handleWithdrawal() {
         std::cout << "Enter amount to withdraw: $";
        double amount;
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (amount <= 0 || amount > m_users[m_currentUser].account.balance) {
            std::cout << "Invalid amount or insufficient funds." << std::endl;
            sleep(1500);
            return;
        }
        
        m_users[m_currentUser].account.balance -= amount;
        logTransaction(m_currentUser, "WITHDRAWAL", -amount);
        std::cout << "Withdrawal successful." << std::endl;
        std::cout << "\nPress Enter to return to the menu...";
        std::cin.get();
    }

    void viewTransactions() {
        clearScreen();
        std::cout << "\n--- Transaction Log for " << m_currentUser << " ---" << std::endl;
        std::ifstream file(m_currentUser + "_transactions.log");
        if(!file) {
            std::cout << "No transactions found." << std::endl;
        } else {
            std::string line;
            while(std::getline(file, line)) {
                std::cout << line << std::endl;
            }
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    void loadData() {
        std::ifstream file(m_dataFile);
        if (!file) return;

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            User u;
            ss >> u.username >> u.hashedPassword >> u.email >> u.account.accountNumber >> u.account.balance;
            m_users[u.username] = u;
        }
    }

    void saveData() {
        std::ofstream file(m_dataFile);
        for (const auto& pair : m_users) {
            const User& u = pair.second;
            file << u.username << " " << u.hashedPassword << " " << u.email << " " << u.account.accountNumber << " " << u.account.balance << "\n";
        }
    }

    std::string generateAccountNumber() {
        std::uniform_int_distribution<int> dist(0, 9);
        std::string number = "ACC";
        for (int i = 0; i < 9; ++i) {
            number += std::to_string(dist(m_rng));
        }
        return number;
    }

    void logTransaction(const std::string& from, const std::string& to, double amount) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &time);
        timeStr[24] = '\0';

        std::ofstream file(from + "_transactions.log", std::ios::app);
        file << "[" << timeStr << "] ";
        if(to == "DEPOSIT") {
             file << "Deposited $" << std::fixed << std::setprecision(2) << amount;
        } else if (to == "WITHDRAWAL") {
             file << "Withdrew $" << std::fixed << std::setprecision(2) << -amount;
        } else {
             file << "Transferred $" << std::fixed << std::setprecision(2) << amount << " to " << to;
             std::ofstream recipientFile(to + "_transactions.log", std::ios::app);
             recipientFile << "[" << timeStr << "] " << "Received $" << std::fixed << std::setprecision(2) << amount << " from " << from << "\n";
        }
        file << "\n";
    }
};

int main() {
    BankSystem bank("bank_data.dat");
    bank.run();
    return 0;
}
