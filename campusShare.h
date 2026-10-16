#ifndef CAMPUS_SHARE_H
#define CAMPUS_SHARE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>
#include <iomanip>
#include <ctime>

using namespace std;

// ==================== USER CLASS ====================
class User {
protected:
    string studentId;
    string name;
    string gmail;
    string passwordHash;
    string role;
    vector<int> wishlist; // Added wishlist vector

public:
    User() {}
    User(string sId, string n, string g, string pass, string r)
        : studentId(sId), name(n), gmail(g), passwordHash(hashPassword(pass)), role(r) {}
    virtual ~User() {}

    string getStudentId() const { return studentId; }
    string getName() const { return name; }
    string getGmail() const { return gmail; }
    string getPasswordHash() const { return passwordHash; }
    string getRole() const { return role; }
    vector<int> getWishlist() const { return wishlist; }

    void setPasswordHash(const string& hash) { passwordHash = hash; }
    void setRole(const string& r) { role = r; }

    void addToWishlist(int itemId) {
        // Check if already in wishlist
        for (int id : wishlist) {
            if (id == itemId) return;
        }
        wishlist.push_back(itemId);
    }

    void removeFromWishlist(int itemId) {
        for (auto it = wishlist.begin(); it != wishlist.end(); ++it) {
            if (*it == itemId) {
                wishlist.erase(it);
                break;
            }
        }
    }

    static string hashPassword(const string& password) {
        hash<string> hasher;
        return to_string(hasher(password));
    }

    virtual string serialize() const {
        string wishStr = "";
        for (size_t i = 0; i < wishlist.size(); ++i) {
            wishStr += to_string(wishlist[i]);
            if (i + 1 < wishlist.size()) wishStr += ",";
        }
        return studentId + "|" + name + "|" + gmail + "|" + passwordHash + "|" + role + "|" + wishStr;
    }

    static User deserialize(const string& line) {
        stringstream ss(line);
        string sId, n, g, passHash, r, wishStr;
        getline(ss, sId, '|');
        getline(ss, n, '|');
        getline(ss, g, '|');
        getline(ss, passHash, '|');
        getline(ss, r, '|');
        getline(ss, wishStr, '|');

        User u(sId, n, g, "", r);
        u.setPasswordHash(passHash);

        if (!wishStr.empty()) {
            stringstream wss(wishStr);
            string itemStr;
            while (getline(wss, itemStr, ',')) {
                if (!itemStr.empty()) {
                    u.wishlist.push_back(stoi(itemStr));
                }
            }
        }
        return u;
    }
};
// ==================== ITEM CLASS ====================
class Item {
private:
    int itemId;
    string sellerId;
    string name;
    string category;
    string description;
    double price;
    double rentalPrice;
    string condition;
    string listingType;
    string availabilityStatus;
    double rating;
    double priorityScore;
    long long timestamp;

public:
    Item() : itemId(0), price(0.0), rentalPrice(0.0), rating(0.0), priorityScore(0.0), timestamp(0) {}

    Item(int id, string sId, string n, string cat, string desc, 
         double p, double rp, string cond, string lType, string status, double rat, long long time)
        : itemId(id), sellerId(sId), name(n), category(cat), description(desc), 
          price(p), rentalPrice(rp), condition(cond), listingType(lType), 
          availabilityStatus(status), rating(rat), timestamp(time) {}

