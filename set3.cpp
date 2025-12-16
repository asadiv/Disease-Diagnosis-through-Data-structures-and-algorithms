#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Disease
{
    string name;
    string category;
    vector<string> symptoms;    // for the symptoms file
    vector<string> treatments;  // for the treatments file
    vector<string> preventions; // for the preventions file

    Disease() {} // is added so objects can still be created without passing argument as if you desclare a constructor then you cant use default constructor
    Disease(string n)
    {
        name = n;
    }
};
// AVLL
struct Node
{
    Disease data;
    Node *left;
    Node *right;
    Node(Disease d)
    {
        data = d;
        left = nullptr;
        right = nullptr;
    }
};

Node *root = nullptr;

// filenames as output insde the folder so we have go out so that why we use ../
const string fname_category = "../dataset/Diseasecategory.txt";
const string fname_symptoms = "../dataset/Symptoms.txt";
const string fname_treatpre = "../dataset/treatmentAndPrevention.txt";

string trim(string s)
{
    s.erase(0, s.find_first_not_of(" \t\r\n")); // erase gap before string so -------h will be h
    s.erase(s.find_last_not_of(" \t\r\n") + 1); // erase gap after the string if hello it will make it hell so plus 1 makes it just till last elemt
    return s;
}
// we are spliting and jiong to covert the txt/csv format to list form wihtout commas

vector<string> split_csv(string s) // it converts the different names in vector as different names sperarlty
{
    vector<string> out;
    stringstream ss(s); // Treats works like char you can check each char one by one.
    string item;
    while (getline(ss, item, ','))
    {
        string t = trim(item);
        if (!t.empty())
            out.push_back(t);
    }
    return out; // this would give apple as one then banana as two not in one string .
}
// this combines the string back into line with comma when we are done with the editing
string join_csv(vector<string> v)
{
    string out;
    for (size_t i = 0; i < v.size(); i++)
    {
        out += v[i];
        if (i + 1 < v.size())
            out += ", ";
    }
    return out;
}

