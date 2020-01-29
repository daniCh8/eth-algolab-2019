/**
 * It's a bottom-up dp where the memory is a matrix n*(k+max_volume).
 * m[i][j] = min{m[i-1][j-liters[i]] + drink[i], m[i-1][j], m[i][j-liters[i] + drink[i]]}
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

pair<int, int> min_tuple(pair<int, int> &i, pair<int, int> &j) {
    if(i.first < j.first) return i;
    if(i.first > j.first) return j;
    if(i.second > j.second) return i;
    return j;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t > 0) {
        int n, k;
        cin >> n >> k;

        vector<pair<int, int>> cost_volume(n);
        int max_volume = -1;
        for(int i = 0; i < n; i++) {
            int c, v;
            cin >> c >> v;
            pair<int, int> c_v(c, v);
            cost_volume[i] = c_v;
            max_volume = max(max_volume, v);
        }

        pair<int, int> default_pair(0, 0);
        //matrix[i][j] is a pair <tot_cost, number_of_different_beverages>, and j represents the volume reached;
        vector<vector<pair<int, int>>> beverages_matrix(n, vector<pair<int, int>>(k + max_volume, default_pair));

        for(int i = 1; i < k + max_volume; i++) {
            int how_many = i / cost_volume[0].second;
            if(i % cost_volume[0].second != 0) how_many++;
            pair<int, int> tuple(how_many*cost_volume[0].first, 1);
            beverages_matrix[0][i] = tuple;
        }

        for(int i = 1; i < n; i++) {
            for(int j = 1; j < k + max_volume; j++) {
                beverages_matrix[i][j] = beverages_matrix[i-1][j];
                if(cost_volume[i].second > j) continue;
                pair<int, int> tuple_a = beverages_matrix[i-1][j-cost_volume[i].second];
                pair<int, int> tuple_b = beverages_matrix[i][j-cost_volume[i].second];
                tuple_a.first += cost_volume[i].first;
                tuple_a.second++;
                tuple_b.first += cost_volume[i].first;
                beverages_matrix[i][j] = min_tuple(beverages_matrix[i][j], tuple_a);
                beverages_matrix[i][j] = min_tuple(beverages_matrix[i][j], tuple_b);
            }
        }

        pair<int, int> best_volume(INT32_MAX, INT32_MAX);
        for(int i = k; i < k+max_volume; i++) {
            best_volume = min_tuple(best_volume, beverages_matrix[n-1][i]);
        }

        cout << best_volume.first << " " << best_volume.second << "\n";
    }
}
