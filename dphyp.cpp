//heloo
#include <iostream>      
#include <string>        
#include <bitset>        
#include <cstdio>
#include <map>
#include <vector>
#include <bitset>
#include <cmath>
using namespace std;


//global variable
map<string,int> dp_table;

struct relation_graph
{

	int GraphSize;
	map<string,string> edges;

	int size()
	{
		return GraphSize;
	}

};

//global variable
relation_graph graph;


struct bit_vector
{
	bitset<128> arr;
	int bit_size;

	bit_vector()
	{
		arr.reset();
		bit_size=graph.size();
	};

	void print_bitset()
	{
		cout<<arr.to_string();

		return;
	}

	bool check_subset(string s)
	{
		bitset<128> temp(s);
		if((arr&temp)==temp)
		{
			return true;
		}

		return false;

	}

	bool check_intersection(string s)
	{
		bitset<128> temp(s);
		if((arr&temp)!=0)
		{
			return true;
		}

		return false;

	}

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
					a.set_index(i);
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
				v.push_back(i);
			}
		}

		return v;
	}

};

struct node
{
	int cost;
	bit_vector rel;
	int child[2];

	node()
	{
		cost=-1;
		child[0]=-1;
		child[1]=-1;
	}

	void assign_bit_vector(bit_vector &s)
	{
		rel.assign(s);

		return ;
	}
	void assign_cost(int c)
	{
		cost=c;
		return ;
	}
	string to_string()
	{
		return rel.to_string();
	}
	void set_children(int a,int b)
	{
		if(a<b)
		{
			child[0]=a;
			child[1]=b;
		}
		else
		{
			child[0]=b;
			child[1]=a;
		}
		return ;
	}
	int get_cost()
	{
		return cost;
	}

};

//global variable
vector<node> node_list;



bit_vector neigh(bit_vector &s,bit_vector &x)
{
	bit_vector neighbour;


	for (map<string,string>::iterator it=graph.edges.begin(); it!=graph.edges.end(); ++it)
	{
		if(s.check_subset(it->first))
		{
			if(!(x.check_intersection(it->second)))
			{
				for(int i=it->second.size()-1;i>=0;i--)
				{
					if(it->second[i]=='1')
					{
						neighbour.set_index(i);
						break;
					}
				}
			}
		}

		if(s.check_subset(it->second))
		{
			if(!(x.check_intersection(it->first)))
			{
				for(int i=it->first.size()-1;i>=0;i--)
				{
					if(it->first[i]=='1')
					{
						neighbour.set_index(i);
						break;
					}
				}
			}
		}

	}
 	return neighbour; 
}

bool check_edge(bit_vector &a, bit_vector &b)
{
	for (map<string,string>::iterator it=graph.edges.begin(); it!=graph.edges.end(); ++it)
	{
		if(a.check_subset(it->first))
		{
			if(b.check_subset(it->second))
			{
				return true;
			}
		}

		if(a.check_subset(it->second))
		{
			if(b.check_subset(it->first))
			{
				return true;
			}
		}
	}
 	return false;   

}

int cost_calc(int a,int b)
{
	return 1;
}


void EmitCsgCmp(bit_vector &s1, bit_vector &s2)
{
	cout<<s1.to_string()<<" "<<s2.to_string()<<endl;


	int s1_ind,s2_ind;

	s1_ind=dp_table[s1.to_string()];
	s2_ind=dp_table[s2.to_string()];

	int cost=cost_calc(s1_ind,s2_ind);

	bit_vector s;
	s.assign(s1);
	s.OR(s2);

	if(dp_table.find(s.to_string())==dp_table.end())
	{
		node temp;
		temp.assign_bit_vector(s);
		temp.assign_cost(cost);
		temp.set_children(s1_ind,s2_ind);

		node_list.push_back(temp);

		dp_table[temp.to_string()]=node_list.size()-1;

	}
	else
	{
		int s_ind;

		if(cost<node_list[s_ind].get_cost())
		{
			node_list[s_ind].assign_cost(cost);
			node_list[s_ind].set_children(s1_ind,s2_ind);
		}
	}

	return ;

}


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
		temp.OR(n);

		if(dp_table.find(temp.to_string())!=dp_table.end())
		{
			EmitCsg(temp);
		}

	}

	bit_vector new_x;

	new_x.assign(x);
	new_x.OR(neighbour);

	for(int i=1;i<count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector new_s1;

		new_s1.assign(s1);
		new_s1.OR(n);

		

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

int ordering_benefit(bitset<128> a,bitset<128> b,bitset<128> c,bitset<128> d)
{
	return 1;
}

bool check_cycle(bitset<128> sl1,bitset<128> sr1, bitset<128> sl2,bitset<128> sr2)
{
	return false;
}


bool SimplifyGraph()
{
	int M=-1;
	map<string,string>::iterator it1,it2,j1,j2;

	int c=0;

	// bitset<128> jl1(0),jr1(0),jl2(0),jr2(0);
	
	for(it2=graph.edges.begin(); it2!=graph.edges.end(); ++it2)
	{
		for(it1=graph.edges.begin(); it1!=graph.edges.end(); ++it1)
		{
			if(it2==it1)
			{
				continue;
			}

			bitset<128> sl1(it1->first),sl2(it2->first),sr1(it1->second),sr2(it2->second);

			if((sl1&sl2)==sl1 || (sr1&sl2)==sr1)
			{
				if(((sl1|sr1)&sl2) != (sl1|sr1))
				{
					int b=ordering_benefit(sl1,sr1,sl2,sr2);

					if(b>M && !(check_cycle(sl1,sr1,sl2,sr2)))
					{
						M=b;
						c=1;
						j1=it1;
						j2=it2;

					}

				}
			}
		}

	}



	return true;

}





int main()
{
	
	int n;
	string s1,s2;

	cin>>n;
	cin>>graph.GraphSize;

	for(int i=0;i>n;i++)
	{
		cin>>s1>>s2;
		graph.edges[s1]=s2;
	}

	Solve();

	return 0;
}