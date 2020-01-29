#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, G))));
    return dist_map[t];
}

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m, q;
    cin >> n >> m >> q;

    weighted_graph g(n);
    weight_map weights = boost::get(boost::edge_weight, g);
    for(int i = 0; i < n; i++) {
        int lon, lat;
        cin >> lon >> lat;
    }

    for(int i = 0; i < m; i++) {
        int from, to, weight;
        cin >> from >> to >> weight;
        edge_desc e = boost::add_edge(from, to, g).first;
        weights[e] = weight;
    }

    for(int i = 0; i < q; i++) {
        int from, to; 
        cin >> from >> to;
        int distance = dijkstra_dist(g, from, to);
        distance == INT32_MAX ? cout << "unreachable\n" : cout << distance << "\n";
    }
}
