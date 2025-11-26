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

void solve()
{
    int n, m;
    cin >> n >> m;
    capacity.resize(n + 1, vector<int>(n + 1, 0));
    graph.resize(n + 1);

    for (int i = 1; i <= m; i++)
    {
        int from, to, c;
        cin >> from >> to >> c;

        graph[from].push_back(to);
        graph[to].push_back(from);
        capacity[from][to] += c;
    }
    int src, snk;
    cin >> src >> snk;
    int mf = maxflow(n,src, snk);

    int v;
    int maxflowv=INT_MIN;
    for (int i = 1; i <= n; i++)
    {
       /// cout<<"node :"<<i<<" flow:"<<flow_per_vertex[i]<<"  \n";
        if(maxflowv<=flow_per_vertex[i] and i!=src and i!=snk)
        {
            maxflowv=flow_per_vertex[i];
            v=i;
        }
    }
    cout<<"(a)\n"<<v<<" "<<maxflowv<<"\n";
    min_cut_visit.resize(n + 1, 0);

    dfs(src);//finding min cut

    vector<int> s;
    vector<int> t;
    for (int i = 1; i <= n; i++)
        if (min_cut_visit[i])
            s.push_back(i);
        else
            t.push_back(i);

    cout << "(b)\n[{";
    for (int i = 0; i < s.size(); i++)
    {
        if (i == s.size() - 1)
            cout << s[i] << "},{";
        else
            cout << s[i] << ",";
    }
    for (int i = 0; i < t.size(); i++)
    {
        if (i == t.size() - 1)
            cout << t[i] << "}]\n";
        else
            cout << t[i] << ",";
    }
    cout << mf << endl;


}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int test_case = 1;
    while (test_case--)
    {
        solve();
    }
}
