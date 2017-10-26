#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <assert.h>
#include "relationGraph.h"

using namespace std;

class ExploredNode;

class FrontierNode;

class ExploredNode
{
    int N;
    vector<bool> relationVec;
    float cost;
    vector<ExploredNode*> parents;
    vector<ExploredNode*> childs;
    // bool isleaf;

public:
    ExploredNode(int N_, const vector<bool>& relationVec_, float cost_, const vector<ExploredNode*>& childs_)
    {
        N = N_;
        relationVec = relationVec_;
        cost = cost_;
        
        childs = childs_;
        // if (childs.empty())
        // {
        //     isLeaf = true;
        // }
        // else
        // {
        //     assert(achilds_.size() == 2 && "Each explored node should have either none or two childs");
        // }
        parents.clear();
    }

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

    float getCost() const
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
    float cost;
    vector<ExploredNode*> childs;
public:
    FrontierNode(float cost_, const vector<ExploredNode*>& childs_)
    {
        // TO DO: Use BitSet OR fn
        // assert childs_.size() == 2
        N = childs_[0]->getNumRelations();
        relationVec.resize(N);
        for(int i = 0; i < N; i++) {
            relationVec[i] = childs_[0]->hasRelation(i) || childs_[1]->hasRelation(i);
        }
        cost = cost_;
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
    float getCost() const
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

float joinCost(ExploredNode* node1, ExploredNode* node2)
{
    return 1.0;
}


class Explored
{
    int N;
    vector<ExploredNode*> leafNodes;
    // map<vector<bool>, ExploredNode*> nodeMap;
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
                    joinCandidates.push_back(p);
                    vector<ExploredNode*> ancestralCandidates = getAncestralJoinCandidates(p, targetRel, neighRel);
                    joinCandidates.insert(joinCandidates.begin(), ancestralCandidates.begin(), ancestralCandidates.end());
                }
            }
        }
        return joinCandidates;
    }

public:
    Explored(int numRelations)
    {
        N = numRelations;
        leafNodes.resize(N);
        for (int i = 0; i < N; i++)
        {
            vector<bool> relationVec(N, false);
            relationVec[i] = true;
            leafNodes[i] = new ExploredNode(N, relationVec, 0.0, vector<ExploredNode*>());
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

    vector<ExploredNode*> getJoinCandidates(const vector<bool>& targetRel, const vector<bool>& neighRel)
    {
        vector<ExploredNode* > candidates;
        for (int i = 0; i < N; ++i)
        {
            if (neighRel[i])
            {
                candidates.push_back(leafNodes[i]);
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
    map<vector<bool>, FrontierNode*> nodeMap;
public:
    Frontier(int N_, const vector<ExploredNode*>& leafNodes, const vector<pair<int, int> >& edges)
    {
        N = N_;
        // assert leafNodes[i]->relationVec[i] = 1 and rest are 0

        for (auto& e: edges)
        {
            int x = e.first;
            int y = e.second;

            float cost = leafNodes[x]->getCost() + leafNodes[y]->getCost() + joinCost(leafNodes[x], leafNodes[y]);

            vector<ExploredNode*> childs;
            childs.push_back(leafNodes[x]);
            childs.push_back(leafNodes[y]);

            FrontierNode* fnode = new FrontierNode(cost, childs);
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
        }
        else
        {
            FrontierNode* prevNode = it->second;
            if (fnode->getCost() < prevNode->getCost())
            {
                frontierNodes.erase(prevNode);
                frontierNodes.insert(fnode);
                nodeMap[relationVec] = fnode;
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
    int N = 5;
    vector<pair<int, int> > edges { {0, 1}, {1, 2}, {2, 3}, {3, 4} };

    RelationGraph graph(N, edges);
    Explored explored(N);
    Frontier frontier(N, explored.getLeafNodes(), graph.getEdges());

    while(!explored.targetAchieved())
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
        
        for(int i = 0; i < numNewNodes; i++)
        {
            vector<ExploredNode*> childs(2);
            childs[0] = enode;
            childs[1] = candidates[i];
            float cost = candidates[i]->getCost() + enode->getCost() + joinCost(candidates[i], enode);
            newFrontierNodes[i] = new FrontierNode(cost, childs);
        }

        explored.addNode(enode);
        frontier.addNodes(newFrontierNodes);
        delete fnode;
    }

    cout<<explored.getTargetNode()->getCost()<<endl;
    return 0;
}