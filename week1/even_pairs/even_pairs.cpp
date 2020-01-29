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
        int n, count = 0, n_zero = 0, n_one = 0;
        bool even = true;
        cin >> n;

        while(n > 0) {
            int tmp;
            cin >> tmp;
            if(tmp == 0) {
                count++; //the couple (pair, pair) is already an even pair.
                if(even) {
                    count += n_zero; //found a zero and the subset is even: all the previous zero-starting-positions can be coupled with the zero I've just found.
                    n_zero++; //a new zero-starting-position exists.
                } else {
                    count += n_one; //found a zero and the subset is not even: all the previous one-starting-positions can be coupled with the zero I've just found.
                    n_one++; //a new one-starting-position exists.
                }
            } else {
                if(!even) {
                    even = true; //the even property only changes when a one occurs.
                    n_zero++; 
                    n_one++;
                    count += n_zero; //found a one and the subset is not even: the subset becomes even and all the previous zero-starting locations can be couples with the one I've just found.
                } else {
                    even = false;
                    count += n_one; //found a one and the subset is even: the subset becomes not-even and all the previous 1-starting locations can be coupled with the one I've just found.
                }
            }

            n--;
        }

        cout << count << "\n";

        t--;
    }
}
