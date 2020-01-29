/**
 * This solution is a top-down DP that uses a matrix [n*k] as memory, where memo[i][j] = # of points that can be scored from position i and with j remaining moves.
 * The answer of the problem is then "Impossible" if memo[0][j] is less than the claimed score of the chief for every j between 0 and k-1, 
 * otherwise it's the first occurrence of that j.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef std::pair<int, int> edge;

using namespace std;

long int dp(vector<vector<long int>> &memo, vector<vector<edge>> &edges, int position, int moves) {
    if(memo[position][moves] != -1) return memo[position][moves];
    long int max_points = numeric_limits<long>::min();
    for(int i = 0; i < (int)edges[position].size(); i++) {
        if(edges[position][i].second == -1) max_points = max(max_points, dp(memo, edges, 0, moves));
        else max_points = max(max_points, edges[position][i].second + dp(memo, edges, edges[position][i].first, moves-1));
    }
    memo[position][moves] = max_points;
    return max_points;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m, k;
        long int x;
        cin >> n >> m >> x >> k;

        vector<vector<edge>> edges(n, vector<edge>());
        for(int i = 0; i < m; i++) {
            int u, v, p;
            cin >> u >> v >> p;
            edges[u].push_back(make_pair(v, p));
        }
        for(int i = 0; i < n; i++)
            if(edges[i].size() == 0) edges[i].push_back(make_pair(0, -1));

        vector<vector<long int>> memo(n, vector<long int>(k+1, -1));
        for(int i = 0; i < n; i++) memo[i][0] = 0;
        bool possible = false;
        for(int i = 1; i <= k; i++) {
            if(dp(memo, edges, 0, i) >= x) {
                cout << i << "\n";
                possible = true;
                i = k;
            }
        }
        if(!possible) cout << "Impossible\n";
    }
}
