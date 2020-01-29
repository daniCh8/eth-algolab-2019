/**
 * Given a graph that has the people as vertices, I connect only the people that has a number of properties in common greater than f.
 * If on the graph that I've just built it exists a maximum matching that comprehends all the vertices, than the answer to the problem is yes: otherwise, it's no.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

using namespace std;

bool maximum_matching(const weighted_graph &G) {
    int n = boost::num_vertices(G);
    vector<bool> checker(n, false);
    std::vector<vertex_desc> mate_map(n); // exterior property map
    const vertex_desc NULL_VERTEX = boost::graph_traits<weighted_graph>::null_vertex();
    boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    for (int i = 0; i < n; ++i) {
        if (mate_map[i] != NULL_VERTEX && i < mate_map[i]) {
            checker[i] = true;
            checker[mate_map[i]] = true;
        }
    }
    for(int i = 0; i < n; ++i) {
        if(!checker[i]) return false;
    }
    return true;
}

bool mysort(const pair<string, int> &i, const pair<string, int> &j) {
    return i.first.compare(j.first) < 0;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t > 0) {
        int n, c, f;
        cin >> n >> c >> f;

        vector<pair<string, int>> characteristics(n*c);
        int count = 0;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < c; j++) {
                string characteristic;
                cin >> characteristic;
                pair<string, int> char_pair(characteristic, i);
                characteristics[count++] = char_pair;
            }
        }
        sort(characteristics.begin(), characteristics.end(), mysort);

        weighted_graph g(n);
        weight_map weights = boost::get(boost::edge_weight, g);
        vector<vector<int>> edges(n, vector<int>(n, 0));
        for(int i = 0; i < c*n-1; i++) {
            int counter = 1;
            while(i+counter < c*n && characteristics[i].first.compare(characteristics[i+counter].first) == 0) {
                int from = min(characteristics[i].second, characteristics[i+counter].second), to = max(characteristics[i].second, characteristics[i+counter].second);
                edges[from][to]++;
                counter++;
            }
        }

        for(int i = 0; i < n; i++) {
            for(int j = i+1; j < n; j++) {
                if(edges[i][j] > f)
                    weights[boost::add_edge(i, j, g).first] = edges[i][f];
            }
        }

        bool fuzzy_fu = maximum_matching(g);
        if(fuzzy_fu) cout << "not optimal" << "\n";
        else cout << "optimal" << "\n";

        t--;
    }
}
