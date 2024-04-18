

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>



struct client
{
    std::string login;
    std::string Name;
    std::string LName;
    std::string pass;

};

std::string Password()
{
    std::string pass;
    std::random_device r;
    std::default_random_engine ps(r());
    std::uniform_int_distribution<int> PassAsciiPool(33, 126);
    int PassAscii;
    char PassChar;
    int x = 0;
    while (x < 21)
    {
        PassAscii = PassAsciiPool(ps);
        PassChar = char(PassAscii);
        pass += PassChar;
        ++x;
    }
    std::cout << pass;
    return pass;
}

void SignUp()
{
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> UserNumPool(1000, 9999);
    int UserNum = UserNumPool(e1);
    std::string DBName;
    std::string UserFile;
    client New;
    std::cout << "Please, enter your first name: ";
    std::cin >> New.Name;
    std::cout << "Please, enter your last name: ";
    std::cin >> New.LName;
    New.login += New.Name[0];
    New.login += ".";
    New.login += New.LName;
    New.login += std::to_string(UserNum);
    DBName += New.login + "_transactions.txt";
    New.pass = Password();
    std::cout << "\nFirst name: " << New.Name << "\nLast name: " << New.LName << "\nLogin: " << New.login << "\nPassword: " << New.pass;
    New.login += ".txt";
    std::ofstream outfile(New.login);
    outfile << New.pass << '\n' << New.Name << '\n' << New.LName << std::endl;
    outfile.close();
    std::ofstream DBFile(DBName);
    DBFile.close();


}

std::string SignIn()
{
    client exist;
    std::string line;
    std::string temp;
    int n = 0;
    while (n == 0) {
        std::cout << "\nPlease log in into the system: \nUsername: ";
        std::cin >> exist.login;
        std::cout << "Password: ";
        std::cin >> exist.pass;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        temp += exist.login;
        temp += ".txt";
        std::ifstream myfile(temp);
        if (myfile.is_open())
        {

            myfile >> line;
            int check = exist.pass.compare(line);
            if (check == 0)
            {
                myfile >> exist.Name;
                myfile >> exist.LName;
                std::cout << "Hello, " << exist.Name << " " << exist.LName;
                ++n;
                return exist.login;

            }
            else {
                std::cout << "Wrong pass or login, man! \nTry again\n";
            }


        }
        else {
            std::cout << "Account with this username has not been found \nPlease, sign up to the system\n";
            SignUp();
        }
    }
    

}







struct Transaction {
    std::string type; 
    double amount;
    std::string category; 
    std::string date; 
};


std::vector<Transaction> readTransactions(const std::string& filename) {
    std::vector<Transaction> transactions;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return transactions;
    }

    std::string type, category, date;
    double amount;
    while (file >> type >> amount >> category >> date) {
        transactions.push_back({ type, amount, category, date });
    }
    file.close();
    return transactions;
}


void writeTransactions(const std::string& filename, const std::vector<Transaction>& transactions) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& transaction : transactions) {
        file << transaction.type << " " << transaction.amount << " " << transaction.category
            << " " << transaction.date << std::endl;
    }
    file.close();
}


double calculateBalance(const std::vector<Transaction>& transactions) {
    double balance = 0.0;
    for (const auto& transaction : transactions) {
        if (transaction.type == "Income") {
            balance += transaction.amount;
        }
        else if (transaction.type == "Expense") {
            balance -= transaction.amount;
        }
    }
    return balance;
}

void displayTransactionsForMonth(const std::vector<Transaction>& transactions, int year, int month) {
    std::cout << "\nTransactions for " << year << "-" << month << ":\n";
    for (const auto& transaction : transactions) {
        std::tm t = {};
        std::istringstream ss(transaction.date);
        ss >> std::get_time(&t, "%Y-%m-%d");
        if (t.tm_year + 1900 == year && t.tm_mon + 1 == month) {
            std::cout << transaction.date << " | " << transaction.type << " | "
                << transaction.amount << " | " << transaction.category << std::endl;
        }
    }
}

std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now); 
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    return buffer;
}

int main() {
    std::string username;
    while (true) {
        std::cout << "\n1. Sign In\n2. Sign Up\n3. Exit\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            username = SignIn();
            if (!username.empty()) {
                break;
            }
        }
        else if (choice == 2) {
            SignUp();
            std::cout << "Sign up successful! Please sign in.\n";
        }
        else if (choice == 3) {
            std::cout << "Goodbye!\n";
            return 0;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    std::string filename = username + "_transactions.txt";
    std::vector<Transaction> transactions = readTransactions(filename);
    std::cout << filename;

    while (true) {
        std::cout << "\n1. Add Income\n2. Add Expense\n3. View Balance\n4. View Transactions for a Month\n5. Exit\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            double amount;
            std::string date = getCurrentDate();
            std::cout << "Enter income amount: $";
            std::cin >> amount;
            transactions.push_back({ "Income", amount, "Salary", date});
            writeTransactions(filename, transactions);
            std::cout << "Income added successfully!\n";
        }
        else if (choice == 2) {
            std::string type = "Expense";
            double amount;
            std::string category;
            std::string date = getCurrentDate();
            std::cout << "Enter expense amount: $";
            std::cin >> amount;
            std::cout << "Enter category (Food/House/Entertainment): ";
            std::cin >> category;
            transactions.push_back({ type, amount, category, date });
            writeTransactions(filename, transactions);
            std::cout << type << " added successfully!\n";
        }
        else if (choice == 3) {
            double balance = calculateBalance(transactions);
            std::cout << "Your balance: $" << balance << std::endl;
        }
        else if (choice == 4) {
            std::cout << "Choose an option:\n";
            std::cout << "1. View transactions for the current month\n";
            std::cout << "2. View transactions for a specific date range\n";
            int viewChoice;
            std::cin >> viewChoice;

            if (viewChoice == 1) {
                
                std::time_t now = std::time(nullptr);
                std::tm localTime;
                localtime_s(&localTime, &now); 
                int currentYear = localTime.tm_year + 1900;
                int currentMonth = localTime.tm_mon + 1;
                displayTransactionsForMonth(transactions, currentYear, currentMonth);
            }
            else if (viewChoice == 2) {
                int year, month;
                std::cout << "Enter year and month (YYYY MM): ";
                std::cin >> year >> month;
                displayTransactionsForMonth(transactions, year, month);
            }
        }
        else if (choice == 5) {
            std::cout << "Goodbye!\n";
            return 0;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}

 