    int getItemId() const { return itemId; }
    string getSellerId() const { return sellerId; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    double getRentalPrice() const { return rentalPrice; }
    string getCondition() const { return condition; }
    string getListingType() const { return listingType; }
    string getAvailabilityStatus() const { return availabilityStatus; }
    double getRating() const { return rating; }
    double getPriorityScore() const { return priorityScore; }
    long long getTimestamp() const { return timestamp; }

    void setName(const string& n) { name = n; }
    void setCategory(const string& cat) { category = cat; }
    void setDescription(const string& desc) { description = desc; }
    void setPrice(double p) { price = p; }
    void setRentalPrice(double rp) { rentalPrice = rp; }
    void setCondition(const string& cond) { condition = cond; }
    void setListingType(const string& lType) { listingType = lType; }
    void setAvailabilityStatus(const string& status) { availabilityStatus = status; }
    void setRating(double r) { rating = r; }
    void setPriorityScore(double score) { priorityScore = score; }

    void display() const {
        cout << "----------------------------------------\n";
        cout << "Item ID     : " << itemId << "\n";
        cout << "Name        : " << name << "\n";
        cout << "Category    : " << category << "\n";
        cout << "Type        : " << listingType << "\n";
        cout << "Condition   : " << condition << "\n";
        cout << "Price       : Rs. " << fixed << setprecision(2) << price;
        if (rentalPrice > 0) cout << " (Rent: Rs. " << rentalPrice << "/day)";
        cout << "\nRating      : " << rating << "/5.0\n";
        cout << "Status      : " << availabilityStatus << "\n";
        cout << "Seller ID   : " << sellerId << "\n";
        cout << "Description : " << description << "\n";
        cout << "----------------------------------------\n";
    }

    string serialize() const {
        stringstream ss;
        ss << itemId << "|" << sellerId << "|" << name << "|" << category << "|"
           << description << "|" << price << "|" << rentalPrice << "|" << condition << "|"
           << listingType << "|" << availabilityStatus << "|" << rating << "|" << priorityScore << "|" << timestamp;
        return ss.str();
    }

    static Item deserialize(const string& line) {
        stringstream ss(line);
        string segment;
        vector<string> parts;

        while (getline(ss, segment, '|')) {
            parts.push_back(segment);
        }

        if (parts.size() < 13) return Item();

        int id = stoi(parts[0]);
        string sId = parts[1];
        string n = parts[2];
        string cat = parts[3];
        string desc = parts[4];
        double p = stod(parts[5]);
        double rp = stod(parts[6]);
        string cond = parts[7];
        string lType = parts[8];
        string status = parts[9];
        double rat = stod(parts[10]);
        double score = stod(parts[11]);
        long long time = stoll(parts[12]);

        Item item(id, sId, n, cat, desc, p, rp, cond, lType, status, rat, time);
        item.setPriorityScore(score);
        return item;
    }
};

// ==================== FILE MANAGER ====================
class FileManager {
public:
    static void saveUsers(const vector<User>& users) {
        ofstream file("students.dat");
        for (const auto& u : users) {
            file << u.serialize() << "\n";
        }
        file.close();
    }

    static vector<User> loadUsers() {
        vector<User> users;
        ifstream file("students.dat");
        if (!file.is_open()) return users;

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                users.push_back(User::deserialize(line));
            }
        }
        file.close();
        return users;
    }

    static void saveItems(const vector<Item>& items) {
        ofstream file("items.dat");
        for (const auto& item : items) {
            file << item.serialize() << "\n";
        }
        file.close();
    }

    static vector<Item> loadItems() {
        vector<Item> items;
        ifstream file("items.dat");
        if (!file.is_open()) return items;

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                items.push_back(Item::deserialize(line));
            }
        }
        file.close();
        return items;
    }
};

// ==================== ADMIN MANAGER ====================
class AdminManager {
public:
    static void showAnalytics(const vector<User>& users, const vector<Item>& items) {
        cout << "\n========================================\n";
        cout << "         ADMIN ANALYTICS DASHBOARD      \n";
        cout << "========================================\n";
        cout << "Total registered students : " << users.size() << "\n";
        cout << "Total items listed        : " << items.size() << "\n";

        int activeCount = 0, reservedCount = 0;
        for (const auto& item : items) {
            if (item.getAvailabilityStatus() == "Available") {
                activeCount++;
            } else {
                reservedCount++;
            }
        }
        cout << "Active items available    : " << activeCount << "\n";
        cout << "Reserved / Sold items     : " << reservedCount << "\n";

        cout << "\n--- Category-wise Breakdown ---\n";
        vector<string> categories;
        vector<int> counts;
        for (const auto& item : items) {
            string cat = item.getCategory();
            bool found = false;
            for (size_t i = 0; i < categories.size(); ++i) {
                if (categories[i] == cat) {
                    counts[i]++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                categories.push_back(cat);
                counts.push_back(1);
            }
        }

        if (categories.empty()) {
            cout << "No categories found yet.\n";
        } else {
            for (size_t i = 0; i < categories.size(); ++i) {
                cout << categories[i] << " : " << counts[i] << " item(s)\n";
            }
        }
        cout << "========================================\n";
    }
};

#endif