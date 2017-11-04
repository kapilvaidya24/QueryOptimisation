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

using namespace std;


const double ATTR_SIZE = 16;
const double FUDGE = 1.2;
const double MEM_SIZE = 1625;
const double PAGE_SIZE = 8000;
const double TL = 8.3;
const double TS = 9.5;
const double TX = 2.6;
const double I1 = 1;
const double I2 = 1;


class ExploredNode;

class FrontierNode;

double joinCost(const ExploredNode* a, const ExploredNode* b);

class ExploredNode
{
    int N;
    vector<bool> relationVec;
    double numTuples;
    int numAttributes;
    double cost;
    vector<ExploredNode*> parents;
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
        numAttributes = relGraph.getNumAttributes(index);
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

    int getNumRelations() const
    {
        return N;
    }

    vector<bool> getRelationVec() const
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

    int getNumAttributes() const
    {
        return numAttributes;
    }

    double getCost() const
    {
        return cost;
    }

    vector<ExploredNode*> getParents() const
    {
        return parents;
    }

    vector<ExploredNode*> getChilds() const
    {
        return childs;
    }

    void addParent(ExploredNode* parent)
    {
        parents.push_back(parent);
    }
};

class FrontierNode
{
    int N;
    vector<bool> relationVec;
    double numTuples;
    int numAttributes;
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

        numTuples = childs[0]->getNumTuples() *
                    childs[1]->getNumTuples() *
                    relGraph.getCrossSelectivity(childs[0]->getRelationVec(), childs[1]->getRelationVec());
        numAttributes = childs[0]->getNumAttributes() +
                    childs[1]->getNumAttributes() -
                    relGraph.getNumCrossEdges(childs[0]->getRelationVec(), childs[1]->getRelationVec());

        cost = childs[0]->getCost() + childs[1]->getCost() + joinCost(childs[0], childs[1]);
    }

    int getNumRelations() const
    {
        return N;
    }

    vector<bool> getRelationVec() const
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

    int getNumAttributes() const
    {
        return numAttributes;
    }

    double getCost() const
    {
        return cost;
    }

    vector<ExploredNode*> getChilds() const
    {
        return childs;
    }
};


