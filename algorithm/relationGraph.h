#include <vector>
#include <utility>

using namespace std;

// RelationGraph represnts an undirected graph and does not support self-edges
class RelationGraph
{
    int N;
    vector<vector<bool> > adjMatrix;

public:

    RelationGraph(int N_, vector<vector<bool> > adjMatrix_)
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
            //assert No self edge
            int x = edges[i].first;
            int y = edges[i].second;
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

    vector<pair<int, int> > getEdges()
    {
        vector<pair<int, int> > edges;
        for(int i = 0; i < N; i++)
        {
            for(int j = i + 1; j < N; j++)
            {
                if(adjMatrix[i][j])
                {
                    edges.push_back(make_pair(i, j));
                }
            }
        }
        return edges;
    }

};