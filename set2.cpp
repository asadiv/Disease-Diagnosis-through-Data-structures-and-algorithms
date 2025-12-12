#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
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

// holds all data about a disease
class DiseaseNode{
    public:
    Node* symptomHead;
    Node* preventionHead;
    Node* treatmentHead;
    string category;
    DiseaseNode* next;
    DiseaseNode(){
        symptomHead=nullptr;
        preventionHead=nullptr;
        treatmentHead=nullptr;
        next=nullptr;
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
    ifstream symptomFile("Symptoms.txt",ios::in);
    string Symptomtemp;
    while (getline(symptomFile,Symptomtemp))
    {
        string disease,symptomslist;
        stringstream ss(Symptomtemp);
        getline(ss,disease,'|');
        getline(ss,symptomslist);
        disease.erase(disease.find_last_not_of(" ") + 1);
        stringstream ss2(symptomslist);
        string sympEach;
        while (getline(ss2,sympEach,','))
        {
            // trim spaces
            sympEach.erase(0, sympEach.find_first_not_of(" "));
            sympEach.erase(sympEach.find_last_not_of(" ") + 1);
            // create a disease node assign and get its symphead and store symptoms in that symphead
        }
        
    }
        
}



int main(){



    return 0;
}