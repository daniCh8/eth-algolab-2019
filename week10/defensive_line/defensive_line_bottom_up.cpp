/**
 * This solution uses a bottom-up DP.
 * The defenders combinations are precomputed in the @compute_from_here method.
 * memo[i][j].first stores how many defenders I've attacked so far, memo[i][j].second stores how many attackers I've used so far.
 * Then, memo[i][j] = max(memo[i][j-1], memo[i-1][j-defenders_combination[j]]+defenders_combination[j])
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void compute_from_here(vector<pair<int, int>> &defenders, int goal, int index, int n) {
    int index_a = 0, index_b = 0, sum = 0;
    while(index_a < n && index_b < n) {
        while(sum < goal && index_b < n) sum += defenders[index_b++].first;
        while(sum > goal && index_a < n) sum -= defenders[index_a++].first;
        if(sum == goal && index_a < n) {
            defenders[index_b-1].second = index_b - index_a;
            sum -= defenders[index_a++].first;
        }
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m, k;
        cin >> n >> m >> k;

        vector<pair<int, int>> defenders(n);
        for(int i = 0; i < n; i++) {
            int defense;
            cin >> defense;
            defenders[i] = make_pair(defense, 0);
        }
        compute_from_here(defenders, k, 0, n);

        vector<vector<pair<int, int>>> memo(m+1, vector<pair<int, int>>(n+1, make_pair(0, 0)));
        for(int i = 1; i <= m; i++)
            for(int j = 1; j <= n; j++) {
                if(memo[i-1][j-defenders[j-1].second].first + defenders[j-1].second > memo[i][j-1].first) 
                    memo[i][j] = make_pair(memo[i-1][j-defenders[j-1].second].first + defenders[j-1].second, memo[i-1][j-defenders[j-1].second].second + 1);
                else memo[i][j] = memo[i][j-1];
            }

        if(memo[m][n].second != m) cout << "fail\n";
        else cout << memo[m][n].first << "\n";
    }
}