ExploredNode::ExploredNode(const FrontierNode& fnode)
{
    N = fnode.getNumRelations();
    relationVec = fnode.getRelationVec();
    numTuples = fnode.getNumTuples();
    numAttributes = fnode.getNumAttributes();
    cost = fnode.getCost();
    childs = fnode.getChilds();
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

double joinCost(const ExploredNode* R, const ExploredNode* S)
{
    if (R->getNumTuples() > S->getNumTuples())
    {
        return joinCost(S, R);
    }
    auto newEdge = make_pair(R->getRelationVec(), S->getRelationVec());

    double aPages = ceil(R->getNumTuples() * R->getNumAttributes() * ATTR_SIZE / PAGE_SIZE);
    double bPages = ceil(S->getNumTuples() * S->getNumAttributes() * ATTR_SIZE / PAGE_SIZE);

    double b = ceil((aPages * FUDGE) / (MEM_SIZE - I1));
    double o = floor((MEM_SIZE - I1) / b);

    double nx = 3 * (aPages + bPages);
    double nio = ceil(aPages/I1) + ceil(aPages/o) + ceil(bPages/I1) + ceil(bPages/o) + b + ceil(bPages/I2);
    double ns = 2 + ceil(aPages/o) + ceil(bPages/o) + 2*b;

    double cost = nx*TX + ns*TS + nio*TL;

    // cout<<boolString(R->getRelationVec())<<"\t"<<R->getNumTuples()<<"\t"<<R->getNumAttributes()<<endl;;
    // cout<<boolString(S->getRelationVec())<<"\t"<<S->getNumTuples()<<"\t"<<S->getNumAttributes()<<endl;;
    // cout<<cost<<endl<<endl;
    // cout<<fnode->getCost()<<" "<<fnode->getNumTuples()<<" "<<fnode->getNumAttributes()<<endl;

    return cost;
}

class Explored
{
    int N;
    vector<ExploredNode*> leafNodes;
    unordered_map<vector<bool>, ExploredNode*> nodeMap;
    // TO DO: see if need to store tragets Node for efficiency?
    bool isTargetAchieved;
    ExploredNode* targetNode;

    vector<ExploredNode*> getAncestralJoinCandidates(ExploredNode* node, const vector<bool>& targetRel, const vector<bool>& neighRel)
    {
        vector<ExploredNode*> joinCandidates;
        vector<ExploredNode*> parents = node->getParents();
        for (auto& p: parents) {
            // Add parent only if this is the first child of parent OR 1st child of parent is not a possible join candidate
            if (AreRelationsExclusive(p->getRelationVec(), targetRel)) {
                if (p->getChilds()[0] == node ||
                    !isJoinCandidate(p->getChilds()[0]->getRelationVec(), targetRel, neighRel))
                {
                    if (!isResultNodeExplored(targetRel, p->getRelationVec()))
                    {
                        joinCandidates.push_back(p);
                    }
                    vector<ExploredNode*> ancestralCandidates = getAncestralJoinCandidates(p, targetRel, neighRel);
                    joinCandidates.insert(joinCandidates.begin(), ancestralCandidates.begin(), ancestralCandidates.end());
                }
            }
        }
        return joinCandidates;
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
    }

    vector<ExploredNode*> getLeafNodes()
    {
        return leafNodes;
    }

    bool targetAchieved()
    {
        return isTargetAchieved;
    }

    ExploredNode* getTargetNode()
    {
        return targetNode;
    }

    bool isNodeExplored(const vector<bool> & vec)
    {
        return (nodeMap.count(vec) > 0);
    }

    bool isResultNodeExplored(const vector<bool>& a, const vector<bool>& b)
    {
        vector<bool> result(N, false);
        for (int i = 0; i < N; i++)
        {
            result[i] = a[i] || b[i];
        }
        return (nodeMap.count(result) > 0);
    }
    void addNode(ExploredNode* enode)
    {
        int i;
        for (i = 0; i < N; i++)
        {
            if (!enode->hasRelation(i))
            {
                break;
            }
        }
        if (i == N)
        {
            isTargetAchieved = true;
            targetNode = enode;
        }
        vector<ExploredNode*> childs = enode->getChilds();

        for (auto& child: childs)
        {
            child->addParent(enode);
        }

        nodeMap[enode->getRelationVec()] = enode;
    }

    bool isJoinCandidate(const vector<bool>& candidate, const vector<bool>& targetRel, const vector<bool>& neighRel)
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

    bool AreRelationsExclusive(const vector<bool>& rel1, const vector<bool>& rel2)
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
    // Also don't return nodes such that joining targetRel and the node results in a node which is already there in explored
    vector<ExploredNode*> getJoinCandidates(const vector<bool>& targetRel, const vector<bool>& neighRel)
    {
        vector<ExploredNode* > candidates;
        for (int i = 0; i < N; ++i)
        {
            if (neighRel[i])
            {
                if (!isResultNodeExplored(targetRel, leafNodes[i]->getRelationVec()))
                {
                    candidates.push_back(leafNodes[i]);
                }
                vector<ExploredNode*> ancestralCandidates = getAncestralJoinCandidates(leafNodes[i], targetRel, neighRel);
                candidates.insert(candidates.begin(), ancestralCandidates.begin(), ancestralCandidates.end());
            }
        }
        return candidates;
    }
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
            if (fnode->getCost() < prevNode->getCost())
            {
                frontierNodes.erase(prevNode);
                frontierNodes.insert(fnode);
                nodeMap[relationVec] = fnode;
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
};


int main()
{
    int N, E;
    // vector<pair<int, int> > edges { {0, 1}, {1, 2}, {1, 3}, {2, 4} };
    vector<pair<int, int> > edges;
    vector<double> selectivities;
    vector<int> numTuples;
    vector<int> numAttributes;

    cin>>N>>E;

    edges.resize(E);
    selectivities.resize(E);
    numTuples.resize(N);
    numAttributes.resize(N);

    string s;
    int x, y;
    for(int i = 0; i < E; i++)
    {
        cin>>s;
        for (x = 0; s[x] != '1'; x++);
        cin>>s;
        for (y = 0; s[y] != '1'; y++);

        edges[i] = make_pair(x, y);
        cin>>selectivities[i];
    }
    for(int i = 0; i < N; i++)
    {
        cin>>numTuples[i]>>numAttributes[i];
    }

    RelationGraph graph(N, edges, selectivities, numTuples, numAttributes);
    Explored explored(graph);
    Frontier frontier(N, explored.getLeafNodes(), graph);

    while (!explored.targetAchieved())
    {
        FrontierNode* fnode = frontier.removeMinNode();
        ExploredNode* enode = new ExploredNode(*fnode);

        vector<bool> relationVec = fnode->getRelationVec();

        vector<bool> neighbourRel(N, false);
        for (int i = 0; i < N; i++)
        {
            if (relationVec[i])
            {
                vector<bool> neighbours = graph.getNeighbourVec(i);
                for (int i = 0; i < N; i++)
                {
                    neighbourRel[i] = neighbourRel[i] || (neighbours[i] && !relationVec[i]);
                }
            }
        }

        vector<ExploredNode*> candidates = explored.getJoinCandidates(relationVec, neighbourRel);

        int numNewNodes = candidates.size();
        vector<FrontierNode*> newFrontierNodes(numNewNodes);

        for (int i = 0; i < numNewNodes; i++)
        {
            vector<ExploredNode*> childs(2);
            childs[0] = enode;
            childs[1] = candidates[i];
            newFrontierNodes[i] = new FrontierNode(childs, graph);
        }

        explored.addNode(enode);
        frontier.addNodes(newFrontierNodes);
        delete fnode;
    }

    cout<<explored.getTargetNode()->getCost()<<endl;
    return 0;
}