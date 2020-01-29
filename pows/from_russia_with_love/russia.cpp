/**
 * It's basically the same as burning coins: the memo saves the maximum value that you can collect given the starting and ending position in the vector of coins.
 * This time, there are more than two people playing the game and therefore every next turn that I will play is the one where I can collect the minimum value between the possible one,
 * because the other players will be playing all against me.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int dp(vector<int> &coins, vector<vector<int>> &memo, int start, int stop, int m) {
    if(start > stop) return 0;
    if(memo[start][stop] != -1) return memo[start][stop];
    if(start == stop) {
        memo[start][stop] = coins[start];
        return memo[start][stop];
    }

    int box_start = INT32_MAX, box_stop = INT32_MAX;
    for(int i = 0; i < m; i++) {
        box_start = min(box_start, dp(coins, memo, start+m-i, stop-i, m));
        box_stop = min(box_stop, dp(coins, memo, start+i, stop-m+i, m));
    }

    memo[start][stop] = max(coins[start] + box_start, coins[stop] + box_stop);
    return memo[start][stop];
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> coins(n);
        vector<vector<int>> memo(n, vector<int>(n, -1));
        for(int i = 0; i < n; i++) {
            cin >> coins[i];
        }

        int result = INT32_MAX;
        for(int i = 0; i <= k; i++)
            result = min(result, dp(coins, memo, i, n-k+i-1, m));

        cout << result << "\n";
    }
}
