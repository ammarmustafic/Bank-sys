#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <chrono>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <random>
#include <windows.h>
using namespace std;

unordered_map<string, string> database;
unordered_map<string, double> accountBalances;

string generateRandomResponse() {
int response = rand() % 5;
if (response == 0) {
	Sleep(600);
return "We're sorry, but we cannot help you with that issue at this time. Please visit our Bank location in your city for assistance.";
} else if (response == 1) {
	Sleep(600);
return "Thank you for contacting Bank support. We have forwarded your issue to the appropriate department.";
} else if (response == 2) {
	Sleep(600);
return "We apologize for the inconvenience, but that feature is currently under maintenance. Please try again later.";
} else if (response == 3) {
	Sleep(600);
return "Thank you for bringing this issue to our attention. We will work to resolve it as soon as possible.";
} else if (response == 4) {
	Sleep(600);
return "We're sorry, but that service is not currently available. Please check back later or visit a Unicredit Bank location for assistance.";
}
}


// save balance
void saveBalances(string username) {
    ifstream input("Accounts/balances.txt");
    ofstream output("Accounts/balances_temp.txt");
    string line;
    bool userFound = false;
    while (getline(input, line)) {
        stringstream ss(line);
        string file_username;
        double balance;
        ss >> file_username >> balance;
        if (file_username == username) {
            output << username << " " << accountBalances[username] << endl;
            userFound = true;
        } else {
            output << line << endl;
        }
    }
    input.close();
    output.close();

    if (!userFound) {
        ofstream output("Accounts/balances_temp.txt", ios_base::app);
        output << username << " " << accountBalances[username] << endl;
        output.close();
    }

    remove("Accounts/balances.txt");
    rename("Accounts/balances_temp.txt", "Accounts/balances.txt");
}

// load balance
void loadBalances(string username) {
  ifstream input("Accounts/balances.txt");
    string line;
    while (getline(input, line)) {
        stringstream ss(line);
        string file_username;
        double balance;
        ss >> file_username >> balance;
        if(file_username == username){
            accountBalances[username] = balance;
            break;
        }
    }
    input.close();
}

