#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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
    string treatment;
    string prevention;
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
        } else if(isFull()) {
            return;
        }else {
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
            diseases[i].prob = (double(diseases[i].count)/double(c))*100;
        }
        
    }

     void setCategories() {

        ifstream categoriesFile("C:\\Disease-Diagnosis-through-Data-structures-and-algorithms\\dataset\\Diseasecategory.txt",ios::in);

        string line;

        if(!categoriesFile.is_open()) return;

        while(getline(categoriesFile, line)) {

            for(int i = 0; i< line.length(); i++) {
                line[i] = tolower(line[i]);
            }
            for(int i = front; i <= rear; i++) {

                if(line.find(diseases[i].disease) != string::npos) {

                    string cate = "";

                    bool dash = false;

                    for(int j = 0; j< line.length(); j++) {

                        if(dash) {

                            cate += line[j];

                        }else if(line[j] == '|') {

                            dash = true;

                        }

                    }

                    diseases[i].category = cate;

                }

            }

        }

        categoriesFile.close();

    }

    void setTandP() {

        ifstream TandPFile("C:\\Disease-Diagnosis-through-Data-structures-and-algorithms\\dataset\\treatmentAndPrevention.txt",ios::in);

        string line;

        if(!TandPFile.is_open()) {
            cout << "no";
            return;
        }
        while(getline(TandPFile, line)) {

            for(int i = 0; i< line.length(); i++) {
                line[i] = tolower(line[i]);
            }
            for(int i = front; i <= rear; i++) {

                if(line.find(diseases[i].disease) != string::npos) {

                    string t = "";

                    string p = "";

                    bool dash1 = false;

                    bool dash2 = false;

                    for(int j = 0; j< line.length(); j++) {
                        if(line[j] == '|' && !dash1) {

                            dash1 = true;
                            continue;

                        } else if(line[j] == '|' && dash1) {

                            dash2 = true;
                            continue;

                        }
                        if(dash1 && !dash2) {

                            t+=line[j];

                        } else if(dash1 && dash2) {

                            p+=line[j];

                        }

                        

                    }

                    diseases[i].treatment = t;

                    diseases[i].prevention = p;

                }

            }

        }

        TandPFile.close();

    }



    void display() {
        cout << "\nHere are the diseases and the probabilty of having them based on your symptoms:\n";
        int count = 1;
        for(int i = front; i<= rear; i++) {
            cout << count << ". \n";
            cout << "Disease: " << diseases[i].disease << "\n";
            cout << "Probabilty: " << diseases[i].prob << "%\n";
            cout << "Category: " << diseases[i].category << "\n";
            cout << "Treatment: " << diseases[i].treatment << "\n";
            cout << "Prevention: " << diseases[i].prevention << "\n";

            count++;
            cout << "\n";
        }
    }
};

QueueS makeQ(vector <Disease> sortedDiseases) {
    QueueS q;
    for(int i = 0; i < 3; i++) {
        q.push(sortedDiseases[i]);
    }
    return q;
}

int main() {
    NodeS* symptoms;
    symptoms = getSymptomes();

    vector <Disease> matchedDiseases;
    matchedDiseases = getMatchedDiseases(symptoms);

    displayMatchedDiseases(matchedDiseases);

    vector <Disease> sortedDiseases;
    sortedDiseases = sortDiseases(matchedDiseases);

    QueueS q = makeQ(sortedDiseases);
    q.setCategories();
    q.setProbs();
    q.setTandP();
    q.display();

}