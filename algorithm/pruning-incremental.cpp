#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <utility>
#include <assert.h>
#include "relationGraph.h"
#include <sys/time.h>
using namespace std;

const double EPS = 0.0001;
const double ATTR_SIZE = 16;
const double FUDGE = 1.2;
const double MEM_SIZE = 1625;
const double PAGE_SIZE = 8000;
const double TL = 8.3;
const double TS = 9.5;
const double TX = 2.6;
const double I1 = 1;
const double I2 = 1;

const int NUM_ITERATIONS = 10;

class ExploredNode;

class FrontierNode;

class Explored;

class StoredFrontierNodes;

class Frontier;

void printRelationVec(const vector<bool>& vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i])
            cout<<"1";
        else
            cout<<"0";
    }
}

// double joinCost(const ExploredNode* a, const ExploredNode* b);

long long limitFn(int n, int r, const RelationGraph& relGraph);

class ExploredNode
{
    int N;
    vector<bool> relationVec;
    double numTuples;
    // int numAttributes;
    double cost;
    set<ExploredNode*> parents;
    vector<ExploredNode*> childs;
    // bool isleaf;

public:

    ExploredNode(int index, const RelationGraph& relGraph)
    {
        N = relGraph.getNumRelations();
        relationVec.resize(N, false);
        relationVec[index] = true;
        cost = 0.0;
        numTuples = relGraph.getNumTuples(index);
        // numAttributes = relGraph.getNumAttributes(index);
        childs.empty();
        parents.empty();
    }
    // ExploredNode(int N_, const vector<bool>& relationVec_, const vector<ExploredNode*>& childs_, const RelationGraph& relGraph)
    // {
    //     //assert childs.size() == 2

    //     N = relGraph.getNumRelations();
    //     relationVec = relationVec_;

    //     childs = childs_;

    //     cost = childs[0]->getCost() + childs[1]->getCost() + joinCost(childs[0], childs[1]);

    //     numTuples = childs[0]->getNumTuples() *
    //                 childs[1]->getNumTuples() *
    //                 relGraph.getCrossSelectivity(childs[0]->getRelationVec(), childs[1]->getRelationVec());

    //     numAttributes = childs[0]->getNumAttributes() +
    //                 childs[1]->getNumAttributes() -
    //                 relGraph.getNumCrossEdges(childs[0]->getRelationVec(), childs[1]->getRelationVec());

    //     parents.clear();
    // }

    ExploredNode(const FrontierNode& fnode);

    void updateCostModel(FrontierNode* fnode);

    int getNumRelations() const
    {
        return N;
    }

    const vector<bool>& getRelationVec() const
    {
        return relationVec;
    }

    bool hasRelation(int relIndex) const
    {
        return relationVec[relIndex];
    }

    double getNumTuples() const
    {
        return numTuples;
    }

    // int getNumAttributes() const
    // {
    //     return numAttributes;
    // }

    double getCost() const
    {
        return cost;
    }

    const set<ExploredNode*>& getParents() const
    {
        return parents;
    }

    const vector<ExploredNode*>& getChilds() const
    {
        return childs;
    }

    void addParent(ExploredNode* parent)
    {
        parents.insert(parent);
    }

    void removeParent(ExploredNode* parent)
    {
        int removed = parents.erase(parent);
        if (removed != 1)
        {
            cout<<"Unexpected Behaviour: Parent not found\n";
        }
    }

    void adoptParents(ExploredNode* node)
    {
        parents = move(node->parents);
        for (auto p : parents)
        {
            p->updateChild(node, this);
        }
    }

    void updateChild(ExploredNode* prevChild, ExploredNode* newChild)
    {
        for (int i = 0; i < childs.size(); i++)
        {
            if (childs[i] == prevChild)
            {
                childs[i] = newChild;
                return;
            }
        }
    }
};