// user menu
void userMenu(string username, string password) {
	loadBalances(username);
	cout << "Welcome to the Bank, " << username << endl;
	usergoto:
    cout << "Choose what do you want" << endl;
    cout << "1. Transfer money" << endl;
    cout << "2. Withdraw money" << endl;
    cout << "3. Change password" << endl;
    cout << "4. Change username" << endl;
    cout << "5. Contact support" << endl;
    cout << "6. Log out" << endl;
    int choice;
    cin >> choice;
    
    
    if (choice == 1) {
    cout << "Enter the username of the recipient: ";
    string recipient;
    cin >> recipient;
    double amount;
    cout << "Enter the amount you want to transfer: ";
    cin >> amount;
    if(recipient == username) {
        cout<<"You cannot send money to yourself.";
        exit(0);
    }
    ifstream usersFile1("Accounts/users.txt");
    string line;
    bool recipientExists = false;
    while (getline(usersFile1, line)) {
        if (line.find(recipient + ":") == 0) {
            recipientExists = true;
            break;
        }
    }
    usersFile1.close();

    if (!recipientExists) {
        cout << "The recipient does not have an account." << endl;
        return;
    } else if (accountBalances[username] < amount) {
        cout << "You do not have sufficient funds to make this transfer." << endl;
        return;
    }
    accountBalances[username] -= amount;
    accountBalances[recipient] += amount;
    saveBalances(username);
    saveBalances(recipient);
    cout << "Successfully transferred $" << amount << " to " << recipient << "." << endl;

    // adding transaction to file 
    ofstream transactionsFile("Accounts/transactions.txt", ios_base::app);
    transactionsFile << username << " transferred $" << amount << " to " << recipient << endl;
    transactionsFile.close();
}
		else if (choice == 2) {
			Sleep(300);
		  system("CLS");
		  againwdr:
          double amount;
          cout << "Enter the amount you want to withdraw: ";
          cin >> amount;
          if (amount > accountBalances[username]) {
            cout << "You do not have sufficient funds to make this withdrawal." << endl;
            goto againwdr;
          }
		  else if (amount <= 0) {
		  	cout<<"You can not withdraw nothing. Try again." << endl;
		  	Sleep(700);
		  	system("CLS");
		  	goto againwdr;
		  }
		   else {
          accountBalances[username]-= amount;
          saveBalances(username);
          Sleep(400);
          cout << "Successfully withdrew $" << amount << "." << endl;
          Sleep(800);
          cout << "Please login again because of our security."<<endl;
          Sleep(900);
          system("CLS");
         } 
		} 
		  else if (choice == 3) {
          againburke:
          	Sleep(300);
		  system("CLS");
          cout << "Enter your new password: ";
          string newPassword;
          cin >> newPassword;
          if(newPassword == password) {
            cout << "You cannot change your password to your current password. Please enter a different password." << endl;
            Sleep(800);
            system("CLS");
            goto againburke;
          }
          password = newPassword;
          Sleep(350);
          cout << "Successfully changed password." << endl;
          
          // update password in users.txt file 
          fstream file("Accounts/users.txt");
          string line;
          stringstream buffer;
          while (getline(file, line)) {
            string key = line.substr(0, line.find(":"));
            if (key == username) {
              buffer << username << ":" << newPassword << endl;
            } else {
              buffer << line << endl;
            }
          }
          file.close();
          ofstream output("Accounts/users.txt");
          output << buffer.str();
          output.close();
        } 
		else if (choice == 4) {
		  opetuser:
		  Sleep(300);
		  system("CLS");
          cout << "Enter your new username: ";
          string newUsername;
          cin >> newUsername;
          
          if(newUsername == username) {
            cout << "Invalid. You cannot change your username to your current username. Please enter a different username." << endl;
            Sleep(800);
            system("CLS");
            goto opetuser;
        }
        
          // update username in database
          database[newUsername] = database[username];
          database.erase(username);
          
          // update username in users.txt 
          fstream file("Accounts/users.txt");
          string line;
          stringstream buffer;
          while (getline(file, line)) {
            string key = line.substr(0, line.find(":"));
            if (key == username) {
              buffer << newUsername << line.substr(line.find(":")) << endl;
            } else {
              buffer << line << endl;
            }
          }
          file.close();
          ofstream output("Accounts/users.txt");
          output << buffer.str();
          output.close();
          Sleep(350);
          cout << "Successfully changed username. Please Login again." << endl;
          
          // update username in balances.txt
          fstream f("Accounts/balances.txt");
          string pronadji;
          stringstream buffer2;
          while (getline(f, pronadji)) {
            string key = pronadji.substr(0, pronadji.find(" "));
            if (key == username) {
              buffer2 << newUsername << pronadji.substr(pronadji.find(" ")) << endl;
            } else {
              buffer2 << pronadji << endl;
            }
          }
          f.close();
          ofstream prikaz("Accounts/balances.txt");
          prikaz << buffer2.str();
          prikaz.close();
        }
        else if (choice == 5) {
        	system("CLS");
          cout << "Wait, you are calling support..." << endl;
          Sleep(800);
          cout << "Operator: Hello, how can I help you?" << endl;
          string problem;
          cin.ignore();
          getline(cin, problem);
          cout << generateRandomResponse() << endl;
        } else if (choice == 6) {
        	cout<<"Logging you out, wait...";
        	Sleep(800);
        	system("CLS");
          return;
        } else {
          cout << "Invalid choice. Try again." << endl;
          Sleep(800);
          system("CLS");
          goto usergoto;
        }
}

