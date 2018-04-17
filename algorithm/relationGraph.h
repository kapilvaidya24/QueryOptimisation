#include <vector>
#include <utility>
#include <string>

using namespace std;


string boolString(const vector<bool> & a)
{
    string str = "";
    for(int i = 0; i < a.size(); i++)
    {
        if (a[i])
            str += '1';
        else
            str += '0';
    }
    return str;
}
// RelationGraph represnts an undirected graph and does not support self-edges
// TO DO: consider using adjacency lists
class RelationGraph
{
    int N;
    int numEdges;
    vector<vector<int> > adjList;
    vector<vector<double> > selectivityList;
    vector<long long> numTuples;
    // vector<int> numAttributes;

public:

    // RelationGraph(int N_, const vector<vector<double> >& adjMatrix_, const vector<int>& numTuples_, const vector<int>& numAttributes_)
    // {
    //     N = N_;
    //     adjMatrix = adjMatrix_;
    //     numTuples = numTuples_;
    //     numAttributes = numAttributes_;
    //     int count = 0;
    //     for (int i = 0; i < N; i++)
    //     {
    //         for (int j = i + 1; j < N; j++)
    //         {
    //             if (adjMatrix[i][j] > 0)
    //             {
    //                 count++;
    //             }
    //         }
    //     }
    //     numEdges = count;
    // }

    RelationGraph(int N_, const vector<pair<int, int> >& edges, const vector<double>& selectivities, const vector<long long>& numTuples_)
    {
        N = N_;
        adjList.resize(N);
        selectivityList.resize(N);
        for (int i = 0; i < edges.size(); i++)
        {
            //assert edges[i].first < N && edges[i].second < N
            //assert No self edge
            int x = edges[i].first;
            int y = edges[i].second;
            adjList[x].push_back(y);
            selectivityList[x].push_back(selectivities[i]);
            adjList[y].push_back(x);
            selectivityList[y].push_back(selectivities[i]);
        }
        numTuples = numTuples_;
        numEdges = edges.size();
    }

    int getNumRelations() const
    {
        return N;
    }

    vector<int> getNeighbours(int index) const
    {
        return adjList[index];
    }

    vector<pair<int, int> > getEdges() const
    {
        vector<pair<int, int> > edges;
        for (int i = 0; i < N; i++)
        {
            for(int j = 0; j < adjList[i].size(); j++)
            {
                if(i < adjList[i][j])
                {
                    edges.push_back(make_pair(i, adjList[i][j]));
                }
            }
        }
        return edges;
    }

    int getNumEdges() const
    {
        return numEdges;
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

    // int getNumAttributes(int index) const
    // {
    //     return numAttributes[index];
    // }

    long long getNumTuples(int index) const
    {
        return numTuples[index];
    }

    // double getCrossSelectivity(const vector<bool>& a, const vector<bool>& b) const
    // {
    //     double selectivity = 1.0;
    //     vector<int> relA, relB;
    //     for(int i = 0; i < N; i++)
    //     {
    //         if (a[i])
    //         {
    //             relA.push_back(i);
    //         }

    //         if (b[i])
    //         {
    //             relB.push_back(i);
    //         }
    //     }

    //     for (int i = 0; i < relA.size(); i++)
    //     {
    //         for (int j = 0; j < relB.size(); j++)
    //         {
    //             if (adjMatrix[relA[i]][relB[j]] > 0)
    //             {
    //                 selectivity *= adjMatrix[relA[i]][relB[j]];
    //             }
    //         }
    //     }

    //     // cout<<boolString(a)<<"\t"<<boolString(b)<<"\t"<<selectivity<<endl;
    //     return selectivity;
    // }
    double getCrossSelectivity(const vector<bool>& a, const vector<bool>& b) const
    {
        double selectivity = 1.0;

        for (int i = 0; i < N; i++)
        {
            if (a[i])
            {
                for (int j = 0; j < adjList[i].size(); j++)
                {
                    int index = adjList[i][j];
                    if ((!a[index]) and (b[index]))
                    {
                        selectivity *= selectivityList[i][j];
                    }
                }
            }
        }

        // cout<<boolString(a)<<"\t"<<boolString(b)<<"\t"<<selectivity<<endl;
        return selectivity;
    }

    // int getNumCrossEdges(const vector<bool>& a, const vector<bool>& b) const
    // {
    //     int count = 0;
    //     vector<int> relA, relB;
    //     for(int i = 0; i < N; i++)
    //     {
    //         if (a[i])
    //         {
    //             relA.push_back(i);
    //         }

    //         if (b[i])
    //         {
    //             relB.push_back(i);
    //         }
    //     }

    //     for (int i = 0; i < relA.size(); i++)
    //     {
    //         for (int j = 0; j < relB.size(); j++)
    //         {
    //             if (adjMatrix[relA[i]][relB[j]] > 0)
    //             {
    //                 count++;
    //             }
    //         }
    //     }
    //     return count;
    // }
};