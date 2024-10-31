#include "accessData.h"

char currentUser[25] = "";

void DataAccess::addAccount(const string& username, const string& password, const string& gender)const {
    // Open accounts.csv file
    ofstream file("../data/accounts.csv", ios_base::app);
    // Write username and password
    file << username << ',' << password <<',' << gender <<  "\n";
    // Close the file
    file.close();
}

void DataAccess::addAccountInfo(const string& monhtlyIncome, const string& savings, const string& balance)const {
    // Open accounts.csv file
    ofstream file("../data/accountData.csv", ios_base::app);
    // Write username and password
    file << monhtlyIncome << ',' << savings << ',' << balance << "\n";
    // Close the file
    file.close();
}
