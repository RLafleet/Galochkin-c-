/*{
    20. Реализовать алгоритм нахождения остовного дерева  Прима
    и проиллюстрировать в таблице по шагам этапы его выполнения(10).               

    Назаров Максим Дмитриевич Пс-21

    C++/Visual Studio Code

    -
}*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <string>
#include <conio.h>
#include <sys/time.h>

using namespace std;

struct Edge
{
    int destination;
    int weight;
};

class Graph
{
  private:
    set<int> uniqueData;

  public:
    int vertices;
    vector<vector<Edge>> adjacencyList;

    Graph(int v) : vertices(v), adjacencyList(v) {}

    void readGraph(Edge &edge, std::ifstream &inputFile, std::ofstream &outputFile, std::vector<int> &data)
    {
        std::string line;
        int a, b, c;
        int maxLen = data[0];

        while (getline(inputFile, line))
        {
            std::istringstream iss(line);

            if ((iss >> a) && (iss >> b) && (iss >> c))
            {
                maxLen = std::max(maxLen, static_cast<int>(std::to_string(c).length()));
                maxLen = std::max(maxLen, static_cast<int>(std::to_string(b).length()));
                maxLen = std::max(maxLen, static_cast<int>(std::to_string(a).length()));
                uniqueData.insert(a);
                uniqueData.insert(b);
                addEdge(a, b, c);
            }
        }

        if (!uniqueData.empty())
        {
            const int infinLen = 3;
            maxLen = std::max(infinLen, 2 * maxLen + 4);
            data[0] = maxLen;
            data[1] = uniqueData.size();
            vector<int> data(uniqueData.begin(), uniqueData.end());
            sort(data.begin(), data.end());
            drawTable(outputFile, data, maxLen);
        }

        outputFile << endl;
        std::cout << std::endl;
    }

    void primMST(std::ofstream &outputFile, const std::vector<int> &data)
    {
        const int spaces = data[0];
        const int infinLen = 3;
        vector<bool> visited(vertices, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        const string INFINITY = "inf";
        int src = 0;

        vector<int> parent(vertices, -1);
        vector<int> key(vertices, INT_MAX);

        key[src] = 0;
        pq.push({0, src});

        const int znaks = 2;

        int step = 1;
        timeval currentTime;
        timeval useTime;
        gettimeofday(&useTime, nullptr);
        int par = 0;
        int u = 0;
        int calc = 0;
        vector<int> mas1;
        vector<int> mas;

        while (!pq.empty())
        {
            gettimeofday(&currentTime, nullptr);

            if (((useTime.tv_sec > currentTime.tv_sec + 0.05) || (useTime.tv_sec < currentTime.tv_sec - 0.05)) && (_getch() == 224))
            {
                useTime.tv_sec = currentTime.tv_sec;
                int stepInst = 2;
                div_t delInstr = std::div(step, 2);
                stepInst = delInstr.rem;
                if (delInstr.rem == 1)
                {
                    u = pq.top().second;
                    par = u;
                    visited[u] = true;
                    calc += 1;
                }
                if (delInstr.rem == 0)
                {
                    pq.pop();

                    for (const Edge &edge : adjacencyList[u])
                    {
                        int v = edge.destination;
                        int weight = edge.weight;
                        mas1.push_back(par);
                        mas1.push_back(v);
                        mas1.push_back(weight);

                        if (!visited[v] && weight < key[v])
                        {
                            parent[v] = u;
                            key[v] = weight;
                            pq.push({weight, v});
                        }
                    }
                }
                if (calc <= data[1])
                {
                    mas.clear();
                    div_t del = std::div(spaces, 2);
                    outputFile << "|";
                    std::cout << "|";
                    int elemLen = std::to_string(step).length();
                    drawTableSideRem(outputFile, elemLen, del);
                    outputFile << step;
                    std::cout << step;
                    drawTableSideQuot(outputFile, elemLen, del);
                    outputFile << "|";
                    std::cout << "|";
                    for (int i = 0; i < data[1]; i++)
                    {
                        if (key[i] == INT_MAX)
                            elemLen = infinLen;
                        else
                        {
                            if (key[i] != 0)
                            {
                                elemLen = std::to_string(key[i]).length() + std::to_string(parent[i]).length() + znaks;
                                if (visited[i])
                                    elemLen += znaks;
                            }
                            else
                                elemLen = std::to_string(key[i]).length() + znaks;
                        }

                        drawTableSideQuot(outputFile, elemLen, del);
                        if (key[i] == INT_MAX)
                        {
                            std::cout << INFINITY;
                            outputFile << INFINITY;
                        }
                        else
                        {
                            if (key[i] != 0)
                            {
                                if (visited[i])
                                {
                                    mas.push_back(parent[i]);
                                    mas.push_back(key[i]);
                                    outputFile << "<" << parent[i] << "(";
                                    outputFile << key[i];
                                    outputFile << ")"
                                               << ">";
                                    std::cout << "<" << parent[i] << "(";
                                    std::cout << key[i];
                                    std::cout << ")"
                                              << ">";
                                }
                                else
                                {
                                    outputFile << parent[i] << "(";
                                    outputFile << key[i];
                                    outputFile << ")";
                                    std::cout << parent[i] << "(";
                                    std::cout << key[i];
                                    std::cout << ")";
                                }
                            }
                            else
                            {
                                std::cout << "<" << key[i] << ">";
                                outputFile << "<" << key[i] << ">";
                            }
                        }
                        drawTableSideRem(outputFile, elemLen, del);
                        if (i < data[1] - 1)
                        {
                            outputFile << "|";
                            std::cout << "|";
                        }
                    }
                    outputFile << "|" << std::endl;
                    std::cout << "|" << std::endl;
                    step++;
                    instructions(stepInst, par, mas1);
                }
                else
                    break;
            }
        }
        instructions(3, par, mas1);
        std::cout << "Ребра " << std::endl;
        int sum = 0;
        for (size_t i = 0; 2 * i < mas.size(); i++)
        {
            std::cout << i + 1 << "-" << mas[2 * i] << std::endl;
            sum += mas[2 * i + 1];
        }
        std::cout << "Сумма " << sum << std::endl;
    }

  private:
    void instructions(int stepInst, int &parent, vector<int> &mas)
    {
        int dest = 0;
        int min = INT_MAX;
        if (stepInst == 1)
        {
            for (size_t i = 0; 3 * i < mas.size(); i++)
            {
                std::cout << mas[i] << " ";
                if (mas[3 * i + 1] == parent)
                {
                    if (min > mas[3 * i + 2])
                    {
                        min = mas[3 * i + 2];
                        dest = mas[3 * i];
                    }
                }
            }
        }

        switch (stepInst)
        {
        case 0:
            std::cout << std::endl;
            std::cout << "Обновление доступных для вершины смежных вершин и их веса." << std::endl;
            std::cout << std::endl;
            break;
        case 1:
            std::cout << std::endl;
            std::cout << "Выбор смежной вершины с минимальным весом. Это вершина " << parent;
            std::cout << ". Добавили ребро " << dest << "-" << parent << std::endl;
            std::cout << std::endl;

            break;
        case 2:
            std::cout << std::endl;
            std::cout << "Инициализация исходной вершиной." << std::endl;
            std::cout << std::endl;
            break;
        case 3:
            std::cout << "Неотмеченных вершин больше нет. Конец Алгоритма." << std::endl;
            break;
        default:
            break;
        }
    }

    void addEdge(int source, int destination, int weight)
    {
        Edge edge = {destination, weight};
        adjacencyList[source].push_back(edge);
        edge = {source, weight};
        adjacencyList[destination].push_back(edge);
    }

    void drawTable(std::ofstream &outputFile, const vector<int> &data, const int &CHAR_SPACES)
    {
        div_t del = std::div(CHAR_SPACES, 2);
        int elemLen = 1;
        drawTableNone(outputFile, CHAR_SPACES);
        drawTableSideQuot(outputFile, elemLen, del);
        for (size_t i = 0; i < data.size(); i++)
        {
            outputFile << data[i];
            std::cout << data[i];
            elemLen = std::to_string(data[i]).length();
            drawTableSideRem(outputFile, elemLen, del);
            outputFile << "|";
            std::cout << "|";
            drawTableSideQuot(outputFile, elemLen, del);
        }
    }

    void drawTableNone(std::ofstream &outputFile, const int &CHAR_SPACES)
    {
        outputFile << "|";
        std::cout << "|";
        for (size_t i = 0; i < CHAR_SPACES / 2 - 1; i++)
        {
            outputFile << " ";
            std::cout << " ";
        }
        outputFile << "Step";
        std::cout << "Step";
        for (size_t i = 0; i < CHAR_SPACES / 2 - 1; i++)
        {
            outputFile << " ";
            std::cout << " ";
        }
        outputFile << "|";
        std::cout << "|";
    }

    void drawTableSideQuot(std::ofstream &outputFile, int &elemLen, div_t &del)
    {
        const int NUMBER_SPACES_SIDES = 1;
        div_t delP = std::div(elemLen, 2);
        for (size_t i = delP.quot; i < NUMBER_SPACES_SIDES + del.quot; i++)
        {
            outputFile << " ";
            std::cout << " ";
        }
    }

    void drawTableSideRem(std::ofstream &outputFile, int &elemLen, div_t &del)
    {
        const int NUMBER_SPACES_SIDES = 1;
        div_t delP = std::div(elemLen, 2);

        for (size_t i = delP.quot + delP.rem; i < NUMBER_SPACES_SIDES + del.quot + del.rem; i++)
        {
            outputFile << " ";
            std::cout << " ";
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
        std::cerr << "Error opening files. Check your file. " << std::endl;
        return 0;
    }
    std::cout << "Старт Алгоритма Прима" << std::endl;

    Edge edges;
    int a;
    std::string line;
    getline(inputFile, line);
    std::istringstream iss(line);
    std::vector<int> resultArray(2, 0);
    iss >> a;

    Graph g(a);

    g.readGraph(edges, inputFile, outputFile, resultArray);
    g.primMST(outputFile, resultArray);

    return 0;
}
