#include<bits/stdc++.h>
using namespace std;
#define INTMAX 100000
int knapsack(ifstream &in,ofstream &out)
{
    int n,capacity_of_knapsack;
    in>>n;
    in>>capacity_of_knapsack;
    vector<int> value(n+1,0);
    vector<int> weight(n+1,0);
    int target_value=0;
    for(int i=1;i<=n;i++)
    {
        in>>value[i];
        target_value+=value[i];
        in>>weight[i];
        //cout<<value[i]<<" "<<weight[i]<<endl;
    }
    //cout<<target_value;

   /* int dp[n+1][target_value+1];

      for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=target_value;j++)
        {
            dp[i][j]=0;
        }
    }*/

    vector<vector<int>> dp(n + 1, vector<int>(target_value + 1, 0));
    for(int i=0;i<=n;i++) dp[i][0]=0;
    for(int i=1;i<=target_value;i++) dp[0][i]=INTMAX;


    // for(int i=0;i<=n;i++)
    // {
    //     for(int j=0;j<=15;j++)
    //     {
    //         cout<<dp[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=target_value;j++)
        {
            if(j<value[i]) dp[i][j]=INTMAX;
            else dp[i][j]=min(dp[i-1][j],weight[i]+dp[i-1][j-value[i]]);
        }
    }
    // for(int i=0;i<=n;i++)
    // {
    //     for(int j=0;j<=15;j++)
    //     {
    //         cout<<dp[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }


    int maxValueObtainted=-1;
    for(int i=0;i<=target_value;i++)
        if(dp[n][i]<INTMAX and dp[n][i]<=capacity_of_knapsack)
    {
          maxValueObtainted=i;
    }
    int maxWeightObtained=dp[n][maxValueObtainted];
    int maxValueObtainted2=maxValueObtainted;
    vector<int> temp;
    for(int i=n;i>=1;i--)
    {
        if(dp[i][maxValueObtainted2]<INTMAX and maxValueObtainted2>0 and dp[i][maxValueObtainted2]==dp[i-1][maxValueObtainted2-value[i]]+weight[i])
        {temp.push_back(i);
            maxValueObtainted2-=value[i];}
    }
    //reverse(temp.begin(),temp.end());

out<<"answer: "<<maxValueObtainted<<"\n";
out<<"Used Weight: "<<maxWeightObtained<<"\n";
out<<"Indices: ";
for(auto x:temp) out<<x<<" ";
out<<"\n";
return maxValueObtainted;
}


void FPTAS(ifstream &in,ofstream &out,double epsilon,int original)
{

    int n,capacity_of_knapsack;
    in>>n;
    in>>capacity_of_knapsack;
    vector<int> value(n+1,0);
    vector<int> value2(n+1,0);
    vector<int> weight(n+1,0);
    int target_value=0;
    double theta;
    int mx=-1;
    for(int i=1;i<=n;i++)
    {
        in>>value[i];
        target_value+=value[i];
        mx=max(mx,value[i]);
        in>>weight[i];
        //cout<<value[i]<<" "<<weight[i]<<endl;
    }
    value2=value;
    theta=(double)mx*epsilon/(2.0*n);
    //cout<<theta;
    for(int i=1;i<=n;i++)
    {
        value[i]=ceil(value[i]/theta);
        //cout<<value[i]<<" ";
        target_value+=value[i];
    }
    //cout<<target_value;

   /* int dp[n+1][target_value+1];

      for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=target_value;j++)
        {
            dp[i][j]=0;
        }
    }*/

    vector<vector<int>> dp(n + 1, vector<int>(target_value + 1, 0));
    for(int i=0;i<=n;i++) dp[i][0]=0;
    for(int i=1;i<=target_value;i++) dp[0][i]=INTMAX;


    // for(int i=0;i<=n;i++)
    // {
    //     for(int j=0;j<=15;j++)
    //     {
    //         cout<<dp[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=target_value;j++)
        {
            if(j<value[i]) dp[i][j]=INTMAX;
            else dp[i][j]=min(dp[i-1][j],weight[i]+dp[i-1][j-value[i]]);
        }
    }

    // for(int i=0;i<=n;i++)
    // {
    //     for(int j=0;j<=15;j++)
    //     {
    //         cout<<dp[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }


    int maxValueObtainted=-1;
    for(int i=0;i<=target_value;i++)
        if(dp[n][i]<INTMAX and dp[n][i]<=capacity_of_knapsack)
    {
          maxValueObtainted=i;
    }


    int maxWeightObtained=dp[n][maxValueObtainted];

    int maxValueObtainted2=maxValueObtainted;
    //cout<<maxValueObtainted2<<" ";

    vector<int> temp;
    for(int i=n;i>=1;i--)
    {
        if(dp[i][maxValueObtainted2]<INTMAX and maxValueObtainted2>0 and dp[i][maxValueObtainted2]==dp[i-1][maxValueObtainted2-value[i]]+weight[i])
        {temp.push_back(i);


            maxValueObtainted2-=value[i];
            ///cout<<maxValueObtainted2<<" ";
            }

    }

    //reverse(temp.begin(),temp.end());

out<<"\n\n";
out<<"Rounded Instance with Eps: "<<epsilon<<"\n";
out<<fixed<<setprecision(16)<<"Theta: "<<theta<<"\n";
out<<"Answer of reduced instance: "<<maxValueObtainted<<"\n";
out<<fixed<<setprecision(12)<<"Answer of reduced instance multiplied by theta: "<<maxValueObtainted*theta<<"\n";
out<<"Indices: ";
for(auto x:temp) out<<x<<" ";
out<<"\n";

int original_roundedup=0;
for(auto x:temp) original_roundedup+=value2[x];
out<<"Answer of original instance (rounded up): "<<original_roundedup<<"\n";
out<<"Used Weight: "<<maxWeightObtained<<"\n";
out<<fixed<<setprecision(16)<<"Ratio: "<<(double)original/original_roundedup<<"\n";
}




int main()
{

    ifstream in("test_case4.txt");
    ofstream out("out14.txt");
    int result=knapsack(in,out);
     in.seekg(0, ios::beg);
     FPTAS(in,out,0.002,result);
     in.seekg(0, ios::beg);
     FPTAS(in,out,0.05,result);
     in.seekg(0, ios::beg);

    FPTAS(in,out,0.5,result);
     in.seekg(0, ios::beg);

      FPTAS(in,out,0.3,result);
     in.seekg(0, ios::beg);
    FPTAS(in,out,0.2,result);
     in.seekg(0, ios::beg);
    FPTAS(in,out,0.1,result);
     in.seekg(0, ios::beg);
    FPTAS(in,out,0.05,result);
    in.close();
    out.close();
}