// register
void registerUser() {
  system("CLS");
  string email;
  string username;
  string password;
  string number;
  string name;
  
  opetbatice:
  cout << "Enter your email: ";
  cin >> email;

bool isValid = false;
bool hasDot = false;
bool hasTextAfterAtSymbol = false;

for (int i = 0; i < email.length(); i++) {
  char c = email[i];
  if (c == '@') {
    isValid = true;
    if (i + 1 < email.length() && email[i+1] != '.') {
      hasTextAfterAtSymbol = true;
    }
  }
  if (isValid && c == '.') {
    hasDot = true;
    break;
  }
}

if (!isValid) {
  cout << "Invalid email address, try again." << endl;
  Sleep(800);
  system("CLS");
  goto opetbatice;
}

if (!hasTextAfterAtSymbol) {
  cout << "Invalid email address, missing text after '@' symbol." << endl;
  Sleep(800);
  system("CLS");
  goto opetbatice;
}

if (!hasDot) {
  cout << "Invalid email address, missing dot after '@' symbol." << endl;
  Sleep(800);
  system("CLS");
  goto opetbatice;
}

isValid = false;

for (char c : email) {
  if (!isupper(c)) {
    isValid = true;
    break;
  }
}

if (!isValid) {
  cout << "Email address cannot be all in caps." << endl;
  Sleep(800);
  system("CLS");
  goto opetbatice;
}
  
  // checks if email already exists in file
    ifstream input1("Accounts/users.txt");
    string line1;
    bool emailInUse = false;
    while (getline(input1, line1)) {
        if (line1.find(email) != string::npos) {
            emailInUse = true;
            break;
        }
    }
    input1.close();

    if (emailInUse) {
        cout << "This email address is already in use. Please enter a different email address." << endl;
        Sleep(800);
        system("CLS");
        goto opetbatice;
    }
  ponovo3:
  cout << "Enter your username: ";
  cin >> username;
  
  // checks if username already exists in file 
  ifstream input("Accounts/users.txt");
  string line;
  bool taken = false;
  while (getline(input, line)) {
    if (line.find(username + ":") == 0) {
      taken = true;
      break;
    }
  }
  
  input.close();
  if (taken) {
    cout << "This username is already taken. Try again." << endl;
    Sleep(800);
    system("CLS");
    goto ponovo3;
  }
  
  isValid = false;
  for (char c : username) {
    if (!isupper(c)) {
      isValid = true;
      break;
    }
  }
  
  if (!isValid) {
    cout << "Username cannot be all in caps." << endl;
    return;
  }
  
  cout << "Enter your password: ";
  cin >> password;
  while (true) {
  	ponovo:
    cout << "Enter your phone number: ";
    cin >> number;
    isValid = true;
    for (char c : number) {
      if (!isdigit(c)) {
        isValid = false;
        break;
      }
    }
    if (number.length() > 12) {
  isValid = false;
}
	else if(number.length() < 6) {
		isValid = false;
	}
    if (isValid) {
      break;
    } else {
      cout << "You did not enter a valid phone number. Please try again." << endl;
      Sleep(800);
      system("CLS");
      goto ponovo;
}
}
ponovo2:
cout << "Enter your name: ";
cin >> name;
bool isValid1 = true;
  for (char f : name) {
    if (isdigit(f)) {
      isValid1 = false;
      break;
    }
  }
  if (!isValid1) {
    cout << "Name cannot contain digits. Try again" << endl;
    Sleep(800);
      system("CLS");
      goto ponovo2;
  }

// generates random 16 digits for credit card
unsigned long long rand_seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine rand_gen(rand_seed);
uniform_int_distribution<unsigned long long> rand_dist(1000000000000000, 9999999999999999);
string creditCard = to_string(rand_dist(rand_gen));


cout << "Successfully registered.\nYour credit card number is: " << creditCard << endl;
Sleep(650);

// creating folder and txt in it 
  CreateDirectory("Accounts", NULL);
  ofstream output("Accounts/users.txt", ios::app);
  output << username << ":" << password << endl;
  output << "email:" << email << endl;
  output << "number:" << number << endl;
  output << "name:" << name << endl;
  output << "creditCard:" << creditCard << endl;
  output << endl;
  output.close();
  accountBalances.emplace(username, 350.0);
  saveBalances(username);
  system("CLS");
  userMenu(username, password);
}


// login
void login() {
system("CLS");
string username;
string password;

cout << "Enter your username: ";
cin >> username;
cout << "Enter your password: ";
cin >> password;

// checks if account exists
ifstream input("Accounts/users.txt");
  string line;
  bool found = false;
  while (getline(input, line)) {
    if (line.find(username + ":") == 0) {
      size_t separatorIndex = line.find(":");
      string value = line.substr(separatorIndex + 1);
      if (value == password) {
        found = true;
        break;
      }
    }
  }
  input.close();

  if (!found) {
    cout << "Invalid username or password, please try again." << endl;
    Sleep(600);
    system("CLS");
    return;
  }
  cout<<"Please wait..";
  Sleep(500);
  system("CLS");
  userMenu(username,password);
  }

// main
int main() {
  cout << "Welcome to the Bank." << endl;
  while (true) {
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    int choice;
    cin >> choice;
    if (cin.fail()) {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "Invalid input. Please enter a number." << endl;
	Sleep(600);
	system("CLS");
	continue;
	}
    if (choice == 1) {
      registerUser();
    } else if (choice == 2) {
      login();
    } else if (choice == 3) {
    	system("CLS");
      cout<<"Exiting...";
      Sleep(600);
      return 0;
    } else {
      cout << "Invalid choice." << endl;
    }
  }
    return 0;
}
