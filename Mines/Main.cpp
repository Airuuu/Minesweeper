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


struct node;

struct edge
{
    node* src/*, *dest*/;
    vector<node*> dest;
    //int weight;
};

struct node
{
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
                    //dests.push_back(check);
                    (newEdge->dest).push_back(check);
                }
            }
            src->edges.push_back(newEdge);
            
            /*for (auto& destination : dests)
            {
                //newEdge->weight = weight;
            }*/

            obtainable.clear();
        }
    }
    void addMines(int mines)
    {
        node* src = nullptr;
        int id = 0;
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<>  rng(1, 64);
        vector<int> mineIndexes;
        int rngHolder;
        int mineCounter = 0;
        while (mineCounter < mines)
        {
            rngHolder = rng(mt);
            if (find(mineIndexes.begin(), mineIndexes.end(), rngHolder) == mineIndexes.end())
            {
                mineIndexes.push_back(rngHolder);
                mineCounter++;
            }
        }
        int valueHolder;
        sort(mineIndexes.begin(), mineIndexes.end());

        for (auto& x : mineIndexes)
            cout << x << " ";
        cout << endl;

        mineCounter = 0;
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
                for (auto& now : edge->dest)
                {
                    if (now->index != 9)
                    {
                        (now->value)++;
                        mineCounter++;

                    }
                }
                if (src->index == 1)
                    src = edge->dest[0];
                else
                    src = edge->dest[1];
                if (mineCount = mines)
                    break;
                mineIndexes.erase(mineIndexes.begin());
            }


                //cout << now->index << end;

        }
        /*for (auto& node : nodes)
        {
            valueHolder = mineIndexes[mineCounter];
            cout << "value holder " << valueHolder << endl;
            if (node->index == valueHolder)
            {
                src = node;

                for (auto& edge : src->edges)
                {
                    cout << "yes " << edge->dest->index << endl;;
                    
                    //(edge->dest->value) = 1;
                    //if (src->value)
                    //cout << "in" << endl;
                    //(edge->dest->value) ;
                }
                return;
                node->value = 9;
                mineCounter++;
            }
            if (mineCounter >= 15)
                break;
        }*/
    }

    void printField()
    {
        int counter = 0;
        for (auto& node : nodes)
        {
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

    void testEdges()
    {
        node* src = nullptr;
        for (auto& node : nodes)
        {
            if (node->index == 8)
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

};


int main(int argc, char* args[])
{
    int size = 8;
    int mines = 15;
    Field f;
    for (int i = 1; i <= pow(size,2); i++)
    {
        f.CreateNode(i);
    }
    f.connectNodes();

    f.testEdges();
     //f.addMines(mines);
    //f.printField();
    //cout << endl;
    //f.testPrintMinefield();

    /*random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<>  rng(1, 64);
    vector<int> mineIndexes;
    int mineCounter = 0;
    while (mineCounter < mines)
    {
        mineIndexes.push_back(rng(mt));
        mineCounter++;
    }
    for (auto& x : mineIndexes)
        cout << x << " ";
        */
    /*srand(time(0));

    RenderWindow app(VideoMode(400, 400), "Minesweeper!");

    int w = 32;
    int grid[12][12];
    int sgrid[12][12]; //for showing

    Texture t;
    t.loadFromFile("images/tiles.jpg");
    Sprite s(t);

    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            sgrid[i][j] = 10;
            if (rand() % 5 == 0)  grid[i][j] = 9;
            else grid[i][j] = 0;
        }

    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            int n = 0;
            if (grid[i][j] == 9) continue;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;
            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;
            grid[i][j] = n;
        }

    while (app.isOpen())
    {
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;

        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left) sgrid[x][y] = grid[x][y];
                else if (e.key.code == Mouse::Right) sgrid[x][y] = 11;
        }

        app.clear(Color::White);

        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                if (sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];
                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                app.draw(s);
            }

        app.display();
    }*/

    return 0;
}