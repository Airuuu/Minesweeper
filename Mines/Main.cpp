#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <algorithm>
#include <random>
#include <Windows.h>

using namespace std;

struct node;

struct edge
{
    node* src;
    vector<node*> dest;
};

struct node
{
    int covered; // 0 - covered , 1 - flag, 2 - uncovered
    int flagsAround;
    int index;
    vector<edge*> edges;
    int value; // 1-8 save but mine is nearby , 9 - mine, 0 - no mines nearby
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
        newNode->flagsAround = 0;

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
                (find(restricted.begin(), restricted.end(), rngHolder) == restricted.end()))
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
    /////////IGNORE/////////
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
    ////////////////////////
    ///TEST FUNCTIONS END///
    ////////////////////////
};
int minesFlaggedCorrectly(vector<node*> nodes)
{
    int counter = 0;
    for (auto& node : nodes)
    {
        if (node->value == 9 && node->covered == 1)
        {
            counter++;
        }
    }
    return counter;
}

int countFlagged(vector<node*> nodes)
{
    int counter = 0;
    for (auto& node : nodes)
    {
        if (node->covered == 1)
        {
            counter++;
        }
    }
    return counter;
}

void isGameOver(vector<node*> nodes, int nodeAImove, int mines)
{
    bool anyCovered = false;
    int minesFlagged = countFlagged(nodes);
    int minesLeft = mines - minesFlaggedCorrectly(nodes);
    for (auto& node : nodes)
    {
        //check remaining bombs and compare to remaining fields instead
        if (node->covered == 0)
        {
            anyCovered = true;
        }

        if (node->value == 9 && node->covered == 2)
        {
            cout << "GAME OVER, SAPPER IS WRONG ONLY ONCE!" << endl;
            cout << "Moves : " << nodeAImove << endl <<
                "Flags : " << minesFlagged << endl <<
                "Mines left : " << minesLeft << endl <<
                "Explosion at node: " << node->index << endl << endl << endl;
            exit(9);
        }
    }
    if (!anyCovered)
    {
        cout << "COMPUTER WON!" << std::endl;

        cout << "Moves : " << nodeAImove << endl <<
            "Flags : " << minesFlagged << endl <<
            "Mines left : " << minesLeft << endl;
        exit(0);
    }
}

void printGameBoard(vector<node*> nodes)
{
    int counter = 0;
    int boardsize = sqrt(nodes.size());
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
        if (counter == boardsize)
        {
            cout << endl;
            counter = 0;
        }
    }
}

vector<node*> AI_uncover(vector<node*> &nodes ,node* n)
{
    if (n->covered == 1)
    {
        return nodes;
    }
    if (n->covered == 0)
    {
        n->covered = 2;
        if (n->value == 0) // works - marker for me (if error occurs, its propably here)
        {
            AI_uncover(nodes, n);
        }
        return nodes;
    }
    for (auto& edge : n->edges)
    {
        for (auto& now : edge->dest)
        {
            if (now->covered == 0)
            {
                now->covered = 2;
                if (now->value == 0)
                {
                    AI_uncover(nodes, now);
                }
            }
        }
    }
    return nodes;
}

int getFreeNode(vector<node*> nodes)
{
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<>  rng(1, nodes.size());
    int rngHolder;


    vector<int> freeNodes;
    for (auto& node : nodes)
    {
        if (node->covered == 0)
            freeNodes.push_back( node->index);
    }

    while (true)
    {
        rngHolder = rng(mt);
        if (find(freeNodes.begin(), freeNodes.end(), rngHolder) != freeNodes.end())
        {
            return rngHolder;
        }
    }

}

vector<node*> AI_randomMove(Field &f, vector<node*> nodes, int freeNode)
{
    bool uncover = true;
    if (freeNode == 0) uncover = false;
    if (freeNode == 0)
        freeNode = getFreeNode(nodes);
    cout << "RANDOM MOVE AT " << freeNode << endl;

    vector<node*> copy;

    for (auto& node : nodes)
    {

        if (node->index == freeNode)
        {
            if (uncover) node->covered = 2;
            copy = AI_uncover(nodes, node);
            break;

        }
    }
    nodes = copy;
    return nodes;
}

int checkNodesAround(node* node, bool checkFlagged = false)
{
    int freeNodeCheck = 0;
    for (auto& edge : node->edges)
    {
        for (auto& now : edge->dest)
        {
            if (now->covered == 0 || (checkFlagged && now->covered == 1))
                freeNodeCheck++;
            
        }
    }
    return freeNodeCheck;
}

