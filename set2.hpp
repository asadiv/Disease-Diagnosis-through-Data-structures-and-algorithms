#ifndef SET2_HPP
#define SET2_HPP


using namespace std;

// used to loads all data from files into linkedlists each node keeping a statmemt
class NodeSet2{
    public:
    string term;
    NodeSet2* next;
    NodeSet2(string temp){
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
    NodeSet2* symptomHead;
    NodeSet2* preventionHead;
    NodeSet2* treatmentHead;
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
NodeSet2* symptomHead=nullptr;
NodeSet2* preventionHead=nullptr;
NodeSet2* treatmentHead=nullptr;

// Delete all nodes in a NodeSet2 linked list
void deleteNodeList(NodeSet2* head) {
    while (head) {
        NodeSet2* temp = head;
        head = head->next;
        delete temp;
    }
}

// Delete the entire disease linked list and all its sub-lists
void deleteAllDiseases() {
    DiseaseNode* current = DiseaseHead;
    while (current) {
        DiseaseNode* next = current->next;
        
        deleteNodeList(current->symptomHead);
        deleteNodeList(current->preventionHead);
        deleteNodeList(current->treatmentHead);
        
        delete current;
        current = next;
    }
    DiseaseHead = nullptr;
}

// Recursively delete category tree
void deleteCategoryTree(CategoryTree* node) {
    if (!node) return;
    for (auto child : node->children) {
        deleteCategoryTree(child);
        delete child;
    }
    node->children.clear();
}

// Clear everything
void clearAllData() {
    deleteAllDiseases();
    deleteCategoryTree(categoryHead);
    delete categoryHead;
    categoryHead = nullptr;
    categoryMap.clear();
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

void loadDataSet2(){
    // Clear any previously loaded data to prevent duplication
    clearAllData();
    
    // Reset global temporary heads
    symptomHead = nullptr;
    preventionHead = nullptr;
    treatmentHead = nullptr;

    ifstream categoryFile("dataset/Diseasecategory.txt",ios::in);
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
            disease.erase(0, category.find_first_not_of(" "));
            category.erase(0, category.find_first_not_of(" "));
            category.erase(category.find_last_not_of(" ") + 1);
            categoryMap[disease] = category; //for easy access incase of needinf category of single disease
            bool catFound = false;
            for(auto& child:treetemp->children){
                if(child->name==category){
                    child->children.push_back(new CategoryTree(disease));
                    catFound=true;
                    break;
                }
            }
            if(catFound==false){
                treetemp->children.push_back(new CategoryTree(category));
                CategoryTree* lastCateg = treetemp->children.back();
                lastCateg->children.push_back(new CategoryTree(disease));
            }
        }

    }
    categoryFile.close();


    // read symptoms and store it in DiseaseNode
    ifstream symptomFile("dataset/Symptoms.txt",ios::in);
    if(!symptomFile){
        cout<<"Symptoms.txt file failed to open"<<endl;
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
                NodeSet2* sympNode = new NodeSet2(sympEach);
                symptomHead = sympNode;
            }else{
                NodeSet2* iterationTemp = symptomHead;
                while(iterationTemp->next){
                    iterationTemp = iterationTemp->next;
                }
                iterationTemp->next = new NodeSet2(sympEach);
            }
        }
        createDisease(disease); //create a disease node with this name and connect the symplist to sympHead of DiseaseNode
        
    }
    symptomFile.close();

    // now read treatments and prevention
    ifstream TreatAndPreventionFile("dataset/treatmentAndPrevention.txt",ios::in);
    if(!TreatAndPreventionFile){
        cout<<"treatmentAndPrevention.txt file failed to open"<<endl;
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
        stringstream ss3(preventList);
        string treatEach,preventEach;
        // store treatments in linklist and connect to treatmentHead
        while (getline(ss2,treatEach,','))
        {
            // trim spaces
            treatEach.erase(0, treatEach.find_first_not_of(" "));
            treatEach.erase(treatEach.find_last_not_of(" ") + 1);
            if(treatmentHead==nullptr){
                NodeSet2* treatNode = new NodeSet2(treatEach);
                treatmentHead = treatNode;
            }else{
                NodeSet2* iterationTemp = treatmentHead;
                while(iterationTemp->next){
                    iterationTemp = iterationTemp->next;
                }
                iterationTemp->next = new NodeSet2(treatEach);
            }
        }
        // store preventions in linklist and connect to preventionHead
        while (getline(ss3,preventEach,','))
        {
            // trim spaces
            preventEach.erase(0, preventEach.find_first_not_of(" "));
            preventEach.erase(preventEach.find_last_not_of(" ") + 1);
            if(preventionHead==nullptr){
                NodeSet2* treatNode = new NodeSet2(preventEach);
                preventionHead = treatNode;
            }else{
                NodeSet2* iterationTemp = preventionHead;
                while(iterationTemp->next){
                    iterationTemp = iterationTemp->next;
                }
                iterationTemp->next = new NodeSet2(preventEach);
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
    NodeSet2* sympTemp = Disease->symptomHead;
    while (sympTemp)
    {
        cout<<"   -> "<<sympTemp->term<<endl;
        sympTemp=sympTemp->next;
    }
    cout<<"PREVENTIONS: "<<endl;
    NodeSet2* preventTemp = Disease->preventionHead;
    while (preventTemp)
    {
        cout<<"   -> "<<preventTemp->term<<endl;
        preventTemp=preventTemp->next;
    }
    cout<<"TREATMENT: "<<endl;
    NodeSet2* treatTemp = Disease->treatmentHead;
    while (treatTemp)
    {
        cout<<"   -> "<<treatTemp->term<<endl;
        treatTemp=treatTemp->next;
    }
    
}
void DiseaseLookup(){
    cout<<"\n\n         WELCOME TO DISEASE LOOKUP       \n";
    cout<<"Enter the valid name of disease: ";
    string inputdiseaseName;
    getline(cin,inputdiseaseName);
    transform(inputdiseaseName.begin(), inputdiseaseName.end(), inputdiseaseName.begin(), ::tolower);
    DiseaseNode* tempDisease = DiseaseHead;
    if(!tempDisease){
        cout<<"Sorry Disease list is empty\n";
        return;
    }else{
        vector<DiseaseNode*> similarDiseases;  //for storing nearly matching diseases
        while (tempDisease)
        {
            string name = tempDisease->name;
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            if(name==inputdiseaseName){
                PrintDiseaseInfo(tempDisease);
                return;
            }
            if(name.size()>=2 && inputdiseaseName.size()>=2 && 
                name.substr(0,2)==inputdiseaseName.substr(0,2)){
                similarDiseases.push_back(tempDisease);
            }
            tempDisease = tempDisease->next;
        }
        if(!similarDiseases.empty()){
            cout<<"\nCouldnot find the exact disease, Did you mean: \n";
            int index = 1;
            for(auto sugg:similarDiseases){
                cout<<index<<". "<<sugg->name<<endl;
                index++;
            }
            cout<<"Enter a digit to see respective disease or press any other key to exit: ";
            int choice;
            cin>>choice;
            if(cin.fail()){  // means if non integer input is there
                cin.clear(); //clear the input
                cin.ignore(100,'\n');//ignore all chars entered
                return;
            }
            cin.ignore();

            if(choice>=1 && choice<=similarDiseases.size()){
                PrintDiseaseInfo(similarDiseases[choice-1]);
                similarDiseases.clear();
                return;
            }else{
                similarDiseases.clear();
                return;
            }
        }else{
            cout<<"Sorry could not find the disease: "<<inputdiseaseName<<endl;
        }
        
    }
}


void exploreCategory(){
    cout<<"\n\n         DISEASE CATEGORIES\n";
    CategoryTree* tempRoot = categoryHead;
    if(!tempRoot){
        cout<<"Category tree is empty. (calling from exploreCategory())"<<endl;
        return;
    }
    int index=1;
    for(auto categ:tempRoot->children){
        cout<<index<<". "<<categ->name<<endl;
        index++;
    }
    cout<<"Choose a category:";
    int choice;
    cin>>choice;
    if(cin.fail()){
        cin.clear();
        cin.ignore(100,'\n');
        cout<<"Invalid input. returning...\n";
        return;
    }
    cin.ignore();
    if(choice>=1 && choice<=tempRoot->children.size()){
        cout<<"\nCategory: "<<tempRoot->children[choice-1]->name<<endl;
        for(auto subcat:tempRoot->children[choice-1]->children){
            cout<<subcat->name<<endl;
        }
    }else{
        cout<<"Invalid input. returning...\n";
        return;
    }
}





#endif