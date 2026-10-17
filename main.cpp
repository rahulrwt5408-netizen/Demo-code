#include "campusShare.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    // Load existing data from files at startup using fixed arrays
    User users[100];
    Item items[100];
    int userCount = FileManager::loadUsers(users);
    int itemCount = FileManager::loadItems(items);

    int portalChoice;
    do {
        cout << "\n========================================\n";
        cout << "       CAMPUS SHARE - WELCOME PORTAL     \n";
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
                cout << "         STUDENT AUTHENTICATION          \n";
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
                    getline(cin, name);
                    cout << "Enter Gmail: ";
                    cin >> gmail;
                    cout << "Enter Password: ";
                    cin >> pass;
                    cout << "Enter Role (Buyer/Seller): ";
                    cin >> role;

                    if (userCount < 100) {
                        users[userCount++] = User(sId, name, gmail, pass, role);
                        FileManager::saveUsers(users, userCount);
                        cout << "Registration successful! Please login now.\n";
                    } else {
                        cout << "Error: User limit reached!\n";
                    }

                } else if (authChoice == 2) {
                    string gmail, pass;
                    cout << "Enter Gmail: ";
                    cin >> gmail;
                    cout << "Enter Password: ";
                    cin >> pass;

                    int loggedInIndex = -1;
                    string hashedPassword = User::hashPassword(pass);
                    
                    for (int i = 0; i < userCount; ++i) {
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
                            cout << "     STUDENT DASHBOARD (" << u.getName() << ")    \n";
                            cout << "----------------------------------------\n";
                            cout << "1. Explore Available Listings\n";
                            cout << "2. Add Item Listing (Seller)\n";
                            cout << "3. Campus Navigation (BFS)\n";
                            cout << "4. Search Items\n";
                            cout << "5. Rate / Buy Item (Buyer)\n";
                            cout << "6. View Wishlist\n";
                            cout << "7. Add to Wishlist\n";
                            cout << "8. Logout\n";
                            cout << "Enter your choice: ";
                            cin >> studentChoice;

                            switch (studentChoice) {
                                case 1: {
                                    if (itemCount == 0) {
                                        cout << "No items listed yet.\n";
                                    } else {
                                        for (int i = 0; i < itemCount; ++i) {
                                            items[i].display();
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

                                    if (itemCount < 100) {
                                        items[itemCount++] = Item(id, sId, name, cat, desc, price, rPrice, cond, "Sell", "Available", 0.0, time(0));
                                        FileManager::saveItems(items, itemCount);
                                        cout << "Item listed successfully!\n";
                                    } else {
                                        cout << "Error: Item limit reached!\n";
                                    }
                                    break;
                                }
                                case 3: {
                                    cout << "\n========================================\n";
                                    cout << "      CAMPUS GRAPH NAVIGATION (BFS)      \n";
                                    cout << "========================================\n";
                                    
                                    string spots[6] = {"Gate 1", "Auditorium", "CS Block", "Library", "Cafeteria", "Admin Block"};
                                    // Adjacency matrix for 6 campus locations
                                    int graph[6][6] = {
                                        {0, 1, 0, 0, 0, 1}, // Gate 1
                                        {1, 0, 1, 0, 0, 0}, // Auditorium
                                        {0, 1, 0, 1, 0, 0}, // CS Block
                                        {0, 0, 1, 0, 1, 1}, // Library
                                        {0, 0, 0, 1, 0, 0}, // Cafeteria
                                        {1, 0, 0, 1, 0, 0}  // Admin Block
                                    };

                                    cout << "Available Campus Spots:\n";
                                    for(int i = 0; i < 6; ++i) {
                                        cout << "- " << spots[i] << "\n";
                                    }
                                    cout << "\n";

                                    string start, dest;
                                    cout << "Enter Starting Location: ";
                                    cin.ignore();
                                    getline(cin, start);
                                    cout << "Enter Destination: ";
                                    getline(cin, dest);

                                    int startIdx = -1, destIdx = -1;
                                    for(int i = 0; i < 6; ++i) {
                                        if(spots[i] == start) startIdx = i;
                                        if(spots[i] == dest) destIdx = i;
                                    }

                                    if (startIdx == -1 || destIdx == -1) {
                                        cout << "Invalid location entered! Please choose from the available spots.\n";
                                        break;
                                    }

                                    // Array-based Queue for BFS traversal
                                    int q[10];
                                    int front = 0, rear = 0;
                                    bool visited[6] = {false};
                                    int parent[6];
                                    for(int i = 0; i < 6; ++i) parent[i] = -1;

                                    q[rear++] = startIdx;
                                    visited[startIdx] = true;

                                    bool found = false;
                                    while (front < rear) {
                                        int curr = q[front++];

                                        if (curr == destIdx) {
                                            found = true;
                                            break;
                                        }

                                        for (int next = 0; next < 6; ++next) {
                                            if (graph[curr][next] == 1 && !visited[next]) {
                                                visited[next] = true;
                                                parent[next] = curr;
                                                q[rear++] = next;
                                            }
                                        }
                                    }

                                    if (found) {
                                        int path[6];
                                        int pathLen = 0;
                                        int curr = destIdx;
                                        while (curr != -1) {
                                            path[pathLen++] = curr;
                                            curr = parent[curr];
                                        }

                                        cout << "\n[Shortest Route Found via Graph BFS]\nPath: ";
                                        for (int i = pathLen - 1; i >= 0; --i) {
                                            cout << spots[path[i]];
                                            if (i > 0) cout << " -> ";
                                        }
                                        cout << "\n";
                                    } else {
                                        cout << "No path found between " << start << " and " << dest << "!\n";
                                    }
                                    break;
                                }
                                case 4: {
                                    if (itemCount == 0) {
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
                                        for (int i = 0; i < itemCount; ++i) {
                                            if (items[i].getName().find(query) != string::npos) {
                                                items[i].display();
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
                                        for (int i = 0; i < itemCount; ++i) {
                                            if (items[i].getCategory().find(catQuery) != string::npos) {
                                                items[i].display();
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
                                    int count = u.getWishlistCount();
                                    if (count == 0) {
                                        cout << "Your wishlist is empty.\n";
                                    } else {
                                        cout << "\n========================================\n";
                                        cout << "             YOUR WISHLIST              \n";
                                        cout << "========================================\n";
                                        for (int i = 0; i < count; ++i) {
                                            int wId = u.getWishlistElement(i);
                                            bool found = false;
                                            for (int j = 0; j < itemCount; ++j) {
                                                if (items[j].getItemId() == wId) {
                                                    items[j].display();
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
                                    for (int i = 0; i < itemCount; ++i) {
                                        if (items[i].getItemId() == wId) {
                                            itemExists = true;
                                            break;
                                        }
                                    }

                                    if (itemExists) {
                                        u.addToWishlist(wId);
                                        FileManager::saveUsers(users, userCount);
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
                AdminManager::showAnalytics(users, userCount, items, itemCount);
            } else {
                cout << "Error: Incorrect admin passcode! Access Denied.\n";
            }

        } else if (portalChoice == 3) {
            cout << "Exiting Campus Share. Saving data to files...\n";
            FileManager::saveUsers(users, userCount);
            FileManager::saveItems(items, itemCount);
            break;
        } else {
            cout << "Invalid selection! Please choose 1, 2, or 3.\n";
        }

    } while (portalChoice != 3);

    return 0;
}