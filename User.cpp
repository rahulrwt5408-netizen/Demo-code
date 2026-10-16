#include "campusShare.h"
#include <iostream>
#include <sstream>
#include <functional>

using namespace std;

User::User() {}

User::User(string sId, string n, string g, string pass, string r) {
    studentId = sId;
    name = n;
    gmail = g;
    passwordHash = hashPassword(pass);
    role = r;
}

string User::getStudentId() const {
    return studentId;
}

string User::getName() const {
    return name;
}

string User::getGmail() const {
    return gmail;
}

string User::getPasswordHash() const {
    return passwordHash;
}

string User::getRole() const {
    return role;
}

void User::setPasswordHash(const string& hash) {
    passwordHash = hash;
}

void User::setRole(const string& r) {
    role = r;
}

string User::hashPassword(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

string User::serialize() const {
    stringstream ss;
    ss << studentId << "|"
       << name << "|"
       << gmail << "|"
       << passwordHash << "|"
       << role;
    return ss.str();
}

// Buyer functions

Buyer::Buyer() : User() {}

Buyer::Buyer(string sId, string n, string g, string pass, string r)
    : User(sId, n, g, pass, r) {}

void Buyer::searchItems() const {
    cout << "Searching items by price, name, date, and condition...\n";
}

void Buyer::rateItem(int itemId, double rating) {
    cout << "Item " << itemId << " rated " << rating << " stars.\n";
}

void Buyer::buyItem(int itemId) {
    cout << "Purchase request placed for item " << itemId << ".\n";
}

// Seller functions

Seller::Seller() : User() {}

Seller::Seller(string sId, string n, string g, string pass, string r)
    : User(sId, n, g, pass, r) {}

void Seller::addItemListing(int itemId, const string& name,
                            const string& cat, double price,
                            const string& cond) {
    cout << "Item " << itemId << ": " << name
         << ", Category: " << cat
         << ", Price: Rs. " << price
         << ", Condition: " << cond << "\n";
}

void Seller::editItemListing(int itemId, const string& newName,
                             double newPrice, const string& newDesc) {
    cout << "Item " << itemId << " updated: "
         << newName << ", Price: Rs. " << newPrice
         << ", Description: " << newDesc << "\n";
}

void Seller::lendOrSellItem(int itemId, const string& transactionType) {
    cout << "Item " << itemId << " is available for "
         << transactionType << ".\n";
}