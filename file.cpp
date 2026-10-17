#include "campusShare.h"
#include <fstream>
#include <sstream>

using namespace std;

void FileManager::saveUsers(User users[], int userCount) {
    ofstream file("students.dat");
    for (int i = 0; i < userCount; ++i) {
        file << users[i].serialize() << "\n";
    }
    file.close();
}

int FileManager::loadUsers(User users[]) {
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

void FileManager::saveItems(Item items[], int itemCount) {
    ofstream file("items.dat");
    for (int i = 0; i < itemCount; ++i) {
        file << items[i].serialize() << "\n";
    }
    file.close();
}

int FileManager::loadItems(Item items[]) {
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