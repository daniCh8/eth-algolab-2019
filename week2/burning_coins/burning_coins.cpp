/**
 * It's a dp where the memory saves how much coins you can win given the starting and ending position inside the vector of coin.
 * Your "friend" will play to minimise your winnings, so he will always take the coin that will minimise your possible winnings in the next round.
 * You, obviously, want to maximise your winnings and therefore will always take the coin that will return the best value.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int dp(vector<int> &coins, vector<vector<int>> &memo, int start, int stop) {
    if(start > stop) return 0;
    if(memo[start][stop] != -1) return memo[start][stop];
    if(start == stop) {
        memo[start][stop] = coins[start];
        return memo[start][stop];
    }

    memo[start][stop] = max(coins[start] + min(dp(coins, memo, start+2, stop), dp(coins, memo, start+1, stop-1)), coins[stop] + min(dp(coins, memo, start+1, stop-1), dp(coins, memo, start, stop-2)));
    return memo[start][stop];
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<int> coins(n);
        vector<vector<int>> memo(n, vector<int>(n, -1));
        for(int i = 0; i < n; i++) {
            cin >> coins[i];
        }

        cout << dp(coins, memo, 0, n-1) << "\n";
    }
}
