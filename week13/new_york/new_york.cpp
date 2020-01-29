/**
 * This solution to this problem creates a tree and visits it from every leaf to the root.
 * In every visit, it checks the points in the path between the leaf and the root that are safe_spots (critical points where the captain can start a trip).
 * It also makes sure that no starting point of the visit is visited twice.
 **/ 

#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

void dfs(vector<int> &father, vector<bool> &safe_spots, vector<bool> &memo, vector<int> &h, int m, int k, int index) {
    if(memo[index]) return;
    queue<int> path;
    multiset<int> temps;
    path.push(index);
    memo[index] = true;
    temps.insert(h[index]);
    int max_temp = h[index], min_temp = max_temp, next = index;
    while(true) {
        if(max_temp - min_temp > k || (int)path.size() == m) {
            memo[index] = true;
            if((!(max_temp - min_temp > k)) && ((int)path.size() == m)) safe_spots[next] = true;
            path.pop();
            temps.erase(temps.find(h[index]));
            index = father[index];
            if(memo[index]) return;
        }
        next = father[next];
        if(next == -1) return;
        path.push(next);
        temps.insert(h[next]);
        auto it = temps.end();
        it--;
        max_temp = *(it);
        min_temp = *(temps.begin());
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

        vector<int> h(n);
        for(int i = 0; i < n; i++) cin >> h[i];

        vector<int> father(n, -1);
        vector<bool> beginner(n, true), memo(n, false), safe_spots(n, false);
        for(int i = 0; i < n-1; i++) {
            int from, to;
            cin >> from >> to;
            father[to] = from;
            beginner[from] = false;
        }

        for(int i = 0; i < n; i++) 
            if(beginner[i]) dfs(father, safe_spots, memo, h, m, k, i);
        
        bool feasible = false;
        for(int i = 0; i < n; i++)
            if(safe_spots[i]) {
                feasible = true;
                cout << i << " ";
            }
        if(!feasible) cout << "Abort mission\n";
        else cout << "\n";
    }
}