#pragma once

#include "pch.h"

extern char currentUser[25];


class DataAccess {
public:
    void addAccount(const string& username, const string& password,const string& gender) const;
    
};