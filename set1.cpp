#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
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

class LinkedListS {
    public:
    NodeS* head;

    LinkedListS() {
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



static int maxCount = 0;

class Disease {
    public:
    string disease;
    LinkedListS symptoms;
    LinkedListS treatment;
    LinkedListS prevention;
    string category;
    int count;
    double prob;

    Disease() {
        symptoms.head = nullptr;
        count = 0;
        prob = 0.0;
    }
}; 

NodeS* getSymptomes() {
    

    cout << "Enter the symptoms you have one by one (enter \"done\" when you are done entering): \n";
    string inp;
    int count = 1;
    LinkedListS symptoms;
    do {
        cout << count << ": ";
        getline(cin, inp);
        for(int i = 0; i < inp.length(); i++) {
            inp[i] = tolower(inp[i]);
        }
        if (inp != "done") {
            symptoms.add(inp);
        } 

        count++;
    } while(inp != "done");
     
    return symptoms.head;
}



vector<Disease> getMatchedDiseases(NodeS* s) {
    vector <Disease> fetchedDiseases;
    ifstream symptomsFile("C:\\Disease-Diagnosis-through-Data-structures-and-algorithms\\dataset\\Symptoms.txt",ios::in);
    
    if(!symptomsFile.is_open()) {
        cout << "There is a problem in the symptoms data set you provided!" << endl;
        return fetchedDiseases;
    }
    
    string line;

    string symptoms;
    string disease;
    while(getline(symptomsFile, line)) {
        
        
        disease = "";
        symptoms = "";
        bool space = true;
        for(int i = 0; i < line.length(); i++) {
            if(line[i] == '|') space = false;
            if(space) {
                disease += tolower(line[i]);
            } else {
                symptoms+= tolower(line[i]);
            }
        }
        NodeS* temp = s;
        int count = 0;
        
        bool found = false;
        while(temp != nullptr) {
            if(symptoms.find(temp->symptom) != string::npos) {
                found = true;
                count++;
            }
            temp = temp->next;
        }
        if(count < maxCount) maxCount = count;
        if(found) {
            Disease d;
            d.disease = disease;
            d.count = count;
            NodeS* curr = s;
            while(curr != nullptr) {
                d.symptoms.add(curr->symptom);
                curr = curr->next;
            }

            fetchedDiseases.push_back(d);
        }

        
    }

    symptomsFile.close();
    return fetchedDiseases;
}

void displayMatchedDiseases(vector <Disease> mD) {
    cout << "\nYour symptoms matches these Disease(s):\n";
    int count = 1;
    for(Disease d: mD) {
        bool prevS = false;
        cout << count << ". " << char(toupper(d.disease[0]));
        for(int i = 1; i < d.disease.length(); i++) {
            if(prevS) {
                cout << char(toupper(d.disease[i]));
                prevS = false;
            } else {
                cout << d.disease[i];
                if(d.disease[i] == ' ') prevS = true;
            }
        }
        cout << endl;
        count++;
    }
}

bool compareDiseases(const Disease& a, const Disease& b) {
    // returns true if 'a' has a higher count than 'b' (Descending order)
    return a.count > b.count; 
}

vector <Disease> sortDiseases(vector <Disease> matchedDiseases) {
    vector <Disease> sortedDiseases = matchedDiseases;

    sort(sortedDiseases.begin(), sortedDiseases.end(), compareDiseases);

    return sortedDiseases;
}

class QueueS {
    public:
    Disease diseases[3];
    int front, rear;

    QueueS(){
        front = rear = -1;
    }

    bool isEmpty() {
        if(front == -1 || rear== -1) {
            return true;
        } 
        return false;
    }
    bool isFull() {
        if(front > rear || rear == 2) {
            return true;
        }
        return false;
    }
    void push(Disease d) {
        if(isEmpty()) {
            front = rear = 0;
            diseases[rear] = d;
        } else {
            diseases[++rear] = d;
        }
    }

    Disease pop() {
        Disease temp;
        if (isEmpty()) {
            return temp;
        }
        temp = diseases[front++];
        return temp;
    }

    int totalCount() {
        int c = 0;
        for(int i = 0; i < 3; i++) {
            c += diseases[i].count;
        }
        return c;
    }

    void setProbs() {
        int c = totalCount();
        for(int i = 0; i < 3; i++) {
            diseases[i].prob = (diseases[i].count/c)*100;
        }
    }
};

int main() {
    NodeS* symptoms;
    symptoms = getSymptomes();

    vector <Disease> matchedDiseases;
    matchedDiseases = getMatchedDiseases(symptoms);

    vector <Disease> sortedDiseases;
    sortedDiseases = sortDiseases(matchedDiseases);


}