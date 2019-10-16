#include <vector>
#include <algorithm>

using namespace std;

int solveDP(vector<int> & weights, vector<int> & costs, int knapsackCap){
    vector<int> dp[] = {vector<int>(knapsackCap+1, 0), vector<int>(knapsackCap+1, 0)};

    int pos = 0;

    for (int i = 0; i < weights.size(); i++) {
        for (int cap = 1; cap <= knapsackCap; cap++)    {
            if (cap < weights[i]){
                dp[pos][cap] = dp[!pos][cap];
            }else{
                dp[pos][cap] = max(dp[!pos][cap], costs[i] + dp[!pos][cap - weights[i]]);
            }
        }
        pos = !pos;
    }
    
    return dp[!pos][knapsackCap];
}