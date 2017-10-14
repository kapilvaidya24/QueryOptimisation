#include <vector>
#include <pair>

class RelationGraph
{
    int N;
    vector<vector<bool> > adjMatrix;

public:

    RelationGraph(int N_, vector<bool> adjMatrix_)
    {
        N = N_;
        adjMatrix = adjMatrix_;
    }

    RelationGraph(int N_, vector<pair<int, int> > edges)
    {
        N = N_;
        adjMatrix.resize(N, vector<bool>(N, false));
        for (int i = 0; i < edges.size(); i++)
        {
            //assert edges[i].first < N && edges[i].second < N
            int x = edges[i].first;
            int y = edges[i].second
            adjMatrix[x][y] = true;
            adjMatrix[y][x] = true;
        }
    }

    int getSize()
    {
        return N;
    }

    vector<bool> getNeighbourVec(int index)
    {
        return adjMatrix[index];
    }

};