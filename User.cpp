#include "campusShare.h"
#include <iostream>
#include <sstream>

using namespace std;

User::User() : wishlistCount(0) {
    for (int i = 0; i < 20; ++i) wishlist[i] = 0;
}

User::User(string sId, string n, string g, string pass, string r) {
    studentId = sId;
    name = n;
    gmail = g;
    passwordHash = hashPassword(pass);
    role = r;
    wishlistCount = 0;
    for (int i = 0; i < 20; ++i) wishlist[i] = 0;
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

int User::getWishlistCount() const {
    return wishlistCount;
}

int User::getWishlistElement(int index) const {
    return wishlist[index];
}

void User::setPasswordHash(const string& hash) {
    passwordHash = hash;
}

void User::setRole(const string& r) {
    role = r;
}

void User::addToWishlist(int itemId) {
    for (int i = 0; i < wishlistCount; ++i) {
        if (wishlist[i] == itemId) return;
    }
    if (wishlistCount < 20) {
        wishlist[wishlistCount++] = itemId;
    }
}

void User::removeFromWishlist(int itemId) {
    int index = -1;
    for (int i = 0; i < wishlistCount; ++i) {
        if (wishlist[i] == itemId) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index; i < wishlistCount - 1; ++i) {
            wishlist[i] = wishlist[i + 1];
        }
        wishlistCount--;
    }
}

// Custom hash function (No STL std::hash)
string User::hashPassword(const string& password) {
    unsigned long long hashVal = 0;
    for (char c : password) {
        hashVal = (hashVal * 31) + c;
    }
    return to_string(hashVal);
}

string User::serialize() const {
    string wishStr = "";
    for (int i = 0; i < wishlistCount; ++i) {
        wishStr += to_string(wishlist[i]);
        if (i + 1 < wishlistCount) wishStr += ",";
    }
    return studentId + "|" + name + "|" + gmail + "|" + passwordHash + "|" + role + "|" + wishStr;
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