class FrontierNode
{
    int N;
    vector<bool> relationVec;
    double numTuples;
    // int numAttributes;
    double cost;
    vector<ExploredNode*> childs;
public:
    FrontierNode(const vector<ExploredNode*>& childs_, const RelationGraph& relGraph)
    {
        // TO DO: Use BitSet OR fn
        // assert childs_.size() == 2
        N = childs_[0]->getNumRelations();
        relationVec.resize(N);
        for(int i = 0; i < N; i++) {
            relationVec[i] = childs_[0]->hasRelation(i) || childs_[1]->hasRelation(i);
        }

        // Make sure childs are ordered
        if (childs_[0]->getRelationVec() < childs_[1]->getRelationVec())
        {
            childs.push_back(childs_[0]);
            childs.push_back(childs_[1]);
        }
        else
        {
            childs.push_back(childs_[1]);
            childs.push_back(childs_[0]);
        }

        numTuples = 1.0 * childs[0]->getNumTuples() *
                    childs[1]->getNumTuples() *
                    relGraph.getCrossSelectivity(childs[0]->getRelationVec(), childs[1]->getRelationVec());
        /*numAttributes = childs[0]->getNumAttributes() +
                    childs[1]->getNumAttributes() -
                    relGraph.getNumCrossEdges(childs[0]->getRelationVec(), childs[1]->getRelationVec());
*/
        // cost = childs[0]->getCost() + childs[1]->getCost() + joinCost(childs[0], childs[1]);
        cost = childs[0]->getCost() + childs[1]->getCost() + numTuples;
    }

    int getNumRelations() const
    {
        return N;
    }

    const vector<bool>& getRelationVec() const
    {
        return relationVec;
    }

    bool hasRelation(int relIndex) const
    {
        return relationVec[relIndex];
    }

    double getNumTuples() const
    {
        return numTuples;
    }

    // int getNumAttributes() const
    // {
    //     return numAttributes;
    // }

    double getCost() const
    {
        return cost;
    }

    const vector<ExploredNode*>& getChilds() const
    {
        return childs;
    }
};


ExploredNode::ExploredNode(const FrontierNode& fnode)
{
    N = fnode.getNumRelations();
    relationVec = fnode.getRelationVec();
    numTuples = fnode.getNumTuples();
    // numAttributes = fnode.getNumAttributes();
    cost = fnode.getCost();
    childs = fnode.getChilds();
}

void ExploredNode::updateCostModel(FrontierNode* fnode)
{
    cost = fnode->getCost();
    childs = fnode->getChilds();
}

class FrontierNodeComparator
{
public:
    bool operator()(const FrontierNode* a, const FrontierNode* b)
    {

        if (a->getCost() == b->getCost())
        {
            return a->getRelationVec() < b->getRelationVec();
        }
        return a->getCost() < b->getCost();
    }
};

// double joinCost(const ExploredNode* R, const ExploredNode* S)
// {
//     if (R->getNumTuples() > S->getNumTuples())
//     {
//         return joinCost(S, R);
//     }

//     double aPages = ceil(R->getNumTuples() * R->getNumAttributes() * ATTR_SIZE / PAGE_SIZE);
//     double bPages = ceil(S->getNumTuples() * S->getNumAttributes() * ATTR_SIZE / PAGE_SIZE);

//     double b = ceil((aPages * FUDGE) / (MEM_SIZE - I1));
//     double o = floor((MEM_SIZE - I1) / b);

//     double nx = 3 * (aPages + bPages);
//     double nio = ceil(aPages/I1) + ceil(aPages/o) + ceil(bPages/I1) + ceil(bPages/o) + b + ceil(bPages/I2);
//     double ns = 2 + ceil(aPages/o) + ceil(bPages/o) + 2*b;

//     double cost = nx*TX + ns*TS + nio*TL;