vector<node*> AI_flagging(vector<node*> nodes)
{
    vector<node*> uncoveredAroundMines;
    for (auto& node : nodes)
    {
        if ((node->covered == 2 && node->value != 9) && node->value != 0)
            uncoveredAroundMines.push_back(node);
        
    }
    int nodeValue = 0;
    int freeNodeCheck;

    for (auto& node : uncoveredAroundMines)
    {
        nodeValue = node->value;
        freeNodeCheck = checkNodesAround(node, true);
        if (freeNodeCheck == nodeValue)
        {
            for (auto& edge : node->edges)
            {
                for (auto& now : edge->dest)
                {
                    if (now->covered == 0)
                    {
                        now->covered = 1;
                        for (auto& nowEdge : now->edges)
                        {
                            for (auto& newNow : nowEdge->dest)
                            {
                                (newNow->flagsAround)++;
                            }
                        }
                    }
                }
            }
        }
    }
    return nodes;
}


bool movePossible(node* n)
{
    for (auto& edge : n->edges)
    {
        for (auto& now : edge->dest)
        {
            if (now->covered == 0)
                return true;
        }
    }
    return false;
}

vector<node*> AI_makeAmove(vector<node*> nodes, Field &f)
{
    node* n = nullptr;
    vector<node*> copy = nodes;
    for (auto& node : nodes)
    {
        if (node->covered == 2 && node->value != 0)
        {
            if (node->value == node->flagsAround && movePossible(node) && node->covered == 2)
            {
                n = node;
                break;
            }
        }
    }
    if (n != nullptr)
    {
        n->covered = 2;
        copy = AI_uncover(nodes, n);
    }
    else
    {
        nodes = AI_randomMove(f, nodes, 0);
    }
    nodes = copy;
    return nodes;
}

vector<node*> AI_move(Field f, vector<node*> nodes, int rngHolder, int nodeAImove, int mines)
{
    // flagging operation is not counted as move
    
    if (nodeAImove == 0)
    {
        nodes = AI_randomMove(f, nodes, rngHolder);
        return nodes;
    }
    else
    {
        nodes = AI_flagging(nodes);
        cout << "FLAGS" << endl;
        printGameBoard(nodes);
        isGameOver(nodes, nodeAImove, mines);
        cout << endl;
        nodes = AI_makeAmove(nodes, f);
    }


    return nodes;
}



void AI_GameStart(Field f, int nodesTotal, int mines)
{
    int nodeAImove = 0;
    int freeNodes = nodesTotal - mines;
    int minesFlagged = 0;
    int minesLeft = mines;
    int movesCounter = 0;

    //1st move is always random, during that, generate the GameBoard and set mines
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<>  rng(27, 29);
    uniform_int_distribution<>  rng2(35, 37);
    uniform_int_distribution<>  rng3(43, 45);
    vector<int> roller = { rng(mt), rng2(mt), rng3(mt)};
    int random = rand() % roller.size();
    int rngHolder = roller[random]; // 1st move

    for (int i = 1; i <= nodesTotal; i++)
    {
        f.CreateNode(i);
    }
    f.connectNodes();
    f.addMines(mines, rngHolder);


    vector<node*> nodes = f.GetNodes();
    nodes = AI_move(f, nodes, rngHolder, nodeAImove, mines);

    printGameBoard(nodes);
    cout << "Moves : " << ++nodeAImove << endl <<
        "Flags : " << minesFlagged << endl <<
        "Mines left : " << minesLeft << endl << endl << endl;

    while (true)
    {
        Sleep(2000);
        cout << endl;
        nodes = AI_move(f, nodes, 0, nodeAImove, mines);
        //cout << minesLeft << endl;
        //minesLeft--;
        printGameBoard(nodes);
        isGameOver(nodes, nodeAImove, mines);


        minesFlagged = countFlagged(nodes);
        minesLeft = mines - minesFlaggedCorrectly(nodes);
        cout << "Moves : " << ++nodeAImove << endl <<
            "Flags : " << minesFlagged << endl <<
            "Mines left : " << minesLeft << endl << endl << endl;
    }
}



int main(int argc, char* args[])
{
    int size = 8;
    int mines = 15;
    Field f;
    AI_GameStart(f, pow(size,2), mines);

    return 0;
}