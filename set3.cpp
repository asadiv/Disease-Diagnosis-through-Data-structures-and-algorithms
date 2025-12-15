#include <iostream>
#include <fstream>
using namespace std;

void loadDiseasesFromFile(const string& filename)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string diseaseName;
    int diseaseCount;

    while (file >> diseaseName >> diseaseCount)
    {
        Disease d(diseaseName, diseaseCount);
        root = insert(root, d);
    }

    file.close();
    cout << "Diseases loaded successfully from file.\n";
}

// Disease structure using avl tree
struct Disease
{
    string name;
    int count;

    Disease() {}
    Disease(string n, int c)
    {
        name = n;
        count = c;
    }
};

struct Node
{
    Disease data;
    Node *left;
    Node *right;
    int height;

    Node(Disease dis) : data(dis), left(NULL), right(NULL), height(1) {}
};

int getHeight(Node *n)
{
    return n ? n->height : 0;
}

int getBalance(Node *n)
{
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node *insert(Node *root, Disease d)
{
    if (!root)
        return new Node(d);

    if (d.name < root->data.name)
        root->left = insert(root->left, d);
    else if (d.name > root->data.name)
        root->right = insert(root->right, d);
    else
    {
        root->data.count += d.count; // increment count if same disease
        return root;
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && d.name < root->left->data.name)
        return rightRotate(root);
    if (balance < -1 && d.name > root->right->data.name)
        return leftRotate(root);
    if (balance > 1 && d.name > root->left->data.name)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && d.name < root->right->data.name)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void inorder(Node *root)
{
    if (!root)
        return;
    inorder(root->left);
    cout << root->data.name << " (" << root->data.count << ") ";
    inorder(root->right);
}

void findMostCommon(Node *root, string &name, int &maxCount)
{
    if (!root)
        return;

    if (root->data.count > maxCount)
    {
        maxCount = root->data.count;
        name = root->data.name;
    }

    findMostCommon(root->left, name, maxCount);
    findMostCommon(root->right, name, maxCount);
}

// now need to figure some way of intermixing the both of these with our task

void add_Disease()
{
    cout << "Adding new disease...\n";
}

void updat_Disease()
{
    cout << "Updating existing disease...\n";
}

void delete_Disease()
{
    cout << "Deleting disease...\n";
}

void view_Diseases()
{
    cout << "Showing all diseases...\n";
    void view_Diseases()
{
    if (!root)
    {
        cout << "No diseases found.\n";
        return;
    }

    cout << "Disease List (Inorder Traversal):\n";
    inorder(root);
    cout << endl;
}

}
void Count_Diseases()
{
    // maintians a small dataset of how many times each disease was pridicted
}
int main()
{
    // Main for calling the functions for adding , updating , deleting , view and exit.
    int choice;
    cout << "\nADMIN DISEASE MANAGEMENT\n";
    cout << "1. Add Disease\n";
    cout << "2. Update Disease\n";
    cout << "3. Delete Disease\n";
    cout << "4. View Disease List\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
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
