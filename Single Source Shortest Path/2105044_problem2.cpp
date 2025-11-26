#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define endl "\n"
ll INF=1e17;
ll NINF=(-1)*INF;

class Edge
{
public:
    ll first;
    ll second;
    ll weight;
};

ll bellman_ford(vector<Edge> edges,ll source,ll destination,ll number_of_nodes)
{
    vector<ll> dist;
    dist.resize(number_of_nodes+1,INF);
    dist[source]=0;

    for(int i = 1; i < number_of_nodes; i++)
    {
        for(auto e: edges)
        {
            int u = e.first;
            int v = e.second;
            int d = e.weight;
            if(dist[u] == INF) continue;
            dist[v] = min(dist[v], d+dist[u]);
            dist[v] = max(dist[v], NINF);
        }
    } /// n-1 relaxations

    return dist[destination];
}

void solve(ll i)
{
    int n,m;
    cin>>n>>m;
    vector<Edge> edges;
    for(ll i=0; i<m; i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        Edge e;
        e.first=u;
        e.second=v;
        e.weight=w;
        edges.push_back(e);
    }

    ll A,B;///where we can add edge--- run bellman ford from B to A;
    cin>>A>>B;

    ll L,H;///Limiting value of edges
    cin>>L>>H;

    ll temp=bellman_ford(edges,B,A,n);
    temp=max(L,-1*temp);

    ll src,dst;
    cin>>src>>dst;

    if(temp>H)
    {
        cout<<"impossible\n";
        return;
    }
    ll pre=bellman_ford(edges,src,dst,n);
    edges.push_back({A,B,temp});
    ll post=bellman_ford(edges,src,dst,n);



    if(post<pre)
    {
        cout<<temp<<" "<<post<<endl;
    }
    else cout<<"impossible\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll test_cases=1;
    ///cin>>test_cases;
    for(ll i=1; i<=test_cases; i++)
    {
        solve(i);
    }
}
