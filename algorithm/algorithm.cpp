// #include <bitset>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <pair>
#include <assert.h>
#include "relationGraph.h"

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
    ExploredNode(int N_, vector<bool> relationVec_, float cost_, vector<ExploredNodes*> childs_)
    {
        N = N_;
        relationVec = relationVec_;
        cost = cost_;
        
        childs = childs_;
        if (childs.empty())
        {
            isLeaf = true;
        } else
        {
            assert(a == b && "Each explored node should have either none or two childs");
        }
        parents.clear();
    }

    ExploredNode(FrontierNode& fnode)
    {
        relationVec = fnode.getRelationVec();
        cost = fnode.getCost();
        childs = fnode.getChilds();
    }

    int getNumRelations()
    {
        return N;
    }

    vector<bool> getRelationVec()
    {
        return relationVec;
    }

    bool hasRelation(int relIndex)
    {
        return relationVec[relIndex;]
    }

    float getCost()
    {
        return cost;
    }

    void addParent(ExploredNode* parent)
    {
        parents.push_back(parent);
    }
};

class FrontierNode
{
    // int N;
    vector<bool> relationVec;
    float cost;
    ExploredNode* childs[2];
public:
    FrontierNode(float cost_, ExploredNode* childs_[2])
    {
        // TO DO: Use BitSet OR fn
        int N = childs_[0]->getNumRelations();
        relationVec.resize(N);
        for(int i = 0; i < N; i++) {
            relationVec[i] = childs_[0][i] || childs_[1][i];
        }
        cost = cost_;
        // Make sure childs are ordered
        if (childs_[0]->getRelationVec() < childs_[1].getRelationVec())
        {
            childs[0] = childs_[0];
            childs[1] = childs_[1];
        }
        else
        {
            childs[0] = childs_[1];
            childs[1] = childs_[0];
        }
    }

    vector<bool> getRelationVec()
    {
        return relationVec;
    }

    bool hasRelation(int relIndex)
    {
        return relationVec[relIndex;]
    }
    float getCost()
    {
        return cost;
    }

    ExploredNode** getChilds()
    {
        return childs;
    }
};

class FrontierNodeComparator
{
public:
    bool operator()(const FrontierNode* a, FrontierNode* b)
    {

        if (a->getCost() == b->getCost())
        {
            return a->getRelationVec() < b->getRelationVec();
        }
        return a->getCost() < b->getCost()
    }
};

float joinCost(ExploredNode* node1, ExploredNode* node2)
{
    return 1.0;
}


class Explored
{
    int N;
    vector<ExploredNodes*> leafNodes;
    // map<vector<bool>, ExploredNode*> nodeMap;
    bool isTargetAchieved;
    ExploredNodes* TargetNode;

    vector<ExploredNode*> getPossibleJoinAncestors(ExploredNode* node, vector<bool> targetRel, vector<bool> neighRel)
    {
        vector<ExploredNode*> possibleJoins;
        vector<ExploredNode*> parents = node->getParents();
        for(auto p: parents) {
            // Add parent only if this is the first child of parent OR 1st child of parent is not a possible join candidate
            if (AreRelationsExclusive(p->getRelationVec(), targetRel)) {
                if (p->getChilds[0] == node || !isPossibleJoinCandidate(p->getChilds[0]->getRelationVec(), targetRel, neighRel))
                {
                    possibleJoins.push_back(p);
                    vector<ExploredNode*> possibleJoinAncestors = getPossibleJoinAncestors(p, targetRel, neighRel);
                    possibleJoins.insert(possibleJoins.begin(), possibleJoinAncestors.begin(), possibleJoinAncestors.end());
                }
            }
        }
        return possibleJoins;
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
            leafNodes[i] = new ExploredNode(relationVec_, vector<ExploredNodes*>());
        }

