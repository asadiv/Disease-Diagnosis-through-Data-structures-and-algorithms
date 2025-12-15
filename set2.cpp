#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>
using namespace std;

// used to loads all data from files into linkedlists each node keeping a statmemt
class Node{
    public:
    string term;
    Node* next;
    Node(string temp){
        term = temp;
        next = nullptr;
    }
};

// disease has diff categories and those categories has many diseases whose vectors are empty.
// might implement a dfs for finding category of speciffic disease.  or just simply traverse each array of category to find the disease(but i can find category through disease anyway , its for printing whole category)
class CategoryTree{
    public:
    string name;
    vector<CategoryTree*> children;
    CategoryTree(string tempname){
        name=tempname;
    }
};
CategoryTree* categoryHead=nullptr;
unordered_map<string,string> categoryMap;
// holds all data about a disease
class DiseaseNode{
    public:
    Node* symptomHead;
    Node* preventionHead;
    Node* treatmentHead;
    string category,name;
    DiseaseNode* next;
    DiseaseNode(string name){
        symptomHead=nullptr;
        preventionHead=nullptr;
        treatmentHead=nullptr;
        next=nullptr;
        this->name=name;
    }
};
DiseaseNode* DiseaseHead= nullptr;
// heads to be used again and again for each disease, once assigns the head to DiseaseNode than can be used for other disease
Node* symptomHead=nullptr;
Node* preventionHead=nullptr;
Node* treatmentHead=nullptr;

DiseaseNode* lastDisease(){
    DiseaseNode* temp = DiseaseHead;
    if(temp==nullptr){
        return nullptr;
    }
    while (temp->next)
    {
        temp=temp->next;
    }
    return temp;
}


void createDisease(string name){
    DiseaseNode* newDisease = new DiseaseNode(name);
    if(DiseaseHead==nullptr){
        DiseaseHead = newDisease;
    }else{
        DiseaseNode* itrtemp = DiseaseHead;
        while (itrtemp->next){
            itrtemp = itrtemp->next;
        }
        itrtemp->next = newDisease;
    }
    newDisease->symptomHead = symptomHead;
    symptomHead = nullptr;
    // safer access to map
    if (categoryMap.find(name) != categoryMap.end()) {
        newDisease->category = categoryMap[name];
    }

}

DiseaseNode* findDisease(string disease){
    if(DiseaseHead==nullptr){
        cout<<"Disease list empty. (calling from findDisease())"<<endl;
        return nullptr;
    }
    DiseaseNode* temp = DiseaseHead;
    while (temp)
    {
        if(temp->name==disease){
            return temp;
        }
        temp=temp->next;
    }
    return nullptr;
}