//     // cout<<boolString(R->getRelationVec())<<"\t"<<R->getNumTuples()<<"\t"<<R->getNumAttributes()<<endl;;
//     // cout<<boolString(S->getRelationVec())<<"\t"<<S->getNumTuples()<<"\t"<<S->getNumAttributes()<<endl;;
//     // cout<<cost<<endl<<endl;
//     // cout<<fnode->getCost()<<" "<<fnode->getNumTuples()<<" "<<fnode->getNumAttributes()<<endl;

//     return cost;
// }

long long limitFn(int n, int r, const RelationGraph& relGraph)
{
    // long long res = 1;
    // for (int i = 1; i <= r; i++)
    // {
    //     res = res * (n-r+i);
    //     res = res / i;
    // }
    // return ceil(relGraph.getNumEdges() * ( r * (n-r) / min (relGraph.getNumEdges(), n - 1)));
    // return ceil(sqrt( r * (n-r) / min (relGraph.getNumEdges(), n - 1)));
    return ceil(sqrt(r*(n-r)));
    // return 1;
    // return res;
    // return ceil(1.0 *res / 100000000);
}

// Once a Node has been inserted in Explored, we never delete it as it may be used by many FrontierNodes
// While updating we only update the content of node and not the memory address where it is stored
class Explored
{
    int N;
    vector<ExploredNode*> leafNodes;
    unordered_map<vector<bool>, ExploredNode*> nodeMap;
    // TO DO: see if need to store tragets Node for efficiency?
    bool isTargetAchieved;
    ExploredNode* targetNode;
    vector<long long> levelSizes;
    vector<long long> limits;

    void getAncestralJoinCandidates(const ExploredNode* node, const vector<bool>& targetRel, const vector<bool>& neighRel, vector<ExploredNode*>& resultCandidates)
    {
        // count++;
        const set<ExploredNode*>& parents = node->getParents();
        for (auto& p: parents) {
            // Add parent only if this is the first child of parent OR 1st child of parent is not a possible join candidate
            if (AreRelationsExclusive(p->getRelationVec(), targetRel)) {
                if (p->getChilds()[0] == node ||
                    !isJoinCandidate(p->getChilds()[0]->getRelationVec(), targetRel, neighRel))
                {
                    // if (canResultNodeBeAdded(targetRel, p->getRelationVec()))
                    // {
                        resultCandidates.push_back(p);
                    // }
                    getAncestralJoinCandidates(p, targetRel, neighRel, resultCandidates);
                }
            }
        }
    }

public:
    Explored(const RelationGraph& relGraph)
    {
        N = relGraph.getNumRelations();
        leafNodes.resize(N);
        for (int i = 0; i < N; i++)
        {
            leafNodes[i] = new ExploredNode(i, relGraph);
            nodeMap[leafNodes[i]->getRelationVec()] = leafNodes[i];
        }

        // TO DO: handle case of numRelations = 1
        isTargetAchieved = false;
        targetNode = NULL;
        levelSizes.resize(N, 0);
        limits.resize(N, 0);
        levelSizes[0] = N;
        for(int i = 1; i < N; i++)
        {
            limits[i] = limitFn(N, i, relGraph);
        }
    }

    const vector<ExploredNode*>& getLeafNodes() const
    {
        return leafNodes;
    }

    bool targetAchieved() const
    {
        return isTargetAchieved;
    }

    ExploredNode* getTargetNode() const
    {
        return targetNode;
    }

    void setTargetAchievedFalse()
    {
        isTargetAchieved = false;
    }

    bool isNodeExplored(const vector<bool> & vec) const
    {
        return (nodeMap.count(vec) > 0);
    }

    bool isBetterCostNode(const vector<bool> & relationVec, double cost)
    {
        return ((nodeMap.count(relationVec) > 0) && (nodeMap[relationVec]->getCost() > (1+EPS) * cost));
    }

    // bool canResultNodeBeAdded(const vector<bool>& a, const vector<bool>& b) const
    // {
    //     vector<bool> result(N, false);
    //     for (int i = 0; i < N; i++)
    //     {
    //         result[i] = a[i] || b[i];
    //     }
    //     return (nodeMap.count(result) == 0) and canAddNode(result);
    // }

