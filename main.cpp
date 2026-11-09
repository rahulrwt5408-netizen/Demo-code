#include "campusShare.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

using namespace std;

// Quick Sort 
int partition(Item items[], int low, int high) {
    double pivot = items[high].getPrice();
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; ++j) {
        if (items[j].getPrice() <= pivot) {
            i++;
            Item temp = items[i];
            items[i] = items[j];
            items[j] = temp;
        }
    }
    Item temp = items[i + 1];
    items[i + 1] = items[high];
    items[high] = temp;
    return (i + 1);
}

void quickSortByPrice(Item items[], int low, int high) {
    if (low < high) {
        int pi = partition(items, low, high);
        quickSortByPrice(items, low, pi - 1);
        quickSortByPrice(items, pi + 1, high);
    }
}

int main() {
    // Load existing data from files at startup using fixed arrays
    User users[100];
    Item items[100];
    int userCount = FileManager::loadUsers(users);
    int itemCount = FileManager::loadItems(items);

    int portalChoice;
    do {
        cout << "\n========================================\n";
        cout << "        CAMPUS SHARE - WELCOME PORTAL     \n";
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
                cout << "          STUDENT AUTHENTICATION          \n";
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
                    cout << "Enter Role (Buyer/Seller/Both): ";
                    cin >> role;

                    // Check if Student ID or Gmail already exists
                    bool alreadyExists = false;
                    for (int i = 0; i < userCount; ++i) {
                        if (users[i].getGmail() == gmail || users[i].getStudentId() == sId) {
                            alreadyExists = true;
                            break;
                        }
                    }

                    if (alreadyExists) {
                        cout << "Error: An account with this Student ID or Gmail already exists!\n";
                    } else if (gmail.find('@') == string::npos || name.length() < 2 || sId.length() < 3) {
                        cout << "Error: Invalid registration details! Please enter a valid email and name.\n";
                    } else if (userCount < 100) {
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
                            cout << "3. Delete My Item Listing (Seller)\n";
                            cout << "4. Campus Navigation (BFS)\n";
                            cout << "5. Search Items (with Quick Sort)\n";
                            cout << "6. Rate / Buy Item (Buyer & Invoice)\n";
                            cout << "7. View Wishlist (Sorted by Price)\n";
                            cout << "8. Add to Wishlist\n";
                            cout << "9. Delete My Account\n";
                            cout << "10. Logout\n";
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
                                    cout << "\n--- DELETE MY ITEM LISTING ---\n";
                                    int delId;
                                    cout << "Enter Item ID of the item you want to delete: ";
                                    cin >> delId;

                                    int foundIndex = -1;
                                    for (int i = 0; i < itemCount; ++i) {
                                        if (items[i].getItemId() == delId) {
                                            if (items[i].getSellerId() == u.getStudentId()) {
                                                foundIndex = i;
                                            } else {
                                                cout << "Error: You can only delete items that you listed!\n";
                                                foundIndex = -2;
                                            }
                                            break;
                                        }
                                    }

                                    if (foundIndex >= 0) {
                                        for (int i = foundIndex; i < itemCount - 1; ++i) {
                                            items[i] = items[i + 1];
                                        }
                                        itemCount--;
                                        FileManager::saveItems(items, itemCount);
                                        cout << "Item ID " << delId << " deleted successfully!\n";
                                    } else if (foundIndex == -1) {
                                        cout << "Error: Item ID not found!\n";
                                    }
                                    break;
                                }
                                case 4: {
                                    cout << "\n========================================\n";
                                    cout << "      CAMPUS GRAPH NAVIGATION (BFS)     \n";
                                    cout << "========================================\n";
                                    
                                    string spots[6] = {"Gate 1", "Auditorium", "CS Block", "Library", "Cafeteria", "Admin Block"};
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
                                        cout << "Invalid location entered! Please choose from available spots.\n";
                                        break;
                                    }

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
                                case 5: {
                                    if (itemCount == 0) {
                                        cout << "No items available to search.\n";
                                        break;
                                    }

                                    int searchChoice;
                                    cout << "\n--- SEARCH & SORT ITEMS ---\n";
                                    cout << "1. Search by Name\n";
                                    cout << "2. Search by Category\n";
                                    cout << "Enter your choice: ";
                                    cin >> searchChoice;

                                    cin.ignore();
                                    string query;
                                    if (searchChoice == 1) {
                                        cout << "Enter item name to search: ";
                                        getline(cin, query);
                                    } else if (searchChoice == 2) {
                                        cout << "Enter category (e.g., Books, Electronics): ";
                                        getline(cin, query);
                                    } else {
                                        cout << "Invalid search option!\n";
                                        break;
                                    }

                                    Item matchedItems[100];
                                    int matchCount = 0;

                                    for (int i = 0; i < itemCount; ++i) {
                                        bool isMatch = false;
                                        if (searchChoice == 1 && items[i].getName().find(query) != string::npos) {
                                            isMatch = true;
                                        } else if (searchChoice == 2 && items[i].getCategory().find(query) != string::npos) {
                                            isMatch = true;
                                        }

                                        if (isMatch && matchCount < 100) {
                                            matchedItems[matchCount++] = items[i];
                                        }
                                    }

                                    if (matchCount == 0) {
                                        cout << "No items found matching \"" << query << "\".\n";
                                    } else {
                                        if (matchCount > 1) {
                                            quickSortByPrice(matchedItems, 0, matchCount - 1);
                                        }

                                        cout << "\n[Search Results Sorted by Price via Quick Sort (" << matchCount << " found)]:\n";
                                        for (int i = 0; i < matchCount; ++i) {
                                            matchedItems[i].display();
                                        }
                                    }
                                    break;
                                }
                                case 6: {
                                    if (itemCount == 0) {
                                        cout << "No items available to purchase.\n";
                                        break;
                                    }

                                    cout << "\n========================================\n";
                                    cout << "          BUY ITEM & CHECKOUT           \n";
                                    cout << "========================================\n";
                                    
                                    for (int i = 0; i < itemCount; ++i) {
                                        if (items[i].getAvailabilityStatus() == "Available") {
                                            cout << "ID: " << items[i].getItemId() 
                                                 << " | Name: " << items[i].getName() 
                                                 << " | Price: Rs. " << items[i].getPrice() << "\n";
                                        }
                                    }

                                    int buyId;
                                    cout << "\nEnter Item ID you want to purchase: ";
                                    cin >> buyId;

                                    int itemIndex = -1;
                                    for (int i = 0; i < itemCount; ++i) {
                                        if (items[i].getItemId() == buyId && items[i].getAvailabilityStatus() == "Available") {
                                            itemIndex = i;
                                            break;
                                        }
                                    }

                                    if (itemIndex == -1) {
                                        cout << "Error: Invalid Item ID or item is already sold/reserved!\n";
                                    } else {
                                        items[itemIndex].setAvailabilityStatus("Sold");
                                        FileManager::saveItems(items, itemCount);

                                        time_t now = time(0);
                                        char* dt = ctime(&now);
                                        long long invoiceNo = 100000 + (now % 900000);

                                        cout << "\n\n";
                                        cout << "==================================================\n";
                                        cout << "              CAMPUS SHARE - INVOICE              \n";
                                        cout << "==================================================\n";
                                        cout << " Invoice No   : INV-" << invoiceNo << "\n";
                                        cout << " Date & Time  : " << dt;
                                        cout << "--------------------------------------------------\n";
                                        cout << " BUYER DETAILS:\n";
                                        cout << " Student Name : " << u.getName() << "\n";
                                        cout << " Student ID   : " << u.getStudentId() << "\n";
                                        cout << " Email        : " << u.getGmail() << "\n";
                                        cout << "--------------------------------------------------\n";
                                        cout << " ITEM PURCHASED:\n";
                                        cout << " Item ID      : " << items[itemIndex].getItemId() << "\n";
                                        cout << " Item Name    : " << items[itemIndex].getName() << "\n";
                                        cout << " Category     : " << items[itemIndex].getCategory() << "\n";
                                        cout << " Condition    : " << items[itemIndex].getCondition() << "\n";
                                        cout << " Seller ID    : " << items[itemIndex].getSellerId() << "\n";
                                        cout << "--------------------------------------------------\n";
                                        cout << " TOTAL AMOUNT : Rs. " << fixed << setprecision(2) << items[itemIndex].getPrice() << "\n";
                                        cout << " PAYMENT MODE : Campus P2P Verified / COD\n";
                                        cout << " STATUS       : SUCCESSFUL\n";
                                        cout << "==================================================\n";
                                        cout << " Thank you for using Campus Share!\n";
                                        cout << "==================================================\n\n";

                                        ofstream invoiceLog("invoice_history.txt", ios::app);
                                        if (invoiceLog.is_open()) {
                                            invoiceLog << "INV-" << invoiceNo << " | Buyer: " << u.getStudentId() 
                                                       << " | Item: " << items[itemIndex].getItemId() 
                                                       << " | Price: " << items[itemIndex].getPrice() << "\n";
                                            invoiceLog.close();
                                        }
                                    }
                                    break;
                                }
                                case 7: {
                                    int count = u.getWishlistCount();
                                    if (count == 0) {
                                        cout << "Your wishlist is empty.\n";
                                    } else {
                                        Item wishlistItems[20];
                                        int validWishCount = 0;

                                        for (int i = 0; i < count; ++i) {
                                            int wId = u.getWishlistElement(i);
                                            for (int j = 0; j < itemCount; ++j) {
                                                if (items[j].getItemId() == wId) {
                                                    wishlistItems[validWishCount++] = items[j];
                                                    break;
                                                }
                                            }
                                        }

                                        if (validWishCount > 0) {
                                            if (validWishCount > 1) {
                                                quickSortByPrice(wishlistItems, 0, validWishCount - 1);
                                            }

                                            cout << "\n========================================\n";
                                            cout << "       YOUR WISHLIST (SORTED BY PRICE)  \n";
                                            cout << "========================================\n";
                                            for (int i = 0; i < validWishCount; ++i) {
                                                wishlistItems[i].display();
                                            }
                                        } else {
                                            cout << "No valid items found in your wishlist.\n";
                                        }
                                    }
                                    break;
                                }
                                case 8: {
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
                                case 9: {
                                    char confirm;
                                    cout << "Are you sure you want to delete your account? (y/n): ";
                                    cin >> confirm;
                                    if (confirm == 'y' || confirm == 'Y') {
                                        for (int i = loggedInIndex; i < userCount - 1; ++i) {
                                            users[i] = users[i + 1];
                                        }
                                        userCount--;
                                        FileManager::saveUsers(users, userCount);
                                        cout << "Account deleted successfully. Logging out...\n";
                                        studentChoice = 10;
                                    } else {
                                        cout << "Account deletion cancelled.\n";
                                    }
                                    break;
                                }
                                case 10: {
                                    cout << "Logging out...\n";
                                    break;
                                }
                                default:
                                    cout << "Invalid choice! Try again.\n";
                            }
                        } while (studentChoice != 10);

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
                int adminChoice;
                do {
                    cout << "\n========================================\n";
                    cout << "       ADMIN ANALYTICS & CONTROL        \n";
                    cout << "========================================\n";
                    cout << "1. View Analytics, Users & Item Listings\n";
                    cout << "2. Delete User Account (Remove Fake/Junk)\n";
                    cout << "3. Delete Item Listing (Admin Moderate)\n";
                    cout << "4. Back to Main Portal\n";
                    cout << "Enter your choice: ";
                    cin >> adminChoice;

                    if (adminChoice == 1) {
                        AdminManager::showAnalytics(users, userCount, items, itemCount);

                        // Display Registered Users
                        cout << "\n=========================================================\n";
                        cout << "               ALL REGISTERED ACCOUNTS                   \n";
                        cout << "=========================================================\n";
                        if (userCount == 0) {
                            cout << "No registered accounts found.\n";
                        } else {
                            for (int i = 0; i < userCount; ++i) {
                                cout << i + 1 << ". Student ID : " << users[i].getStudentId() << "\n";
                                cout << "   Name       : " << users[i].getName() << "\n";
                                cout << "   Email      : " << users[i].getGmail() << "\n";
                                cout << "   Role       : " << users[i].getRole() << "\n";
                                cout << "---------------------------------------------------------\n";
                            }
                        }

                        // Display All Item Listings with Seller Details
                        cout << "\n=========================================================\n";
                        cout << "               ALL ITEM LISTINGS & OWNERS                \n";
                        cout << "=========================================================\n";
                        if (itemCount == 0) {
                            cout << "No items listed yet.\n";
                        } else {
                            for (int i = 0; i < itemCount; ++i) {
                                // Find seller name using seller Student ID
                                string sellerName = "Unknown";
                                for (int j = 0; j < userCount; ++j) {
                                    if (users[j].getStudentId() == items[i].getSellerId()) {
                                        sellerName = users[j].getName();
                                        break;
                                    }
                                }

                                cout << i + 1 << ". Item ID    : " << items[i].getItemId() << "\n";
                                cout << "   Item Name  : " << items[i].getName() << "\n";
                                cout << "   Category   : " << items[i].getCategory() << "\n";
                                cout << "   Price      : Rs. " << items[i].getPrice() << "\n";
                                cout << "   Status     : " << items[i].getAvailabilityStatus() << "\n";
                                cout << "   Added By   : " << sellerName << " (Student ID: " << items[i].getSellerId() << ")\n";
                                cout << "---------------------------------------------------------\n";
                            }
                        }
                    } else if (adminChoice == 2) {
                        cout << "\n--- DELETE USER ACCOUNT ---\n";
                        if (userCount == 0) {
                            cout << "No users available to delete.\n";
                        } else {
                            for (int i = 0; i < userCount; ++i) {
                                cout << i + 1 << ". ID: " << users[i].getStudentId() 
                                     << " | Name: " << users[i].getName() 
                                     << " | Email: " << users[i].getGmail() << "\n";
                            }
                            string targetId;
                            cout << "\nEnter Student ID of the user you want to delete: ";
                            cin >> targetId;

                            int foundIdx = -1;
                            for (int i = 0; i < userCount; ++i) {
                                if (users[i].getStudentId() == targetId) {
                                    foundIdx = i;
                                    break;
                                }
                            }

                            if (foundIdx != -1) {
                                for (int i = foundIdx; i < userCount - 1; ++i) {
                                    users[i] = users[i + 1];
                                }
                                userCount--;
                                FileManager::saveUsers(users, userCount);
                                cout << "User account deleted successfully from the system!\n";
                            } else {
                                cout << "Error: Student ID not found!\n";
                            }
                        }
                    } else if (adminChoice == 3) {
                        cout << "\n--- DELETE ITEM LISTING ---\n";
                        if (itemCount == 0) {
                            cout << "No items available to delete.\n";
                        } else {
                            for (int i = 0; i < itemCount; ++i) {
                                cout << "ID: " << items[i].getItemId() 
                                     << " | Name: " << items[i].getName() 
                                     << " | Price: Rs. " << items[i].getPrice() 
                                     << " | Seller ID: " << items[i].getSellerId() << "\n";
                            }
                            int targetItemId;
                            cout << "\nEnter Item ID of the item to delete: ";
                            cin >> targetItemId;

                            int foundIdx = -1;
                            for (int i = 0; i < itemCount; ++i) {
                                if (items[i].getItemId() == targetItemId) {
                                    foundIdx = i;
                                    break;
                                }
                            }

                            if (foundIdx != -1) {
                                for (int i = foundIdx; i < itemCount - 1; ++i) {
                                    items[i] = items[i + 1];
                                }
                                itemCount--;
                                FileManager::saveItems(items, itemCount);
                                cout << "Item ID " << targetItemId << " deleted successfully by Admin!\n";
                            } else {
                                cout << "Error: Item ID not found!\n";
                            }
                        }
                    } else if (adminChoice == 4) {
                        cout << "Returning to Main Portal...\n";
                    } else {
                        cout << "Invalid choice! Try again.\n";
                    }
                } while (adminChoice != 4);

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
