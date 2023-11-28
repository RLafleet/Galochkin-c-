/*{
    2. Ввести  произвольное  сильно  ветвящееся дерево.  Выдать списки вершин:
    1) являющихся листьями;
    2) не являющихся листьями;
    3) родителей листьев;
    4) заданного уровня, считая от вершины (8).                  

    Назаров Максим Дмитриевич Пс-21

    C++/Visual Studio Code

    -
}*/

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

class TreeNode
{
  public:
    char data;
    vector<TreeNode *> children;

    TreeNode(char data)
    {
        this->data = data;
    }
};

class Tree
{
  public:
    TreeNode *root;

    Tree(char data)
    {
        root = new TreeNode(data);
    }

    void addChildren(char parentData, char childData)
    {
        TreeNode *parent = findNode(root, parentData);
        TreeNode *child = new TreeNode(childData);
        parent->children.push_back(child);
    }

    TreeNode *findNode(TreeNode *node, char data)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->data == data)
        {
            return node;
        }
        for (TreeNode *child : node->children)
        {
            TreeNode *found = findNode(child, data);
            if (found != nullptr)
            {
                return found;
            }
        }
        return nullptr;
    }

    vector<char> getLeaves()
    {
        vector<char> leaves;
        getLeavesFunc(root, leaves);
        return leaves;
    }

    void getLeavesFunc(TreeNode *node, vector<char> &leaves)
    {
        if (node == nullptr)
        {
            return;
        }
        if (node->children.size() == 0)
        {
            leaves.push_back(node->data);
            return;
        }
        for (TreeNode *child : node->children)
        {
            getLeavesFunc(child, leaves);
        }
    }

    vector<char> getNonLeaves()
    {
        vector<char> nonLeaves;
        getNonLeavesFunc(root, nonLeaves);
        return nonLeaves;
    }

    void getNonLeavesFunc(TreeNode *node, vector<char> &nonLeaves)
    {
        if (node == nullptr || node->children.size() == 0)
        {
            return;
        }
        nonLeaves.push_back(node->data);
        for (TreeNode *child : node->children)
        {
            getNonLeavesFunc(child, nonLeaves);
        }
    }

    vector<char> getParentsOfLeaves()
    {
        vector<char> parents;
        getParentsOfLeavesFunc(root, parents);
        return parents;
    }

    void getParentsOfLeavesFunc(TreeNode *node, vector<char> &parents)
    {
        if (node == nullptr)
        {
            return;
        }
        if (node->children.size() > 0)
        {
            bool hasLeafChild = false;
            for (TreeNode *child : node->children)
            {
                if (child->children.size() == 0)
                {
                    hasLeafChild = true;
                    break;
                }
            }
            if (hasLeafChild)
            {
                parents.push_back(node->data);
            }
            for (TreeNode *child : node->children)
            {
                getParentsOfLeavesFunc(child, parents);
            }
        }
    }

    vector<char> getLevel(int level)
    {
        vector<char> levelNodes;
        getLevelFunc(root, 0, level, levelNodes);
        return levelNodes;
    }

    void getLevelFunc(TreeNode *node, int currentLevel, int targetLevel, vector<char> &levelNodes)
    {
        if (node == nullptr)
        {
            return;
        }
        if (currentLevel == targetLevel)
        {
            levelNodes.push_back(node->data);
            return;
        }
        for (TreeNode *child : node->children)
        {
            getLevelFunc(child, currentLevel + 1, targetLevel, levelNodes);
        }
    }
    void drawTree(ofstream &outputFile)
    {
        if (root == nullptr)
        {
            outputFile << "Tree is empty." << endl;
            return;
        }
        drawTreeFunc(root, "", true, outputFile);
    }

  private:
    void drawTreeFunc(TreeNode *node, string indent, bool last, ofstream &outputFile)
    {
        if (node != nullptr)
        {
            outputFile << indent;
            if (last)
            {
                outputFile << "\\-";
                indent += "  ";
            }
            else
            {
                outputFile << "|-";
                indent += "| ";
            }
            outputFile << node->data << endl;
            for (size_t i = 0; i < node->children.size(); ++i)
            {
                drawTreeFunc(node->children[i], indent, i == node->children.size() - 1, outputFile);
            }
        }
    }
};

void readAndBuildTree(Tree &tree, ifstream &inputFile)
{
    string line;
    vector<TreeNode *> levelNodes;
    levelNodes.push_back(tree.root);

    while (getline(inputFile, line))
    {
        int depth = 0;
        while (line[depth] == '*')
            depth++;

        if (depth > 0)
        {
            char childData = line[depth];
            TreeNode *parent = levelNodes[depth - 1];

            if (!parent)
            {
                cerr << "Parent not found for " << childData << endl;
                continue;
            }

            TreeNode *child = new TreeNode(childData);
            parent->children.push_back(child);

            if (levelNodes.size() > depth)
            {
                levelNodes[depth] = child;
            }
            else
            {
                levelNodes.push_back(child);
            }
        }
    }
}

void option1()
{
    string inputFileName;
    string outputFileName;

    cout << "Write input and output files: ";
    cin >> inputFileName >> outputFileName;

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile)
    {
        cerr << "Error opening files." << endl;
        return;
    }

    int groupCount;
    char treeRoot;
    inputFile >> groupCount;
    inputFile >> treeRoot;
    Tree tree(treeRoot);
    readAndBuildTree(tree, inputFile);
    tree.drawTree(outputFile);
}

void option2()
{
    string inputFileName;
    string outputFileName;

    cout << "Write input and output files: ";
    cin >> inputFileName >> outputFileName;

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile)
    {
        cerr << "Error opening files." << endl;
        return;
    }
    int groupCount;
    char treeRoot;

    inputFile >> groupCount;
    inputFile >> treeRoot;
    Tree tree(treeRoot);
    readAndBuildTree(tree, inputFile);

    vector<char> leaves = tree.getLeaves();
    outputFile << "Leaves: ";
    for (char leaf : leaves)
    {
        outputFile << leaf << " ";
    }
    outputFile << endl;

    vector<char> nonLeaves = tree.getNonLeaves();
    outputFile << "Non-leaves: ";
    for (char nonLeaf : nonLeaves)
    {
        outputFile << nonLeaf << " ";
    }
    outputFile << endl;

    vector<char> parentsOfLeaves = tree.getParentsOfLeaves();
    outputFile << "Parents of leaves: ";
    for (char parent : parentsOfLeaves)
    {
        outputFile << parent << " ";
    }
    outputFile << endl;

    int level = groupCount;
    vector<char> levelNodes = tree.getLevel(level);
    outputFile << "Level " << level << ": ";
    for (char node : levelNodes)
    {
        outputFile << node << " ";
    }
    outputFile << endl;
};

int main()
{
    cout << "Hello. Choose the option you want: " << endl;
    cout << "1. Show tree" << endl;
    cout << "2. Write leafes, non-leafes, parents of leafes, set level" << endl;

    char option;
    cin >> option;
    if (option == '1')
    {
        option1();
    }
    if (option == '2')
    {
        option2();
    }
    return 0;
}
