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
long long csg_cmp_pair_limit=100000;

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

struct directed_graph
{
	vector<vector<int> > directed_edges;
};

//global variable
directed_graph join_graph;

//global variable
relation_graph graph;

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

	void set_attr_num(int a,int b)
	{
		num_attr=node_list[a].num_attr+node_list[b].num_attr-1;
		return ;
	}

	void set_num_tuples(int a,int b)
	{
		double selectivity_final=1;
		node r=node_list[a],s=node_list[b];

		for(int i=0;i<graph.edge_list.size();i++)
		{
			if(r.rel.check_subset(graph.edge_list[i].p))
			{
				if(s.rel.check_subset(graph.edge_list[i].q))
				{
					selectivity_final*=graph.edge_list[i].selectivity;
					continue;
				}
			}

			if(s.rel.check_subset(graph.edge_list[i].p))
			{
				if(r.rel.check_subset(graph.edge_list[i].q))
				{
					selectivity_final*=graph.edge_list[i].selectivity;
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

	int get_cost()
	{
		return cost;
	}

};

//global variable
vector<node> node_list;

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

double cost_calc(int a,int b)
{
	double nio,nx,ns,r_page,s_page,i1=1,i2=1,b,o;

	node r=node_list[a];
	node s=node_list[b];

	if(r.num_tuples>s.num_tuples)
	{
		r=node_list[b];
		s=node_list[a];
	}

	r_page=r.num_tuples*r.num_attr*attr_size;
	r_page=r_page/page_size;

	s_page=s.num_tuples*s.num_attr*attr_size;
	s_page=s_page/page_size;

	b=ceil((r_page*f)/(mem_size-i1));
	o=floor((mem_size-i1)/b);

	nx=3*(r_page+s_page);
	nio=ceil(r_page/i1)+ceil(r_page/o)+ceil(s_page/i1)+ceil(s_page/o)+b+ceil(s_page/i2);
	ns=2+ceil(r_page/o)+ceil(s_page/o)+2*b;

	double ans=0;

	ans=nx*tx+ns*ts+nio*tl;

	ans=ans+r.cost;
	ans=ans+s.cost;

	return ans;
}

map<int,int> count_csg;

void EmitCsgCmp(bit_vector &s1, bit_vector &s2)
{
	// cout<<"Emitcsg cmp called"<<endl;
	// cout<<s1.to_string()<<" "<<s2.to_string()<<endl;


	int s1_ind,s2_ind;

	s1_ind=dp_table[s1.to_string()];
	s2_ind=dp_table[s2.to_string()];

	double cost=cost_calc(s1_ind,s2_ind);

	bit_vector s;
	s.assign(s1);
	s.OR(s2);

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
		temp.set_attr_num(s1_ind,s2_ind);
		temp.set_num_tuples(s1_ind,s2_ind);

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

	if(check_pair_count)
	{
		csg_cmp_pair_count++;
	}

	// cout<<"Emitcsg cmp returned"<<endl;

	return ;

}

void EnumerateCmpRec(bit_vector &s1, bit_vector &s2, bit_vector &x)
{
	// cout<<endl<<"EnumerateCmpRec called"<<endl;

	// cout<<s1.to_string()<<" is s1"<<endl;
	// cout<<s2.to_string()<<" is s2"<<endl;
	// cout<<x.to_string()<<" is x"<<endl;

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
		Bv.set_index(i);

		EmitCsg(v);
		EnumerateCsgRec(v,Bv);

	}
}

int ordering_benefit(int i,int j)
{
	return 1;
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

	cout<<csg_cmp_pair_count<<" is_csg_cmp_pair_count"<<endl;

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
	int M=-1;

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
				int b= ordering_benefit(i,j);

				if(b>M && !(check_cycle(i,j)))
				{
					j1=i;
					j2=j;
					M=b;
				}
			}
		}
	}

	if(M==-1)
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

int main()
{
	
	int n;
	string s1,s2;

	
	cin>>graph.GraphSize;
	cin>>n;

	// cout<<n<<endl;

	for(int i=0;i<n;i++)
	{
		cin>>s1>>s2;

		edge temp(s1,s2);

		graph.edge_list.push_back(temp);
	}


	// check_csg_cmp_pair();

	join_graph.directed_edges.resize(n);

	Graph_Simplification_Optimizer();

	n=graph.GraphSize;

	int k=0;
	int mult=n-1,comb=1;
	int ans=0;

	for(int i=0;i<n-1;i++)
	{
		int temp=0;
		temp=comb*(mult);



		ans=ans+temp;
		comb=comb*(n-1-i);
		comb=comb/(i+1);
		mult--;
	}

	cout<<endl<<endl<<ans<<" ans is"<<endl;

	//Solve();

	return 0;
}