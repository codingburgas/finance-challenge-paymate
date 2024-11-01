#pragma once

#include "pch.h"

extern char currentUser[25];


class DataAccess {
public:
    void addAccount(const string& username, const string& password,const string& gender) const;
    void addAccountInfo(const string& monhtlyIncome, const string& savings, const string& balance) const;
    void diagramData(const string& username, const string& essentials, const string& security, const string& personal, const string& social) const;
};
