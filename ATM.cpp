#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class User
{
    long long c_number;

public:
    void CardNumber()
    {
        cout << "Enter The card Number: " <<endl;
        cin >> c_number;
    }
};
int main()
{
    User U;
    U.CardNumber();
    return 0;
}