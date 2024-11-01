#include "accessData.h"

char currentUser[25] = "";

void DataAccess::addAccount(const string& username, const string& password, const string& gender)const 
{
    // Open accounts.csv file
    ofstream file("../data/accounts.csv", ios_base::app);
    // Write username and password
    file << username << ',' << password <<',' << gender <<  "\n";
    // Close the file
    file.close();
}

void DataAccess::addAccountInfo(const string& monhtlyIncome, const string& savings, const string& balance)const 
{
    // Open accounts.csv file
    ofstream file("../data/accountData.csv", ios_base::app);
    // Write username and password
    file << monhtlyIncome << ',' << savings << ',' << balance << "\n";
    // Close the file
    file.close();
}

void DataAccess::diagramData(const string& username, const string& essentials, const string& security, const string& personal, const string& social) const
{
    // Open accounts.csv file
    ofstream file("../data/diagramData.csv", ios_base::app);
    // Write username and password
    file << username << ',' << essentials << ',' << security << ',' << personal << ',' << social << "\n";
    // Close the file
    file.close();
}
