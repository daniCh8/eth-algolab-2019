/**
 * Bottom-up solution that gets 100 points. The trick relays in the fact that every optimal solution involves taking just one box in a stack and
 * a subset of boxes of the other stack (I had a hard time convincing myself of this).
 * But with that assumption, we can build every piece of our memo in a costant time.
 **/ 

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<int> a(n), b(n);
        for(int i = 0; i < n; i++)cin >> a[i];
        for(int i = 0; i < n; i++) cin >> b[i];

        vector<vector<int>> memo(n+1, vector<int>(n+1, INT32_MAX));
        memo[0][0] = 0;
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++) {
                memo[i][j] = min(memo[i-1][j], memo[i][j-1]);
                memo[i][j] = min(memo[i][j], memo[i-1][j-1]);
                memo[i][j] += (a[i-1]-1)*(b[j-1]-1);
            }

        cout << memo[n][n] << "\n";
    }
}
