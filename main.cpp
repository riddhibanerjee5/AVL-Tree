#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TreeNode
{
public:
    int ID;
    string name;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : ID(0), name(" "), left(nullptr), right(nullptr) {}
    TreeNode(int x, string y) : ID(x), name(y), left(nullptr), right(nullptr) {}
    TreeNode(int x, string y, TreeNode *left, TreeNode *right) : ID(x), name(), left(left), right(right) {}
};

int calcHeight(TreeNode *node)
{ // Helper function to calculate the height of the tree. This is also the levelCount function

    if (node == NULL)
        return 0;

    int rightTreeHeight = calcHeight(node->right);
    int leftTreeHeight = calcHeight(node->left);

    return ((rightTreeHeight > leftTreeHeight ? rightTreeHeight : leftTreeHeight) + 1);
}

void printInOrder(TreeNode *root)
{ // inOrder is Left Root Right

    if (root != nullptr)
    {

        if (root->left)
        {
            printInOrder(root->left);
            cout << ", "; // Print comma after name to prevent the list starting with a comma
        }

        cout << root->name;

        if (root->right)
        {
            cout << ", "; // Print comma before so the list doesn't end with a comma
            printInOrder(root->right);
        }
    }
}

void printPreOrder(TreeNode *root)
{ // preOrder if Root Left Right

    if (root != nullptr)
    {

        cout << root->name; // Print the root first, no comma before

        if (root->left)
        {
            cout << ", "; // Print comma and then the name
            printPreOrder(root->left);
        }

        if (root->right)
        {
            cout << ", "; // For last element which is right, print comma before the name to prevent list ending with a comma
            printPreOrder(root->right);
        }
    }
}

void printPostOrder(TreeNode *root)
{ // postOrder is Left Right Root

    if (root != nullptr)
    {

        if (root->left)
        {
            printPostOrder(root->left);
            cout << ", ";
        }

        if (root->right)
        {
            printPostOrder(root->right);
            cout << ", "; // Since we still have to print the root, we can print a comma after the right element
        }

        cout << root->name;
    }
}

int getBalanceFactor(TreeNode *node)
{ // Function to get the balance factor of a tree -- this will help me in my balanceTree function

    if (node == NULL)
        return 0;

    return (calcHeight(node->left) - calcHeight(node->right));
}

// The following four functions are the basic rotation methods

TreeNode *rotateLeft(TreeNode *parent)
{
    TreeNode *nParent;
    nParent = parent->right;
    parent->right = nParent->left;
    nParent->left = parent;
    return nParent;
}

TreeNode *rotateRight(TreeNode *parent)
{
    TreeNode *nParent;
    nParent = parent->left;
    parent->left = nParent->right;
    nParent->right = parent;
    return nParent;
}

TreeNode *rotateLeftRight(TreeNode *parent)
{
    TreeNode *t;
    t = parent->left;
    parent->left = rotateLeft(t);
    return rotateRight(parent);
}

TreeNode *rotateRightLeft(TreeNode *parent)
{
    TreeNode *t;
    t = parent->right;
    parent->right = rotateRight(t);
    return rotateLeft(parent);
}

TreeNode *balanceTree(TreeNode *node)
{
    int balFactor = getBalanceFactor(node);

    if (balFactor > 1)
    {

        if (getBalanceFactor(node->left) > 0) // Left imbalance
            node = rotateRight(node);

        else
            node = rotateLeftRight(node); // Left-Right imbalance
    }

    else if (balFactor < -1)
    {

        if (getBalanceFactor(node->right) > 0) // Right-Left imbalance
            node = rotateRightLeft(node);

        else
            node = rotateLeft(node); // Right imbalance
    }
    return node;
}

TreeNode *insertNameID(TreeNode *root, int ID, string name)
{

    if (root == NULL)
    { // If the tree is empty

        TreeNode *temp = new TreeNode(ID, name);
        cout << "successful" << endl;
        return temp;
    }

    if (ID == root->ID)
    { // If there is already an item with the same ID, print "unsuccessful" as per instructions
        cout << "unsuccessful" << endl;
        return root;
    }
    else
    {

        if (ID > root->ID)
        { // If the input ID is bigger than the root ID, we know in a BST to insert to the right subtree
            root->right = insertNameID(root->right, ID, name);
        }

        else
        { // Otherwise it will be going in the left subtree
            root->left = insertNameID(root->left, ID, name);
        }

        root = balanceTree(root);

        return root;
    }
}