    void increaseLimitsByFactor(int factor)
    {
        for (int i = 0; i < N; i++)
        {
            limits[i] *= factor;
        }
    }

    ExploredNode* addNode(FrontierNode* fnode)
    {
        vector<bool> relationVec = fnode->getRelationVec();
        auto it = nodeMap.find(relationVec);
        if (it == nodeMap.end())
        {
            ExploredNode* enode = new ExploredNode(*fnode);
            int i;
            int count = 0;
            for (i = 0; i < N; i++)
            {
                if (relationVec[i])
                {
                    count++;
                }
            }
            // TO DO: assert canAddAtLevel(count - 1);
            if (count == N)
            {
                isTargetAchieved = true;
                targetNode = enode;
            }
            vector<ExploredNode*> childs = enode->getChilds();

            for (auto& child: childs)
            {
                // if(child->getRelationVec().size() != N)
                // {
                //     printRelationVec(enode->getRelationVec());
                //     cout<<"invalid child"<<endl;
                // }
                child->addParent(enode);
            }

            nodeMap[relationVec] = enode;
            levelSizes[count - 1]++;
            // cout<<"Adding node "<<boolString(relationVec)<<endl;
            return enode;
        }
        else
        {
            ExploredNode* curNode = it->second;
            if ((1+EPS) * fnode->getCost() < curNode->getCost())
            {
                int i;
                int count = 0;
                for (i = 0; i < N; i++)
                {
                    if (relationVec[i])
                    {
                        count++;
                    }
                }
                if (count == N)
                {
                    isTargetAchieved = true;
                    targetNode = curNode;
                }

                for (auto& child: curNode->getChilds())
                {
                    child->removeParent(curNode);
                }

                for (auto& child: fnode->getChilds())
                {
                    child->addParent(curNode);
                }

                // enode->adoptParents(curNode);

                curNode->updateCostModel(fnode);
                // cout<<"Deleting "<<curNode<<" : "<<enode<<" , "<<curNode->getCost()<<" : "<<enode->getCost()<<endl;
            }
            return curNode;
        }
        
    }

    bool isJoinCandidate(const vector<bool>& candidate, const vector<bool>& targetRel, const vector<bool>& neighRel) const
    {
        // assert size of all three N
        int status = false;
        for (int i = 0; i < N; i++)
        {
            if (candidate[i] && neighRel[i])
                status = true;
            if (candidate[i] && targetRel[i])
                return false;
        }
        return status;
    }

    bool AreRelationsExclusive(const vector<bool>& rel1, const vector<bool>& rel2) const
    {
        for (int i = 0; i < N; i++)
        {
            if (rel1[i] && rel2[i])
            {
                return false;
            }
        }
        return true;
    }

    // return the nodes that can be joined with node targetRel i.e.
    // nodes that don't have any relation which is present in targetRel but have at least one rel which is in neighRel
    // NO LONGER VALID for incremental algorithm: Also don't return nodes such that joining targetRel and the node results in a node which is already there in explored
    void getJoinCandidates(const vector<bool>& targetRel, const vector<bool>& neighRel, vector<ExploredNode*>& resultCandidates)
    {
        // int count = 0;
        // int x = 0;
        for (int i = 0; i < N; ++i)
        {
            if (neighRel[i])
            {
                //if (canResultNodeBeAdded(targetRel, leafNodes[i]->getRelationVec()))
                // {
                    // x++;
                    resultCandidates.push_back(leafNodes[i]);
                // }
                getAncestralJoinCandidates(leafNodes[i], targetRel, neighRel, resultCandidates);
            }
        }
        // cout<<"Join Candidates : "<<count<<" "<<resultCandidates.size()-x<<endl;
        // count1+=count;
        // count2+=resultCandidates.size()-x;
    }

    int size() const
    {
        return nodeMap.size();
    }

