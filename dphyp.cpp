#include <iostream>      
#include <string>        
#include <bitset>        
#include <cstdio>
#include <map>
#include <vector>

using namespace std;



struct node
{
	int cost;
	BitVector rel;
	int child[2];
};

struct BitVector
{
	int BitSize;
	vector<int> v;

	int MaxIndex()
	{
		for(int i=BitSize-1;i>=0;i--)
		{
			if(v[i]==1)
			{
				return i;
			}
		}

		return -1;
	}

	void SINGLE(int k)
	{
		for(int i=0;i<BitSize;i++)
		{
			v[i]=0;
		}

		if(k<BitSize)
		{
			v[k]=1;
		}
		return ;
	}

	int DIFF(BitVector &u)
	{	
		if(BitSize!=u.BitSize)
		{
			return -1;
		}

		for(int i=0;i<BitSize;i++)
		{
			if(v[i]==1 && u.v[i]==1)
			{
				v[i]=0;
			}
		}

		return 0;
	}

	int AND(BitVector &u)
	{	
		if(BitSize!=u.BitSize)
		{
			return -1;
		}

		for(int i=0;i<BitSize;i++)
		{
			if(v[i]==1 && u.v[i]==1)
			{
				v[i]=1;
			}
			else
			{
				v[i]=0;
			}
		}

		return 0;
	}

	int OR(BitVector &u)
	{	
		if(BitSize!=u.BitSize)
		{
			return -1;
		}

		for(int i=0;i<BitSize;i++)
		{
			if(v[i]==0 && u.v[i]==0)
			{
				v[i]=0;
			}
			else
			{
				v[i]=1;
			}
		}

		return 0;
	}






	int XOR(BitVector &u)
	{	
		if(BitSize!=u.BitSize)
		{
			return -1;
		}

		for(int i=0;i<BitSize;i++)
		{
			if(v[i]==u.v[i])
			{
				v[i]=0;
			}
			else
			{
				v[i]=1;
			}
		}

		return 0;
	}

	void ASSIGN(BitVector &u)
	{	
		BitSize=u.BitSize;

		for(int i=0;i<BitSize;i++)
		{
			v[i]=u.v[i];
			
		}

		return;
	}

	void COMP()
	{	
		
		for(int i=0;i<BitSize;i++)
		{
			if(v[i]==1)
			{
				v[i]=0;
			}
			else
			{
				v[i]=1;
			}
		}

		return ;
	}

	void UPFILL(int k)
	{
		for(int i=0;i<BitSize;i++)
		{
			if(i>k)
			{
				v[i]=1;	
			}
			else
			{
				v[i]=0;
			}
			
		}

		return;
	}

	string GET_STRING()
	{
		string s;

		for(int i=0;i<BitSize;i++)
		{
			if(v[i]==0)
			{
				s=s+'0';
			}
			else
			{
				s=s+'1';
			}
		}

		return s;
	}

};