TreeNode *searchID(TreeNode *root, int ID)
{

    if (root == nullptr || root->ID == ID)
        return root;

    if (root->ID > ID)
        return searchID(root->left, ID);

    if (root->ID < ID)
        return searchID(root->right, ID);
}

vector<TreeNode> foundNodes;

void searchName(TreeNode *root, string name)
{

    if (root == nullptr)
        return;

    if (root->name == name)
        foundNodes.push_back(*root);

    if (root->left)
        searchName(root->left, name);

    if (root->right)
        searchName(root->right, name);
}

void printSearchStatus(vector<TreeNode> nodesFound, const string &searchType)
{ // Function to print out success statements

    if (nodesFound.empty())
    {
        cout << "unsuccessful" << endl;
    }

    else
    {
        for (int i = 0; i < nodesFound.size(); i++)
        {

            if (searchType == "ID")
            {
                cout << nodesFound[i].name << endl;
            }

            else
            {
                cout << nodesFound[i].ID << endl;
            }
        }
    }
}

TreeNode *findMinVal(TreeNode *node)
{ // This will be used to find the new parent in the BST Delete case of there being two children

    while (node != NULL && node->left != NULL)
    { // We know in a BST that left tree will always be smallest value, so we loop until we reach last value

        node = node->left; // Keep going down the left side of the tree until we reach the end -- this is where node->left is null
    }
    return node;
}

TreeNode *removeID(TreeNode *node, int ID)
{
    if (node == NULL)
    {
        return node;
    }

    if (ID < node->ID)
    { // Recursive call used to find the input ID in the tree -- if it is less than the current root, traverse through left subtree
        node->left = removeID(node->left, ID);
        return node;
    }

    if (ID > node->ID)
    {
        node->right = removeID(node->right, ID);
        return node;
    }

    if (node->ID == ID)
    { // If the ID to be deleted is found

        // Current root has no children
        if (node->right == NULL && node->left == NULL)
        {

            delete node; // Simply delete the node
            cout << "successful" << endl;
            return NULL;
        }

        // Current root has one child
        if (node->right == NULL)
        { // Current root has a left child

            TreeNode *temp = node->left;
            delete node;
            cout << "successful" << endl;
            return temp;
        }

        if (node->left == NULL)
        { // Current root has a right child

            TreeNode *temp = node->right; // The child once again becomes the new root
            delete node;
            cout << "successful" << endl;
            return temp;
        }

        // Current root has two children
        else
        {

            TreeNode *temp = findMinVal(node->right); // The new parent/root will be the node in the right subtree with the minimum value (so traversing through left side of right subtree)
            node->ID = temp->ID;                      // Setting the "successor" to be the new parent/root
            node->name = temp->name;
            node->right = removeID(node->right, temp->ID); // Fixing the rest of the right subtree and essentially moving everything up
            return node;
        }
    }
}

// Global vectors for storing the IDs and names
vector<int> vID;
vector<string> vName;

void getIDs(TreeNode *root)
{ // Storing all the IDs and names in vectors
    if (root != nullptr)
    {

        if (root->left)
        {
            getIDs(root->left);
        }

        vID.push_back(root->ID);
        vName.push_back(root->name);

        if (root->right)
        {
            getIDs(root->right);
        }
    }
}

TreeNode *removeInOrder(TreeNode *root, int nthOrder)
{
    vID.clear();
    getIDs(root);

    if (vID.size() > 0)
    {
        return removeID(root, vID[nthOrder]); // Since we have stored the IDs in a vector, we can access the nth node at the nth index
    }

    else
    {
        return root;
    }
}

