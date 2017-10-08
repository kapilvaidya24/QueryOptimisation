//heloo
#include <iostream>      
#include <string>        
#include <bitset>        
#include <cstdio>
#include <map>
#include <vector>
#include <bitset>
using namespace std;



struct node
{
	int cost;
	BitVector rel;
	int child[2];

	node()
	{
		cost=-1;
		child[0]=-1;
		child[1]=-1;
	}

};


struct bit_vector
{
	bitset<128> arr(0);
	int bit_size;

	BitVector()
	{
		bit_size=graph.size();
	};

	void assign(bit_vector &a)
	{
		arr=a.arr;
		return ;
	}

	void OR(bit_vector &a)
	{
		arr|=a.arr;
		return;
	}

	void set_size(int n)
	{
		bit_size=n;
		return ;
	}

	void reset()
	{
		arr.reset();
		return ;
	}

	void set_index(int n)
	{
		arr[n]=1;
		return ;
	}

	void set_lower(int n)
	{
		for(int i=0;i<n;i++)
		{
			arr[i]=1;
		}
	}

	int count()
	{
		return arr.count();
	}

	void subset_enum(bit_vector &a,long long b)
	{
		a.reset();

		for(int i=0;i<bit_size && b>0;i++)
		{
			if(arr[i]==1)
			{
				if(b%2==1)
				{
					a.set_index(i)=1;
				}
				b=b/2;
			}
		}

	}

	int lowest_set_bit()
	{
		int i=0;
		for(i=0;i<bit_size;i++)
		{
			if(arr[i]==1)
			{
				break;
			}
		}
		return i;
	}

	string to_string()
	{
		return arr.to_string();
	}

	vector<int> set_location()
	{
		vector<int> v;
		for(int i=0;i<bit_size;i++)
		{
			if(arr[i]==1)
			{
				v.append(i);
			}
		}

		return v;
	}

};

struct relation_graph
{

	int GraphSize;
	map<string,string> edges;

	int size()
	{
		return GraphSize;
	}

};

EnumerateCmpRec(S1, S2, X)
for each N ⊆ N (S2, X): N 6= ∅
if dpTable[S2 ∪ N]6= ∅ ∧
∃(u, v) ∈ E : u ⊆ S1 ∧ v ⊆ S2 ∪ N
EmitCsgCmp(S1, S2 ∪ N)
X = X ∪ N (S2, X)
for each N ⊆ N (S2, X): N 6= ∅
EnumerateCmpRec(S1, S2 ∪ N, X)


void EnumerateCmpRec(bit_vector &s1, bit_vector &s2, bit_vector &x)
{
	bit_vector neighbour=neigh(s2,x);
	bit_vector n;
	long long count=exp2(neighbour.count());

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector temp;
		temp.assign(s2);
		temp.OR(n);

		if(dp_table.find(temp.to_string())!=dp_table.end())
		{
			if(check_edge(s1,temp))
			{
				EmitCsgCmp(s1,temp);
			}
		}
	}

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector temp;
		temp.assign(s2);
		temp.OR(n);

		EnumerateCmpRec(s1,temp,x);
	}

	return ;
}


void EmitCsg(bit_vector &s1)
{
	bit_vector x;
	int lsb=s1.lowest_set_bit();
	x.reset();
	x.assign(s1);
	x.set_lower(lsb);

	bit_vector neighbour=neigh(s1,x);

	vector<int> v=neighbour.set_location();

	for(int i=v.size()-1;i>=0;i--)
	{
		bit_vector s2;
		s2.set_index(v[i]);

		if(check_edge(s1,s2))
		{
			EmitCsgCmp(s1,s2);
		}

		EnumerateCmpRec(s1,s2,x);


	}

	return ;
}


void EnumerateCsgRec(bit_vector &s1, bit_vector &x)
{
	bit_vector neighbour=neigh(s1,x);
	bit_vector n;
	long long count=exp2(neighbour.count());

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector temp;

		temp.assign(s1);
		temp.or(n);

		if(dp_table.find(temp.to_string())!=dp_table.end())
		{
			EmitCsg(temp);
		}

	}

	bit_vector new_x;

	new_x.assign(x);
	new_x.or(neighbour);

	for(int i=1;i<count;i++)
	{
		neighbour(n,i);
		bit_vector new_s1;

		new_s1.assign(s1);
		new_s1.or(n);

		

		EnumerateCsgRec(new_s1,new_x);

	}

	return ;

}


void Solve()
{
	for(int i=0;i<graph.size();i++)
	{
		node temp;
		temp.rel.set_size(graph.size());
		temp.rel.set_index(i);

		node_list.push_back(temp);
		dp_table[temp.rel.to_string()]=node_list.size()-1;
	}

	for(int i=graph.size()-1;i>=0;i--)
	{
		bit_vector v;
		v.set_size(graph.size());
		v.set_index(i);

		bit_vector Bv;
		Bv.set_size(graph.size());
		Bv.set_lower(i);

		EmitCsg(v);
		EnumerateCsgRec(v,Bv);


	}
}


//global variable
map<string,int> dp_table;
vector<node> node_list;
relation_graph graph;

int main()
{

	return 0;
}