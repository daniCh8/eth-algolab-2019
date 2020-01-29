#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t = 0;
    cin >> t;

    while(t > 0) {
        int dim;
        cin >> dim;
        vector<vector<int>> matrix(dim+1, vector<int>(dim+1, 0));
        for(int i = 1; i <= dim; i++) {
            for(int j = 1; j <= dim; j++) {
                cin >> matrix[i][j];
            }
        }

        vector<vector<int>> memo(dim+1, vector<int>(dim+1, 0));
        for(int i = 1; i <= dim; i++) {
            for(int j = 1; j <= dim; j++) {
                memo[i][j] = memo[i][j-1] + matrix[i][j] + memo[i-1][j] - memo[i-1][j-1];
            }
        }

        int counter = 0;
        for(int i1 = 1; i1 <= dim; i1++) {
            for(int i2 = i1; i2 <= dim; i2++) {
                for(int j1 = 1; j1 <= dim; j1++) {
                    for(int j2 = j1; j2 <= dim; j2++) {
                        if((memo[i2][j2] - memo[i2][j1-1] - memo[i1-1][j2] + memo[i1-1][j1-1]) % 2 == 0)
                            counter++;
                    }
                }
            }
        }

        cout << counter << "\n";

        t--;
    }
}
