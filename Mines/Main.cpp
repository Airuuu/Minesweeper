#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <algorithm>
#include <random>
#include <Windows.h>

using namespace std;
using namespace sf;

bool uncoveredNodes = false;

struct node;

struct edge
{
    node* src;
    vector<node*> dest;
    //int weight;
};

struct node
{
    int covered; // 0 - covered , 1 - flag, 2 - uncovered
    int index;
    vector<edge*> edges;
    int value; // 1-8 save but mine is nearby , 9 - mine, 0 - no mines nearby
    //but textures are : 0 - empty (nothing), 1-8 - numbers (mines nearby), 9 - mine, 10 - block covered, 11 - block flagged
};

class Field
{
private:
    vector<node*> nodes;
    int mineCount = 0;
public:
    vector<node*>& GetNodes()
    {
        return nodes;
    }
    void CreateNode(int index)
    {
        node* newNode = new node();
        newNode->index = index;
        newNode->value = 0;
        newNode->covered = 0;

        nodes.push_back(newNode);
    }


    template<typename T>
    bool contains(vector<T> vec, const T& elem)
    {
        bool result = false;
        for (auto& x : vec)
        {
            if (x == elem)
            {
                result = true;
                break;
            }
        }
        return result;
    }


    void connectNodes() // zoptymalizowac
    {
        node* src;
        vector<node*> dests;
        int id = 0;
        vector<int> obtainable;


        for (auto& node : nodes)
        {
            src = node;
            id = src->index;

            switch (id)
            {
            case (1): //LUC
                obtainable = { id + 1, id + 8, id + 9 };
                break;
            case(8): // RUC
                obtainable = { id - 1, id + 7, id + 8 };
                break;
            case(57): // LLC
                obtainable = { id - 8,id - 7,id + 1 };
                break;
            case(64): // RLC
                obtainable = { id - 9,id - 8,id - 1 };
                break;
            case 9 :case 17: case 25: case 33: case 41 : case 49: // LeB
                obtainable = { id - 8,id - 7,id + 1, id + 9, id + 8 };
                break;
            case 2:case 3: case 4: case 5: case 6: case 7: // UB
                obtainable = { id - 1,id + 7,id + 8, id + 9, id + 1 };
                break;
            case 16:case 24: case 32: case 40: case 48: case 56: // RB
                obtainable = { id - 8,id - 9,id - 1, id + 7, id + 8 };
                break;
            case 58:case 59: case 60: case 61: case 62: case 63: // LoB
                obtainable = { id - 8,id - 7,id + 1, id - 9, id - 1 };
                break;
            default: // default setup - 8 elements around
                obtainable = { id-9, id - 8,id - 7,id - 1, id + 1, id + 7, id + 9, id + 8 };
                break;
            }
            sort(obtainable.begin(), obtainable.end());


            edge* newEdge = new edge();
            newEdge->src = src;
            for (auto& check : nodes)
            {
                if (contains(obtainable, check->index))
                {
                    (newEdge->dest).push_back(check);
                }
            }
            src->edges.push_back(newEdge);
            obtainable.clear();
        }
    }
    void addMines(int mines, int AI_1stmove)
    {
        node* src = nullptr;
        int id = 0;
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<>  rng(1, 64);
        vector<int> mineIndexes;
        int rngHolder;
        int mineCounter = 0;

        vector<int> restricted = { AI_1stmove - 9, AI_1stmove - 8,AI_1stmove - 7,AI_1stmove - 1, AI_1stmove + 1,
            AI_1stmove + 7, AI_1stmove + 9, AI_1stmove + 8, AI_1stmove };

        while (mineCounter < mines)
        {
            rngHolder = rng(mt);
            if ((find(mineIndexes.begin(), mineIndexes.end(), rngHolder) == mineIndexes.end()) && 
                (find(restricted.begin(), restricted.end(), rngHolder) == restricted.end())/*rngHolder != AI_1stmove*/)
            {
                mineIndexes.push_back(rngHolder);
                mineCounter++;
            }
        }
        int valueHolder;
        sort(mineIndexes.begin(), mineIndexes.end());


        mineCounter = 0;
        while (mineCounter != mines)
        {
            for (auto& node : nodes)
            {
                if (node->index == mineIndexes[0])
                    src = node;
            }
            if (src == nullptr)
            {
                cout << "Something went wrong in addMines" << endl;
                exit;
                return;
            }
            for (auto& edge : src->edges)
            {
                if (edge->src->index == mineIndexes[0])
                {
                    edge->src->value = 9;
                    mineCounter++;
                    for (auto& now : edge->dest)
                    {
                        if (now->value != 9)
                        {
                            (now->value)++;
                        }
                    }
                    if (src->index == 1)
                        src = edge->dest[0];
                    else
                        src = edge->dest[1];
                }
                mineIndexes.erase(mineIndexes.begin());
            }
        }
    }

    void printGameBoard()
    {
        int counter = 0;
        for (auto& node : nodes)
        {
            if (counter == 0)
            {
                cout << " | ";
            }
            if (node->covered == 0)
            {
                cout << char(254u) << " | ";
            }
            else if (node->covered == 1)
            {
                cout << "F" << " | ";
            }
            else
            {
                if (node->value == 9)
                {
                    cout << "*" << " | ";
                }
                else
                {
                    cout << node->value << " | ";
                }
            }
            counter++;
            if (counter == 8)
            {
                cout << endl;
                counter = 0;
            }
        }
    }

    ////////////////////////
    /////TEST FUNCTIONS/////
    ////////////////////////

