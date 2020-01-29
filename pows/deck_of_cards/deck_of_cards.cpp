/**
 * It's just a sliding window that goes through the deck of cards and keeps track of the subset sum that is closer to the number we're looking for.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, k;
        cin >> n >> k;

        vector<int> values(n);
        for(int i = 0; i < n; i++) {
            cin >> values[i];
        }

        int sum = values[0], best_sum = values[0], best_i = 0, best_j = 0, curr_i = 0, curr_j = 0;
        for(int i = 1; i < n; i++) {
            curr_j++;
            if(best_sum == k) {
                i = n;
            } else {
                sum += values[i];
                if(abs(sum - k) < abs(best_sum - k)) {
                    best_i = curr_i;
                    best_j = curr_j;
                    best_sum = sum;
                }
                while(sum > k && curr_i < curr_j) {
                    sum -= values[curr_i];
                    curr_i++;
                }
                if(abs(sum - k) < abs(best_sum - k)) {
                    best_i = curr_i;
                    best_j = curr_j;
                    best_sum = sum;
                }
            }
        }

        cout << best_i << " " << best_j << "\n";
    }
}
