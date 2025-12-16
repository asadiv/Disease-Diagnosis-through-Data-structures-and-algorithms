#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Disease {
    string name;
    string category;
    vector<string> symptoms;
    vector<string> treatments;
    vector<string> preventions;
    Disease() {}
    Disease(string n) { name = n; }
};

struct Node {
    Disease data;
    Node *left;
    Node *right;
    Node(Disease d) { data = d; left = right = nullptr; }
};

Node *root = nullptr;

string trim(string s) {
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}

vector<string> split_csv(const string &s) {
    vector<string> out;
    stringstream ss(s);
    string item;
    while (getline(ss, item, ',')) {
        string t = trim(item);
        if (!t.empty()) out.push_back(t);
    }
    return out;
}

string join_csv(const vector<string> &v) {
    string out;
    for (size_t i = 0; i < v.size(); i++) {
        out += v[i];
        if (i + 1 < v.size()) out += ", ";
    }
    return out;
}

void mergeUniqueVector(vector<string> &base, vector<string> add) {
    for (int i = 0; i < add.size(); i++) {
        string s = add[i];
        if (s == "") continue;
        bool exists = false;
        for (int j = 0; j < base.size(); j++) {
            if (base[j] == s) { exists = true; break; }
        }
        if (!exists) base.push_back(s);
    }
}

Node *insertNode(Node *node, const Disease &d) {
    if (!node) return new Node(d);
    string key = trim(d.name);
    string nodeKey = trim(node->data.name);
    if (key < nodeKey) node->left = insertNode(node->left, d);
    else if (key > nodeKey) node->right = insertNode(node->right, d);
    else {
        if (!d.category.empty() && node->data.category.empty())
            node->data.category = d.category;
        mergeUniqueVector(node->data.symptoms, d.symptoms);
        mergeUniqueVector(node->data.treatments, d.treatments);
        mergeUniqueVector(node->data.preventions, d.preventions);
        return node;
    }
    return node;
}

Node *findNode(Node *node, const string &name) {
    if (!node) return nullptr;
    if (name == node->data.name) return node;
    if (name < node->data.name) return findNode(node->left, name);
    return findNode(node->right, name);
}

void inorderPrint(Node *node) {
    if (!node) return;
    inorderPrint(node->left);
    cout << node->data.name << "\n";
    inorderPrint(node->right);
}

void loadAllFromFiles() {
    ifstream fcat("../dataset/Diseasecategory.txt");
    if (fcat) {
        string line;
        while (getline(fcat, line)) {
            string s = trim(line);
            if (s.empty()) continue;
            size_t pos = s.find('|');
            string disease = (pos != string::npos) ? trim(s.substr(0, pos)) : s;
            string category = (pos != string::npos) ? trim(s.substr(pos + 1)) : "";
            if (!disease.empty()) {
                Disease d(disease);
                d.category = category;
                root = insertNode(root, d);
            }
        }
        fcat.close();
    }
    ifstream fsym("../dataset/Symptoms.txt");
    if (fsym) {
        string line;
        while (getline(fsym, line)) {
            string s = trim(line);
            if (s.empty()) continue;
            size_t pos = s.find('|');
            if (pos == string::npos) continue;
            string disease = trim(s.substr(0, pos));
            string rest = trim(s.substr(pos + 1));
            Disease d(disease);
            d.symptoms = split_csv(rest);
            root = insertNode(root, d);
        }
        fsym.close();
    }
    ifstream ft("../dataset/treatmentAndPrevention.txt");
    if (ft) {
        string line;
        while (getline(ft, line)) {
            string s = trim(line);
            if (s.empty()) continue;
            stringstream ss(s);
            string disease, treatList, preventList;
            getline(ss, disease, '|');
            getline(ss, treatList, '|');
            getline(ss, preventList, '|');
            Disease d(trim(disease));
            d.treatments = split_csv(treatList);
            d.preventions = split_csv(preventList);
            root = insertNode(root, d);
        }
        ft.close();
    }
}

void saveAllToFiles() {
    ofstream fcat("../dataset/Diseasecategory.txt", ios::trunc);
    ofstream fsym("../dataset/Symptoms.txt", ios::trunc);
    ofstream ft("../dataset/treatmentAndPrevention.txt", ios::trunc);
    if (!fcat || !fsym || !ft) return;
    // TODO: implement inorder save
}

void add_Disease() {
    string name;
    cout << "Enter disease name: ";
    getline(cin >> ws, name);
    name = trim(name);
    if (name.empty()) return;
    string category;
    cout << "Enter category: ";
    getline(cin, category);
    category = trim(category);
    string s;
    cout << "Enter symptoms CSV: ";
    getline(cin, s);
    vector<string> syms = split_csv(s);
    cout << "Enter treatments CSV: ";
    getline(cin, s);
    vector<string> treats = split_csv(s);
    cout << "Enter preventions CSV: ";
    getline(cin, s);
    vector<string> prevs = split_csv(s);
    Disease d(name);
    d.category = category;
    d.symptoms = syms;
    d.treatments = treats;
    d.preventions = prevs;
    root = insertNode(root, d);
    saveAllToFiles();
    cout << "Disease added/merged successfully.\n";
}

int main() {
    loadAllFromFiles();
    while (true) {
        cout << "\n1. Add Disease\n2. Update Disease\n3. View Disease\n4. List all disease names\n5. Exit\nEnter choice: ";
        int ch;
        if (!(cin >> ch)) break;
        cin.ignore();
        switch (ch) {
        case 1: add_Disease(); break;
        case 4: inorderPrint(root); break;
        case 5: return 0;
        }
    }
}
