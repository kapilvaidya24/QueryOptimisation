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

struct bit_vector
{
	bitset<128> arr;
	int bit_size;

	bit_vector()
	{
		arr.reset();
		bit_size=128;
		
	};

	void print_bitset()
	{
		cout<<arr.to_string();

		return;
	}

	void set_string(string s)
	{
		bitset<128> t(s);
		arr=t;
		return;
	}

	bool check_subset(bit_vector &a)
	{
		// bitset<128> temp(s);
		if((arr&a.arr)==a.arr)
		{
			return true;
		}

		return false;

	}

	bool check_intersection(bit_vector &a)
	{
		// bitset<128> temp(s);
		if((arr&a.arr)!=0)
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

struct edge
{
	bit_vector p,q;

	edge(string s1,string s2)
	{
		p.set_string(s1);
		q.set_string(s2);

		return ;
	}


};

struct relation_graph
{

	int GraphSize;
	vector<edge> edge_list;

	int size()
	{
		return GraphSize;
	}

};



//global variable
relation_graph graph;

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
	cout<<"neigh called"<<endl;
	cout<<s.to_string()<<" is s "<<endl;
	cout<<x.to_string()<<" is x "<<endl;
	bit_vector neighbour;

	cout<<"size of the map is "<<graph.edge_list.size()<<endl;


	for (int i=0;i<graph.edge_list.size();i++)
	{

		cout<<graph.edge_list[i].p.to_string()<<" is p"<<endl;
		cout<<graph.edge_list[i].q.to_string()<<" is q"<<endl;

		
		if(s.check_subset(graph.edge_list[i].p))
		{
			if(!(x.check_intersection(graph.edge_list[i].q)) && !(s.check_intersection(graph.edge_list[i].q)))
			{
				cout<<"q selected"<<endl;
				neighbour.set_index(graph.edge_list[i].q.lowest_set_bit());
				
					
			}
		}

		if(s.check_subset(graph.edge_list[i].q))
		{
			if(!(x.check_intersection(graph.edge_list[i].p))  && !(s.check_intersection(graph.edge_list[i].p)))
			{
				cout<<"p selected"<<endl;
				neighbour.set_index(graph.edge_list[i].p.lowest_set_bit());
				
			}
		}

	}

	cout<<"neigh returned"<<endl;
 	return neighbour; 
}

bool check_edge(bit_vector &a, bit_vector &b)
{
	for (int i=0;i<graph.edge_list.size();i++)
	{
		if(a.check_subset(graph.edge_list[i].p))
		{
			if(b.check_subset(graph.edge_list[i].q))
			{
				return true;
			}
		}

		if(a.check_subset(graph.edge_list[i].q))
		{
			if(b.check_subset(graph.edge_list[i].p))
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
	cout<<"Emitcsg cmp called"<<endl;
	cout<<s1.to_string()<<" "<<s2.to_string()<<endl;


	int s1_ind,s2_ind;

	s1_ind=dp_table[s1.to_string()];
	s2_ind=dp_table[s2.to_string()];

	int cost=cost_calc(s1_ind,s2_ind);

	bit_vector s;
	s.assign(s1);
	s.OR(s2);

	cout<<"ahem"<<endl;
	cout<<s1.to_string()<<" is s1"<<endl;
	cout<<s2.to_string()<<" is s2"<<endl;
	cout<<s.to_string()<<" is new node"<<endl;

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

	cout<<"Emitcsg cmp returned"<<endl;

	return ;

}


void EnumerateCmpRec(bit_vector &s1, bit_vector &s2, bit_vector &x)
{
	cout<<endl<<"EnumerateCmpRec called"<<endl;

	cout<<s1.to_string()<<" is s1"<<endl;
	cout<<s2.to_string()<<" is s2"<<endl;
	cout<<x.to_string()<<" is x"<<endl;

	bit_vector neighbour=neigh(s2,x);
	bit_vector n;
	long long count=exp2(neighbour.count());
	count--;

	cout<<neighbour.to_string()<<" is neighbour"<<endl;
	cout<<count<<" is count"<<endl;

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

	cout<<endl<<"EnumerateCmpRec returned"<<endl;

	return ;
}


void EmitCsg(bit_vector &s1)
{
	cout<<endl<<"EmitCsg called"<<endl;



	bit_vector x;
	int lsb=s1.lowest_set_bit();
	x.reset();
	x.assign(s1);
	x.set_lower(lsb);



	bit_vector neighbour=neigh(s1,x);

	cout<<s1.to_string()<<" is s"<<endl;
	cout<<x.to_string()<<" is x"<<endl;
	cout<<neighbour.to_string()<<" is neigh"<<endl;

	vector<int> v=neighbour.set_location();

	cout<<v.size()<<" this is size of neighbour"<<endl;

	for(int i=v.size()-1;i>=0;i--)
	{
		bit_vector s2;
		s2.set_index(v[i]);

		cout<<"in the neighbour gen loop"<<endl;

		if(check_edge(s1,s2))
		{
			EmitCsgCmp(s1,s2);
		}

		EnumerateCmpRec(s1,s2,x);

		x.set_index(v[i]);

		
	}

	cout<<"Emitcsg returned"<<endl<<endl;

	return ;
}


void EnumerateCsgRec(bit_vector &s1, bit_vector &x)
{
	bit_vector neighbour=neigh(s1,x);
	bit_vector n;
	long long count=exp2(neighbour.count());
	count--;

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


void print_map(map<string,int> a)
{
	map<string,int>::iterator it=a.begin();

	for(it;it!=a.end();it++)
	{
		cout<<it->first<<" "<<endl;
	}


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

	// print_map(dp_table);

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


// bool SimplifyGraph()
// {
// 	int M=-1;
// 	map<string,string>::iterator it1,it2,j1,j2;

// 	int c=0;

// 	// bitset<128> jl1(0),jr1(0),jl2(0),jr2(0);
	
// 	for(it2=graph.edges.begin(); it2!=graph.edges.end(); ++it2)
// 	{
// 		for(it1=graph.edges.begin(); it1!=graph.edges.end(); ++it1)
// 		{
// 			if(it2==it1)
// 			{
// 				continue;
// 			}

// 			bitset<128> sl1(it1->first),sl2(it2->first),sr1(it1->second),sr2(it2->second);

// 			if((sl1&sl2)==sl1 || (sr1&sl2)==sr1)
// 			{
// 				if(((sl1|sr1)&sl2) != (sl1|sr1))
// 				{
// 					int b=ordering_benefit(sl1,sr1,sl2,sr2);

// 					if(b>M && !(check_cycle(sl1,sr1,sl2,sr2)))
// 					{
// 						M=b;
// 						c=1;
// 						j1=it1;
// 						j2=it2;

// 					}

// 				}
// 			}
// 		}

// 	}



// 	return true;

// }





int main()
{
	
	int n;
	string s1,s2;

	
	cin>>graph.GraphSize;
	cin>>n;

	cout<<n<<endl;

	for(int i=0;i<n;i++)
	{
		cin>>s1>>s2;

		edge temp(s1,s2);

		graph.edge_list.push_back(temp);
	}

	Solve();

	return 0;
}