struct relation_graph
{

int GraphSize;
vector<BitVector> node_array;

void GetAllPair()
{
	for(int i=0;i<GraphSize;i++)
	{
		BitVector s,ns,x;
		s.BitSize=GraphSize;
		ns.BitSize=GraphSize;
		x.BitSize=GraphSize;

		node temp;
		temp.cost=1;
		temp.rel.ASSIGN(s);

		nodes.push_back(temp);
		final_tree[s.GET_STRING()]=nodes.size()-1;

		s.SINGLE(i);
		ns.ASSIGN(node_array[i]);
		x.UPFILL(i);
		x.v[i]=1;

		EnumerateCsg(s,ns,x);
	}

	return ;
}

void EnumerateCsg(BitVector &S,BitVector &NS,BitVector &X);
{

	BitVector N;
	N.ASSIGN(NS);
	N.DIFF(X);

	BitVector s,ns,x;

	x.ASSIGN(X);

	for(int i=0;i<N.BitSize;i++)
	{
		
		if(N.v[i]==1)
		{
			s.SINGLE(i);
			x.v[i]=1;
			EnumerateCmp(S,s,node_array[i],x);
		}
	}

	N.ASSIGN(NS);
	N.DIFF(X);

	vector<int> perm,bin;

	for(int i=0;i<N.BitSize;i++)
	{
		if(N.v[i]==1)
		{
			perm.push_back(i);
			bin.push_back(0);
		}
	}

	while(true)
	{
		int c=0;
		for(int i=0;i<bin.size();i++)
		{
			if(bin[i]==0)
			{
				c=1;
				break;
			}
		}

		if(c==0)
		{
			break;
		}

		c=1;

		for(int i=0;;i++)
		{
			if(bin[i]+c>1)
			{
				bin[i]=0;
				c=1;
			}
			else
			{
				bin[i]=bin[i]+c;
				c=0;
				break;
			}
		}

		BitVector s,ns,x;

		x.ASSIGN(X);
		s.ASSIGN(S);
		ns.ASSIGN(NS);

		for(int i=0;i<bin.size();i++)
		{
			if(bin[i]==1)
			{
				s.v[perm[i]]=1;
				ns.OR(node_array[perm[i]]);
			}
		}

		EnumerateCsg(s,ns,x);


	}




	return ;
}

void EnumerateCmp(BitVector &S1,BitVector &S,BitVector &NS,BitVector &X)
{

	BitVector test;

	test.ASSIGN(S);
	test.OR(S1);

	string test_string=test.GET_STRING();

	string s_str,s1_str,s_ind,s1_ind;

	s_str=S.GET_STRING();
	s1_str=S1.GET_STRING();

	s_ind=final_tree[s_str];
	s1_ind=final_tree[s1_str];

	if(final_tree.find(test_string)!=final_tree.end())
	{
		node temp;
		temp.rel.ASSIGN(test);
		temp.cost=0;
		nodes.push_back(temp);
		final_tree[test_string]=nodes.size()-1;

		nodes[final_tree[test_string]].cost=nodes[s_ind].cost+nodes[s1_ind].cost;
		nodes[final_tree[test_string]].cost+=join_cost(nodes[s_ind],nodes[s1_ind]);

		nodes[final_tree[test_string]].child[0]=s_ind;
		nodes[final_tree[test_string]].child[1]=s1_ind;

	}
	else
	{
		int temp_cost;

		temp_cost=nodes[s_ind].cost+nodes[s1_ind].cost;
		temp_cost+=join_cost(nodes[s_ind],nodes[s1_ind]);

		if(nodes[final_tree[test_string]].cost>temp_cost)
		{
			nodes[final_tree[test_string]].cost=temp_cost;
			nodes[final_tree[test_string]].child[0]=s_ind;
			nodes[final_tree[test_string]].child[1]=s1_ind;	
		}



	}

	BitVector N;
	BitVector s,ns,x;

	

	N.ASSIGN(NS);
	N.DIFF(X);

	vector<int> perm,bin;

	for(int i=0;i<N.BitSize;i++)
	{
		if(N.v[i]==1)
		{
			perm.push_back(i);
			bin.push_back(0);
		}
	}

	while(true)
	{
		int c=0;
		for(int i=0;i<bin.size();i++)
		{
			if(bin[i]==0)
			{
				c=1;
				break;
			}
		}

		if(c==0)
		{
			break;
		}

		c=1;

		for(int i=0;;i++)
		{
			if(bin[i]+c>1)
			{
				bin[i]=0;
				c=1;
			}
			else
			{
				bin[i]=bin[i]+c;
				c=0;
				break;
			}
		}

		BitVector s,ns,x;

		x.ASSIGN(X);
		s.ASSIGN(S);
		ns.ASSIGN(NS);

		for(int i=0;i<bin.size();i++)
		{
			if(bin[i]==1)
			{
				s.v[perm[i]]=1;
				ns.OR(node_array[perm[i]]);
			}
		}

		EnumerateCmp(S1,s,ns,x);


	}
}


};



//global variable
map<string,int> final_tree;
vector<node> nodes;

int main()
{

	int n;

	cin>>n;

	//Relation_input();
	//GENERATE_GRAPH();


	return 0;
}