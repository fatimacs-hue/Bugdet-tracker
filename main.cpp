#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <windows.h>
using namespace std;

class Entry {
public:
    char type[10];
    char category[20];
    float amount;
    char date[15];

    void input(const char* t) {
        strcpy_s(type, sizeof(type), t);
        cout << "Enter Category: ";
        cin.ignore();
        cin.getline(category, 20);

        cout << "Enter Amount: ";
        cin >> amount;

        cout << "Enter Date (DD/MM/YYYY): ";
        cin >> date;
    }

    void display() const {
        cout << "| " << left << setw(8) << type
            << " | " << setw(18) << category
            << " | " << setw(10) << fixed << setprecision(2) << amount
            << " | " << setw(12) << date << " |" << endl;
    }
};

class FinanceManager {
public:
    void addEntry(const char* type) {
        Entry e;
        ofstream fout("finance.dat", ios::binary | ios::app);

        cout << "\n--- Adding " << type << " ---\n";
        e.input(type);
        fout.write((char*)&e, sizeof(e));
        fout.close();

        system("color B0"); // Soft green 
        Beep(800, 200);
        cout << "\nEntry added successfully!\n";
        system("pause");
    }

    void showReport() {
        ifstream fin("finance.dat", ios::binary);
        if (!fin) {
            system("color C0"); // Red for error
            cout << "No records found!\n";
            system("pause");
            return;
        }

        Entry e;
        float totalIncome = 0, totalExpense = 0;

        system("color F0"); // Background soft beige
        cout << "\n===============================================\n";
        system("color 2F");
        cout << "  Expense Eye: Track, Save and Spend Smart\n";
        system("color F0");
        cout << "===============================================\n\n";

        // Table header
        system("color 2F");
        cout << "+----------+------------------+------------+--------------+\n";
        cout << "| Type     | Category         | Amount     | Date         |\n";
        cout << "+----------+------------------+------------+--------------+\n";
        system("color F0"); // reset to soft beige text

        while (fin.read((char*)&e, sizeof(e))) {
            if (strcmp(e.type, "Income") == 0) system("color A0"); // Light green text on beige
            else system("color B0"); // Soft mint text on beige
            e.display();

            if (strcmp(e.type, "Income") == 0) totalIncome += e.amount;
            else totalExpense += e.amount;
        }

        system("color 2F"); // Footer dark green
        cout << "+----------+------------------+------------+--------------+\n";

        fin.close();

        // Summary
        system("color 3F"); // Soft teal text on beige
        float savings = totalIncome - totalExpense;
        cout << "\nSummary:\n";
        cout << "---------------------------------\n";
        cout << left << setw(20) << "Total Income:" << totalIncome << endl;
        cout << left << setw(20) << "Total Expense:" << totalExpense << endl;
        cout << left << setw(20) << "Savings:" << savings << endl;

        if (savings > 0) cout << "Status: Good Savings\n";
        else if (savings == 0) cout << "Status: Balanced Budget\n";
        else cout << "Status: Overspending\n";

        system("pause");
        system("color E6"); // Back to menu soft yellow-beige
    }

    void modifyEntry() {
        ifstream fin("finance.dat", ios::binary);
        ofstream fout("temp.dat", ios::binary);
        Entry e;
        char category[20];
        bool found = false;

        cout << "Enter category of entry to modify: ";
        cin.ignore();
        cin.getline(category, 20);

        while (fin.read((char*)&e, sizeof(e))) {
            if (strcmp(e.category, category) == 0) {
                system("color B0"); // Soft cyan processing
                cout << "Entry found:\n";
                e.display();
                cout << "\nEnter new details:\n";
                e.input(e.type);
                found = true;
            }
            fout.write((char*)&e, sizeof(e));
        }

        fin.close();
        fout.close();

        if (found) {
            remove("finance.dat");
            rename("temp.dat", "finance.dat");
            system("color B0"); // success
            Beep(1000, 200);
            cout << "Entry modified successfully!\n";
        }
        else {
            system("color C0"); // error
            cout << "Entry not found.\n";
            remove("temp.dat");
        }

        system("pause");
        system("color E6");
    }

    void deleteEntry() {
        ifstream fin("finance.dat", ios::binary);
        ofstream fout("temp.dat", ios::binary);
        Entry e;
        char category[20];
        bool found = false;

        cout << "Enter category of entry to delete: ";
        cin.ignore();
        cin.getline(category, 20);

        while (fin.read((char*)&e, sizeof(e))) {
            if (strcmp(e.category, category) == 0) {
                system("color B0");
                cout << "Deleting entry:\n";
                e.display();
                found = true;
                continue;
            }
            fout.write((char*)&e, sizeof(e));
        }

        fin.close();
        fout.close();

        if (found) {
            remove("finance.dat");
            rename("temp.dat", "finance.dat");
            system("color B0");
            Beep(500, 300);
            cout << "Entry deleted successfully!\n";
        }
        else {
            system("color C0");
            cout << "Entry not found.\n";
            remove("temp.dat");
        }

        system("pause");
        system("color E6");
    }

    void backupData() {
        ifstream fin("finance.dat", ios::binary);
        ofstream fout("finance_backup.dat", ios::binary);

        Entry e;
        while (fin.read((char*)&e, sizeof(e))) {
            fout.write((char*)&e, sizeof(e));
        }

        fin.close();
        fout.close();

        system("color B0"); // success
        cout << "\nBackup created successfully!\n";
        system("pause");
        system("color E6");
    }
};

// Animated processing dots
void loading() {
    cout << "\nProcessing";
    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) system("color F9"); // light blue
        else system("color F0"); // soft beige
        cout << ".";
        Sleep(300);
    }
    cout << endl;
    system("color E6"); // Back to menu
}

int main() {
    FinanceManager fm;
    int choice;

    do {
        system("cls");
        system("color E6"); // Menu soft yellow-beige

        cout << "========================================\n";
        cout << "        Expense Eye: Track, Save and Spend Smart\n";
        cout << "========================================\n\n";

        cout << "1. Add Income\n";
        cout << "2. Add Expense\n";
        cout << "3. View Report\n";
        cout << "4. Modify Entry\n";
        cout << "5. Delete Entry\n";
        cout << "6. Backup Data\n";
        cout << "7. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        loading();

        switch (choice) {
        case 1: fm.addEntry("Income"); break;
        case 2: fm.addEntry("Expense"); break;
        case 3: fm.showReport(); break;
        case 4: fm.modifyEntry(); break;
        case 5: fm.deleteEntry(); break;
        case 6: fm.backupData(); break;
        case 7: cout << "Exiting program...\n"; break;
        default:
            system("color C0");
            cout << "Invalid choice! Try again.\n";
            system("pause");
            system("color E6");
        }

    } while (choice != 7);

    return 0;
}
