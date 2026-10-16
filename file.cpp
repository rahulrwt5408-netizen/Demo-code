#include "campusShare.h"
#include <fstream>
#include <sstream>

using namespace std;

void FileManager::saveUsers(const vector<User>& users) {
    ofstream file("students.dat");
    for (const auto& u : users) {
        file << u.serialize() << "\n";
    }
    file.close();
}

vector<User> FileManager::loadUsers() {
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

void FileManager::saveItems(const vector<Item>& items) {
    ofstream file("items.dat");
    for (const auto& item : items) {
        file << item.serialize() << "\n";
    }
    file.close();
}

vector<Item> FileManager::loadItems() {
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