    bool canAddAtLevel(int level) const
    {
        if (level < 0 or level >= N)
            return false;
        return levelSizes[level] < limits[level];
    }

    bool canAddNode(vector<bool> relationVec) const
    {
        // TO DO: Do i need to check if node already exists in explored
        int count = 0;
        for (int i = 0; i < relationVec.size(); i++)
        {
            if (relationVec[i])
                count++;
        }
        return canAddAtLevel(count - 1);
    }
    void printLevelSizes() const
    {
        for(int i = 0; i < N; i++)
        {
            cout<<levelSizes[i]<<"/"<<limits[i]<<" ";
        }
        cout<<endl;
    }

    void printPath(ExploredNode* enode, const vector<string>& names)
    {
        if (enode->getChilds().size() == 0)
        {
            for(int i = 0; i < N; i++)
            {
                if(enode->hasRelation(i))
                    cout<<'(' + names[i] + " " + to_string(enode->getNumTuples()) + " " + to_string(enode->getCost()) + ")";
            }
        }
        else
        {
            cout<<"(";
            printPath(enode->getChilds()[0], names);
            cout<<" join ";
            printPath(enode->getChilds()[1], names);
            cout<<" " + to_string(enode->getNumTuples()) + " " + to_string(enode->getCost()) + ")";
        }
    }
    void printPath(const vector<string>& names)
    {
        printPath(targetNode, names);
        cout<<endl;
    }
};

class StoredFrontierNodes
{
    int N;
    set<FrontierNode*, FrontierNodeComparator> excessNodes;
    unordered_map<vector<bool>, FrontierNode*> excessNodeMap;
    vector<long long> limits;
    vector<long long> levelSizes;

public:
    StoredFrontierNodes(int N_, int factor, RelationGraph relGraph)
    {
        N = N_;
        limits.resize(N);
        levelSizes.resize(N, 0);
        for(int i = 1; i < N; i++)
        {
            limits[i] = factor * limitFn(N, i, relGraph);
        }
    }

    void handleNode(FrontierNode* fnode)
    {
        const vector<bool>& relationVec = fnode->getRelationVec();
        auto it = excessNodeMap.find(relationVec);
        if (it == excessNodeMap.end())
        {
            int count = 0;
            for (int i = 0; i < relationVec.size(); i++)
            {
                if (relationVec[i])
                    count++;
            }

            if (canAddAtLevel(count - 1))
            {
                excessNodes.insert(fnode);
                excessNodeMap[relationVec] = fnode;
            }
            else
            {
                // Assumption here is that nodes are coming in increasong order
                // Otherwise check if there is a node of hogher cost at this level
                delete fnode;
            }
        }
        else
        {
            // Assumption here is that nodes are coming in increasong order
            // Otherwise check if there is a node of higher cost at this level
            delete fnode;
        }
    }

    bool canAddAtLevel(int level) const
    {
        if (level < 0 or level >= N)
            return false;
        return levelSizes[level] < limits[level];
    }

    bool canAddNode(vector<bool> relationVec) const
    {
        int count = 0;
        for (int i = 0; i < relationVec.size(); i++)
        {
            if (relationVec[i])
                count++;
        }
        return canAddAtLevel(count - 1);
    }

    void clear()
    {
        excessNodes.clear();
        excessNodeMap.clear();
    }

    friend class Frontier;
};

class Frontier
{
    int N;
    set<FrontierNode*, FrontierNodeComparator> frontierNodes;
    unordered_map<vector<bool>, FrontierNode*> nodeMap;
public:
    Frontier(int N_, const vector<ExploredNode*>& leafNodes, const RelationGraph& relGraph)
    {
        N = N_;
        // assert leafNodes[i]->relationVec[i] = 1 and rest are 0
        vector<pair<int, int> > edges = relGraph.getEdges();
        for (auto& e: edges)
        {
            int x = e.first;
            int y = e.second;

            vector<ExploredNode*> childs;
            childs.push_back(leafNodes[x]);
            childs.push_back(leafNodes[y]);

            FrontierNode* fnode = new FrontierNode(childs, relGraph);
            frontierNodes.insert(fnode);
            nodeMap[fnode->getRelationVec()] = fnode;
        }
    }

