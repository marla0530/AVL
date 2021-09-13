#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Node {
    string id;
    string name;
    Node* right;
    Node* left;
    int height;
    int balance;
    Node(string& name, string& id) {
        this->name = name;
        this->id = id;
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
        this->balance = 0;
    }
};
class GatorTree {

public:
    Node* root = NULL;
    Node* insert(Node* root, string& name, string& id);
    Node* remove(Node* root, string& id);
    Node* searchId(Node* root, string& id);
    void searchName(Node* root, vector<string>& id, string& name);
    void printInorder(Node* root, vector<string>& names);
    void printPreorder(Node* root, vector<string>& names);
    void printPostorder(Node* root, vector<string>& names);
    void inorderList(Node* root, vector<string>& names);
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* rotateLeftRight(Node* root);
    Node* rotateRightLeft(Node* root);
    int heightCheck(Node* root);
};
//calculate the height of the tree
int GatorTree::heightCheck(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int left = 0;
    int right = 0;
    if (root->left != NULL) {
        left = root->left->height;
    }
    if (root->right != NULL) {
        right = root->right->height;
    }
    return max(left, right) + 1;
}
//insert a Node into the tree and self balance
Node* GatorTree::insert(Node* root, string& name, string& id) {
    //recursive bst insert from class video Trees(3d)
    if (root == NULL) {
        cout << "successful" << endl;
        return new Node(name, id);
    }
    //traverse left
    if (id < root->id) {
        root->left = insert(root->left, name, id);
    }
    //traverse right
    else if (id > root->id)
    {
        root->right = insert(root->right, name, id);
    }
    //if they equal print out the unsucessful because it already exist
    else {
        cout << "unsuccessful" << endl;
        return root;
    }
    //set height as we work our way up
    root->height = heightCheck(root);
    //calculate balance factor while recursive our way back up
    root->balance = (heightCheck(root->left) - heightCheck(root->right));
    //coditions described in the table in video Trees (4d): AVL Trees and Properties
    if (root->balance == 2) {
        if (root->left->balance == 1) {
            root = rotateRight(root);
        }
        else {
            root = rotateLeftRight(root);
        }
    }
    else if (root->balance == -2) {
        if (root->right->balance == -1) {
            root = rotateLeft(root);
        }
        else {
            root = rotateRightLeft(root);
        }
    }

    //return the root to keep the recursive working
    return root;


}

//bst remove a tree
Node* GatorTree::remove(Node* root, string& id) {
    //empty tree
    if (root == NULL) {
        cout << "unsuccessful" << endl;
        return root;
    }
    //search left or right in the tree
    if (id > root->id) {
        root->right = remove(root->right, id);
    }
    else if (id < root->id) {
        root->left = remove(root->left, id);
    }
    //if find the target id
    else {
        //no child
        if (root->left == NULL && root->right == NULL) {
            delete root;
            cout << "successful" << endl;
            return NULL;
        }
        //one child
        else if (root->left == NULL && root->right != NULL) {
            Node* temp = root->right;
            delete root;
            cout << "successful" << endl;
            return temp;
        }
        else if (root->right == NULL && root->left != NULL) {
            Node* temp = root->left;
            delete root;
            cout << "successful" << endl;
            return temp;
        }
        //2 child
        else {
            //find the left most in right sub tree and change the value of it with root
            Node* curr = root->right;
            while (curr->left != NULL) {
                curr = curr->left;
            }
            //swap them
            root->name = curr->name;
            root->id = curr->id;
            //search from the right side of the root to delete the repeated Node
            root->right = remove(root->right, curr->id);
        }

    }
    //return so the recursive works
    return root;
}

//id search following psudo code from Trees (3d): BST Search and Insert
Node* GatorTree::searchId(Node* root, string& id) {
    //if tree is empty
    if (root == NULL) {
        cout << "unsuccessful" << endl;
        return root;
    }
    //if Node match the target value
    else if (id == root->id) {
        return root;
    }
    //if the Node value is greater than target value
    else if (id < root->id) {
        return searchId(root->left, id);
    }
    //if the Node vaule is lesser than target value
    else {
        return searchId(root->right, id);
    }
}

//seach name
void GatorTree::searchName(Node* root, vector<string>& ids, string& name) {
    //if Node is empty
    if (root == NULL) {
        return;
    }
    //pre order traversal and push back result in a vector
    else {
        if (root->name == name) {
            ids.push_back(root->id);
        }
        searchName(root->left, ids, name);
        searchName(root->right, ids, name);
    }

}

//inorder traversal
void GatorTree::printInorder(Node* root, vector<string>& names) {
    //psudo code showed in video Trees(3f)
    if (root == NULL) {
        return;
    }
    printInorder(root->left, names);
    names.push_back(root->name);
    printInorder(root->right, names);
}

void GatorTree::inorderList(Node* root, vector<string>& names) {
    if (root == NULL) {
        return;
    }
    inorderList(root->left, names);
    names.push_back(root->id);
    inorderList(root->right, names);
}

