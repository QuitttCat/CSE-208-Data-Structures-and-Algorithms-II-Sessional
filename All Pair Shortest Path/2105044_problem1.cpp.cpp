#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

int INF=1000000007;
#define endl "\n"



void solve(int i)
{
    ll n,m;
    cin>>n>>m;
    int d[n+1][n+1];
    for (ll i = 0; i <= n; ++i)
    {
        for (ll j = 0; j <= n; ++j)
        {
            if(i!=j) d[i][j]=INF;
            else d[i][j]=0;
        }
    }


    for (ll i = 1; i <= m; ++i)
    {
        ll a,b,w;
        cin>>a>>b>>w;
        if(d[a][b]==INF or (d[a][b]>w and d[a][b]!=INF))
        {
            d[a][b]=w;
            d[b][a]=w;
        }
    }



    for (ll k = 1; k <= n; ++k)
    {
        for (ll i = 1; i <= n; ++i)
        {
            for (ll j = 1; j <= n; ++j)
            {
                if (d[i][k] < INF && d[k][j] < INF)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
    int th;
    cin>>th;
    vector<int> nodes_info;
    for (ll k = 1; k <= n; ++k)
    {
        int cnt=0;
        for (ll i = 1; i <= n; ++i)
        {
            if(k!=i and d[k][i]<=th) cnt++;
        }
        nodes_info.push_back(cnt);
    }
    /*
    for (ll k = 1; k <= n; ++k)
    {

        for (ll i = 1; i <= n; ++i)
        {
            cout<<d[k][i]<<" ";
        }
        cout<<endl;

    }*/
    int minimum=*min_element(nodes_info.begin(), nodes_info.end());
    ///cout<<minimum<<endl;
    for(int i=0;i<n;i++) if(nodes_info[i]==minimum) cout<<i+1<<" ";
    cout<<endl;


}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int test_cases=1;
    //cin>>test_cases;
    for(int i=1; i<=test_cases; i++)
    {
        solve(i);
    }
}