vector<string> getParsedInputs()
{
    vector<string> params; // Will store the parsed inputs/parameters in a vector of parsed inputs

    string inputLine, paramOne, paramTwo, paramThree;

    getline(cin, inputLine);

    int firstQuotePos = 0;
    int secondQuotePos = 0;

    int firstSpacePos = inputLine.find_first_of(' '); // The first space separating parameter one from parameter two

    if (firstSpacePos == string::npos)
    { // If there is no space in the inputLine, the entire inputLine is the single input
        params.push_back(inputLine);
    }

    else
    {
        paramOne = inputLine.substr(0, firstSpacePos); // The first parsed input is from the start to where the first space is
        params.push_back(paramOne);                    // Add this parsed input into the vector

        firstQuotePos = inputLine.find_first_of('"', firstSpacePos);

        if (firstQuotePos != string::npos)
        { // As long as the first quote position is there

            secondQuotePos = inputLine.find_first_of('"', firstQuotePos + 1); // We can get the second quote position

            if (secondQuotePos != string::npos)
            {
                paramTwo = inputLine.substr(firstQuotePos + 1, secondQuotePos - firstQuotePos - 1); // Second parsed input is between the quotes
                params.push_back(paramTwo);
            }

            else
            {
                throw runtime_error("Error in input: Ending quote not found!");
            }

            if (inputLine.size() == secondQuotePos + 1)
            { // No more character exists in inputLine
            }

            else
            {
                paramThree = inputLine.substr(secondQuotePos + 2, string::npos); // The third parsed input is from after the second quote until the end of the input
                params.push_back(paramThree);
            }
        }

        else
        { // This is the case if the second input has no quotes and the entire inputLine only consists of two parsed inputs
            paramTwo = inputLine.substr(firstSpacePos + 1, string::npos);
            params.push_back(paramTwo);
        }
    }

    return params;
}

bool isNumeric(const string inputString)
{                                                                       // Helper function for the situation where there are two inputs, and it could either be a name or a number
    return inputString.find_first_not_of("0123456789") == string::npos; // Looking for the first input that is not any of the numbers
}

int main()
{

    int numCommands = 0;
    string inputLine;
    getline(cin, inputLine);

    numCommands = stoi(inputLine);

    TreeNode *root = NULL;

    vector<string> inputParams;

    for (int i = 0; i < numCommands; i++)
    {

        inputParams.clear();
        inputParams = getParsedInputs();

        if (inputParams.size() == 1)
        { // If the input is only a command, it must be one of the three print function or the levelCount function

            if (inputParams[0] == "printInorder")
            {
                printInOrder(root);
                cout << endl;
            }

            if (inputParams[0] == "printPreorder")
            {
                printPreOrder(root);
                cout << endl;
            }

            if (inputParams[0] == "printPostorder")
            {
                printPostOrder(root);
                cout << endl;
            }

            if (inputParams[0] == "printLevelCount")
            {
                cout << calcHeight(root) << endl; // My levelCount function is basically the height function, which is used in balancing the tree
            }
        }

        if (inputParams.size() == 2)
        { // If the parsed inputs are only a command and a name or a command and an ID

            if (inputParams[0] == "search")
            {
                foundNodes.clear(); // clear previous search results

                if (isNumeric(inputParams[1]))
                { // If the second input is an ID

                    if (inputParams[1].size() == 8)
                    { // If the ID is strictly 8 digits long
                        TreeNode *temp = searchID(root, stoi(inputParams[1]));
                        if (temp != NULL)
                            foundNodes.push_back(*temp);

                        printSearchStatus(foundNodes, "ID");
                    }

                    else
                    {
                        throw runtime_error("Trying to search for ID that is not 8 digits long");
                    }
                }

                else
                {
                    searchName(root, inputParams[1]);
                    printSearchStatus(foundNodes, "Name"); // Second input is a name, so we searchName
                }
            }

            if (inputParams[0] == "remove")
            {
                TreeNode *temp = removeID(root, stoi(inputParams[1]));
                root = temp;
            }

            if (inputParams[0] == "removeInorder")
            {
                TreeNode *temp = removeInOrder(root, stoi(inputParams[1]));
                root = temp;
            }
        }

        if (inputParams.size() == 3)
        { // If the string is parsed into three separate inputs (command, name, ID)

            if (inputParams[0] == "insert")
            {

                if (inputParams[2].size() == 8)
                { // ID must be 8 digits long

                    int ID = stoi(inputParams[2]);
                    TreeNode *temp = insertNameID(root, ID, inputParams[1]);
                    root = temp;
                }

                else if (inputParams[2].size() != 8)
                {
                    cout << "ID is not 8 digits long";
                }

                // else { throw runtime_error("ID is not 8 digits long");}
            }
        }
    }

    return 0;
}
