#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <ctime>

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
    bool isAccountNumberValid()
    {
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
                    if (accNumber == accountNumber)
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
    void generateReceipt(const string &transactionType, double amount = 0)
    {
        ofstream receiptFile(RECEIPT_FILE, ios::app);
        if (receiptFile.is_open())
        {
            time_t now = time(0);
            char *dt = ctime(&now);

            stringstream receiptContent;
            receiptContent << "Date/Time: " << dt;
            receiptContent << "Account Number: " << accountNumber << endl;
            receiptContent << "Transaction: " << transactionType << endl;
            if (transactionType != "Check Balance")
            {
                receiptContent << "Amount: $" << fixed << setprecision(2) << amount << endl;
            }
            receiptContent << "Balance: $" << fixed << setprecision(2) << balance << endl;
            receiptContent << "---------------------------------------" << endl;

            receiptFile << receiptContent.str();
            receiptFile.close();

            // Print the receipt to the terminal
            cout << "\nReceipt:\n"
                 << receiptContent.str();
        }
        else
        {
            cout << "Unable to open receipt file." << endl;
        }
    }

public:
    ATM()
    {
        balance = 0;
        cout << "Enter your account number: ";
        cin >> accountNumber;
        if (isAccountNumberValid())
        {
            readAccountData();
        }
        else
        {
            cout << "Invalid account number. Please try again!!!" << endl;
            ATM();
        }
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
        }
    }

    void checkBalance()
    {
        cout << "Your current balance is: Rs " << fixed << setprecision(2) << balance << endl;
        char ch;
        cout << "Do you want the receipt!![y/n]" << endl;
        cin >> ch;
        switch (ch)
        {
        case 'y':
        case 'Y':
            generateReceipt("Check Balance");
            cout << "Thank you for using the ATM!" << endl;
            break;
        case 'n':
        case 'N':
            cout << "Thank you for using the ATM!" << endl;
            break;
        default:
            exit(1);
        }
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
            cout << "Withdrawal successful. Take Your Cash!!" << fixed << setprecision(2) << balance << endl;
            updateAccountData();
            cout << "Do you want the receipt!![y/n]" << endl;
            char ch;
            cin >> ch;
            switch (ch)
            {
            case 'y':
            case 'Y':
                generateReceipt("Withdraw", amount);
                cout << "Thank you for using the ATM!" << endl;
                break;
            case 'n':
            case 'N':
                cout << "Thank you for using the ATM!" << endl;
                break;
            default:
                exit(1);
            }
        }
    }

    void deposit()
    {
        double amount;
        cout << "Enter the amount to deposit: ";
        cin >> amount;
        balance += amount;
        cout << "Deposited successfully." << fixed << setprecision(2) << balance << endl;
        cout << "Thank you For using  ATM!!" << endl;
        updateAccountData();
        char ch;
        cout << "Do you want the receipt!![y/n]" << endl;
        cin >> ch;
        switch (ch)
        {
        case 'y':
        case 'Y':
            generateReceipt("Deposit", amount);
            cout << "Thank you for using the ATM!" << endl;
            break;
        case 'n':
        case 'N':
            cout << "Thank you for using the ATM!" << endl;
            break;
        default:
            exit(1);
        }
    }
};

int main()
{

    ATM atm;
    atm.run();
    return 0;
}
