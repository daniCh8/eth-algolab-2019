/**
 * The solution is a greedy algorithm that sorts the boats and always takes the one with the least possible endpoint.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool mysort(const pair<int, int> &i, const pair<int, int> &j) {
    return i.second < j.second;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<pair<int, int>> l_p(n);
        for(int i = 0; i < n; i++) {
            int l, p;
            cin >> l >> p;
            pair<int, int> box(l, p);
            l_p[i] = box;
        }

        sort(l_p.begin(), l_p.end(), mysort);

        int counter = 1, pos = l_p[0].second;
        l_p.erase(l_p.begin());
        while(l_p.size() > 0) {
            int index = 0, best_index = 0, next_pos = max(l_p[index].first + pos, l_p[index].second);
            while(++index < (int)l_p.size() && next_pos > l_p[index].second) {
                int tmp = max(l_p[index].first + pos, l_p[index].second);
                if(tmp < next_pos) {
                    next_pos = tmp;
                    best_index = index;
                }
            }

            pos = next_pos;
            counter++;

            best_index++;
            while(best_index < (int)l_p.size() && l_p[best_index].second < pos) best_index++;
            l_p.erase(l_p.begin(), l_p.begin() + best_index);

        }

        cout << counter << "\n";
    }
}
