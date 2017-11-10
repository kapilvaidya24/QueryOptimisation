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

//global variable
double page_size=8000,ts=9.5,tl=8.3,tx=2.6,fudge=1.2,mem_size=1625;
double attr_size=16;

//global variable
bool check_pair_count=false;
long long csg_cmp_pair_count=0;
long long csg_cmp_pair_limit=10000000;


struct bit_vector
{
	bitset<128> arr;
	int bit_size;

	bit_vector()
	{
		arr.reset();
		bit_size=128;
		
	};

	int to_int()
	{
		return (int)(arr.to_ulong());
	}

	void print_bitset()
	{
		// cout<<arr.to_string();

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

	void DIFF(bit_vector &a)
	{
		arr&=~a.arr;
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
	double selectivity;

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

struct node
{
	double cost,num_tuples,num_attr;
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
	void assign_cost(double c)
	{
		cost=c;
		return ;
	}

	void assign_num_tuples(double n)
	{
		num_tuples=n;
		return;
	}

	void assign_num_attr(double n)
	{
		num_attr=n;
		return;
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

	void set_attr_num(int a,int b,vector<node> &node_list,relation_graph &orig_graph)
	{
		double count=0;
		node r=node_list[a],s=node_list[b];

		for(int i=0;i<orig_graph.edge_list.size();i++)
		{
			if(r.rel.check_subset(orig_graph.edge_list[i].p))
			{
				if(s.rel.check_subset(orig_graph.edge_list[i].q))
				{
					count++;
					continue;
				}
			}

			if(s.rel.check_subset(orig_graph.edge_list[i].p))
			{
				if(r.rel.check_subset(orig_graph.edge_list[i].q))
				{
					count++;
					continue;
				}
			}


		}

		num_attr=r.num_attr+s.num_attr-count;
		
		return ;
	}

	void set_num_tuples(int a,int b,vector<node> &node_list,relation_graph &orig_graph)
	{
		double selectivity_final=1;
		node r=node_list[a],s=node_list[b];

		for(int i=0;i<orig_graph.edge_list.size();i++)
		{
			if(r.rel.check_subset(orig_graph.edge_list[i].p))
			{
				if(s.rel.check_subset(orig_graph.edge_list[i].q))
				{
					selectivity_final*=orig_graph.edge_list[i].selectivity;
					continue;
				}
			}

			if(s.rel.check_subset(orig_graph.edge_list[i].p))
			{
				if(r.rel.check_subset(orig_graph.edge_list[i].q))
				{
					selectivity_final*=orig_graph.edge_list[i].selectivity;
					continue;
				}
			}


		}

		double ans=1.0;
		ans=r.num_tuples*selectivity_final;
		ans*=s.num_tuples;

		num_tuples=ans;
		
		return ;
	}

	void self_calc_num_attr(vector<node> &orig_node_list,relation_graph &orig_graph)
	{
		double count=0;
		

		for(int i=0;i<orig_graph.edge_list.size();i++)
		{
			if(rel.check_subset(orig_graph.edge_list[i].p))
			{
				if(rel.check_subset(orig_graph.edge_list[i].q))
				{
					count--;
					continue;
				}
			}
		}

		for(int i=0;i<orig_node_list.size();i++)
		{
			if(rel.check_subset(orig_node_list[i].rel))
			{
				count=count+orig_node_list[i].num_attr;
			}
		}

		num_attr=count;
		
		return ;
	}


	void self_calc_num_tuples(vector<node> &orig_node_list,relation_graph &orig_graph)
	{
		vector<double> sel,tup;


		for(int i=0;i<orig_graph.edge_list.size();i++)
		{
			if(rel.check_subset(orig_graph.edge_list[i].p))
			{
				if(rel.check_subset(orig_graph.edge_list[i].q))
				{
					sel.push_back(orig_graph.edge_list[i].selectivity);
					continue;
				}
			}
		}

		for(int i=0;i<orig_node_list.size();i++)
		{
			if(rel.check_subset(orig_node_list[i].rel))
			{
				tup.push_back(orig_node_list[i].num_tuples);
			}
		}

		double ans=1.0;

		while(true)
		{
			int c=0;
			if(sel.size()!=0)
			{
				ans=ans*sel[sel.size()-1];
				sel.pop_back();
				c=1;
			}

			if(tup.size()!=0)
			{
				ans=ans*tup[tup.size()-1];
				tup.pop_back();
				c=1;
			}

			if(c==0)
			{
				break;
			}
		}



		num_tuples=ans;
		
		return ;
	}

	double get_cost()
	{
		return cost;
	}

};

//global variable
vector<node> node_list;

//global_variable
relation_graph orig_graph;

// global variable 
vector<node> orig_node_list;

//global_variable
vector<double> attr_list,tuple_list;



struct directed_graph
{
	vector<vector<int> > directed_edges;
};

//global variable
directed_graph join_graph;

//global variable
relation_graph graph;



bit_vector neigh(bit_vector &s,bit_vector &x)
{
	// cout<<"neigh called"<<endl;
	// cout<<s.to_string()<<" is s "<<endl;
	// cout<<x.to_string()<<" is x "<<endl;
	bit_vector neighbour;

	// cout<<"size of the map is "<<graph.edge_list.size()<<endl;


	for (int i=0;i<graph.edge_list.size();i++)
	{

		// cout<<graph.edge_list[i].p.to_string()<<" is p"<<endl;
		// cout<<graph.edge_list[i].q.to_string()<<" is q"<<endl;

		
		if(s.check_subset(graph.edge_list[i].p))
		{
			if(!(x.check_intersection(graph.edge_list[i].q)) && !(s.check_intersection(graph.edge_list[i].q)))
			{
				// cout<<"q selected"<<endl;
				neighbour.set_index(graph.edge_list[i].q.lowest_set_bit());
				
					
			}
		}

		if(s.check_subset(graph.edge_list[i].q))
		{
			if(!(x.check_intersection(graph.edge_list[i].p))  && !(s.check_intersection(graph.edge_list[i].p)))
			{
				// cout<<"p selected"<<endl;
				neighbour.set_index(graph.edge_list[i].p.lowest_set_bit());
				
			}
		}

	}

	// cout<<"neigh returned"<<endl;
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

double cost_calc(int a1,int a2)
{

	double nio,nx,ns,r_page,s_page,i1=1,i2=1,b,o;

	node r=node_list[a1];
	node s=node_list[a2];

	// cout<<"cost_calc"<<endl;

	// cout<<endl;
	// cout<<"cost calc called "<<endl;

	// cout<<r.rel.to_int()<<" "<<s.rel.to_int()<<endl;
	// cout<<r.num_tuples<<" "<<s.num_tuples<<endl;

	if(r.num_tuples>s.num_tuples)
	{
		r=node_list[a2];
		s=node_list[a1];
	}

	r_page=r.num_tuples*r.num_attr*attr_size;
	r_page=ceil(r_page/page_size);

	s_page=s.num_tuples*s.num_attr*attr_size;
	s_page=ceil(s_page/page_size);

	b=ceil((r_page*fudge)/(mem_size-i1));
	o=floor((mem_size-i1)/b);

	nx=3*(r_page+s_page);
	nio=ceil(r_page/i1)+ceil(r_page/o)+ceil(s_page/i1)+ceil(s_page/o)+b+ceil(s_page/i2);
	ns=2+ceil(r_page/o)+ceil(s_page/o)+2*b;

	double ans=0;

	ans=nx*tx+ns*ts+nio*tl;

	// cout<<ans<<" costcalc is returning "<<endl<<endl;

	return ans;
}

map<int,int> count_csg;

bool func_check_limit()
{
	if(csg_cmp_pair_count>csg_cmp_pair_limit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EmitCsgCmp(bit_vector &s1, bit_vector &s2)
{
	// cout<<"Emitcsg cmp called"<<endl;
	// cout<<s1.to_int()<<" "<<s2.to_int()<<endl<<endl<<endl;


	// cout<<"emitcsg"<<endl;

	if(func_check_limit())
	{
		return ;
	}

	int s1_ind,s2_ind;

	s1_ind=dp_table[s1.to_string()];
	s2_ind=dp_table[s2.to_string()];

	// for(int i=0;i<node_list.size();i++)
	// {
	// 	cout<<node_list[i].rel.to_int()<<" is a node "<<i<<endl;
	// 	cout<<node_list[max(node_list[i].child[0],0)].rel.to_int()<<" childeren "<<node_list[max(node_list[i].child[1],0)].rel.to_int()<<endl;
	// 	cout<<node_list[i].cost<<endl;
	// 	cout<<endl<<endl<<endl;
	// }

	double cost=cost_calc(s1_ind,s2_ind);

	cost=cost+node_list[s1_ind].cost+node_list[s2_ind].cost;

	bit_vector s;
	s.assign(s1);
	s.OR(s2);

	// cout<<s1.to_int()<<" "<<s2.to_int()<<" "<<s.to_int()<<endl;


	// cout<<"ahem"<<endl;
	// cout<<s1.to_string()<<" is s1"<<endl;
	// cout<<s2.to_string()<<" is s2"<<endl;
	// cout<<s.to_int()<<" is new node"<<endl;

	int pa=(256*s1.to_int())+s2.to_int(),qb=(256*s2.to_int())+s1.to_int();

	

	if(dp_table.find(s.to_string())==dp_table.end())
	{

		node temp;
		temp.assign_bit_vector(s);
		temp.assign_cost(cost);
		temp.set_children(s1_ind,s2_ind);
		temp.set_attr_num(s1_ind,s2_ind,node_list,orig_graph);
		temp.set_num_tuples(s1_ind,s2_ind,node_list,orig_graph);

		node_list.push_back(temp);

		dp_table[temp.to_string()]=node_list.size()-1;

	}
	else
	{
		int s_ind=dp_table[s.to_string()];

		// cout<<"cost updated "<<cost <<" "<<node_list[s_ind].get_cost()<<endl;
		// cout<<node_list[s_ind].rel.to_int()<<" is a node "<<endl<<endl;

		if(cost<node_list[s_ind].get_cost())
		{

			
			node_list[s_ind].assign_cost(cost);
			node_list[s_ind].set_children(s1_ind,s2_ind);
		}
	}

	if(check_pair_count)
	{
		csg_cmp_pair_count++;
		// cout<<csg_cmp_pair_count<<endl;
	}


	// for(int i=0;i<node_list.size();i++)
	// {
	// 	cout<<node_list[i].rel.to_int()<<" is a node "<<i<<endl;
	// 	cout<<node_list[max(node_list[i].child[0],0)].rel.to_int()<<" childeren "<<node_list[max(node_list[i].child[1],0)].rel.to_int()<<endl;
	// 	cout<<node_list[i].cost<<endl;
	// 	cout<<endl<<endl<<endl;
	// }

	// cout<<"Emitcsg cmp returned"<<endl;

	return ;

}



void EnumerateCmpRec(bit_vector &s1, bit_vector &s2, bit_vector &x)
{
	// cout<<endl<<"EnumerateCmpRec called"<<endl;

	// cout<<s1.to_string()<<" is s1"<<endl;
	// cout<<s2.to_string()<<" is s2"<<endl;
	// cout<<x.to_string()<<" is x"<<endl;

	if(func_check_limit())
	{
		return ;
	}	

	bit_vector neighbour=neigh(s2,x);
	bit_vector n;
	long long count=exp2(neighbour.count());
	count--;

	// cout<<neighbour.to_string()<<" is neighbour"<<endl;
	// cout<<count<<" is count"<<endl;

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

		if(csg_cmp_pair_count>csg_cmp_pair_limit)
		{
			return;
		}



	}

	bit_vector t;
	t.assign(x);
	t.OR(neighbour);

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector temp;
		temp.assign(s2);
		temp.OR(n);

		if(csg_cmp_pair_count>csg_cmp_pair_limit)
		{
			return;
		}

		EnumerateCmpRec(s1,temp,t);
	}

	// cout<<endl<<"EnumerateCmpRec returned"<<endl;

	return ;
}

void EmitCsg(bit_vector &s1)
{
	// cout<<endl<<"EmitCsg called"<<endl;

	if(func_check_limit())
	{
		return ;
	}

	bit_vector x;
	int lsb=s1.lowest_set_bit();
	x.reset();
	x.assign(s1);
	x.set_lower(lsb);

	bit_vector neighbour=neigh(s1,x);

	// cout<<s1.to_string()<<" is s"<<endl;
	// cout<<x.to_string()<<" is x"<<endl;
	// cout<<neighbour.to_string()<<" is neigh"<<endl;

	vector<int> v=neighbour.set_location();

	// cout<<v.size()<<" this is size of neighbour"<<endl;

	for(int i=v.size()-1;i>=0;i--)
	{
		bit_vector s2;
		s2.set_index(v[i]);

		// cout<<"in the neighbour gen loop"<<endl;

		if(csg_cmp_pair_count>csg_cmp_pair_limit)
		{
			return;
		}

		if(check_edge(s1,s2))
		{
			EmitCsgCmp(s1,s2);
		}

		if(csg_cmp_pair_count>csg_cmp_pair_limit)
		{
			return;
		}

		EnumerateCmpRec(s1,s2,x);

		x.set_index(v[i]);

		
	}

	// cout<<"Emitcsg returned"<<endl<<endl;

	return ;
}

void EnumerateCsgRec(bit_vector &s1, bit_vector &x)
{

	if(func_check_limit())
	{
		return ;
	}

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

		if(csg_cmp_pair_count>csg_cmp_pair_limit)
		{
			return;
		}

		if(dp_table.find(temp.to_string())!=dp_table.end())
		{
			EmitCsg(temp);
		}

	}

	bit_vector new_x;

	new_x.assign(x);
	new_x.OR(neighbour);

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector new_s1;

		new_s1.assign(s1);
		new_s1.OR(n);

		if(csg_cmp_pair_count>csg_cmp_pair_limit)
		{
			return;
		}

		EnumerateCsgRec(new_s1,new_x);

	}

	return ;

}

void print_map(map<string,int> a)
{
	map<string,int>::iterator it=a.begin();

	for(it;it!=a.end();it++)
	{
		// cout<<it->first<<" "<<endl;
	}

}

void Solve()
{
	dp_table.clear();
	node_list.resize(0);

	for(int i=0;i<graph.size();i++)
	{
		node temp;
		temp.rel.set_size(graph.size());
		temp.rel.set_index(graph.size()-i-1);
		temp.assign_cost(0);
		temp.assign_num_tuples(tuple_list[i]);
		temp.assign_num_attr(attr_list[i]);

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
		Bv.set_index(i);

		EmitCsg(v);
		EnumerateCsgRec(v,Bv);

	}
}

double ordering_benefit(int i,int j)
{
	node x,r1,r2,x_r1,x_r2;
	bit_vector tempx,tempr1,tempr2,tempx_r1,tempx_r2;

	int c=0;

	if(graph.edge_list[i].p.check_subset(graph.edge_list[j].q) && c==0)
	{
		c=1;
		
		tempx.assign(graph.edge_list[i].p);
		tempx.OR(graph.edge_list[j].q);
		tempr1.assign(graph.edge_list[i].q);
		tempr2.assign(graph.edge_list[j].p);
		
	}

	if(graph.edge_list[i].p.check_subset(graph.edge_list[j].p) && c==0)
	{
		c=1;
		
		tempx.assign(graph.edge_list[i].p);
		tempx.OR(graph.edge_list[j].p);
		tempr1.assign(graph.edge_list[i].q);
		tempr2.assign(graph.edge_list[j].q);
		
	}

	if(graph.edge_list[i].q.check_subset(graph.edge_list[j].p) && c==0)
	{
		c=1;
		
		tempx.assign(graph.edge_list[i].q);
		tempx.OR(graph.edge_list[j].p);
		tempr1.assign(graph.edge_list[i].p);
		tempr2.assign(graph.edge_list[j].q);
		
	}

	if(graph.edge_list[i].q.check_subset(graph.edge_list[j].q) && c==0)
	{
		c=1;
		
		tempx.assign(graph.edge_list[i].q);
		tempx.OR(graph.edge_list[j].q);
		tempr1.assign(graph.edge_list[i].p);
		tempr2.assign(graph.edge_list[j].p);
		
	}

	tempx_r1.assign(tempx);
	tempx_r1.OR(tempr1);
	tempx_r2.assign(tempx);
	tempx_r2.OR(tempr2);

	x.assign_bit_vector(tempx);
	r1.assign_bit_vector(tempr1);
	r2.assign_bit_vector(tempr2);
	x_r1.assign_bit_vector(tempx_r1);
	x_r2.assign_bit_vector(tempx_r2);

	x.assign_cost(0);
	r1.assign_cost(0);
	r2.assign_cost(0);
	x_r1.assign_cost(0);
	x_r2.assign_cost(0);

	x.self_calc_num_attr(orig_node_list,orig_graph);
	r1.self_calc_num_attr(orig_node_list,orig_graph);
	r2.self_calc_num_attr(orig_node_list,orig_graph);
	x_r1.self_calc_num_attr(orig_node_list,orig_graph);
	x_r2.self_calc_num_attr(orig_node_list,orig_graph);

	x.self_calc_num_tuples(orig_node_list,orig_graph);
	r1.self_calc_num_tuples(orig_node_list,orig_graph);
	r2.self_calc_num_tuples(orig_node_list,orig_graph);
	x_r1.self_calc_num_tuples(orig_node_list,orig_graph);
	x_r2.self_calc_num_tuples(orig_node_list,orig_graph);

	double cost_num=0,cost_den=0;
	int size=node_list.size();

	node_list.push_back(x);
	node_list.push_back(r1);

	cost_num+=cost_num+cost_calc(size,size+1);

	node_list.pop_back();
	node_list.pop_back();

	node_list.push_back(x_r1);
	node_list.push_back(r2);

	cost_num+=cost_num+cost_calc(size,size+1);

	node_list.pop_back();
	node_list.pop_back();

	node_list.push_back(x);
	node_list.push_back(r2);

	cost_den+=cost_den+cost_calc(size,size+1);

	node_list.pop_back();
	node_list.pop_back();

	node_list.push_back(x_r2);
	node_list.push_back(r1);

	cost_den+=cost_den+cost_calc(size,size+1);

	node_list.pop_back();
	node_list.pop_back();


	return cost_num/cost_den;
}

bool dfs(vector<int> &visited,int ind)
{
	visited[ind]=1;

	for(int i=0;i<join_graph.directed_edges[ind].size();i++)
	{
		int temp=join_graph.directed_edges[ind][i];
		if(visited[temp]==0)
		{
			visited[temp]=1;
			if(dfs(visited,temp))
			{
				return true;
			}
		}
		else
		{
			if(visited[temp]==1)
			{
				return true;	
			}
			
		}
	}

	visited[ind]=2;

	return false;
}

bool check_csg_cmp_pair()
{
	check_pair_count=true;
	csg_cmp_pair_count=0;

	Solve();

	check_pair_count=false;

	// cout<<csg_cmp_pair_count<<" is_csg_cmp_pair_count"<<endl;

	if(csg_cmp_pair_count>csg_cmp_pair_limit)
	{
		return false;
	}
	else
	{
		return true;
	}

	

	return false;
}

bool check_cycle(int p,int q)
{
	
	join_graph.directed_edges[p].push_back(q);

	vector<int> visited(join_graph.directed_edges.size(),0);

	int c=0;

	for(int i=0;i<join_graph.directed_edges.size();i++)
	{
		if(visited[i]==0)
		{
			
			if(dfs(visited,i))
			{
				c=1;
				break;
			}
		}
	}


	join_graph.directed_edges[p].pop_back();

	if(c==0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool SimplifyGraph()
{
	

	double M=-1;

	int j1=-1,j2=-1;

	for(int i=0;i<graph.edge_list.size();i++)
	{
		for(int j=0;j<graph.edge_list.size();j++)
		{
			if(i==j)
			{
				continue;
			}

			int c=0;

			// cout<<i<<" "<<j<<endl;

			if(graph.edge_list[i].p.check_subset(graph.edge_list[j].q) || graph.edge_list[i].p.check_subset(graph.edge_list[j].p))
			{
				bit_vector temp;
				temp.assign(graph.edge_list[j].p);
				temp.OR(graph.edge_list[j].q);

				if(!(graph.edge_list[i].p.check_subset(temp)))
				{
					c=1;
				}
			}

			if(graph.edge_list[i].q.check_subset(graph.edge_list[j].q) || graph.edge_list[i].q.check_subset(graph.edge_list[j].p))
			{
				bit_vector temp;
				temp.assign(graph.edge_list[j].p);
				temp.OR(graph.edge_list[j].q);

				if(!(graph.edge_list[i].q.check_subset(temp)))
				{
					c=1;
				}
			}

			// cout<<c<<" c val "<<graph.edge_list[i].p.to_int()<<" "<<graph.edge_list[i].q.to_int()<<endl;
			// cout<<c<<" c val "<<graph.edge_list[j].p.to_int()<<" "<<graph.edge_list[j].q.to_int()<<endl;
			// cout<<graph.edge_list[i].p.check_subset(graph.edge_list[j].p)<<endl;


			if(c==1)
			{
				double b= ordering_benefit(i,j);

				if(b>M && !(check_cycle(i,j)))
				{
					j1=i;
					j2=j;
					M=b;
				}
			}
		}
	}

	if(M<0)
	{
		cout<<"returned false"<<endl;
		return false;
	}

	join_graph.directed_edges[j1].push_back(j2);

	if(graph.edge_list[j1].p.check_subset(graph.edge_list[j2].p) || graph.edge_list[j1].p.check_subset(graph.edge_list[j2].q))
	{
		bit_vector temp;
		temp.assign(graph.edge_list[j2].p);
		temp.OR(graph.edge_list[j2].q);

		if(!(graph.edge_list[j1].p.check_subset(temp)))
		{
			bit_vector update;
			update.assign(temp);
			update.OR(graph.edge_list[j1].p);


			// cout<<temp.to_int()<<" update j1 p "<<update.to_int()<<endl;

			graph.edge_list[j1].p.assign(update);

			return true;
		}
	}
	else
	{
		bit_vector temp;
		temp.assign(graph.edge_list[j2].p);
		temp.OR(graph.edge_list[j2].q);

		bit_vector update;
		update.assign(temp);
		update.OR(graph.edge_list[j1].q);

		// cout<<temp.to_int()<<" update j1 q "<<update.to_int()<<endl;

		graph.edge_list[j1].q.assign(update);

		return true;
	}


	return true;

}

void Graph_Simplification_Optimizer()
{
	vector<relation_graph> graph_vector;
	graph_vector.push_back(graph);

	// cout<<"here below"<<endl;

	// for(int i=0;i<graph.edge_list.size();i++)
	// {
	// 	 cout<<graph.edge_list[i].p.to_int()<<" "<<graph.edge_list[i].q.to_int()<<endl;
	// }

	// cout<<endl;
	
	while(true)
	{
		if(SimplifyGraph())
		{
			graph_vector.push_back(graph);

			relation_graph temp_graph;
			temp_graph=graph_vector[graph_vector.size()-1];
			
			// cout<<"here below"<<endl;

			// for(int i=0;i<temp_graph.edge_list.size();i++)
			// {
			// 	 cout<<temp_graph.edge_list[i].p.to_int()<<" "<<temp_graph.edge_list[i].q.to_int()<<endl;
			// }

			// cout<<endl;


		}
		else
		{
			break;
		}
	}


	cout<<graph_vector.size()<<" is the size of the graph_vector"<<endl;

	relation_graph temp_graph;
	temp_graph=graph_vector[graph_vector.size()-1];

	for(int i=0;i<temp_graph.edge_list.size();i++)
	{
		 cout<<temp_graph.edge_list[i].p.to_int()<<" "<<temp_graph.edge_list[i].q.to_int()<<endl;
	}

	graph=graph_vector[0];
	cout<<"original"<<endl;
	check_csg_cmp_pair();
	graph=graph_vector[graph_vector.size()-1];
	cout<<"final"<<endl;
	check_csg_cmp_pair();

	int start=0,end=graph_vector.size();

	while(true)
	{
		if(start+2>=end)
		{
			break;
		}

		int mid=(start+end)/2;
		graph=graph_vector[mid];

		if(check_csg_cmp_pair())
		{
			// cout<<"aloha1"<<endl;
			start=mid;
		}
		else
		{
			// cout<<"aloha2"<<endl;
			end=mid;
		}
	}

	graph=graph_vector[start];

	return;
}

bool is_comp=false;
bit_vector s_org;
vector<bit_vector> edge_list_opt;


bit_vector neigh_opt(bit_vector &S)
{
	bit_vector ans;

	for(int i=0;i<S.bit_size;i++)
	{
		if(S.arr[i]==1)
		{
			ans.OR(edge_list_opt[i]);
		}
	}

	return ans;
}

void EnumerateCmp_opt(bit_vector &S);


void EnumerateCsgRec_opt(bit_vector &S,bit_vector &X)
{
	bit_vector neighbour=neigh_opt(S);
	neighbour.DIFF(X);

	long long count=exp2(neighbour.count());
	bit_vector n;
	count--;

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector temp,temp2;

		temp.assign(S);
		temp.OR(n);
		
		if(is_comp)
		{
			temp2.assign(s_org);
			EmitCsgCmp(temp2,temp);
		}
		else
		{
			is_comp=true;
			s_org=temp;
			EnumerateCmp_opt(temp);
			is_comp=false;
		}

	}

	count=exp2(neighbour.count());
	n;
	count--;
	bit_vector x_n;
	x_n.assign(X);
	x_n.OR(neighbour);

	for(int i=1;i<=count;i++)
	{
		neighbour.subset_enum(n,i);
		bit_vector temp,temp2;

		temp.assign(S);
		temp.OR(n);
		
		EnumerateCsgRec(temp,x_n);

	}

}


void EnumerateCmp_opt(bit_vector &S)
{
	bit_vector X;
	int min_s=S.lowest_set_bit();
	X.set_lower(min_s);
	X.OR(S);

	bit_vector neighbour=neigh_opt(S);
	neighbour.DIFF(X);
	bit_vector x_n;
	x_n.assign(X);
	x_n.OR(neighbour);

	for(int i=0;i<neighbour.bit_size;i++)
	{
		if(neighbour.arr[i]==1)
		{
			bit_vector vi;
			vi.set_index(i);
			EmitCsgCmp(S,vi);
			EnumerateCsgRec_opt(vi,x_n);
		}
	}
	return ;


}


void Solve_opt()
{
	dp_table.clear();
	node_list.resize(0);

	for(int i=0;i<graph.size();i++)
	{
		node temp;
		temp.rel.set_size(graph.size());
		temp.rel.set_index(graph.size()-i-1);
		temp.assign_cost(0);
		temp.assign_num_tuples(tuple_list[i]);
		temp.assign_num_attr(attr_list[i]);

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
		Bv.set_index(i);

		EmitCsg(v);
		EnumerateCsgRec_opt(v,Bv);

	}
}


int main()
{
	
	int n;
	string s1,s2;
	
	cin>>graph.GraphSize;
	cin>>n;

	// cout<<n<<endl;

	edge_list_opt.resize(graph.GraphSize);

	for(int i=0;i<n;i++)
	{
		cin>>s1>>s2;

		edge temp(s1,s2);

		bit_vector bs1,bs2;
		bs1.set_string(s1);
		bs2.set_string(s2);

		int ns1=bs1.lowest_set_bit(),ns2=bs2.lowest_set_bit();

		edge_list_opt[ns1].OR(bs2);
		edge_list_opt[ns2].OR(bs1);

		cin>>temp.selectivity;

		graph.edge_list.push_back(temp);
	}

	for(int i=0;i<graph.GraphSize;i++)
	{
		double a,b;
		cin>>a>>b;
		tuple_list.push_back(a);
		attr_list.push_back(b);
	}

	orig_graph=graph;

	orig_node_list.resize(0);

	for(int i=0;i<graph.size();i++)
	{
		node temp;
		temp.rel.set_size(graph.size());
		temp.rel.set_index(graph.size()-i-1);
		temp.assign_cost(0);
		temp.assign_num_tuples(tuple_list[i]);
		temp.assign_num_attr(attr_list[i]);

		orig_node_list.push_back(temp);
		
	}


	// check_csg_cmp_pair();

	join_graph.directed_edges.resize(n);

	check_csg_cmp_pair();

	double ans=1.0;

	for(int i=0;i<graph.GraphSize;i++)
	{
		ans*=2;
	}

	ans=ans-1;
	// cout<<ans<<endl;

	for(int i=0;i<node_list.size();i++)
	{
		if(node_list[i].rel.to_int()!=ans)
		{

			continue;
		}

		// cout<<node_list[i].rel.to_int()<<" is a node "<<i<<endl;
		cout<<node_list[i].cost<<" "<<csg_cmp_pair_count<<" "<<node_list.size()<<endl;
		// cout<<node_list[max(node_list[i].child[0],0)].rel.to_int()<<" childeren "<<node_list[max(node_list[i].child[1],0)].rel.to_int()<<endl;
		// cout<<endl<<endl<<endl;
	}

	// Graph_Simplification_Optimizer();

	// for(int i=0;i<node_list.size();i++)
	// {
	// 	cout<<node_list[i].rel.to_int()<<" is a node "<<i<<endl;
	// 	cout<<node_list[i].child[0]<<" childeren "<<node_list[i].child[1]<<endl;
	// 	cout<<endl<<endl<<endl;
	// }

	
	return 0;
}