// works like this
//  s = "cherry" → not in base → add → base = ["apple", "banana", "cherry"] easy :)
// just change base to refrence otherwise the orginila will be changed
void mergeUniqueVector(vector<string> &base, vector<string> add)
{
    for (int i = 0; i < add.size(); i++)
    {
        string s = add[i];
        if (s == "")
            continue;

        bool exists = false;
        for (int j = 0; j < base.size(); j++)
        {
            if (base[j] == s)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
            base.push_back(s);
    }
}


// BST starts here
// it basically checks from alphabets
Node *insertNode(Node *node, Disease newDisease)
{
    if (!node)
        return new Node(newDisease);

    string newDiseaseName = trim(newDisease.name);
    string currentNodeName = trim(node->data.name);

    if (newDiseaseName < currentNodeName)
        node->left = insertNode(node->left, newDisease);
    else if (newDiseaseName > currentNodeName)
        node->right = insertNode(node->right, newDisease);
    else
    {
        // merge details if disease already exists
        if (!newDisease.category.empty() && node->data.category.empty())
            node->data.category = newDisease.category;
        mergeUniqueVector(node->data.symptoms, newDisease.symptoms);
        mergeUniqueVector(node->data.treatments, newDisease.treatments);
        mergeUniqueVector(node->data.preventions, newDisease.preventions);
        return node;
    }

    return node;
}

Node *findNode(Node *node, string name)
{
    if (!node)
        return nullptr;
    if (name == node->data.name)
        return node;
    if (name < node->data.name)
        return findNode(node->left, name);
    return findNode(node->right, name);
}

void inorderPrint(Node *node)
{
    if (!node)
        return;
    inorderPrint(node->left);
    cout << node->data.name << "\n";
    inorderPrint(node->right);
}
// this will fill the whole TXT with BST
void inorderSave(Node *node, ofstream &fcat, ofstream &fsym, ofstream &ftreat)
{
    if (!node)
        return;
    inorderSave(node->left, fcat, fsym, ftreat);

    fcat << node->data.name << " | " << (node->data.category.empty() ? "unknown" : node->data.category) << "\n";
    fsym << node->data.name << " | " << join_csv(node->data.symptoms) << "\n";
    ftreat << node->data.name << " | " << join_csv(node->data.treatments) << " | " << join_csv(node->data.preventions) << "\n";

    inorderSave(node->right, fcat, fsym, ftreat);
}

void loadAllFromFiles()
{
    // first we fetch the data of category of discease
    ifstream fcat(fname_category);
    if (fcat)
    {
        string line;
        // this will keep going until the data ends
        while (getline(fcat, line))
        {
            string s = trim(line);
            if (s.empty())
                continue;
            size_t pos = s.find('|'); // size_t returns the | as a string so we can easily perfom other tasks
            // everything before the |
            string disease = (pos != string::npos) ? trim(s.substr(0, pos)) : s;
            // everything after |
            string category = (pos != string::npos) ? trim(s.substr(pos + 1)) : "";
            if (!disease.empty())
            {
                Disease d(disease);
                d.category = category;
                root = insertNode(root, d);
            }
        }
        fcat.close();
    }

    ifstream fsym(fname_symptoms);
    if (fsym)
    {
        string line;
        while (getline(fsym, line))
        {
            string s = trim(line);
            if (s.empty())
                continue;
            size_t pos = s.find('|');
            if (pos == string::npos)
                continue;
            string disease = trim(s.substr(0, pos));
            string rest = trim(s.substr(pos + 1));
            Disease d(disease);
            d.symptoms = split_csv(rest);
            root = insertNode(root, d);
        }
        fsym.close();
    }

    ifstream ft(fname_treatpre);
    if (ft)
    {
        string line;
        while (getline(ft, line))
        {
            string s = trim(line);
            if (s.empty())
                continue;

            size_t pos1 = s.find('|');
            //string::npos means not find empty 
            if (pos1 == string::npos)
                continue;
            //pos1 and 0 makes the length of string 
            string disease = trim(s.substr(0, pos1));
            string rest = trim(s.substr(pos1 + 1));

            // Now split rest into treatments and preventions
            size_t pos2 = rest.find('|');
            string treatList = (pos2 != string::npos) ? trim(rest.substr(0, pos2)) : rest;
            string preventList = (pos2 != string::npos) ? trim(rest.substr(pos2 + 1)) : "";

            Disease d(disease);
            d.treatments = split_csv(treatList);
            d.preventions = split_csv(preventList);

            root = insertNode(root, d);
        }
        ft.close();
    }
}
// this will call the function and trunc makes it empty and from zero it fills from the txt files from inorder save
void saveAllToFiles()
{
    ofstream fcat(fname_category, ios::trunc);
    ofstream fsym(fname_symptoms, ios::trunc);
    ofstream ft(fname_treatpre, ios::trunc);
    if (!fcat || !fsym || !ft)
        return;
    inorderSave(root, fcat, fsym, ft);
}
//user activities
void add_Disease()
{
    string name;
    cout << "Enter disease name: ";
    getline(cin >> ws, name);
    name = trim(name);
    if (name.empty())
        return;

    string category;
    cout << "Enter category: ";
    getline(cin, category);
    category = trim(category);

    string s;
    cout << "Enter symptoms: ";
    getline(cin, s);
    vector<string> syms = split_csv(s);

    cout << "Enter treatments: ";
    getline(cin, s);
    vector<string> treats = split_csv(s);

    cout << "Enter preventions: ";
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

void update_Disease()
{
    string name;
    cout << "Enter disease name to update: ";
    getline(cin >> ws, name);
    name = trim(name);
    Node *found = findNode(root, name);
    if (!found)
    {
        cout << "Disease not found.\n";
        return;
    }

    cout << "Current data for " << found->data.name << ":\n";
    cout << "Category: " << found->data.category << "\n";
    cout << "Symptoms: " << join_csv(found->data.symptoms) << "\n";
    cout << "Treatments: " << join_csv(found->data.treatments) << "\n";
    cout << "Preventions: " << join_csv(found->data.preventions) << "\n";
}

void viewDiseaseInfo()
{
    string name;
    cout << "Enter disease name to view: ";
    getline(cin >> ws, name);
    Node *f = findNode(root, trim(name));
    if (!f)
    {
        cout << "Not found.\n";
        return;
    }
    cout << "=== " << f->data.name << " ===\n";
    cout << "Category: " << f->data.category << "\n";
    cout << "Symptoms: " << join_csv(f->data.symptoms) << "\n";
    cout << "Treatments: " << join_csv(f->data.treatments) << "\n";
    cout << "Preventions: " << join_csv(f->data.preventions) << "\n";
}

int main()
{
    loadAllFromFiles();
    while (true)
    {
        cout << "\n1. Add Disease\n2. Update Disease\n3. View Disease\n4. List all disease names\n5. Exit\nEnter choice: ";
        int ch;
        if (!(cin >> ch))
            break;
        cin.ignore();
        switch (ch)
        {
        case 1:
            add_Disease();
            break;
        case 2:
            update_Disease();
            break;
        case 3:
            viewDiseaseInfo();
            break;
        case 4:
            inorderPrint(root);
            break;
        case 5:
            return 0;
        }
    }
}