/**
 * 60 points - top-down DP: matrix[n*n] where each position (i, j) saves the smallest possible sum of costs if the stacks are at positions i and j.
 * The precompute matrix is to not have duplicated sum.
 * This solution took 100 points when the problem was uploaded, but then the whole problem was rejudged with new tests and now takes only 60.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int dp(vector<vector<int>> &memo, vector<vector<int>> &precompute, int i_a, int i_b) {
    if(i_a == 0 || i_b == 0) memo[i_a][i_b] = precompute[i_a][i_b];
    if(memo[i_a][i_b] != INT32_MAX) return memo[i_a][i_b];
    memo[i_a][i_b] = precompute[i_a][i_b];
    for(int i = 1; i <= i_a; i++)
        memo[i_a][i_b] = min(memo[i_a][i_b], precompute[i_a][i_b] - precompute[i_a-i][i_b] - precompute[i_a][i_b-1] + precompute[i_a-i][i_b-1] + dp(memo, precompute, i_a-i, i_b-1));
    for(int j = 1; j <= i_b; j++)
        memo[i_a][i_b] = min(memo[i_a][i_b], precompute[i_a][i_b] - precompute[i_a-1][i_b] - precompute[i_a][i_b-j] + precompute[i_a-1][i_b-j] + dp(memo, precompute, i_a-1, i_b-j));
    return memo[i_a][i_b];
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<int> a(n), b(n), sum_a(n+1, 0), sum_b(n+1, 0);
        for(int i = 0; i < n; i++) {
            cin >> a[i];
            sum_a[i+1] += (sum_a[i] + a[i]);
        }
        for(int i = 0; i < n; i++) {
            cin >> b[i];
            sum_b[i+1] += (sum_b[i] + b[i]);
        }

        vector<vector<int>> memo(n, vector<int>(n, INT32_MAX)), precompute(n, vector<int>(n));
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                precompute[i][j] = (sum_a[i+1] - (i+1))*(sum_b[j+1] - (j+1));

        cout << dp(memo, precompute, n-1, n-1) << "\n";
    }
}
