/**
 * This solution uses a top-down DP and is way more burdensome than the other one.
 * The memory saves how many defenders can be tackled starting at a given position and with j attackers remaining.
 * The defenders combinations are precomputed in the @compute_from_here method.
 * The dp @defenders_tackled sees whether it's better to tackle the defenders starting from a given position or just pass to the next defender.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * This is just an helper function when only one attacker remains.
 **/ 
int one_attacker(vector<pair<int, int>> &defenders, int n, int index) {
    int max_value = -1;
    for(int i = index; i < n; i++) max_value = max(max_value, defenders[i].second);
    if(max_value != -1) return max_value+1;
    return max_value;
}

void compute_from_here(vector<pair<int, int>> &defenders, int goal, int index, int n) {
    int index_a = 0, index_b = 0, sum = 0;
    while(index_a < n && index_b < n) {
        while(sum < goal && index_b < n) sum += defenders[index_b++].first;
        while(sum > goal && index_a < n) sum -= defenders[index_a++].first;
        if(sum == goal && index_a < n) {
            defenders[index_a].second = index_b - index_a - 1;
            sum -= defenders[index_a++].first;
        }
    }
}

int defenders_tackled(vector<pair<int, int>> &defenders, vector<vector<int>> &memo, int n, int m, int attackers_used, int index) {
    if(attackers_used >= m || index >= n) return -1;
    if(attackers_used == m-1) {
        memo[attackers_used][index] = one_attacker(defenders, n, index);
        return memo[attackers_used][index];
    }
    if(memo[attackers_used][index] != -2) return memo[attackers_used][index];
    if(defenders[index].second == -1) {
        memo[attackers_used][index] = defenders_tackled(defenders, memo, n, m, attackers_used, index+1);
        return memo[attackers_used][index];
    }
    
    int box1 = defenders_tackled(defenders, memo, n, m, attackers_used+1, index+1+defenders[index].second);
    int box2 = defenders_tackled(defenders, memo, n, m, attackers_used, index+1);
    if(box1 == -1) memo[attackers_used][index] = box2;
    else memo[attackers_used][index] = max(1 + defenders[index].second + box1, box2);
    return memo[attackers_used][index];
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
            defenders[i] = make_pair(defense, -1);
        }
        compute_from_here(defenders, k, 0, n);

        vector<vector<int>> memo(m, vector<int>(n, -2));
        int result = defenders_tackled(defenders, memo, n, m, 0, 0);
        if(result == -1) cout << "fail\n";
        else cout << result << "\n";
    }
}
