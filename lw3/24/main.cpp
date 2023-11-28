/*{
     24. В  листьях  И-ИЛИ  дерева,  соответствующего некоторому
    множеству  конструкций,  заданы   значения   массы.   Известно
    максимально допустимое значение массы изделия. Требуется усечь
    дерево   так,   чтобы   дерево    включало    все    элементы,
    соответствующие  допустимым  значениям массы,  но не содержало
    "лишних" вершин.  Конечное дерево выдать на экран в  наглядном
    виде (13).                

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
    std::string mass;
    int maxMassD;
    int minMassD;
    std::vector<TreeNode *> children;

    TreeNode(char data, std::string mass = "n", int maxMassD = 0, int minMassD = 0) : data(data), mass(mass), maxMassD(maxMassD), minMassD(minMassD) {}
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

    void drawTree(std::ofstream &outputFile)
    {
        if (root == nullptr)
        {
            outputFile << "Tree is empty." << endl;
            return;
        }
        drawTreeFunc(root, "", true, outputFile);
    }

  private:
    void drawTreeFunc(TreeNode *node, std::string indent, bool last, std::ofstream &outputFile)
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
            if (node->mass[0] >= '0' && node->mass[0] <= '9')
            {
                outputFile << node->data << " <Mass: " << node->mass << ">" << endl;
            }
            else
            {
                outputFile << node->data << " <Range [ "
                           << node->minMassD << ", " << node->maxMassD << "]>" << endl;
            }
            for (size_t i = 0; i < node->children.size(); ++i)
            {
                drawTreeFunc(node->children[i], indent, i == node->children.size() - 1, outputFile);
            }
        }
    }
};

bool massCheck(const int &mass, int &massConstruction, const int &availableMass)
{
    massConstruction += mass;
    if (massConstruction > availableMass)
    {
        massConstruction -= mass;
        return false;
    }
    else
    {
        return true;
    }
}

void readAndBuildTree(Tree &tree, std::ifstream &inputFile)
{
    std::string line;
    std::vector<TreeNode *> levelNodes;
    levelNodes.push_back(tree.root);
    bool rootMassCheck = false;
    int maxMassD = 0;

    while (getline(inputFile, line))
    {
        int depth = 0;
        while (line[depth] == '*')
        {
            depth++;
        }

        std::istringstream stream(line.substr(depth));
        char childData;
        std::string stateTree;

        int mass;
        if (!rootMassCheck)
        {
            stream >> stateTree;
            if (stateTree[0] >= '0' && stateTree[0] <= '9')
            {
                mass = std::stoi(stateTree);
                stateTree = std::to_string(mass);
            }

            tree.root->mass = stateTree;
            rootMassCheck = !rootMassCheck;
        }
        else
        {
            stream >> childData >> stateTree;
            if (stateTree[0] >= '0' && stateTree[0] <= '9')
            {
                mass = std::stoi(stateTree);
                stateTree = std::to_string(mass);
            }
        }

        if (depth > 0)
        {
            TreeNode *parent = levelNodes[depth - 1];

            if (!parent)
            {
                cerr << "Parent not found for " << childData << endl;
                continue;
            }

            TreeNode *child = new TreeNode(childData, stateTree, maxMassD);
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

bool trimTree(TreeNode *node, const int &maxMass, int &currentMass, bool &state, int &maximum)
{
    if (node == nullptr)
        return true;

    if (node->children.empty())
    {
        int nodeMass = stoi(node->mass);
        node->minMassD = nodeMass;
        currentMass = (nodeMass <= maxMass) ? nodeMass : 0;
        maximum = currentMass;
        return true;
    }

    if (node->mass == "a")
    {
        int totalMass = 0;
        int totalMassMax = 0;
        bool allValid = true;
        for (auto &child : node->children)
        {
            int childMass = 0;
            int childMassMax = 0;
            trimTree(child, maxMass, childMass, state, childMassMax);
            totalMass += childMass;
            totalMassMax += childMassMax;
        }
        if (totalMass > maxMass)
        {
            state = false;
        }
        else
        {
            currentMass = totalMass;
            maximum = totalMassMax;
        }
        if (node->mass == "a")
        {
            node->minMassD = currentMass;
            node->maxMassD = maximum;
            if (state)
                return true;
            else
                return false;
        }
        // currentMass = (allValid && totalMass <= maxMass) ? totalMass : 0;
    }
    else if (node->mass == "o")
    {
        int minMass = INT_MAX;
        int maxMassP = 0;
        std::vector<TreeNode *> minMassChildren;
        for (auto &child : node->children)
        {
            int childMass = 0;
            int childMassMax = 0;
            trimTree(child, maxMass, childMass, state, childMassMax);
            if (childMassMax > maxMassP)
            {
                maxMassP = childMassMax;
                node->maxMassD = maxMassP;
            }
            if (childMass > 0 && childMass <= minMass)
            {
                if (childMass < minMass)
                {
                    minMass = childMass;
                    node->minMassD = minMass;
                }
                // minMassChildren.push_back(child);
            }
            minMassChildren.push_back(child);
        }
        if (!minMassChildren.empty())
        {
            node->children = minMassChildren;
            currentMass = minMass;
            maximum = maxMassP;
        }
        else
        {
            currentMass = 0;
            maximum = 0;
        }
    }
}

bool trim(TreeNode *node, int &maxMass, int &currentMass, bool &changeState, bool &state)
{
    char name;
    if (node == nullptr)
        return true;

    if (node->children.empty())
    {
        int nodeMass = stoi(node->mass);
        currentMass = nodeMass;
    }
    if (node->mass == "a")
    {
        bool state2 = false;
        if (state)
        {
            state2 = true;
            char name = node->data;
        }
        state = false;
        int totalMass = 0;
        for (auto &child : node->children)
        {
            maxMass -= child->minMassD;
            cout << "check " << child->minMassD << endl;
        }
        for (auto &child : node->children)
        {
            int childMass = 0;
            maxMass += child->minMassD;
            trim(child, maxMass, childMass, changeState, state);
            if ((state2 && name == node->data) || (changeState && child->mass != "a"))
            {
                maxMass -= child->minMassD;
            }
            totalMass += childMass;
        }
        currentMass = totalMass;
    }
    else if (node->mass == "o")
    {
        int mass;
        std::vector<TreeNode *> permissibleChildren;
        for (auto &child : node->children)
        {
            int childMass = 0;
            changeState = false;
            trim(child, maxMass, childMass, changeState, state);
            changeState = true;
            cout << "childMass " << childMass << " maxMass " << maxMass << endl;
            mass = childMass;
            if (childMass > 0 && childMass <= maxMass)
            {
                permissibleChildren.push_back(child);
            }
        }

        node->children = permissibleChildren;

        if (!permissibleChildren.empty())
        {
            currentMass = mass;
        }
        else
        {
            currentMass = 0;
        }
    }
}

bool option()
{
    std::string inputFileName;
    std::string outputFileName;
    int maxPermissibleMass = 0;

    // std::cout << "Write input and output files: " <<;
    // std::cin >> inputFileName >> outputFileName;
    // std::endl;
    inputFileName = "input.txt";
    outputFileName = "output.txt";
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile)
    {
        cerr << "Error opening files. Try again." << endl;
        return false;
    }

    std::cout << "Enter available mass: ";
    std::cin >> maxPermissibleMass;

    if (std::cin.fail())
    {
        std::cerr << "Invalid input. Please enter a valid integer." << std::endl;
        std::cin.clear();
        return false;
    }

    char treeRoot;
    bool readAndBuild = false;
    inputFile >> treeRoot;
    Tree tree(treeRoot);
    readAndBuildTree(tree, inputFile);
    int dummyMass = 0;
    bool a = true;
    bool state = true;
    int maximum = 0;
    a = trimTree(tree.root, maxPermissibleMass, dummyMass, state, maximum);
    if (a)
    {
        dummyMass = 0;
        state = true;
        bool changeState = true;
        trim(tree.root, maxPermissibleMass, dummyMass, changeState, state);
        tree.drawTree(outputFile);
        return true;
    }
    std::cout << "Mass is bigger than possible!" << std::endl;
    return false;
}

int main()
{
    std::cout << "Hello! " << std::endl;
    bool b = option();
    if (b)
    {
        std::cout << "Algoritm works!" << std::endl;
    }
    return 0;
}
