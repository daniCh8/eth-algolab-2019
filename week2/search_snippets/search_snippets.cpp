/**
 * Sliding windows on multiple vectors: I've flatten out the multiple vectors into a single one, sorted it and computed the shortest window that contained all the words.
 **/ 


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool mysort(pair<int, pair<int, int>> const &i, pair<int, pair<int, int>> const &j) {
    return i.first < j.first;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, counter = 0;
        cin >> n;
        vector<int> num_positions(n, 0);
        for(int i = 0; i < n; i++) {
            cin >> num_positions[i];
            counter += num_positions[i];
        }

        vector<pair<int, pair<int, int>>> ordered_positions(counter); //this is the flat vector that will contain all the words. it's a vector of pair<(position of the word), pair<(index of the word, next_position of the word)>.
        //if there's no next position of a word, the pair.second.second will be -1.
        int curr_a = 0, curr_b = -1;
        
        int index = 0;
        for(int i = 0; i < n; i++) {
            vector<int> box(num_positions[i]+1, -1);
            for(int j = 0; j < num_positions[i]; j++) {
                cin >> box[j];
            }
            for(int j = 0; j < num_positions[i]; j++) {
                pair<int, int> word_next(i, box[j+1]);
                pair<int, pair<int, int>> position_word_next(box[j], word_next);
                ordered_positions[index] = position_word_next;
                index++;
            }
            curr_b = max(curr_b, box[0]); //keeping track of the max starting position.
        }

        sort(ordered_positions.begin(), ordered_positions.end(), mysort); //ordered the flat vector by increasing words positions.
        int index_a = 0, index_b = 0;
        while(curr_b != ordered_positions[index_b].first) index_b++; //finding the index of the max starting position.
        curr_a = ordered_positions[index_a].first;
        int best_a = curr_a, best_b = curr_b;
        bool check = true;

        while(check) {
            while(ordered_positions[index_a].second.first == ordered_positions[index_a+1].second.first) index_a++; //looking for the nearest position of the starting word in the array.
            curr_a = ordered_positions[index_a].first;
            if(curr_b-curr_a < best_b-best_a) {
                best_a = curr_a;
                best_b = curr_b;
            }

            pair<int, pair<int, int>> p__w_n = ordered_positions[index_a];
            if(p__w_n.second.second == -1) check = false; //this word has not further occurrences: the cycle ends here.
            else {
                if(p__w_n.second.second > curr_b) curr_b = p__w_n.second.second; //if the next occurrence of the starting word is after the end of the sliding window, I update the endpoint of it.
                curr_a = ordered_positions[++index_a].first;  //updating the starting point of the window with the next word.
            }
        }

        cout << best_b-best_a+1 << "\n";
    }
}
