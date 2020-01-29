/**
 * This solution involves a dfs on the tree of ancestor. Such tree is generated as given by the problem; 
 * The queries are then saved in a vector of vectors, that binds to every species all of its queries.
 * We start visiting the tree from the root, and at every iteration of the dfs we solve all the queries that correspond to the species of the iteration.
 * This way, don't need to build a vector of ancestors every time we encounter a query.
 * Functions @log_search_string and @log_search_age are binary researches for strings and ages to speed up the solution.
 * Note that we need to save the indexes of the queries, because the answers must be given sorted. Therefore we can store all the solution in an array
 * and iterate on that array when it has been fully computed.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

bool mysort(const pair<string, int> &i, const pair<string, int> &j) {
    return i.first < j.first;
}

int log_search_string(vector<pair<string, int>> &species, string &s, int start, int end) {
    int middle = (start + end)/2;
    if(species[middle].first == s) return middle;
    if(species[middle].first > s) return log_search_string(species, s, start, middle-1);
    return log_search_string(species, s, middle+1, end);
}

int log_search_age(vector<pair<int, int>> &path, int &age, int start, int end) {
    int middle = (start + end)/2;
    if(path[middle].first == age && (middle == 0 || path[middle-1].first > age)) return middle;
    if(path[middle].first > age) {
        if(path[middle+1].first <= age) return middle+1;
        return log_search_age(path, age, middle+1, end);
    }
    if(path[middle-1].first > age) return middle;
    return log_search_age(path, age, start, middle-1);
}

void dfs(vector<vector<pair<int, int>>> &queries, vector<int> &result, vector<pair<int, int>> &path, vector<vector<int>> &ancestor, vector<pair<string, int>> &species, int index) {
    for(int i = 0; i < (int)queries[index].size(); i++) result[queries[index][i].second] = path[log_search_age(path, queries[index][i].first, 0, (int)path.size()-1)].second;
    for(int i = 0; i < (int)ancestor[index].size(); i++) {
        path.push_back(make_pair(species[ancestor[index][i]].second, ancestor[index][i]));
        dfs(queries, result, path, ancestor, species, ancestor[index][i]);
    }
    path.pop_back();
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, q;
        cin >> n >> q;

        vector<pair<string, int>> species(n);
        string root;
        int max_age = INT32_MIN;
        for(int i = 0; i < n; i++) {
            string s;
            int age;
            cin >> s >> age;
            species[i] = make_pair(s, age);
            if(age > max_age) {
                max_age = age;
                root = s;
            }
        }
        sort(species.begin(), species.end(), mysort);
        int root_index = log_search_string(species, root, 0, n-1);
        
        vector<vector<int>> ancestor(n);
        for(int i = 0; i < n-1; i++) {
            string s, p;
            cin >> s >> p;
            ancestor[log_search_string(species, p, 0, n-1)].push_back(log_search_string(species, s, 0, n-1));
        }

        vector<vector<pair<int, int>>> queries(n);
        for(int i = 0; i < q; i++) {
            string s;
            int age;
            cin >> s >> age;
            queries[log_search_string(species, s, 0, n-1)].push_back(make_pair(age, i));
        }

        vector<int> result(q);
        vector<pair<int, int>> path;
        path.push_back(make_pair(species[root_index].second, root_index));
        dfs(queries, result, path, ancestor, species, root_index);
        for(int i = 0; i < q; i++) cout << species[result[i]].first << " ";
        cout << "\n";
    }
}
 