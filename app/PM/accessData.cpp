#include "accessData.h"

char currentUser[25] = "";

// Function to add a new account with username, password, and gender to a CSV file.
void DataAccess::addAccount(const string& username, const string& password, const string& gender)const 
{
    ofstream file("../data/accounts.csv", ios_base::app);
    file << username << ',' << password <<',' << gender <<  "\n";
    file.close();
}

// Function to add financial information (monthly income, savings, balance) to a CSV file.
void DataAccess::addAccountInfo(const string& monhtlyIncome, const string& savings, const string& balance)const 
{
    ofstream file("../data/accountData.csv", ios_base::app);
    file << monhtlyIncome << ',' << savings << ',' << balance << "\n";
    file.close();
}

// Function to store data related to a user's expenses in different categories (housing, food, health, gifts).
void DataAccess::diagramData(const string& username, const string& housing, const string& food, const string& health, const string& gifts) const
{
    ofstream file("../data/diagramData.csv", ios_base::app);
    file << username << ',' << housing << ',' << food << ',' << health << ',' << gifts << "\n";
    file.close();
}

// Function to save details of the user's last few expenses to the same CSV file as the diagram data.
void DataAccess::saveLastSpendings(const string& expense1, const string& expense2, const string& expense3, const string& expense4, float balance) const
{
    ofstream file("../data/diagramData.csv", ios_base::app);
    file << expense1 << ',' << expense2 << ',' << expense3 << ',' << expense4 << "\n";
    file.close();
}