#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

const string USER_FILE = "users.txt";
const string BALANCE_FILE = "balances.txt";
const string RECEIPT_FILE = "receipts.txt";

class ATM
{
private:
    double balance;
    string accountNumber;

    bool authenticateUser()
    {
        string pin;
        cout << "Enter your PIN: ";
        cin >> pin;
        ifstream userFile(USER_FILE);
        if (userFile.is_open())
        {
            string line;
            while (getline(userFile, line))
            {
                size_t pos = line.find(',');
                if (pos != string::npos)
                {
                    string accNumber = line.substr(0, pos);
                    string userPin = line.substr(pos + 1);
                    if (accNumber == accountNumber && userPin == pin)
                    {
                        userFile.close();
                        return true;
                    }
                }
            }
            userFile.close();
        }
        else
        {
            cout << "Unable to open user file." << endl;
        }
        return false;
    }

    void readAccountData()
    {
        ifstream balanceFile(BALANCE_FILE);
        if (balanceFile.is_open())
        {
            string line;
            while (getline(balanceFile, line))
            {
                size_t pos = line.find(',');
                if (pos != string::npos)
                {
                    string accNumber = line.substr(0, pos);
                    string balanceStr = line.substr(pos + 1);
                    if (accNumber == accountNumber)
                    {
                        balance = stod(balanceStr);
                        break;
                    }
                }
            }
            balanceFile.close();
        }
        else
        {
            cout << "Unable to open balance file." << endl;
        }
    }

    void updateAccountData()
    {
        vector<string> balances;
        ifstream balanceFile(BALANCE_FILE);
        if (balanceFile.is_open())
        {
            string line;
            while (getline(balanceFile, line))
            {
                balances.push_back(line);
            }
            balanceFile.close();
        }
        else
        {
            cout << "Unable to open balance file." << endl;
        }

        ofstream balanceFileOut(BALANCE_FILE);
        if (balanceFileOut.is_open())
        {
            bool updated = false;
            for (const string &line : balances)
            {
                size_t pos = line.find(',');
                if (pos != string::npos)
                {
                    string accNumber = line.substr(0, pos);
                    if (accNumber == accountNumber)
                    {
                        balanceFileOut << accountNumber << "," << balance << endl;
                        updated = true;
                    }
                    else
                    {
                        balanceFileOut << line << endl;
                    }
                }
            }
            if (!updated)
            {
                balanceFileOut << accountNumber << "," << balance << endl;
            }
            balanceFileOut.close();
        }
        else
        {
            cout << "Unable to open balance file for writing." << endl;
        }
    }

public:
    ATM()
    {
        balance = 0;
        cout << "Enter your account number: ";
        cin >> accountNumber;
        readAccountData();
    }

    void run()
    {
        if (authenticateUser())
        {
            char choice;
            do
            {
                cout << "\nATM Menu:" << endl;
                cout << "1. Check Balance" << endl;
                cout << "2. Withdraw" << endl;
                cout << "3. Deposit" << endl;
                cout << "4. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice)
                {
                case '1':
                    checkBalance();
                    break;
                case '2':
                    withdraw();
                    break;
                case '3':
                    deposit();
                    break;
                case '4':
                    cout << "Thank you for using the ATM!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            } while (choice != '4');
        }
        else
        {
            cout << "Authentication failed. Exiting..." << endl;
       run();
        }
        
    }

    void checkBalance()
    {
        cout << "Your current balance is: $" << fixed << setprecision(2) << balance << endl;
    }

    void withdraw()
    {
        double amount;
        cout << "Enter the amount to withdraw: ";
        cin >> amount;
        if (amount > balance)
        {
            cout << "Insufficient funds." << endl;
        }
        else
        {
            balance -= amount;
            cout << "Withdrawal successful. New balance: $" << fixed << setprecision(2) << balance << endl;
            updateAccountData();
        }
    }

    void deposit()
    {
        double amount;
        cout << "Enter the amount to deposit: ";
        cin >> amount;
        balance += amount;
        cout << "Deposit successful. New balance: $" << fixed << setprecision(2) << balance << endl;
        updateAccountData();
    }
};

int main()
{

    ATM atm;
    atm.run();
    return 0;
}