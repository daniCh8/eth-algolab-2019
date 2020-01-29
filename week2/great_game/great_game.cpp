/**
 * DP where the memory saves the number of moves needed to go towards the goal.
 * I've used two memories, one that saves the number of moves needed to reach the goal when the move is "good" (made to arrive as soon as possible),
 *             and another one that saves the number of moves needed to reach the foal when the move is "bad" (made to arrive as late as possible).
 * If from a given position it's not possible to arrive to the goal, the flag -2 is used.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int dp_good(vector<int> &good_moves, vector<int> &bad_moves, vector<vector<int>> &transitions, int start, int goal);
int dp_bad(vector<int> &good_moves, vector<int> &bad_moves, vector<vector<int>> &transitions, int start, int goal);

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    
    while(t > 0) {
        int n, m, start_r, start_b;
        cin >> n >> m >> start_r >> start_b;
        vector<vector<int>> transitions(n);
        for(int i = 0; i < m; i++) {
            int from, to;
            cin >> from >> to;
            transitions[from-1].push_back(to-1);
        }

        vector<int> good_moves(n, -1), bad_moves(n, -1);

        if(dp_good(good_moves, bad_moves, transitions, start_r-1, n-1) < dp_good(good_moves, bad_moves, transitions, start_b-1, n-1)) cout << "0\n";
        else if(good_moves[start_r-1] == good_moves[start_b-1]) {
            if(good_moves[start_r-1]%2 == 0) cout << "1\n";
            else cout << "0\n";
        } else cout << "1\n";

        t--;
    }
}

int dp_bad(vector<int> &good_moves, vector<int> &bad_moves, vector<vector<int>> &transitions, int start, int goal) {
    if(bad_moves[start] != -1) return bad_moves[start];
    if(start == goal) {
        bad_moves[start] = 0;
        return 0;
    }

    int moves = INT32_MIN;
    for(int i = 0; i < (int)transitions[start].size(); i++) {
        int box = dp_good(good_moves, bad_moves, transitions, transitions[start][i], goal);
        if(box != -2) moves = max(moves, 1 + box);
    }
    
    if(moves != INT32_MIN) bad_moves[start] = moves;
    else bad_moves[start] = -2;
    return bad_moves[start];
}

int dp_good(vector<int> &good_moves, vector<int> &bad_moves, vector<vector<int>> &transitions, int start, int goal) {
    if(good_moves[start] != -1) return good_moves[start];
    if(start == goal) {
        good_moves[start] = 0;
        return 0;
    }

    int moves = INT32_MAX;
    for(int i = 0; i < (int)transitions[start].size(); i++) {
        int box = dp_bad(good_moves, bad_moves, transitions, transitions[start][i], goal);
        if(box != -2) moves = min(moves, 1 + box);
    }

    if(moves != INT32_MAX) good_moves[start] = moves;
    else good_moves[start] = -2;
    return good_moves[start];
}
