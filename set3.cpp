#include <iostream>
using namespace std;

void add_Disease() {
    cout << "Adding new disease...\n";
}

void updat_Disease() {
    cout << "Updating existing disease...\n";
}

void delete_Disease() {
    cout << "Deleting disease...\n";
}

void view_Diseases() {
    cout << "Showing all diseases...\n";
    
}
void Count_Diseases(){
    // maintians a small dataset of how many times each disease was pridicted
}
int main() {
    //Main for calling the functions for adding , updating , deleting , view and exit.
    int choice;
    cout << "\nADMIN DISEASE MANAGEMENT\n";
    cout << "1. Add Disease\n";
    cout << "2. Update Disease\n";
    cout << "3. Delete Disease\n";
    cout << "4. View Disease List\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch(choice) {
        case 1:
            add_Disease();
            break;

        case 2:
            updat_Disease();
            break;

        case 3:
            delete_Disease();
            break;

        case 4:
            view_Diseases();
            break;

        case 5:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
    }

    return 0;
}
