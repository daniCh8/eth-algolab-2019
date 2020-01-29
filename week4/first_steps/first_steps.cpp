#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

using namespace std;

int dijkstra_dist(const graph &G, int s) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    int max_dist = -1;
    for(int i = 0; i < n; i++) {
        max_dist = max(max_dist, dist_map[i]);
    }
    return max_dist;
}

int kruskal(graph &G) {
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    weight_map weights = boost::get(boost::edge_weight, G);
    int sum = 0;
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        sum += weights[*it];
    }
    return sum;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m;
        cin >> n >> m;
        
        graph g(n);
        weight_map weights = boost::get(boost::edge_weight, g);
        for(int i = 0; i < m; i++) {
            int from, to, weight;
            cin >> from >> to >> weight;
            auto e = boost::add_edge(from, to, g).first;
            weights[e] = weight;
        }

        cout << kruskal(g) << " " << dijkstra_dist(g, 0) << "\n";
    }
}
 