//preorder traversal
void GatorTree::printPreorder(Node* root, vector<string>& names) {
    //psudo code showed in video Trees(3f)
    if (root == NULL) {
        return;
    }
    names.push_back(root->name);
    printPreorder(root->left, names);
    printPreorder(root->right, names);
}
void GatorTree::printPostorder(Node* root, vector<string>& names) {
    //psudo code showed in video Trees(3f)
    if (root == NULL) {
        return;
    }
    printPostorder(root->left, names);
    printPostorder(root->right, names);
    names.push_back(root->name);
}

//concept from class code video Trees (4c): Rotations as Tools
Node* GatorTree::rotateLeft(Node* root) {
    Node* grandChild = root->right->left;
    Node* newParent = root->right;
    newParent->left = root;
    root->right = grandChild;
    //reset the height change
    root->height = (heightCheck(root));
    newParent->height = (heightCheck(newParent));
    return newParent;

}

//reverse of rotate left
Node* GatorTree::rotateRight(Node* root)
{
    Node* grandChild = root->left->right;
    Node* newParent = root->left;
    newParent->right = root;
    root->left = grandChild;
    //reset the height change
    root->height = (heightCheck(root));
    newParent->height = (heightCheck(newParent));
    return newParent;
}

//code used on stepik 5.1.1
Node* GatorTree::rotateLeftRight(Node* root)
{
    root->left = rotateLeft(root->left);
    return rotateRight(root);

}

//code used on stepid 5.1.3
Node* GatorTree::rotateRightLeft(Node* root)
{
    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

//check if name is valid
bool checkName(string& name) {
    for (unsigned int i = 0; i < name.size(); i++)
    {
        if (!isalpha(name[i]) && !isspace(name[i])) {
            return false;
        }
    }
    return true;
}

//check if id is vald
bool checkId(string& id) {
    if (id.size() != 8) {
        return false;
    }
    for (unsigned int i = 0; i < id.size(); i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }

    return true;
}



//print names
void printNames(vector<string>& names) {
    if (names.empty()) { cout << ""; }
    else {
        for (int i = 0; i < names.size() - 1; i++) {
            cout << names[i] << ", ";
        }
        cout << names[names.size() - 1] << endl;
    }
}

int main()
{

    //initialize a gator tree object
    GatorTree t;
    //vector for printing numbers
    vector<string> ids;
    //vector for pringing names
    vector<string> names;
    //get how many lines of command
    string temp;
    getline(cin, temp);
    int linesCount = stoi(temp);
    //excute loop by the amount of time inputed
    for (int i = 0; i < linesCount; i++) {
        //reset 
        names.clear();
        ids.clear();
        //get input and parse
        string line;
        getline(cin, line);
        string input = line.substr(0, line.find(' '));
        line.erase(0, line.find('"') + 1);

        //insert function
        if (input == "insert") {
            //parse both name and id
            string name = line.substr(0, line.find('"'));
            string id = line.substr(line.find_last_of(" ") + 1, line.size() - 1);
            //check if both are valid
            if (checkName(name) && checkId(id)) {
                t.root = t.insert(t.root, name, id);
            }
            else {
                cout << "unsuccessful" << endl;
            }

        }

        //remove function
        else if (input == "remove") {
            string id = line.substr(line.find_last_of(" ") + 1, line.size() - 1);
            if (checkId(id))
            {
                t.root = t.remove(t.root, id);
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        //search function
        else if (input == "search") {
            string name = line.substr(0, line.find('"'));
            string id = line.substr(line.find_last_of(" ") + 1, line.size() - 1);
            if (checkId(id))
            {
                Node* temp = t.searchId(t.root, id);
                if (temp != NULL) {
                    cout << temp->name << endl;
                }
            }
            else if (checkName(name)) {
                t.searchName(t.root, ids, name);
                if (ids.empty()) {
                    cout << "unsuccessful" << endl;
                }
                else {
                    for (int i = 0; i < ids.size() - 1; i++)
                    {
                        cout << ids[i] << endl;
                    }
                    cout << ids[ids.size() - 1] << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        //inorder function
        else if (input == "printInorder") {
            t.printInorder(t.root, names);
            printNames(names);
        }

        //preorder function
        else if (input == "printPreorder")
        {
            t.printPreorder(t.root, names);
            printNames(names);

        }

        //postorder function
        else if (input == "printPostorder") {
            t.printPostorder(t.root, names);
            printNames(names);
        }

        //tree level count
        else  if (input == "printLevelCount")
        {
            cout << t.root->height << endl;
        }

        //remove inorder
        else if (input == "removeInorder") {
            //get input n 
            string temp = line.substr(line.find(" ") + 1, line.size() - 1);
            int n = stoi(temp);
            //return a list of id's through inorder traversal
            t.inorderList(t.root, names);
            //if n is out of index of the vector cout unsuceessful
            if (n >= names.size()) {
                cout << "unsuccessful" << endl;
            }
            //call remove function on the id listed.
            else {
                t.root = t.remove(t.root, names[n]);
            }
        }
        else {
            cout << "unsuccessful" << endl;
            continue;
        }

    }
    return 0;
}