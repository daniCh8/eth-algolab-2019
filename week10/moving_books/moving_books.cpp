/**
 * The solution uses a greedy algorithm. It sorts the friends as well as the books by decreasing weight.
 * The number of times that Simone's friends will need to go up and downstairs is 
 * the maximum between all the possible divisions between a subset of friends and the subset of books that the subset of friends can move without the help of any other friend.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int mysort(const int &i, const int &j) {
    return i > j;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m;
        cin >> n >> m;

        vector<int> friends(n), books(m);
        for(int i = 0; i < n; i++) cin >> friends[i];
        for(int i = 0; i < m; i++) cin >> books[i];
        sort(friends.begin(), friends.end(), mysort);
        sort(books.begin(), books.end(), mysort);

        if(friends[0] < books[0]) {
            cout << "impossible\n";
            continue;
        }

        int times = m/n, strenght = books[0], friends_index = 0, books_index = 0;
        if(m%n != 0) times++;
        while(friends_index < n && friends[friends_index] >= strenght) friends_index++;
        while(friends_index < n) {
            while(books_index < m && books[books_index] > friends[friends_index]) books_index++;
            int box = books_index/friends_index;
            if(books_index%friends_index != 0) box++;
            times = max(times, box);
            strenght = books[books_index];
            int prev_friends_index = friends_index;
            while(friends_index < n && friends[friends_index] >= strenght) friends_index++;
            if(prev_friends_index == friends_index) friends_index = n;
        }

        cout << times*2+(times-1) << "\n"; //time to go downstairs (2 minutes) and to go upstairs(1 minute, but not the last time)

    }
}
