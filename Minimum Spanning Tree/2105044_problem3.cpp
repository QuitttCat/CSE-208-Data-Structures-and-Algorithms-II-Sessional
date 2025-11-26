#include<bits/stdc++.h>
using namespace std;
int m,n;
/// n vertex,m edges
///Basic DFS
void DFS(int i,vector<bool> &visited,vector<vector<int>>&adj)
{
    visited[i]=true;
    for(auto x:adj[i])
    {
        if(!visited[x])
            DFS(x,visited,adj);
    }
}


///DSU IMPLEMENTATION
vector<int> parent,rnk;

void make_set(int v)
{
    parent[v] = v;
    rnk[v] = 0;
}

int find_set(int v)
{
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b)
{
    a = find_set(a);
    b = find_set(b);
    if (a != b)
    {
        if (rnk[a] < rnk[b])
            swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b])
            rnk[a]++;
    }
}

///Edge Construction with support of comparator

class Edge
{
public:
    int u, v, weight;
    bool operator<(Edge const& other)
    {
        return weight < other.weight;
    }
    bool operator==(Edge const& other)
    {
        return u==other.u and v==other.v and weight==other.weight;
    }
};

/// finding Minimum Spannning Tree
int mst(vector<Edge> edges,int n)
{
    int cost = 0;
    parent.resize(n);
    rnk.resize(n);
    for (int i = 0; i < n; i++)
        make_set(i);

    sort(edges.begin(), edges.end());

    for (Edge e : edges)
    {
        if (find_set(e.u) != find_set(e.v))
        {
            cost += e.weight;
            union_sets(e.u, e.v);
        }
    }
    return cost;
}


///All subset generation with McN type time complexity
vector<vector<Edge>> edge_subsets;
vector<vector<Edge>> all_mst;
void subset(vector<Edge>& edge_subset,int i,vector<Edge>&edges)
{
    if(i==edges.size())
    {
        if(edge_subset.size()==n-1)edge_subsets.push_back(edge_subset);
        return;
    }
    subset(edge_subset,i+1,edges);
    edge_subset.push_back(edges[i]);
    subset(edge_subset,i+1,edges);
    edge_subset.pop_back();
}



void solve()
{


    vector<Edge> edges;

    cin>>n>>m;
    for(int i=0; i<m; i++)
    {
        int a,b,w;
        cin>>a>>b>>w;


        Edge temp;
        temp.u=a;
        temp.v=b;
        temp.weight=w;
        edges.push_back(temp);
    }
    vector<Edge> temp;
    int cost=mst(edges,n);
    //cout<<cost;
    subset(temp,0,edges);

    for(int i=0; i<edge_subsets.size(); i++)
    {
        vector<vector<int>> adj(n);
        vector<bool> visited(n,false);
        for(auto x:edge_subsets[i])
        {
            adj[x.u].push_back(x.v);
            adj[x.v].push_back(x.u);

        }
        DFS(0,visited,adj);
        bool connected=true;
        for(int j=0; j<n; j++) if(!visited[j]) connected=false;
        if(connected)
        {
            int tempcost=0;
            for(auto x:edge_subsets[i])
                tempcost+=x.weight;
            if(cost==tempcost) all_mst.push_back(edge_subsets[i]);
        }
    }
    vector<int> critical;
    vector<int> pseudo;
    for(int i=0; i<m; i++)
    {
        int countt=0;
        for(auto x:all_mst)
        {
            for(auto y:x)
            {
                if(y==edges[i]) countt++;
            }
        }
        //cout<<countt;
        if(countt==all_mst.size()) critical.push_back(i);
        else if(countt!=0) pseudo.push_back(i);
    }
    cout<<"Critical edges: [";
    for(int i=0; i<critical.size(); i++)
    {
        if(i==critical.size()-1) cout<<critical[i];
        else cout<<critical[i]<<",";
    }
    cout<<"],\n";

    cout<<"Pseudo critical edges: [";
    for(int i=0; i<pseudo.size(); i++)
    {
        if(i==pseudo.size()-1) cout<<pseudo[i];
        else cout<<pseudo[i]<<",";
    }
    cout<<"]";


}

int main()
{
    int test_case=1;
    //cin>>test_case;
    while(test_case--)
    {
        solve();
    }
    return 0;
}
