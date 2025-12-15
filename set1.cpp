#include <iostream>
#include <vector>
#include <string>
using namespace std;


class NodeS {
    public:
    string symptom;
    NodeS* next;

    NodeS(string s="") {
        symptom = s;
        next = nullptr;
    }
};

class LinkedList {
    public:
    NodeS* head;

    LinkedList() {
        head = nullptr;
    }

    void add(string inp) {
        NodeS* newSym = new NodeS(inp);
        if(head == nullptr) {
            head = newSym;
            return;
        }
        NodeS* curr = head;
        while(curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = newSym;
    }
};

NodeS* getSymptomes() {
    

    cout << "Enter the symptoms you have one by one (enter \"done\" when you are done entering): \n";
    string inp;
    int count = 1;
    LinkedList symptoms;
    do {
        cout << count << ": ";
        cin >> inp;
        if (inp != "done") {
            symptoms.add(inp);
        } 

        count++;
    } while(inp != "done");
     
    return symptoms.head;
}

int main() {
    NodeS* symptoms;

    symptoms = getSymptomes();

    
}