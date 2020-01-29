#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t > 0) {
        int sum = 0, n;
        cin >> n;
        
        while(n > 0) {
            int tmp;
            cin >> tmp;
            sum += tmp;
            n--;
        }

        cout << sum << "\n";

        t--;
    }
}
