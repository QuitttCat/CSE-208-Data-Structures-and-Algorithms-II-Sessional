#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

vector<vector<int>> capacity;
vector<vector<int>> graph;
vector<int> min_cut_visit;
vector<int> flow_per_vertex;

void dfs(int n)
{
    min_cut_visit[n] = 1;
    for (int x : graph[n])
    {
        if (!min_cut_visit[x] and capacity[n][x])
            dfs(x);
    }
}

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
    flow_per_vertex.resize(Size + 1, 0);

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

            // Update flow for each vertex
            flow_per_vertex[current] += new_flow;

            current = previous;
        }
    }

    return flow;
}



class human
{
public :
    int height,age;
    bool divorced;
    human(int height,int age,bool divorced)
    {
        this->height=height;
        this->age=age;
        this->divorced=divorced;
    }

};

void solve(int i)
{
    int  m,n;
    cin >> m >> n;
    capacity.clear();
    graph.clear();
    capacity.resize(n+m+2, vector<int>(n+m+2, 0));
    graph.resize(n+m+2);
    vector<human> men;
    vector<human>women;

    for (int i = 1; i <= m; i++)
    {
        int height,age;
        bool divorced;
        cin>>height>>age>>divorced;
        men.push_back(human(height,age,divorced));
    }
    for (int i = 1; i <= n; i++)
    {
        int height,age;
        bool divorced;
        cin>>height>>age>>divorced;
        women.push_back(human(height,age,divorced));
    }


    for (int i = 1; i <=m; i++)
    {
        graph[0].push_back(i);
        graph[i].push_back(0);
        capacity[0][i]+=1;

    }

    for (int i = m+1; i <=n+m; i++)
    {
        graph[i].push_back(n+m+1);
        graph[n+m+1].push_back(i);
        capacity[i][m+n+1]+=1;
    }
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(abs(men[i-1].height-women[j-1].height)<=10 and abs(men[i-1].age-women[j-1].age)<=5 and men[i-1].divorced==women[j-1].divorced)
            {

                        graph[i].push_back(m+j);
                        graph[m+j].push_back(i);
                        capacity[i][m+j]+=1;
            }
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
