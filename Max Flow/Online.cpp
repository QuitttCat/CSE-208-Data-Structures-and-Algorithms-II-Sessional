#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

vector<vector<int>> capacity;
vector<vector<int>> graph;



int bfs(vector<int> &parent,int source,int destination)
{
    fill(parent.begin(), parent.end(), -1);
    parent[source] = -2;
    queue<pair<int, int>> q;
    q.push({source, INF});

    while (!q.empty())
    {
        int current = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : graph[current])
        {
            if (parent[next] == -1 && capacity[current][next])
            {
                parent[next] = current;
                int newflow = min(flow, capacity[current][next]);
                if (next == destination)
                    return newflow;
                q.push({next, newflow});
            }
        }
    }

    return 0;
}

int maxflow(int Size,int source,int destination)
{
    int flow = 0;


    vector<int> parent(Size + 1);
    int new_flow;

    while ((new_flow = bfs(parent,source,destination)))
    {
        flow += new_flow;
        int current = destination;
        while (current != source)
        {
            int previous = parent[current];
            capacity[previous][current] -= new_flow;
            capacity[current][previous] += new_flow;



            current = previous;
        }
    }

    return flow;
}





void solve(int i)
{
    int  n,m;
    cin>>n>>m;
    capacity.clear();
    graph.clear();
    capacity.resize(n+m+2, vector<int>(n+m+2, 0));
    graph.resize(n+m+2);

    for (int i = 1; i <=n; i++)
    {
        graph[0].push_back(i);
        graph[i].push_back(0);
        capacity[0][i]+=1;
    }

    for (int i = n+1; i <=n+m; i++)
    {
        graph[i].push_back(n+m+1);
        graph[n+m+1].push_back(i);
        capacity[i][m+n+1]+=1;
    }

    for(int i=1;i<=m;i++)
    {
        int a,s;
        cin>>a>>s;
        for(int i=1;i<=s;i++)
        {
            int temp;
            cin>>temp;
            graph[temp].push_back(a+n);
            graph[a+n].push_back(temp);
            capacity[temp][a+n]+=1;
        }

    }




    ///cout<<"Case "<<i<<" : "<<maxflow(m+n+1,0,m+n+1)<<"\n";
    cout<<maxflow(m+n+1,0,m+n+1)<<"\n";


}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int test_case = 1;
    //cin>>test_case;
    for(int i=1;i<=test_case;i++)
    {
        solve(i);
    }
}
