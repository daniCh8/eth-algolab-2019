/**
 * We can just keep track of the latest domino that will fall while we read them.
 * Every time we scan a tile, we check whether this tile will fall or not, and if it will, whether it will make fall more tiles than the previous ones.
**/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t, n;
    cin >> t;

    while(t > 0) {
        cin >> n;
        int max_domino = 1, i = 1, box = n;

        while(n > 0) {
            int tmp;
            cin >> tmp;
            int j = tmp + i - 1;
            if(i <= max_domino && max_domino < j) max_domino = min(j, box);
            i++;
            n--;
        }

        cout << max_domino << "\n";
        t--;
    }
}
