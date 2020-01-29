/**
 * This is a dp on a tree. I've used two memories:
 * memo_true is a vector of nodes that saves the cost of the subtree when its root has not been paid, but its cost can be avoided (because its father has been paid).
 * memo_false is a vector of nodes that saves the cost of the subtree when its root has not been paid, and its father has not been paid.
 * therefore, memo_false[i] = min{(cost[i] + cost_of_all_the_childs_of_i_at_true),
 *                                (cost_of_one_child_of_i + cost_of_all_the_childs_of_that_child_at_true + cost_of_all_the_other_childs_at_false)}.
 *            memo_true[i] = min{(cost[i] + cost_of_all_the_childs_of_i_at_true),
 *                               (cost_of_all_childs_of_i_at_false)}.
 * The result of the problem is therefore memo_false[0].
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int dp_true(vector<int> &memo_true, vector<int> &memo_false, vector<vector<int>> &connections, vector<int> &cost, int position);
int dp_false(vector<int> &memo_true, vector<int> &memo_false, vector<vector<int>> &connections, vector<int> &cost, int position);

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        
        vector<int> cost(n);
        vector<vector<int>> connections(n, vector<int>());
        for(int i = 0; i < n-1; i++) {
            int from, to;
            cin >> from >> to;
            connections[from].push_back(to);
        }
        for(int i = 0; i < n; i++)
            cin >> cost[i];

        vector<int> memo_true(n, -1), memo_false(n, -1);
        cout << dp_false(memo_true, memo_false, connections, cost, 0) << "\n";
    }
}

int dp_true(vector<int> &memo_true, vector<int> &memo_false, vector<vector<int>> &connections, vector<int> &cost, int position) {
    if(memo_true[position] != -1) return memo_true[position];
    if(connections[position].size() == 0) {
        memo_true[position] = 0;
        return 0;
    }

    int me_true = cost[position], all_childs_false = 0;
    for(int i = 0; i < (int)connections[position].size(); i++) {
        me_true += dp_true(memo_true, memo_false, connections, cost, connections[position][i]);
        all_childs_false += dp_false(memo_true, memo_false, connections, cost, connections[position][i]);
    }

    memo_true[position] = min(all_childs_false, me_true);
    return memo_true[position];
}

int dp_false(vector<int> &memo_true, vector<int> &memo_false, vector<vector<int>> &connections, vector<int> &cost, int position) {
    if(memo_false[position] != -1) return memo_false[position];
    if(connections[position].size() == 0) {
        memo_false[position] = cost[position];
        return cost[position];
    }

    int me_true = cost[position], me_false = -1, all_childs_false = 0;
    for(int i = 0; i < (int)connections[position].size(); i++) {
        me_true += dp_true(memo_true, memo_false, connections, cost, connections[position][i]);
        all_childs_false += dp_false(memo_true, memo_false, connections, cost, connections[position][i]);
    }

    for(int i = 0; i < (int)connections[position].size(); i++) {
        int grand_childs_true = 0;
        for(int j = 0; j < (int)connections[connections[position][i]].size(); j++)
            grand_childs_true += dp_true(memo_true, memo_false, connections, cost, connections[connections[position][i]][j]);
        if(me_false == -1) {
            me_false = grand_childs_true + all_childs_false - memo_false[connections[position][i]] + cost[connections[position][i]];
        } else me_false = min(me_false, grand_childs_true + all_childs_false - memo_false[connections[position][i]] + cost[connections[position][i]]);
    }

    memo_false[position] = min(me_false, me_true);
    return memo_false[position];
}
