#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
/* Note:
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file on canvas. If you are using multiple header files,
	   you will have to include them directly in main.cpp so we can test your code as a single file.
*/

struct Node {
    public:
    string name;
    int id;
    Node *left;
    Node *right;
    int height;
};
Node *newNode() {
    Node* node = new Node();
    node = nullptr;
    return node;
}
Node *newNode(string name, int id) {
    Node *node = new Node();
    node->name = name;
    node->id = id;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    return node;
}



class Tree {
public:
    Node *root = nullptr;
    vector<Node*> vec;
    // tree destructor recursive function
    void destructTree(Node *node) {
        if (node != nullptr) {
            destructTree(node->left);
            destructTree(node->right);
            delete node;
        }
    }
    ~Tree() {
        destructTree(root);
    }

    // insert (slides 37)
    Node* insert(Node *node, string name, int id) {
        if (node == nullptr) {
            // create new node
            Node *n = newNode(name, id);
            cout << "successful" << endl;
            return n;
        }
        else if (id < node->id) {
            node->left = insert(node->left, name, id);
        }
        else if (id > node->id) {
            node->right = insert(node->right, name, id);
        }
        else if (id == node->id) {
            cout << "unsuccessful" << endl;
            return node;
        }
        // balance tree
        int balanceFactor = getBalanceFactor(node);
        // left left- right rotation
        if (balanceFactor > 1 && getBalanceFactor(node->left) > 0) {
            return leftLeft(node);
        }
        // right right - left rotation
        if (balanceFactor < -1 && getBalanceFactor(node->right) < 0) {
            return rightRight(node);
        }
        // left right bf > 1 left child bf < 0
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            return leftRight(node);
        }
        // right left bf < -1 right child bf > 0
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            return rightLeft(node);
        }
        return node;
    }
    Node* removeID(Node *node, int id) {
        if (node == nullptr) {
            return node;
        }
        // if ids are not equal
        if (id < node->id) {
            node->left = removeID(node->left,  id);
        }
        else if (id > node->id) {
            node->right = removeID(node->right,  id);
        }
        else {
            // no child
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
                return node;
            }
            // 1 child make the child the root node, delete the child node
            else if (node->left == nullptr || node->right == nullptr) {
                if (node->left != nullptr) {
                    Node *temp = node;
                    node = node->left;
                    delete temp;
                    temp = nullptr;
                    return node;
                }
                if (node->right != nullptr) {
                    Node *temp = node;
                    node = node->right;
                    delete temp;
                    temp = nullptr;
                    return node;
                }
            }
            // two children- find minimum element in right subtree
            else if (node->left != nullptr && node->right != nullptr) {
                Node *temp = minimum(node->right);
                node->id = temp->id;
                node->name = temp->name;
                // remove minimum element from right subtree
                node->right = removeID(node->right, node->id);
                return node;
            }
        }
        return node;
    }
    // returns minimum node in right subtree
    Node* minimum(Node *node) {
        if (node->left != nullptr) {
            return minimum(node->left);
        }
        else {
            return node;
        }
    }
    // returns height of node
    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        int height = max(getHeight(node->left), getHeight(node->right)) + 1;
        return height;
    }
    // returns balance factor of tree
    int getBalanceFactor(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        int left = getHeight(node->left);
        int right = getHeight(node->right);
        return (left - right);
    }
    // id search will return one unique match
    Node* searchID(Node *node, int id) {
        if (node == nullptr) {
            return node;
        }
        // continue search
        else if (id < node->id) {
            return searchID(node->left,  id);
        }
        else if (id > node->id) {
            return searchID(node->right,  id);
        }
        // equal- return id
        else if (id == node->id) {
            return node;
        }
        return node;
    }
    // name search- may return multiple matches
    void searchName(Node *node, string name) {
        if (node == nullptr) {
            return;
        }
        // equal- add to vector
        if (name.compare(node->name) == 0) {
            vec.push_back(node);
        }
        // unequal- continue search
        if (node->left) {
            searchName(node->left, name);
        }
        if (node->right) {
            searchName(node->right, name);
        }
    }
    void printInOrder(Node *n) {
        if (n == nullptr) {
            return;
        }
        if (n->left != nullptr) {
            printInOrder(n->left);
        }
        vec.push_back(n);
        if (n->right != nullptr) {
            printInOrder(n->right);
        }
    }
    void printPreOrder(Node *n) {
        if (n != nullptr) {
            vec.push_back(n);
            if (n->left != nullptr) {
                printPreOrder(n->left);
            }
            if (n->right != nullptr) {
                printPreOrder(n->right);
            }
        }
        else {
            return;
        }
    }
    void printPostOrder(Node *n) {
        if (n == nullptr) {
            return;
        }
        if (n->left != nullptr) {
            printPostOrder(n->left);
        }
        if (n->right != nullptr) {
            printPostOrder(n->right);
        }
        vec.push_back(n);
    }
    // prints tree level count
    void printLevelCount(Node *n) {
        if (n == nullptr) {
            cout << "0" << endl;
        }
        else {
            cout << getHeight(n) << endl;
        }
    }
    // remove node at index num "in order"
    void removeInOrder(int num) {
        vec.clear();
        printInOrder(root);
        // if calling a valid index
        if (num < vec.size()) {
            // remove n'th in order node from tree
            Node *n = removeID(root, vec[num]->id);
            if (n != nullptr) {
                cout << "successful" << endl;
            } else {
                if (n->left == nullptr && n->right == nullptr) {
                    cout << "successful" << endl;
                } else {
                    cout << "unsuccessful" << endl;
                }
            }
        }
        // invalid index called
        else {
            cout << "unsuccessful" << endl;
        }
    }
    // tree rotations
    Node* leftLeft(Node *n) {
        Node *top;
        Node *left;
        top = n;
        left = top->left;
        top->left = left->right;
        left->right = top;
        return left;
    }
    Node* rightRight(Node *n) {
        Node *top;
        Node *right;
        top = n;
        right = top->right;
        top->right = right->left;
        right->left = top;
        return right;
    }
    Node* leftRight(Node *n) {
        Node *top;
        Node *L;
        Node *LR;
        top = n;
        L = n->left;
        LR = n->left->right;
        top->left = LR->right;
        L->right = LR->left;
        LR->right = top;
        LR->left = L;
        return LR;
    }
    Node* rightLeft(Node *n) {
        Node *top;
        Node *R;
        Node *RL;
        top = n;
        R = n->right;
        RL = n->right->left;
        top->right = RL->left;
        R->left = RL->right;
        RL->left = top;
        RL->right = R;
        return RL;
    }
};