    FrontierNode* removeMinNode()
    {
        // assert(!frontierNodes.empty() && 'Frontier should not be empty while removing min');
        auto it = frontierNodes.begin();
        FrontierNode* minFNode = *it;
        frontierNodes.erase(it);
        nodeMap.erase(minFNode->getRelationVec());
        // cout<<"Removing node "<<boolString(minFNode->getRelationVec())<<endl;
        return minFNode;
    }

    void addNode(FrontierNode* fnode)
    {
        vector<bool> relationVec = fnode->getRelationVec();
        auto it = nodeMap.find(relationVec);
        if (it == nodeMap.end())
        {
            frontierNodes.insert(fnode);
            nodeMap[relationVec] = fnode;
            // cout<<"Adding node "<<boolString(relationVec)<<endl;
        }
        else
        {
            FrontierNode* prevNode = it->second;
            if ((1+EPS) * fnode->getCost() < prevNode->getCost())
            {
                frontierNodes.erase(prevNode);
                frontierNodes.insert(fnode);
                nodeMap[relationVec] = fnode;
                delete prevNode;
                // cout<<"Updating node "<<boolString(relationVec)<<endl;
            }
        }
    }

    void addNodes(const vector<FrontierNode*>& fnodes)
    {
        for(auto& node: fnodes)
        {
            addNode(node);
        }
    }

    void transfer(StoredFrontierNodes& storedFrontierNodes)
    {
        nodeMap = move(storedFrontierNodes.excessNodeMap);
        frontierNodes = move(storedFrontierNodes.excessNodes);
    }

    void clear()
    {
        frontierNodes.clear();
        nodeMap.clear();
    }

    int size() const
    {
        return frontierNodes.size();
    }

    bool isEmpty() const
    {
        return frontierNodes.empty();
    }
};

