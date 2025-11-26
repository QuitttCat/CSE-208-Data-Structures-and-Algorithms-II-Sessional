#include<bits/stdc++.h>
using namespace std;

#define pii pair<int,int>
int INF=1000000007;
#define endl "\n"

class Current_State
{
public:
    int node,current_fuel,current_cost;
    Current_State(int node,int current_fuel,int current_cost)
    {
        this->node=node;
        this->current_fuel=current_fuel;
        this->current_cost=current_cost;
    }
    bool operator>(const Current_State &c)const
    {
        return current_cost>c.current_cost;
    }
};

int djkastra(int N,int C,vector<vector<pii>>&adList,vector<int>&cost_rate,int src,int dst)
{
    int dp_cost[N][C+1];
    for (int i = 0; i < N; i++) for (int j = 0; j <= C; j++) dp_cost[i][j] = INF;
    priority_queue<Current_State,vector<Current_State>,greater<Current_State>> pq;
    pq.push(Current_State(src,0,0));
    while (!pq.empty())
    {
        Current_State cs = pq.top();
        pq.pop();
        if (dp_cost[cs.node][cs.current_fuel] <cs.current_cost) continue;
        dp_cost[cs.node][cs.current_fuel]=cs.current_cost;
        if(cs.current_fuel+1<=C){
            pq.push(Current_State(cs.node,cs.current_fuel+1,min(dp_cost[cs.node][cs.current_fuel+1],cs.current_cost+cost_rate[cs.node])));
        }

        for (auto x : adList[cs.node])
        {

            int adj_note = x.first, distance = x.second;
            for(int i=0;i<=C;i++){
                if(cs.current_fuel+i<=C and cs.current_fuel+i-distance>=0){
                    if(dp_cost[adj_note][cs.current_fuel+i-distance]>cs.current_cost+i*cost_rate[cs.node])
                    {dp_cost[adj_note][cs.current_fuel+i-distance]=cs.current_cost+i*cost_rate[cs.node];
                    pq.push(Current_State(adj_note,cs.current_fuel+i-distance, dp_cost[adj_note][cs.current_fuel+i-distance]));}
                }
            }
        }
    }

    int ans = INF;
    for (int i = 0; i <= C; i++) ans = min(ans, dp_cost[dst][i]);

    return ans;




}



void solve(int i)
{
    int N,M,C;
    cin>>N>>M>>C;
    vector<vector<pii>> adList;
    adList.resize(N);

    int u,v,d;
    vector<int> cost_rate(N);
    for(auto &x:cost_rate) cin>>x;

    for (int i = 0; i < N; i++) adList[i].clear();///multiple tc support
    for (int i = 0; i < M; i++)
    {
        cin>>u>>v>>d;
        u--,v--; ///underlying 0 indexed,apperantly 1-indexed
        adList[u].push_back({v,d});
        adList[v].push_back({u,d});
    }
    int src,dst;
    cin>>src>>dst;
    src--;
    dst--;
    int total_cost = djkastra(N,C,adList,cost_rate,src,dst);
    if(total_cost==INF) cout<<"impossible";
    else cout<<total_cost<<"\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int test_cases=1;
    ///cin>>test_cases;
    for(int i=1; i<=test_cases; i++)
    {
        solve(i);
    }
}
