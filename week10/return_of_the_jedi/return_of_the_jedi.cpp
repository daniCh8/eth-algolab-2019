/**
 * Instead of finding the second minimum spanning tree (that would re-order the edges every time), the optimal solution is to order the edges only one,
 * and ignoring one of the first mst edges every time when computing another mst. The least expensive between those spanning tree is the one R2D2's looking for.
 **/ 

#include <iostream>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::tuple<int,int,int> edge;

using namespace std;

/** 
 * computes the mst with boost::disjoint_ sets;
 * if the flag register_indexes is true, the indexes of the mst edges will be saved in the used_indexes vector
 * the edge with index to_ignore will be ignored in the computation of the mst
**/  
int compute_mst(int n, int to_ignore, vector<edge> &edges, vector<int> &used_indexes, bool register_indexes) {
    boost::disjoint_sets_with_storage<> uf(n);
    int cost = 0;
    for (int i = 0; i < (int)edges.size(); i++) {
        int c1 = uf.find_set(get<0>(edges[i])), c2 = uf.find_set(get<1>(edges[i]));
        if (c1 != c2 && i != to_ignore) {
            uf.link(c1, c2);
            cost += get<2>(edges[i]);
            if(register_indexes) used_indexes.push_back(i);
            if(--n == 1) break;
        }
    }
    return cost;
}

bool mysort(const edge &i, const edge &j) {
    return get<2>(i) < get<2>(j);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, tatooine;
        cin >> n >> tatooine;

        vector<edge> edges;
        for(int i = 0; i < n-1; i++) {
            for(int j = i+1; j < n; j++) {
                int cost;
                cin >> cost;
                edges.push_back(make_tuple(i, j, cost));
            }
        }
        sort(edges.begin(), edges.end(), mysort); //edges are only sorted once

        vector<int> used_indexes;
        compute_mst(n, -1, edges, used_indexes, true); //this is princess Leila's tree - its indexes are registered, and no edge is ignored

        int minimum_cost = INT32_MAX;
        for(int i = 0; i < (int)used_indexes.size(); i++)
            minimum_cost = min(minimum_cost, compute_mst(n, used_indexes[i], edges, used_indexes, false)); //recomputing the tree, but ignoring one edge of princess Leila's tree every time - used indexes are not registered

        cout << minimum_cost << "\n";
    }
}