int main()
{
    int N, E;
    int nodeCount = 0;
    // vector<pair<int, int> > edges { {0, 1}, {1, 2}, {1, 3}, {2, 4} };
    vector<pair<int, int> > edges;
    vector<double> selectivities;
    vector<long long> numTuples;
    unordered_map<string, int> relIndexMap;
    vector<string> relNames;
    // vector<int> numAttributes;

    cin>>N;

    numTuples.resize(N);
    relNames.resize(N);
    // numAttributes.resize(N);

    int x, y;
    long long cardinality;
    string s1, s2;
    for(int i = 0; i < N; i++)
    {
        cin>>s1;
        cin>>cardinality;
        relIndexMap[s1] = i;
        relNames[i] = s1;
        numTuples[i] = cardinality;
    }
    
    cin>>E;
    edges.resize(E);
    selectivities.resize(E);
    for(int i = 0; i < E; i++)
    {
        cin>>s1>>s2;
        cin>>cardinality;
        x =  relIndexMap[s1];
        y =  relIndexMap[s2];
        edges[i] = make_pair(x, y);
        selectivities[i] = 1.0 * cardinality / numTuples[x] / numTuples[y];
    }

    RelationGraph graph(N, edges, selectivities, numTuples);
    Explored explored(graph);
    Frontier frontier(N, explored.getLeafNodes(), graph);
    StoredFrontierNodes storedFrontierNodes(N, pow(2, NUM_ITERATIONS) - 2, graph);

    nodeCount = frontier.size();

    // int count1 = 0, count2 = 0;
    double optimalCost = -1.0;
    struct timeval start_time, end_time;
    float time_in_s;

    gettimeofday(&start_time, NULL);

    for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++)
    {
        while (!explored.targetAchieved() && !frontier.isEmpty())
        {
            FrontierNode* fnode = frontier.removeMinNode();

            vector<bool> relationVec = fnode->getRelationVec();
            // printRelationVec(relationVec);
            // cout<<endl;
            int count = 0;

            // vector<bool> a = {true, true, true, false, true, true, true, false, true, false, false};

            // for(int l = 0; l < N; l++)
            // {
            //     count += (relationVec[l]);
            // }
            // if(count == N-1)
            // {
            //     cout<<"Found\n";
            // }

            if (optimalCost != -1 && optimalCost < fnode->getCost())
            {
                delete fnode;
                continue;
            }
            if (explored.isNodeExplored(relationVec))
            {
                if(!explored.isBetterCostNode(relationVec, fnode->getCost()))
                {
                    delete fnode;
                    continue;
                }
            }
            else if (!explored.canAddNode(relationVec))
            {
                storedFrontierNodes.handleNode(fnode);
                continue;
            }

            // ExploredNode* enode = new ExploredNode(*fnode);

            vector<bool> neighbourRel(N, false);
            for (int i = 0; i < N; i++)
            {
                if (relationVec[i])
                {
                    vector<int> neighbours = graph.getNeighbours(i);
                    for (int i = 0; i < neighbours.size(); i++)
                    {
                        int index = neighbours[i];
                        if (!relationVec[index])
                            neighbourRel[index] = 1;
                    }
                }
            }

            vector<ExploredNode*> candidates;
            explored.getJoinCandidates(relationVec, neighbourRel, candidates);
            int numNewNodes = candidates.size();
            vector<FrontierNode*> newFrontierNodes(numNewNodes);

            // If node already exists in explored, it will update the cost and childs at existing node
            // and return the pointer corresponding to it
            ExploredNode* enode = explored.addNode(fnode);
            for (int i = 0; i < numNewNodes; i++)
            {
                vector<ExploredNode*> childs(2);
                childs[0] = enode;
                childs[1] = candidates[i];
                newFrontierNodes[i] = new FrontierNode(childs, graph);
            }

            for (int i = 0; i < numNewNodes; i++)
            {
                if ((1+EPS) * newFrontierNodes[i]->getCost() < optimalCost || optimalCost < 0)
                {
                    if (explored.isNodeExplored(newFrontierNodes[i]->getRelationVec()))
                    {
                        if(explored.isBetterCostNode(newFrontierNodes[i]->getRelationVec(), newFrontierNodes[i]->getCost()))
                        {
                            frontier.addNode(newFrontierNodes[i]);
                            nodeCount++;
                        }
                        else
                        {
                            delete newFrontierNodes[i];
                        }
                    }
                    else
                    {
                        frontier.addNode(newFrontierNodes[i]);
                        nodeCount++;
                    }
                }
                else
                {
                    delete newFrontierNodes[i];
                }
            }
            delete fnode;
        }
        // cout<<"AncestralJoin Function called : "<<count1<<", result : "<<count2<<endl;
        gettimeofday(&end_time, NULL);

        time_in_s = (end_time.tv_sec - start_time.tv_sec) + 1.0 * (end_time.tv_usec - start_time.tv_usec) / 1000000;
        cout<<"Iteration #"<<iteration<<" : "<<time_in_s<<" seconds"<<endl;
        // explored.printLevelSizes();
        if(explored.targetAchieved())
        {
            optimalCost = explored.getTargetNode()->getCost();
            cout<<explored.getTargetNode()->getCost()<<" "<<nodeCount<<" "<<explored.size()<<endl;
            // explored.printPath(relNames);
        }
        else
        {
            cout<<"NO UPDATE"<<endl;
        }

        if (iteration < NUM_ITERATIONS -1)
        {
            explored.setTargetAchievedFalse();
            explored.increaseLimitsByFactor(2);
            frontier.clear();

            frontier.transfer(storedFrontierNodes);

            storedFrontierNodes.clear();
        }
    }

    return 0;
}