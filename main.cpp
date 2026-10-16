#include "campusShare.h"
#include <iostream>
#include <vector>
#include <ctime>
#include<unordered_map>
#include<queue>
#include <algorithm>

using namespace std;

int main() {
    // Load existing data from files at startup
    vector<User> users = FileManager::loadUsers();
    vector<Item> items = FileManager::loadItems();

    int portalChoice;
    do {
        cout << "\n========================================\n";
        cout << "       CAMPUS SHARE - WELCOME PORTAL    \n";
        cout << "========================================\n";
        cout << "1. Student / User Portal\n";
        cout << "2. Admin Portal (Analytics & Control)\n";
        cout << "3. Exit & Save\n";
        cout << "Select Portal: ";
        cin >> portalChoice;

        if (portalChoice == 1) {
            int authChoice;
            do {
                cout << "\n----------------------------------------\n";
                cout << "         STUDENT AUTHENTICATION         \n";
                cout << "----------------------------------------\n";
                cout << "1. Register Student\n";
                cout << "2. Login\n";
                cout << "3. Back to Main Portal\n";
                cout << "Enter your choice: ";
                cin >> authChoice;

                if (authChoice == 1) {
                    string sId, name, gmail, pass, role;
                    cout << "Enter Student ID: ";
                    cin >> sId;
                    cout << "Enter Name: ";
                    cin.ignore();
                    getline(cin,name);
                    cout << "Enter Gmail: ";
                    cin >> gmail;
                    cout << "Enter Password: ";
                    cin >> pass;
                    cout << "Enter Role (Buyer/Seller): ";
                    cin >> role;

                    User newUser(sId, name, gmail, pass, role);
                    users.push_back(newUser);
                    FileManager::saveUsers(users);
                    cout << "Registration successful! Please login now.\n";

                } else if (authChoice == 2) {
                    string gmail, pass;
                    cout << "Enter Gmail: ";
                    cin >> gmail;
                    cout << "Enter Password: ";
                    cin >> pass;

                    int loggedInIndex = -1;
                    string hashedPassword = User::hashPassword(pass);
                    
                    for (size_t i = 0; i < users.size(); ++i) {
                        if (users[i].getGmail() == gmail && users[i].getPasswordHash() == hashedPassword) {
                            loggedInIndex = i;
                            break;
                        }
                    }

                    if (loggedInIndex != -1) {
                        User& u = users[loggedInIndex];
                        cout << "Login successful! Welcome, " << u.getName() << " (" << u.getRole() << ")\n";

                        int studentChoice;
                        do {
                            cout << "\n----------------------------------------\n";
                            cout << "      STUDENT DASHBOARD (" << u.getName() << ")    \n";
                            cout << "----------------------------------------\n";
                            cout << "1. Explore Available Listings\n";
                            cout << "2. Add Item Listing (Seller)\n";
                            cout << "3. Campus Navigation\n";
                            cout << "4. Search Items\n";
                            cout << "5. Rate / Buy Item (Buyer)\n";
                            cout << "6. View Wishlist\n";
                            cout << "7. Add to Wishlist\n";
                            cout << "8. Logout\n";
                            cout << "Enter your choice: ";
                            cin >> studentChoice;

                            switch (studentChoice) {
                                case 1: {
                                    if (items.empty()) {
                                        cout << "No items listed yet.\n";
                                    } else {
                                        for (const auto& item : items) {
                                            item.display();
                                        }
                                    }
                                    break;
                                }
                                case 2: {
                                    int id;
                                    string sId, name, cat, desc, cond;
                                    double price, rPrice;
                                    cout << "Enter Item ID (number): ";
                                    cin >> id;
                                    cout << "Enter Seller Student ID: ";
                                    cin >> sId;
                                    cin.ignore();
                                    cout << "Enter Item Name: ";
                                    getline(cin, name);
                                    cout << "Enter Category: ";
                                    getline(cin, cat);
                                    cout << "Enter Description: ";
                                    getline(cin, desc);
                                    cout << "Enter Price: ";
                                    cin >> price;
                                    cout << "Enter Rental Price (0 if not for rent): ";
                                    cin >> rPrice;
                                    cout << "Enter Condition (New/Used): ";
                                    cin >> cond;

                                    Item newItem(id, sId, name, cat, desc, price, rPrice, cond, "Sell", "Available", 0.0, time(0));
                                    items.push_back(newItem);
                                    FileManager::saveItems(items);
                                    cout << "Item listed successfully!\n";
                                    break;
                                }
                                case 3: {
    cout << "\n========================================\n";
    cout << "      CAMPUS GRAPH NAVIGATION (BFS)     \n";
    cout << "========================================\n";
    
    // Graph representation using Adjacency List (unordered_map + vector of pairs)
    unordered_map<string, vector<pair<string, int>>> campusGraph;
    
    campusGraph["Gate 1"] = {{"Auditorium", 100}, {"Admin Block", 150}};
    campusGraph["Auditorium"] = {{"Gate 1", 100}, {"CS Block", 50}};
    campusGraph["CS Block"] = {{"Auditorium", 50}, {"Library", 80}};
    campusGraph["Library"] = {{"CS Block", 80}, {"Cafeteria", 120}, {"Admin Block", 60}};
    campusGraph["Cafeteria"] = {{"Library", 120}};
    campusGraph["Admin Block"] = {{"Gate 1", 150}, {"Library", 60}};

    cout << "Available Campus Spots:\n";
    cout << "- Gate 1\n- Auditorium\n- CS Block\n- Library\n- Cafeteria\n- Admin Block\n\n";

    string start, dest;
    cout << "Enter Starting Location: ";
    cin.ignore();
    getline(cin, start);
    cout << "Enter Destination: ";
    getline(cin, dest);

    if (campusGraph.find(start) == campusGraph.end() || campusGraph.find(dest) == campusGraph.end()) {
        cout << "Invalid location entered! Please choose from the available spots.\n";
        break;
    }

    // BFS Algorithm for shortest path traversal
    unordered_map<string, string> parent;
    unordered_map<string, bool> visited;
    queue<string> q;

    q.push(start);
    visited[start] = true;
    parent[start] = "";

    bool found = false;
    while (!q.empty()) {
        string curr = q.front();
        q.pop();

        if (curr == dest) {
            found = true;
            break;
        }

        for (auto& neighbor : campusGraph[curr]) {
            string nextLoc = neighbor.first;
            if (!visited[nextLoc]) {
                visited[nextLoc] = true;
                parent[nextLoc] = curr;
                q.push(nextLoc);
            }
        }
    }

    if (found) {
        vector<string> path;
        string curr = dest;
        while (curr != "") {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        cout << "\n[Shortest Route Found via Graph BFS]\nPath: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i + 1 < path.size()) cout << " -> ";
        }
        cout << "\n";
    } else {
        cout << "No path found between " << start << " and " << dest << "!\n";
    }
    break;
}
                                case 4: {
                                    if (items.empty()) {
                                        cout << "No items available to search.\n";
                                        break;
                                    }

                                    int searchChoice;
                                    cout << "\n--- SEARCH ITEMS ---\n";
                                    cout << "1. Search by Name\n";
                                    cout << "2. Search by Category\n";
                                    cout << "Enter your choice: ";
                                    cin >> searchChoice;

                                    cin.ignore();
                                    if (searchChoice == 1) {
                                        string query;
                                        cout << "Enter item name to search: ";
                                        getline(cin, query);

                                        bool matchFound = false;
                                        cout << "\nSearch Results:\n";
                                        for (const auto& item : items) {
                                            if (item.getName().find(query) != string::npos) {
                                                item.display();
                                                matchFound = true;
                                            }
                                        }
                                        if (!matchFound) {
                                            cout << "No items found matching \"" << query << "\".\n";
                                        }
                                    } else if (searchChoice == 2) {
                                        string catQuery;
                                        cout << "Enter category (e.g., Books, Electronics): ";
                                        getline(cin, catQuery);

                                        bool matchFound = false;
                                        cout << "\nSearch Results:\n";
                                        for (const auto& item : items) {
                                            if (item.getCategory().find(catQuery) != string::npos) {
                                                item.display();
                                                matchFound = true;
                                            }
                                        }
                                        if (!matchFound) {
                                            cout << "No items found in category \"" << catQuery << "\".\n";
                                        }
                                    } else {
                                        cout << "Invalid search option!\n";
                                    }
                                    break;
                                }
                                case 5: {
                                    cout << "Buyer transaction module running...\n";
                                    break;
                                }
                                case 6: {
                                    vector<int> userWishlist = u.getWishlist();
                                    if (userWishlist.empty()) {
                                        cout << "Your wishlist is empty.\n";
                                    } else {
                                        cout << "\n========================================\n";
                                        cout << "            YOUR WISHLIST               \n";
                                        cout << "========================================\n";
                                        for (int wId : userWishlist) {
                                            bool found = false;
                                            for (const auto& item : items) {
                                                if (item.getItemId() == wId) {
                                                    item.display();
                                                    found = true;
                                                    break;
                                                }
                                            }
                                            if (!found) {
                                                cout << "Item ID " << wId << " is no longer available.\n";
                                            }
                                        }
                                    }
                                    break;
                                }
                                case 7: {
                                    int wId;
                                    cout << "Enter Item ID to add to wishlist: ";
                                    cin >> wId;

                                    bool itemExists = false;
                                    for (const auto& item : items) {
                                        if (item.getItemId() == wId) {
                                            itemExists = true;
                                            break;
                                        }
                                    }

                                    if (itemExists) {
                                        u.addToWishlist(wId);
                                        FileManager::saveUsers(users);
                                        cout << "Item added to your wishlist successfully!\n";
                                    } else {
                                        cout << "Error: Item ID not found!\n";
                                    }
                                    break;
                                }
                                case 8: {
                                    cout << "Logging out...\n";
                                    break;
                                }
                                default:
                                    cout << "Invalid choice! Try again.\n";
                            }
                        } while (studentChoice != 8);

                    } else {
                        cout << "Invalid credentials or user not found!\n";
                    }

                } else if (authChoice == 3) {
                    cout << "Returning to Main Portal...\n";
                } else {
                    cout << "Invalid choice! Try again.\n";
                }
            } while (authChoice != 3);

        } else if (portalChoice == 2) {
            string adminPass;
            cout << "\n--- ADMIN GATEWAY ---\n";
            cout << "Enter Admin Passcode: ";
            cin >> adminPass;
            if (adminPass == "admin123") {
                AdminManager::showAnalytics(users, items);
            } else {
                cout << "Error: Incorrect admin passcode! Access Denied.\n";
            }

        } else if (portalChoice == 3) {
            cout << "Exiting Campus Share. Saving data to files...\n";
            FileManager::saveUsers(users);
            FileManager::saveItems(items);
            break;
        } else {
            cout << "Invalid selection! Please choose 1, 2, or 3.\n";
        }

    } while (portalChoice != 3);

    return 0;
}