void loadData(){
    ifstream categoryFile("Diseasecategory.txt",ios::in);
    if(!categoryFile){
        cout<<"Diseasecategory.txt file failed to open"<<endl;
        return;
    }else{
        CategoryTree* treetemp = new CategoryTree("disease");
        categoryHead = treetemp;
        string temp;
        while (getline(categoryFile, temp)) {

            string disease, category;
            stringstream ss(temp);

            getline(ss, disease, '|');   // before |
            getline(ss, category);       // after |

            // trim spaces
            disease.erase(disease.find_last_not_of(" ") + 1);
            category.erase(0, category.find_first_not_of(" "));
            categoryMap[disease] = category; //for easy access incase of needinf category of single disease
            bool catFound = false;
            for(auto& child:treetemp->children){
                if(child->name==category){
                    child->children.push_back(new CategoryTree(disease));
                    catFound=true;
                }
            }
            if(catFound=false){
                treetemp->children.push_back(new CategoryTree(category));
                for(auto& child:treetemp->children){
                    if(child->name==category){
                        child->children.push_back(new CategoryTree(disease));
                    }
                }
            }
        }

    }
    categoryFile.close();


    // read symptoms and store it in DiseaseNode
    ifstream symptomFile("Symptoms.txt",ios::in);
    if(!symptomFile){
        cout<<"Diseasecategory.txt file failed to open"<<endl;
        return;}
    string Symptomtemp;
    while (getline(symptomFile,Symptomtemp))
    {
        string disease,symptomslist;
        stringstream ss(Symptomtemp);
        getline(ss,disease,'|');
        getline(ss,symptomslist);
        disease.erase(disease.find_last_not_of(" ") + 1);
        disease.erase(0, disease.find_first_not_of(" "));
        stringstream ss2(symptomslist);
        string sympEach;
        while (getline(ss2,sympEach,','))
        {
            // trim spaces
            sympEach.erase(0, sympEach.find_first_not_of(" "));
            sympEach.erase(sympEach.find_last_not_of(" ") + 1);
            if(symptomHead==nullptr){
                Node* sympNode = new Node(sympEach);
                symptomHead = sympNode;
            }else{
                Node* iterationTemp = symptomHead;
                while(iterationTemp->next){
                    iterationTemp = iterationTemp->next;
                }
                iterationTemp->next = new Node(sympEach);
            }
        }
        createDisease(disease); //create a disease node with this name and connect the symplist to sympHead of DiseaseNode
        
    }
    symptomFile.close();

    // now read treatments and prevention
    ifstream TreatAndPreventionFile("treatmentAndPrevention.txt",ios::in);
    if(!TreatAndPreventionFile){
        cout<<"Diseasecategory.txt file failed to open"<<endl;
        return;
    }
    string TreatPreventtemp; //treatment and prevention
    while (getline(TreatAndPreventionFile,TreatPreventtemp))
    {
        string disease,preventList,TreatList;
        stringstream ss(TreatPreventtemp);
        getline(ss,disease,'|');
        getline(ss,TreatList,'|');
        getline(ss,preventList);
        disease.erase(disease.find_last_not_of(" ") + 1);
        disease.erase(0, disease.find_first_not_of(" "));
        stringstream ss2(TreatList);
        string treatEach,preventEach;
        // store treatments in linklist and connect to treatmentHead
        while (getline(ss2,treatEach,','))
        {
            // trim spaces
            treatEach.erase(0, treatEach.find_first_not_of(" "));
            treatEach.erase(treatEach.find_last_not_of(" ") + 1);
            if(treatmentHead==nullptr){
                Node* treatNode = new Node(treatEach);
                treatmentHead = treatNode;
            }else{
                Node* iterationTemp = treatmentHead;
                while(iterationTemp->next){
                    iterationTemp = iterationTemp->next;
                }
                iterationTemp->next = new Node(treatEach);
            }
        }
        // store preventions in linklist and connect to preventionHead
        while (getline(ss2,preventEach,','))
        {
            // trim spaces
            preventEach.erase(0, preventEach.find_first_not_of(" "));
            preventEach.erase(preventEach.find_last_not_of(" ") + 1);
            if(preventionHead==nullptr){
                Node* treatNode = new Node(preventEach);
                preventionHead = treatNode;
            }else{
                Node* iterationTemp = preventionHead;
                while(iterationTemp->next){
                    iterationTemp = iterationTemp->next;
                }
                iterationTemp->next = new Node(preventEach);
            }
        }
        // find this disease in Disease Linkedlist and attach the treatment and prevention linkedlists
        DiseaseNode* targetDisease = findDisease(disease);
        if(!targetDisease){
            cout<<"\ndisease with name \""<<disease<<"\" was not found so skipping treatments and prevention"<<endl;
        }else{
            // attach the linklists and then set them null for reading another line
            targetDisease->treatmentHead = treatmentHead;
            targetDisease->preventionHead = preventionHead;
            treatmentHead = nullptr;
            preventionHead = nullptr;
        }

    }
}


void PrintDiseaseInfo(DiseaseNode* Disease){
    cout<<"\n\n==========DISEASE==========\n";
    cout<<"NAME: "<<Disease->name<<endl;
    cout<<"CATEGORY: "<<Disease->category<<endl;
    cout<<"SYMPTOMS: "<<endl;
    Node* sympTemp = Disease->symptomHead;
    while (sympTemp)
    {
        cout<<"   -> "<<sympTemp->term<<endl;
        sympTemp=sympTemp->next;
    }
    cout<<"PREVENTIONS: "<<endl;
    Node* preventTemp = Disease->preventionHead;
    while (preventTemp)
    {
        cout<<"   -> "<<preventTemp->term<<endl;
        preventTemp=preventTemp->next;
    }
    cout<<"TREATMENT: "<<endl;
    Node* treatTemp = Disease->treatmentHead;
    while (treatTemp)
    {
        cout<<"   -> "<<treatTemp->term<<endl;
        treatTemp=treatTemp->next;
    }
    
}
void DiseaseLookup(){
    cout<<"\n\n         WELCOME TO DISEASE LOOKUP       \n";
    cout<<"Enter the valid name of disease: ";
    string tempdiseaseName;
    getline(cin,tempdiseaseName);
    DiseaseNode* tempDisease = DiseaseHead;
    if(!tempDisease){
        cout<<"Sorry Disease list is empty\n";
        return;
    }else{
        while (tempDisease)
        {
            string name = tempDisease->name;
            vector<string> suggestionList; //for suggesting similar disease if exact not found
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            transform(tempdiseaseName.begin(), tempdiseaseName.end(), tempdiseaseName.begin(), ::tolower);
            if(name==tempdiseaseName){
                PrintDiseaseInfo(tempDisease);
                return;
            }
            if()//  write fun for finding similar words
            tempDisease = tempDisease->next;
        }
        
    }
}



int main(){



    return 0;
}