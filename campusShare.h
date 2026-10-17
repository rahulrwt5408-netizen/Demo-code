#ifndef CAMPUS_SHARE_H
#define CAMPUS_SHARE_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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
    int wishlist[20]; // Fixed array instead of vector
    int wishlistCount;

public:
    User() : wishlistCount(0) {
        for (int i = 0; i < 20; ++i) wishlist[i] = 0;
    }

    User(string sId, string n, string g, string pass, string r)
        : studentId(sId), name(n), gmail(g), passwordHash(hashPassword(pass)), role(r), wishlistCount(0) {
        for (int i = 0; i < 20; ++i) wishlist[i] = 0;
    }
    
    virtual ~User() {}

    string getStudentId() const { return studentId; }
    string getName() const { return name; }
    string getGmail() const { return gmail; }
    string getPasswordHash() const { return passwordHash; }
    string getRole() const { return role; }
    
    int getWishlistCount() const { return wishlistCount; }
    int getWishlistElement(int index) const { return wishlist[index]; }

    void setPasswordHash(const string& hash) { passwordHash = hash; }
    void setRole(const string& r) { role = r; }

    void addToWishlist(int itemId) {
        for (int i = 0; i < wishlistCount; ++i) {
            if (wishlist[i] == itemId) return;
        }
        if (wishlistCount < 20) {
            wishlist[wishlistCount++] = itemId;
        }
    }

    void removeFromWishlist(int itemId) {
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
    static string hashPassword(const string& password) {
        unsigned long long hashVal = 0;
        for (char c : password) {
            hashVal = (hashVal * 31) + c;
        }
        return to_string(hashVal);
    }

    virtual string serialize() const {
        string wishStr = "";
        for (int i = 0; i < wishlistCount; ++i) {
            wishStr += to_string(wishlist[i]);
            if (i + 1 < wishlistCount) wishStr += ",";
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
                    u.addToWishlist(stoi(itemStr));
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
        string parts[13];
        int count = 0;

        while (getline(ss, segment, '|') && count < 13) {
            parts[count++] = segment;
        }

        if (count < 13) return Item();

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
    static void saveUsers(User users[], int userCount) {
        ofstream file("students.dat");
        for (int i = 0; i < userCount; ++i) {
            file << users[i].serialize() << "\n";
        }
        file.close();
    }

    static int loadUsers(User users[]) {
        ifstream file("students.dat");
        if (!file.is_open()) return 0;

        string line;
        int count = 0;
        while (getline(file, line) && count < 100) {
            if (!line.empty()) {
                users[count++] = User::deserialize(line);
            }
        }
        file.close();
        return count;
    }

    static void saveItems(Item items[], int itemCount) {
        ofstream file("items.dat");
        for (int i = 0; i < itemCount; ++i) {
            file << items[i].serialize() << "\n";
        }
        file.close();
    }

    static int loadItems(Item items[]) {
        ifstream file("items.dat");
        if (!file.is_open()) return 0;

        string line;
        int count = 0;
        while (getline(file, line) && count < 100) {
            if (!line.empty()) {
                items[count++] = Item::deserialize(line);
            }
        }
        file.close();
        return count;
    }
};

// ==================== ADMIN MANAGER ====================
class AdminManager {
public:
    static void showAnalytics(User users[], int userCount, Item items[], int itemCount) {
        cout << "\n========================================\n";
        cout << "         ADMIN ANALYTICS DASHBOARD       \n";
        cout << "========================================\n";
        cout << "Total registered students : " << userCount << "\n";
        cout << "Total items listed        : " << itemCount << "\n";

        int activeCount = 0, reservedCount = 0;
        for (int i = 0; i < itemCount; ++i) {
            if (items[i].getAvailabilityStatus() == "Available") {
                activeCount++;
            } else {
                reservedCount++;
            }
        }
        cout << "Active items available    : " << activeCount << "\n";
        cout << "Reserved / Sold items     : " << reservedCount << "\n";

        cout << "\n--- Category-wise Breakdown ---\n";
        string categories[100];
        int counts[100];
        int catCount = 0;

        for (int i = 0; i < itemCount; ++i) {
            string cat = items[i].getCategory();
            bool found = false;
            for (int j = 0; j < catCount; ++j) {
                if (categories[j] == cat) {
                    counts[j]++;
                    found = true;
                    break;
                }
            }
            if (!found && catCount < 100) {
                categories[catCount] = cat;
                counts[catCount] = 1;
                catCount++;
            }
        }

        if (catCount == 0) {
            cout << "No categories found yet.\n";
        } else {
            for (int i = 0; i < catCount; ++i) {
                cout << categories[i] << " : " << counts[i] << " item(s)\n";
            }
        }
        cout << "========================================\n";
    }
};

#endif