/**
 * Yet another sliding window. I compute the windows that have a size under 200, and see which of those have the max number of people inside and the least distance to walk to the middle point.
 * The inserter function is just there to avoid repetitions (it gets burdensome because the coordinates sum can be not even).
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void pos_inserter(vector<int> &best_positions, int coord_sum) {
    if(coord_sum % 2 != 0) {
        if(coord_sum > 0) {
            best_positions.push_back(coord_sum/2);
            best_positions.push_back((coord_sum/2)+1);
        }
        else {
            best_positions.push_back((coord_sum/2)-1);
            best_positions.push_back(coord_sum/2);
        }
    } else best_positions.push_back(coord_sum/2);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<int> parasols_coordinates(n);
        for(int i = 0; i < n; i++)
            cin >> parasols_coordinates[i];
        sort(parasols_coordinates.begin(), parasols_coordinates.end());

        vector<int> best_positions;
        int a = 0, best_distance = 0, best_people = 1;
        best_positions.push_back(a);
        for(int b = 1; b < n; b++) {
            while(a < b && parasols_coordinates[b] - parasols_coordinates[a] > 200) a++;
            int coord_sum = parasols_coordinates[b] + parasols_coordinates[a];
            int new_distance = max(abs(parasols_coordinates[b] - coord_sum/2), abs(coord_sum/2 - parasols_coordinates[a]));
            if((b - a + 1 > best_people) || (b - a + 1 == best_people && best_distance > new_distance)) {
                best_people = b - a + 1;
                best_positions.clear();
                best_distance = new_distance;
                pos_inserter(best_positions, coord_sum);
            } else if(b - a + 1 == best_people && best_distance == new_distance) {
                pos_inserter(best_positions, coord_sum);
            }
        }

        cout << best_people << " " << best_distance << "\n";
        for(int i = 0; i < (int)best_positions.size(); i++) {
            cout << best_positions[i];
            if(i != (int)best_positions.size()-1) cout << " ";
        }
        cout << "\n";
    }
}
