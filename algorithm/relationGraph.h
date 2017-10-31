#include <vector>
#include <utility>

using namespace std;

// RelationGraph represnts an undirected graph and does not support self-edges
// TO DO: consider using adjacency lists
class RelationGraph
{
    int N;
    vector<vector<double> > adjMatrix;
    vector<int> numTuples;
    vector<int> numAttributes;

public:

    RelationGraph(int N_, const vector<vector<double> >& adjMatrix_, const vector<int>& numTuples_, const vector<int>& numAttributes_)
    {
        N = N_;
        adjMatrix = adjMatrix_;
        numTuples = numTuples_;
        numAttributes = numAttributes_;
    }

    RelationGraph(int N_, const vector<pair<int, int> >& edges, const vector<double>& selectivities, const vector<int>& numTuples_, const vector<int>& numAttributes_)
    {
        N = N_;
        adjMatrix.resize(N, vector<double>(N, -1.0));
        for (int i = 0; i < edges.size(); i++)
        {
            //assert edges[i].first < N && edges[i].second < N
            //assert No self edge
            int x = edges[i].first;
            int y = edges[i].second;
            adjMatrix[x][y] = selectivities[i];
            adjMatrix[y][x] = selectivities[i];
        }
        numTuples = numTuples_;
        numAttributes = numAttributes_;
    }

    int getNumRelations() const
    {
        return N;
    }

    vector<bool> getNeighbourVec(int index) const
    {
        vector<bool> neighbourVec(N, false);
        for (int i = 0; i < N; i++)
        {
            if (adjMatrix[index][i] > 0)
            {
                neighbourVec[i] = true;
            }
        }
        return neighbourVec;
    }

    vector<pair<int, int> > getEdges() const
    {
        vector<pair<int, int> > edges;
        for(int i = 0; i < N; i++)
        {
            for(int j = i + 1; j < N; j++)
            {
                if(adjMatrix[i][j] > 0)
                {
                    edges.push_back(make_pair(i, j));
                }
            }
        }
        return edges;
    }

    // double getNumTuples(vector<bool> relationVec)
    // {
    //     //assert relationVec.size() == N

    //     // TO DO: improve efficiency by first filtering out relationVec to get list of all the relations in it
    //     double numTup = 1.0;
    //     for (int i = 0; i < N; i++)
    //     {
    //         if (relationVec[i])
    //         {
    //             numTup *= numTup[i];
    //             for (int j = i + 1; j < N; j++)
    //             {
    //                 if (adjMatrix[i][j] > 0 && relationVec[j])
    //                 {
    //                     numTup *= adjMatrix[i][j];
    //                 }
    //             }
    //         }
    //     }
    //     return numTup;
    // }

    // int getNumAttributes(vector<bool> relation Vec)
    // {
    //     int numAttr = 0;
    //     for (int i = 0; i < N; i++)
    //     {
    //         if (relationVec[i])
    //         {
    //             numAttr += numAttr[i];
    //             for (int j = i + 1; j < N; j++)
    //             {
    //                 if (adjMatrix[i][j] >  && relationVec[j])
    //                 {
    //                     numAttr--;
    //                 }
    //             }
    //         }
    //     }
    //     return numAttr;
    // }

    int getNumAttributes(int index) const
    {
        return numAttributes[index];
    }

    int getNumTuples(int index) const
    {
        return numTuples[index];
    }

    double getCrossSelectivity(const vector<bool>& a, const vector<bool>& b) const
    {
        double selectivity = 1.0;
        vector<int> relA, relB;
        for(int i = 0; i < N; i++)
        {
            if (a[i])
            {
                relA.push_back(i);
            }

            if (b[i])
            {
                relB.push_back(i);
            }
        }

        for (int i = 0; i < relA.size(); i++)
        {
            for (int j = 0; j < relB.size(); j++)
            {
                if (adjMatrix[relA[i]][relB[j]] > 0)
                {
                    selectivity *= adjMatrix[relA[i]][relB[j]];
                }
            }
        }
        return selectivity;
    }

    int getNumCrossEdges(const vector<bool>& a, const vector<bool>& b) const
    {
        int count = 0;
        vector<int> relA, relB;
        for(int i = 0; i < N; i++)
        {
            if (a[i])
            {
                relA.push_back(i);
            }

            if (b[i])
            {
                relB.push_back(i);
            }
        }

        for (int i = 0; i < relA.size(); i++)
        {
            for (int j = 0; j < relB.size(); j++)
            {
                if (adjMatrix[relA[i]][relB[j]] > 0)
                {
                    count++;
                }
            }
        }
        return count;
    }
};