// helper function returns input name
string getName(istringstream &in) {
    string name;
    string skip;
    getline(getline(in, skip, '"'), name, '"');
    for (char c: name) {
        if (isalpha(c) || isspace(c)) {
        } else {
            name = "";
            break;
        }
    }
    return name;
}
// helper function returns input id
string getID(istringstream &in) {
    string id;
    in >> id;
    for (char i: id) {
        if (isdigit(i)) {
        } else {
            id = "";
            break;
        }
    }
    return id;
}

// take input command as string and perform called operations
void parseCommand(Tree &t, istringstream &in) {
    string command;
    string name;
    string id;
    string index;
    in >> command;

    if (command ==  "insert") {
        name = getName(in);
        id = getID(in);
        // if valid name and valid id, insert node to tree
        if (name.size() > 0 && id.size() == 8) {
            t.root = t.insert((t.root), name, stoi(id));
        } else {
            cout << "unsuccessful" << endl;
        }
    }
    else if (command == "remove") {
        // find id and remove account
        id = getID(in);
        if (id.size() == 8) {
            // if tree only has root node
            if (t.root->left == nullptr && t.root->right == nullptr) {
                delete t.root;
                t.root = nullptr;
                cout << "successful" << endl;
            }
            else {
                Node *n = t.removeID(t.root, stoi(id));
                if (n != nullptr) {
                    cout << "successful" << endl;
                } else {
                    if (n->left == nullptr && n->right == nullptr) {
                        cout << "successful" << endl;
                    } else {
                        cout << "unsuccessful" << endl;
                    }
                }
            }
        }
    }
    else if (command == "search") {
        // determine id or name
        in >> name;
        bool isName;
        bool isID;
        // remove quotes
        if (name.find("\"") != string::npos) {
            name = name.substr(1, name.size() - 2);
        }
        // determine if input is name
        for (char c: name) {
            if (isalpha(c) || isspace(c)) {
                isName = true;
            } else {
                id = name;
                isName = false;
                break;
            }
        }
        // determine if input is id
        if (!isName) {
            for (char i: id) {
                if (isdigit(i)) {
                    isID = true;
                } else {
                    isID = false;
                    break;
                }
            }
        }
        // search for name and print id
        if (isName) {
            t.vec.clear();
            t.searchName(t.root, name);
            if (t.vec.size() > 0) {
                for (int i = 0; i < t.vec.size(); i++) {
                    cout << t.vec[i]->id << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }
        // search for id and print name
        if (isID) {
            Node *n = t.searchID(t.root, stoi(id));
            if (n != nullptr) {
                cout << n->name << endl;
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }
    }
    else if (command == "printInorder") {
        t.vec.clear();
        t.printInOrder(t.root);
        for (int i = 0; i < t.vec.size(); i++) {
            cout << t.vec[i]->name;
            if (i < t.vec.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else if (command == "printPreorder") {
        t.vec.clear();
        t.printPreOrder(t.root);
        for (int i = 0; i < t.vec.size(); i++) {
            cout << t.vec[i]->name;
            if (i < t.vec.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else if (command == "printPostorder") {
        t.vec.clear();
        t.printPostOrder(t.root);
        for (int i = 0; i < t.vec.size(); i++) {
            cout << t.vec[i]->name;
            if (i < t.vec.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else if (command == "printLevelCount") {
        t.printLevelCount(t.root);
    }
    else if (command == "removeInorder"){
        in >> index;
        t.removeInOrder(stoi(index));
    }
}


int main(){
    // instantiate AVLTree
    Tree tree;
    // number of commands
    int commands = 0;
    cin >> commands;
    string line;
    while (commands > 0) {
        // read entire line
        getline(cin >> ws, line);
        istringstream in(line);
        // function handles command
        parseCommand(tree, in);
        commands--;
    }
    return 0;
}