        // TO DO: handle case of numRelations = 1
        isTargetAchieved = false;
        TargetNode = NULL;
    }

    vector<ExploredNode*> getLeafNodes()
    {
        return leafNodes;
    }

    bool targetAchieved()
    {
        return isTargetAchieved;
    }

    void addNode(FrontierNode fnode)
    {
        int i;
        for (i = 0; i < N; i++)
        {
            if (!fnode.hasRelation(i))
            {
                break;
            }
        }
        if (i == N)
        {
            isTargetAchieved = true;
            TargetNode = fnode;
        }
        ExploredNode* enode = new ExploredNode(fnode);
        vector<ExploredNode*> childs = enode->getChilds();

        for (auto& c: childs)
        {
            childs->addParent(enode);
        }
    }

    bool isPossibleJoinCandidate(vector<bool> candidate, vector<bool> targetRel, vector<bool> neighRel)
    {
        // assert size of all three N
        int status = false;
        for (int i = 0; i < N; i++)
        {
            if (candidate[i] && neighRel[i])
                status = true;
            if(candidate[i] && targetRel[i])
                return false;
        }
        return status;
    }

    bool AreRelationsExclusive(vector<bool> rel1, vector<bool> rel2)
    {
        for (int i = 0, i < N; i++)
        {
            if(rel1[i] && rel2[i])
            {
                return false;
            }
        }
        return true;
    }

    vector<ExploredNode*> getPossibleJoins(vector<bool> targetRel, vector<bool> neighRel)
    {
        vector<ExploredNode* > possibleNodes;
        for (int i = 0; i < N; ++i)
        {
            if (neighRel[i])
            {
                possibleNodes.push_back(leafNodes[i]);
                vector<ExploredNode*> possibleJoinAncestors = getPossibleJoinAncestors(leafNodes[i], targetRel, neighRel);
                possibleJoins.insert(possibleJoins.begin(), possibleJoinAncestors.begin(), possibleJoinAncestors.end());
            }
        }
        return possibleJoins;
    }
};

class Frontier
{
    int N;
    set<FrontierNode*, FrontierNodeComparator> frontierNodes;
    map<vector<bool>, FrontierNode*> nodeMap;
public:
    Frontier(int N_, vector<ExploredNode*> leafNodes)
    {
        N = N_;
        // assert leafNodes[i]->relationVec[i] = 1 and rest are 0
        for (int i = 0; i <N ; i++)
        {
            for(int j = 0; j< N; j++)
            {
                float cost = leafNodes[i]->getCost() + leafNodes[j]->getCost() + joinCost(leafNodes[i], leafNodes[j]);
                vector<ExploredNode*> childs;
                childs.push_back(leafNodes[i]);
                childs.push_back(leafNodes[j]);

                FrontierNode* fnode = new FrontierNode(cost, childs);
                frontierNodes.insert(fnode);
                nodeMap[rel] = fnode;
            }
        }
    }

    FrontierNode* removeMinNode()
    {
        assert(!frontierNodes.empty() && 'Frontier should not be empty while removing min');
        auto it = frontierNodes.begin();
        FrontierNode* minFNode = *it;
        frontierNodes.erase(it);
        nodeMap.erase(minFNode->getRelationVec());
        return minFNode;
    }

    void addNode(FrontierNode* fnode)
    {
        vector<bool> relationVec = fnode->getRelationVec()
        auto it = nodeMap.find(relationVec);
        if (it == nodeMap.end())
        {
            frontierNodes.insert(make_pair(fnode->cost, fnode));
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

    void addNodes(vector<FrontierNode*> fnodes)
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
    Frontier frontier(N, explored->getLeafNodes());

    while(!explored->targetAchieved())
    {
        FrontierNode* fnode = frontier->removeMinNode();
        ExploredNode* enode = new ExploredNode(*fnode);

        vector<bool> relationVec = fnode->getRelationVec();
        
        vector<bool> neighbourRel(N, false);
        for (int i = 0; i < N; i++)
        {
            if (relationVec[i])
            {
                vector<bool> neighbours = graph.getNeighbourVec(i);
                for(int i = 0; i < N; i++)
                {
                    neighbourRel[i] neighbourRel[i] && neighbours[i];
                }
            }
        }
        
        vector<ExploredNode*> possibleJoins = explored.getPossibleJoins(relationVec, neighbourRel);
        
        int numNewNodes = possibleJoins.size()
        vector<FrontierNode*> newFrontierNodes(numNewNodes);
        
        for(int i = 0; i < numNewNodes; i++)
        {
            vector<ExploredNode*> childs(2);
            childs[0] = enode;
            childs[1] = possibleJoins[i];
            newFrontierNodes[i] = new FrontierNode(joinCost(possibleJoins[i], enode), childs);
        }

        explored.addNode(enode);
        frontier.addNodes(newFrontierNodes);
        delete fnode;
    }
    return 0;
}