    void testPrintField()
    {
        int counter = 0;
        for (auto& node : nodes)
        {
            if (counter == 0)
            {
                cout << " | ";
            }
            cout << node->index << " | ";
            counter++;
            if (counter == 8)
            {
                cout << endl;
                counter = 0;
            }
        }
    }

    void testPrintMinefield()
    {
        int counter = 0;
        for (auto& node : nodes)
        {
            cout << node->value << " | ";
            counter++;
            if (counter == 8)
            {
                cout << endl;
                counter = 0;
            }
        }
    }

    void testEdges(int nodeNumber)
    {
        node* src = nullptr;
        for (auto& node : nodes)
        {
            if (node->index == nodeNumber)
                src = node;
        }
        if (src == nullptr)
        {
            cout << "Something went wrong in addMines" << endl;
            exit;
            return;
        }
        for (auto& edge : src->edges)
        {
            for (auto& now : edge->dest)
            {
                cout << "Dest " << now->index << endl;
            }
        }
    }
    //---------------------------------------------------------------------
};
void printGameBoard(vector<node*> nodes)
{
    int counter = 0;
    for (auto& node : nodes)
    {
        if (counter == 0)
        {
            cout << " | ";
        }
        if (node->covered == 0)
        {
            cout << char(254u) << " | ";
        }
        else if (node->covered == 1)
        {
            cout << "F" << " | ";
        }
        else
        {
            if (node->value == 9)
            {
                cout << "*" << " | ";
            }
            else
            {
                cout << node->value << " | ";
            }
        }
        counter++;
        if (counter == 8)
        {
            cout << endl;
            counter = 0;
        }
    }
}

/*bool bombAround(node* source)
{
    for (auto& node : source->edges)
    {
        for (auto& now : node->dest)
        {
            if (now->value == 9)
                return true;
        }
    }
    return false;
}*/

vector<node*> AI_uncover(vector<node*> nodes ,node* n)
{
    n->covered = 2;
    for (auto& edge : n->edges)
    {
        for (auto& now : edge->dest)
        {
            now->covered = 2;
            for (auto& node : now->edges)
            {
                for (auto& unc : node->dest)
                {
                    if (unc->value == 0 && unc->covered != 2)
                        nodes = AI_uncover(nodes, unc);
                }
            }
        }
    }
    return nodes;
}

int getFreeNode(vector<node*> nodes)
{
    cout << "inside getfree" << endl;
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<>  rng(1, 64);
    int rngHolder;
    rngHolder = rng(mt);

    while (true)
    {
        for (auto& node : nodes)
        {
            if (node->index == rngHolder && node->covered == 0)
                return node->index;
            else break;
        }
        rngHolder = rng(mt);
    }
}

vector<node*> AI_randomMove(Field &f, vector<node*> nodes, int freeNode)
{
    if(freeNode == 0)
        int freeNode = getFreeNode(nodes);

    vector<node*> copy = f.GetNodes();

    for (auto& node : nodes)
    {

        if (node->index == freeNode)
        {

            switch (node->value)
            {
            case 9: // gameover TODO/////////////
                node->covered = 2;
                break;

            default:
                node->covered = 2;
                copy = AI_uncover(nodes, node);
                break;
            }
        }
    }
    nodes = copy;
    return nodes;
}

void AI_flagging(Field f)
{
    cout << "yes" << endl;
}

vector<node*> AI_move(Field f, vector<node*> nodes, int rngHolder, int nodeAImove)
{
    if (uncoveredNodes)
        AI_flagging(f);
    if(nodeAImove == 0)
        nodes = AI_randomMove(f, nodes, rngHolder);


    return nodes;
}

void AI_GameStart(Field f, int nodesTotal, int mines)
{
    //f.printGameBoard();
    //cout << endl;
    int nodeAImove = 0;
    int freeNodes = nodesTotal - mines;
    int minesFlagged = 0;
    int minesLeft = mines;
    int movesCounter = 0;

    //1st move is always random, during that, generate the GameBoard and set mines
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<>  rng(1, 64);
    int rngHolder;
    rngHolder = rng(mt); // 1st move

    for (int i = 1; i <= nodesTotal; i++)
    {
        f.CreateNode(i);
    }
    f.connectNodes();
    f.addMines(mines, rngHolder);
    //cout << rngHolder << endl;
    vector<node*> nodes = f.GetNodes();
    nodes = AI_move(f, nodes, rngHolder, nodeAImove);
    ++nodeAImove;

    printGameBoard(nodes);
    cout << nodeAImove << endl << endl << endl;

    while (minesLeft != 0)
    {
        Sleep(2000);
        nodes = AI_move(f, nodes, rngHolder, nodeAImove);
        //nodeAImove = AI_move(f, nodeAImove, 0);
        //cout << minesLeft << endl;
        //minesLeft--;
        //cout << "\x1B[2J\x1B[H";
        f.printGameBoard();
        cout << ++nodeAImove << endl << endl << endl;
        break;
    }
}

int main(int argc, char* args[])
{
    int size = 8;
    int mines = 15;
    Field f;
    /*for (int i = 1; i <= pow(size, 2); i++)
    {
        f.CreateNode(i);
    }
    f.connectNodes();
    f.addMines(mines);*/

    //int nodeNumber = 1;
    //f.testEdges(nodeNumber);
    
    
    //f.printField();
    //cout << endl;
    //f.testPrintMinefield();
    AI_GameStart(f, pow(size,2), mines);

    return 0;
}