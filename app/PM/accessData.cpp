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
