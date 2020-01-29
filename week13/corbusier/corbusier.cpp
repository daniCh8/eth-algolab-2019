/**
 * This solution is made through a bottom-up dp. The memory is a [n*k] boolean matrix that stores in a cell memo[i][j] whether or not
 * it's possible to build a tower of disks that have a height congruent to j using only disks from 0 to i.
 * memo[i][j] = (memo[i-1][j] because if it was possible with disks from 0 to i-1, it's also possible with disks from 0 to i
 *               || memo[i-1][(j-disk[i]+k)%k] because if I can reach the modulo j-disk[i] without disk[i], I just need to add the latter to have modulo j
 *               || disk[i] == j because I would take that disk only)
 * The answer is then whether memo[n][i] is true or not.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, i, k;
		cin >> n >> i >> k;
		vector<int> disks(++n, 0);
		for(int j = 1; j < n; j++) {
			cin >> disks[j];
			disks[j] %= k;
		}

		vector<vector<bool>> memo(n, vector<bool>(k, false));
		for(int r = 1; r < n; r++)
			for(int c = 0; c < k; c++)
				memo[r][c] = (disks[r] == c || memo[r-1][c] || memo[r-1][(c-disks[r]+k)%k]);
		memo[n-1][i] ? cout << "yes\n" : cout << "no\n";
    }
}