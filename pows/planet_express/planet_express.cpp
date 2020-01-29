/**
 * In this solution, first we create the graph as given by the problem, but changing the direction of every edge,
 * so that in the final step, when we need to compute the distance between planet n-1 and a warehouse, we can compute once from n-1 to the warehouses,
 * instead of several time from the warehouses to n-1.
 * Moreover, the teleportation network edges need to be added: the connected components will be used in order to do so. 
 * Indeed, if two planets of the same teleportation network are in the same component, it means that they are pairwise reachable.
 * We'll add a dummy node per component that contains at least one teleportation network planet,
 * and connect the planets inside the component and the teleportation network through these nodes.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

int dijkstra_dist(const weighted_graph &G, int s, int k) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    int min_dist = INT32_MAX;
    for(int i = 0; i < k; i++)
        min_dist = std::min(min_dist, dist_map[i]);
    return min_dist;
}

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;

    while(tests--) {
        int n, m, k, t;
        cin >> n >> m >> k >> t;

        vector<int> teleport_net(t);
        for(int i = 0; i < t; i++) cin >> teleport_net[i];

        weighted_graph g(n);
        weight_map weights = boost::get(boost::edge_weight, g);
        for(int i = 0; i < m; i++) {
            int from, to, weight;
            cin >> to >> from >> weight;
            edge_desc e = boost::add_edge(from, to, g).first;
            weights[e] = weight;
        }

        vector<int> scc_map(n);
        int nscc = boost::strong_components(g, boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, g)));
        vector<int> scc_count(nscc, -1);
        for(int i = 0; i < t; i++)
            scc_count[scc_map[teleport_net[i]]]++;

        for(int i = 0; i < t; i++) {
            edge_desc e = boost::add_edge(teleport_net[i], n+scc_map[teleport_net[i]], g).first;
            weights[e] = 0;
            e = boost::add_edge(n+scc_map[teleport_net[i]], teleport_net[i], g).first;
            weights[e] = scc_count[scc_map[teleport_net[i]]];
        }
        
        int dist = dijkstra_dist(g, n-1, k);
        if(dist < pow(10, 6)) cout << dist << "\n";
        else cout << "no\n";
    }
}
