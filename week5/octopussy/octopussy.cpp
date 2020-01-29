/**
 * The solution is a greedy one that sorts the bombs in order of explosion-time and tries to defuse them in order.
 * If a bomb can't be defused in time, the answer is no: if all of them can be defused in time, the answer is yes.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool mysort(const pair<int, int> &i, const pair<int, int> &j) {
    return i.second < j.second;
}

bool will_he_make_it(vector<int> &bombs, int &time, vector<bool> &disinnescated, int index, int treshold) {
    if(disinnescated[index]) return true;
    if(index >= treshold) {
        if(time < bombs[index]) {
            disinnescated[index] = true;
            time++;
            return true;
        }
        return false;
    }

    if(time >= bombs[index]) return false;

    if(will_he_make_it(bombs, time, disinnescated, index*2+1, treshold) && will_he_make_it(bombs, time, disinnescated, index*2+2, treshold)) {
        if(time < bombs[index]) {
            disinnescated[index] = true;
            time++;
            return true;
        }
    }
    return false;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<pair<int, int>> bombs(n);
        vector<int> bombs_time(n);
        vector<bool> disinnescated(n, false);
        for(int i = 0; i < n; i++) {
            cin >> bombs_time[i];
            pair<int, int> no_time(i, bombs_time[i]);
            bombs[i] = no_time;
        }
        sort(bombs.begin(), bombs.end(), mysort);

        bool check = true;
        int time = 0;
        for(int i = 0; check && i < n; i++)
            check = will_he_make_it(bombs_time, time, disinnescated, bombs[i].first, (n-1)/2);

        check ? cout << "yes\n" : cout << "no\n";
    }
}