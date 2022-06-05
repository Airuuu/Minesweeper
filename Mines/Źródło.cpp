#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


struct node;

struct edge
{
    node* src, * dest;
    //int weight;
};

struct node
{
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
        nodes.push_back(newNode);
    }
    void CreateEdge(int source, int destination, int weight)
    {
        node* srcNode, * destNode;
        for (auto& curNode : nodes)
        {
            if (curNode->index == source)
                srcNode = curNode;
            if (curNode->index == destination)
                destNode = curNode;
        }
        if (srcNode == nullptr || destNode == nullptr)
        {
            cout << "Node not found, function CreateEdges, Nodes : " << source << " , " << destination << endl;
            return;
        }
        edge* newEdge = new edge();
        newEdge->src = srcNode;
        newEdge->dest = destNode;
        //newEdge->weight = weight;
        srcNode->edges.push_back(newEdge);
    }
    void connectNodes()
    {

    }
    void printField()
    {
        for (auto& node : nodes)
        {
            cout << node->index << endl;
        }
    }
};


int main()
{
    Field f;
    for (int i = 0; i < 64; i++)
    {
        f.CreateNode(i);
    }
    f.connectNodes();
    f.printField();
    /*g.CreateEdge(1, 2, 3);
    g.CreateEdge(1, 3, 5);
    g.CreateEdge(1, 4, 8);
    g.CreateEdge(2, 1, 3);
    g.CreateEdge(2, 3, 6);
    g.CreateEdge(2, 4, 9);
    g.CreateEdge(3, 1, 5);
    g.CreateEdge(3, 2, 6);
    g.CreateEdge(3, 4, 4);
    g.CreateEdge(4, 1, 8);
    g.CreateEdge(4, 2, 8);
    g.CreateEdge(4, 3, 4);*/

    /*PrintPath(TSP(g, 1));
    cout << "=-=-=-=-=-=-=-=-=-=" << endl;
    PrintPath(TSP(g, 2));
    cout << "=-=-=-=-=-=-=-=-=-=" << endl;
    PrintPath(TSP(g, 3));
    cout << "=-=-=-=-=-=-=-=-=-=" << endl;
    PrintPath(TSP(g, 4));
    BestPath(3)*/
    //BestPath(4, g);

    return 0;
}