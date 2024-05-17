/*{
    14. В файле задана  строка  из  целых  чисел  через  пробел.
    Реализовать следующие функции:
        1) построение АВЛ-дерева;
        2) добавление нового элемента с перестроением дерева;
        3) изменение значения заданного элемента с перестроением дерева;
        4) выдачу на экран АВЛ-дерева (11).              

    Назаров Максим Дмитриевич Пс-21

    C++/Visual Studio Code

    -
}*/

#include <iostream>
#include <fstream>
#include <sstream>

class Tree
{
  public:
    struct TreeNode
    {
        int data;
        int height;
        TreeNode *l;
        TreeNode *r;
    };

    TreeNode *root;
    void initTree()
    {
        root = nullptr;
    }

    void readTree(std::ifstream &inputFile, std::ofstream &outputFile)
    {
        int weight;
        std::string dataLine;
        getline(inputFile, dataLine);
        std::istringstream line(dataLine);
        int LRDraw = 0;
        while (line >> weight)
        {
            addElementTree(weight);
            outputFile << std::endl;
            outputFile << "New tree" << std::endl;
            drawTree(getRoot(), "", true, outputFile, LRDraw);
        }
        std::cout << "Check output file." << std::endl;
        outputFile << std::endl;
    }

  private:
    void addElementTree(int weight)
    {
        root = addNode(root, weight);
    }

    TreeNode *addNode(TreeNode *curNode, int weight)
    {
        const int BALANCE_THRESHOLD = 2;
        if (curNode == nullptr)
        {
            curNode = new TreeNode;
            curNode->data = weight;
            curNode->height = 0;
            curNode->l = curNode->r = nullptr;
        }
        else if (weight > curNode->data)
        {
            curNode->r = addNode(curNode->r, weight);
            if (height(curNode->r) - height(curNode->l) == BALANCE_THRESHOLD)
            {
                if (weight > curNode->r->data)
                    curNode = LRotate(curNode);
                else
                    curNode = LLRotate(curNode);
            }
        }
        else if (weight < curNode->data)
        {
            curNode->l = addNode(curNode->l, weight);
            if (height(curNode->l) - height(curNode->r) == BALANCE_THRESHOLD)
            {
                if (weight < curNode->l->data)
                    curNode = RRotate(curNode);
                else
                    curNode = RRRotate(curNode);
            }
        }

        curNode->height = std::max(height(curNode->l), height(curNode->r)) + 1;
        return curNode;
    }

    TreeNode *getRoot()
    {
        return root;
    }

    TreeNode *LRotate(TreeNode *&curNode)
    {
        if (curNode->r != NULL)
        {
            TreeNode *anotherRoot = curNode->r;
            curNode->r = anotherRoot->l;
            if (height(curNode->l) > height(curNode->r))
                curNode->height = height(curNode->l) + 1;
            else
                curNode->height = height(curNode->r) + 1;
            anotherRoot->l = curNode;
            if (height(anotherRoot->l) > height(anotherRoot->r))
                anotherRoot->height = height(anotherRoot->l) + 1;
            else
                anotherRoot->height = height(anotherRoot->r) + 1;
            return anotherRoot;
        }
        return curNode;
    }

    TreeNode *RRotate(TreeNode *&curNode)
    {
        if (curNode->l != NULL)
        {
            TreeNode *anotherRoot = curNode->l;
            curNode->l = anotherRoot->r;
            if (height(curNode->l) > height(curNode->r))
                curNode->height = height(curNode->l) + 1;
            else
                curNode->height = height(curNode->r) + 1;
            anotherRoot->r = curNode;
            if (height(anotherRoot->l) > height(anotherRoot->r))
                anotherRoot->height = height(anotherRoot->l) + 1;
            else
                anotherRoot->height = height(anotherRoot->r) + 1;
            return anotherRoot;
        }
        return curNode;
    }

    TreeNode *RRRotate(TreeNode *&curNode)
    {
        if (curNode->l->r != NULL)
        {
            TreeNode *anotherRoot = curNode->l->r;
            curNode->l->r = anotherRoot->l;
            if (height(curNode->l->l) > height(curNode->l->r))
                curNode->l->height = height(curNode->l->l) + 1;
            else
                curNode->l->height = height(curNode->l->r) + 1;
            anotherRoot->l = curNode->l;
            if (height(anotherRoot->l) > height(anotherRoot->r))
                anotherRoot->height = height(anotherRoot->l) + 1;
            else
                anotherRoot->height = height(anotherRoot->r) + 1;
            curNode->l = anotherRoot;
        }
        return RRotate(curNode);
    }

    TreeNode *LLRotate(TreeNode *&curNode)
    {
        if (curNode->r->l != NULL)
        {
            TreeNode *anotherRoot = curNode->r->l;
            curNode->r->l = anotherRoot->r;
            if (height(curNode->r->l) > height(curNode->r->r))
                curNode->r->height = height(curNode->r->l) + 1;
            else
                curNode->r->height = height(curNode->r->r) + 1;
            anotherRoot->r = curNode->r;
            if (height(anotherRoot->l) > height(anotherRoot->r))
                anotherRoot->height = height(anotherRoot->l) + 1;
            else
                anotherRoot->height = height(anotherRoot->r) + 1;
            curNode->r = anotherRoot;
        }
        return LRotate(curNode);
    }

    TreeNode *minElementTree(TreeNode *curNode)
    {
        if (curNode == nullptr)
            return nullptr;
        else if (curNode->l == nullptr)
            return curNode;
        else
            return minElementTree(curNode->l);
    }

    int height(TreeNode *curNode)
    {
        if (curNode == nullptr)
            return -1;
        else
            return curNode->height;
    }

    void drawTree(TreeNode *curNode, std::string indent, bool last, std::ofstream &outputFile, int &LRDraw)
    {
        if (curNode != nullptr)
        {
            if (last)
            {
                if (LRDraw == 1)
                {
                    outputFile << indent;
                    outputFile << "|" << std::endl;
                }
                outputFile << indent;
                outputFile << "|-";
                outputFile << curNode->data << std::endl;
                indent += "|";
            }
            else
            {
                outputFile << indent;
                outputFile << "|-";
                outputFile << curNode->data << std::endl;
                if (LRDraw == 1)
                {
                    outputFile << indent;
                    outputFile << "|" << std::endl;
                }
                indent += "|";
            }
            LRDraw = 0;
            if (curNode->l)
                LRDraw += 1;
            if (curNode->r)
                LRDraw += 1;

            drawTree(curNode->l, indent, false, outputFile, LRDraw);
            drawTree(curNode->r, indent, true, outputFile, LRDraw);
        }
    }
};

int main()
{
    std::string inputFileName;
    std::string outputFileName;

    inputFileName = "input.txt";
    outputFileName = "output.txt";

    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile)
    {
        std::cerr << "Error opening files. Check your files. " << std::endl;
        return 0;
    }

    Tree tree;
    tree.initTree();
    tree.readTree(inputFile, outputFile);
    return 0;
}
