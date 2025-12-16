#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include<sstream>
#include <unordered_map>
#include <cctype>

using namespace std;
#include "set1.hpp"
#include "set2.hpp"




int main(){

    cout << "  __  __ ______ _____          _____ " << endl;
    cout << " |  \\/  |  ____|  __ \\   /\\   |_   _|" << endl;
    cout << " | \\  / | |__  | |  | | /  \\    | |  " << endl;
    cout << " | |\\/| |  __| | |  | |/ /\\ \\   | |  " << endl;
    cout << " | |  | | |____| |__| / ____ \\ _| |_ " << endl;
    cout << " |_|  |_|______|_____/_/    \\_\\_____|" << endl;
    cout << endl;

    while (true)
    {     
        cout<<"\n\n==========DISEASE DIAGNOSIS PORTAL==========\n";
        cout<<"1. Search Disease by name\n";
        cout<<"2. Explore categories\n";
        cout<<"3. Disease Prediction\n";
        cout<<"4. Symptom-based Search\n";
        cout<<"5. Add Disease\n";
        cout<<"6. Update Disease\n";
    
        cout<<"Choose an option or press any key to exit:";
        int choice;
        cin>>choice;
        if(cin.fail()){  // means if non integer input is there
            cin.clear(); //clear the input
            cin.ignore(100,'\n');//ignore all chars entered
            return 0;
        }
        cin.ignore();
        switch (choice)
        {
        case 1:
            loadDataSet2();
            DiseaseLookup();
            break;
        case 2:
            loadDataSet2();
            exploreCategory();
            break;
        case 3:
            predictDisease();
            break;
        case 4:
            displayMatchedDiseases();
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        
        default:
            return 0;
            break;
        }
    }
    return 0;
}