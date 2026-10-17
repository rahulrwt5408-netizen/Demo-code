#include "campusShare.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Item::Item() : itemId(0), price(0.0), rentalPrice(0.0), rating(0.0), priorityScore(0.0), timestamp(0) {}

Item::Item(int id, string sId, string n, string cat, string desc, 
           double p, double rp, string cond, string lType, string status, double rat, long long time)
    : itemId(id), sellerId(sId), name(n), category(cat), description(desc), 
      price(p), rentalPrice(rp), condition(cond), listingType(lType), 
      availabilityStatus(status), rating(rat), timestamp(time) {}

// Getters
int Item::getItemId() const { return itemId; }
string Item::getSellerId() const { return sellerId; }
string Item::getName() const { return name; }
string Item::getCategory() const { return category; }
string Item::getDescription() const { return description; }
double Item::getPrice() const { return price; }
double Item::getRentalPrice() const { return rentalPrice; }
string Item::getCondition() const { return condition; }
string Item::getListingType() const { return listingType; }
string Item::getAvailabilityStatus() const { return availabilityStatus; }
double Item::getRating() const { return rating; }
double Item::getPriorityScore() const { return priorityScore; }
long long Item::getTimestamp() const { return timestamp; }

// Setters
void Item::setName(const string& n) { name = n; }
void Item::setCategory(const string& cat) { category = cat; }
void Item::setDescription(const string& desc) { description = desc; }
void Item::setPrice(double p) { price = p; }
void Item::setRentalPrice(double rp) { rentalPrice = rp; }
void Item::setCondition(const string& cond) { condition = cond; }
void Item::setListingType(const string& lType) { listingType = lType; }
void Item::setAvailabilityStatus(const string& status) { availabilityStatus = status; }
void Item::setRating(double r) { rating = r; }
void Item::setPriorityScore(double score) { priorityScore = score; }

void Item::display() const {
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

string Item::serialize() const {
    stringstream ss;
    ss << itemId << "|" << sellerId << "|" << name << "|" << category << "|"
       << description << "|" << price << "|" << rentalPrice << "|" << condition << "|"
       << listingType << "|" << availabilityStatus << "|" << rating << "|" << priorityScore << "|" << timestamp;
    return ss.str();
}

Item Item::deserialize(const string& line) {
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