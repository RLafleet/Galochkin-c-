/*{
    24. На  плоскости  расположена  система  из  N  шестеренок,
    которая  приводится  в  движение  вращением  шестеренки  1  по
    часовой стрелке.  Сцепленные шестеренки могут вращаться только
    в    противоположных    направлениях.   Требуется   определить
    направление движения каждой шестеренки  либо  установить,  что
    систему   заклинит.   Некоторые   шестеренки   могут  остаться
    неподвижными (11).                    

    Назаров Максим Дмитриевич Пс-21

    C++/Visual Studio Code

    -
}*/

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

struct Gear
{
    int id;
    bool clockwise;
    Gear(int i, bool c) : id(i), clockwise(c) {}
};

void getMovementGears(istream &inputFile, ostream &outputFile)
{
    int groupCount;
    inputFile >> groupCount;

    vector<vector<int>> gearInfo(groupCount);
    map<int, set<int>> gearConnections;
    set<int> allGears;

    string line;
    getline(inputFile, line);
    for (int i = 0; i < groupCount; i++)
    {
        if (!getline(inputFile, line))
        {
            break;
        }
        stringstream ss(line);
        int a, b;
        ss >> a;
        gearInfo[i].push_back(a);
        gearConnections[a];
        if (ss >> b)
        {
            gearInfo[i].push_back(b);
            gearConnections[a].insert(b);
            gearConnections[b].insert(a);
        }
    }

    vector<Gear> gearQueue;
    set<int> visited;
    map<int, bool> gearDirections;
    gearQueue.push_back(Gear(1, true));
    visited.insert(1);
    gearDirections[1] = true;

    map<int, string> outputMessagesGears;

    for (size_t front = 0; front < gearQueue.size(); front++)
    {
        Gear currentGear = gearQueue[front];

        string movement = " moves ";
        movement += (currentGear.clockwise ? "clockwise" : "counterclockwise");

        const set<int> &connectedGears = gearConnections[currentGear.id];
        for (int connectedGear : connectedGears)
        {
            bool oppositeDirection = !currentGear.clockwise;
            if (visited.find(connectedGear) == visited.end())
            {
                gearQueue.push_back(Gear(connectedGear, oppositeDirection));
                visited.insert(connectedGear);
                gearDirections[connectedGear] = oppositeDirection;
            }
            else if (gearDirections[connectedGear] == currentGear.clockwise)
            {
                outputFile << "The mechanism is stuck." << endl;
                return;
            }
        }
        outputMessagesGears[currentGear.id] = movement;
    }

    for (const auto &connection : gearConnections)
    {
        allGears.insert(connection.first);
    }

    for (int gearId : allGears)
    {
        if (visited.find(gearId) == visited.end())
        {
            outputMessagesGears[gearId] = " doesn't work";
        }
    }

    for (const auto &entry : outputMessagesGears)
    {
        outputFile << entry.first << entry.second << endl;
    }

    cout << "The mechanism is not stuck. Check output file" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Use for input these form: " << argv[0] << " input_filename.txt output_filename.txt" << endl;
        return 1;
    }

    const string inputFileName = argv[1];
    const string outputFileName = argv[2];

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile)
    {
        cerr << "Error opening files." << endl;
        return 1;
    }

    getMovementGears(inputFile, outputFile);

    return 0;
}
