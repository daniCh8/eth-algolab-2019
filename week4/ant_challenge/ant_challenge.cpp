/**
 * Every species can move only between the minimum spanning tree created from the weights of the edges of its species.
 * Therefore, I used a vector of graphs, each graph corresponding to a species. 
 * Then, I computed the minimum spanning tree of each graph and added those edges to the final graph (our boost graph supports parallel edges).
 * Finally, the result is the distance between the two endpoints of the final graph.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

using namespace std;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, G))));
    return dist_map[t];
}

void kruskal(const weighted_graph &G, weight_map &G_map, weighted_graph &final_g, weight_map &final_map) {
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        auto p = boost::add_edge(boost::source(*it, G), boost::target(*it, G), final_g);
        final_map[p.first] = G_map[*it];
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, e, s, a, b;
        cin >> n >> e >> s >> a >> b;

        vector<weighted_graph> g_vector(s, weighted_graph(n));
        vector<weight_map> w_map_vector(s);
        for(int i = 0; i < s; i++) {
            w_map_vector[i] = boost::get(boost::edge_weight, g_vector[i]);
        }
        for(int i = 0; i < e; i++) {
            int from, to;
            cin >> from >> to;
            for(int j = 0; j < s; j++) {
                int w;
                cin >> w;
                edge_desc e = boost::add_edge(from, to, g_vector[j]).first;
                w_map_vector[j][e] = w;
            }
        }

        weighted_graph final_g(n);
        weight_map final_g_map = boost::get(boost::edge_weight, final_g);
        for(int i = 0; i < s; i++) {
            int to_waste;
            cin >> to_waste;
            kruskal(g_vector[i], w_map_vector[i], final_g, final_g_map);
        }
        
        cout << dijkstra_dist(final_g, a, b) << "\n";
    }
}
