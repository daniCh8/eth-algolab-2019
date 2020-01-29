/**
 * This solution is as short as it is bad. It takes 100 point - but maybe in future it won't:
 * it's a top-down dp, that uses a memory n*m: at every recursive call, it just makes all the possible plays and see which one would be the best one.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

double winning_chances(vector<vector<double>> &memo, vector<double> &probabilities, int day, int wealth, int goal, int n) {
    if(wealth >= goal) return 1.0;
    if(day >= n || wealth == 0) return 0.0;
    if(memo[day][wealth] != -1) return memo[day][wealth];

    double max_prob = 0.0;
    for(int i = 0; i <= wealth; i++) {
        double p = probabilities[day];
        double hurra = p*winning_chances(memo, probabilities, day+1, wealth+i, goal, n);
        double ohno = (1-p)*winning_chances(memo, probabilities, day+1, wealth-i, goal, n);
        max_prob = max(max_prob, hurra+ohno);
    }
    memo[day][wealth] = max_prob;
    return memo[day][wealth];
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        int n, k, m;
        cin >> n >> k >> m;
        vector<double> probabilities(n);
        for(int i = 0; i < n; i++) cin >> probabilities[i];

        vector<vector<double>> memo(n, vector<double>(m, -1));
        cout << fixed << setprecision(5) << winning_chances(memo, probabilities, 0, k, m, n) << "\n";
    }
}
