/**
 * BGL-MaxFlow problem: here the solution first creates the graph as given by the problem (but making all the streets undirected, to easy the use of Dijkstra).
 * Then, it computes the dijkstra distances from both the starting point and the ending point.
 * It iterates over all the edges and builds a new graph where the possible distances between the starting and the ending point are only the shortest.
 * Finally, it computes the flow between the starting and the ending point in the last graph.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

using namespace std;

vector<int> dijkstra_dist(const graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m, s, f;
        cin >> n >> m >> s >> f;

        graph g(n), dijkstra_g(n);
        edge_adder adder(g);
        auto weights = boost::get(boost::edge_weight, dijkstra_g);
        auto c_map = boost::get(boost::edge_capacity, g);
        auto w_map = boost::get(boost::edge_weight, g);

        for(int i = 0; i < m; i++) {
            int a, b, c, d;
            cin >> a >> b >> c >> d;
            if(a == b) continue;
            adder.add_edge(a, b, c, d);
            adder.add_edge(b, a, c, d);
            auto e = boost::add_edge(a, b, dijkstra_g).first;
            weights[e] = d;
            e = boost::add_edge(b, a, dijkstra_g).first;
            weights[e] = d;
        }

        vector<int> distances_from_s = dijkstra_dist(dijkstra_g, s), distances_from_t = dijkstra_dist(dijkstra_g, f);
        for(auto it = boost::edges(g).first; it != boost::edges(g).second; it++) {
            if(distances_from_s[(*it).m_source] + w_map[*it] + distances_from_t[(*it).m_target] > distances_from_s[f])
                c_map[*it] = 0;
        }

        cout << boost::push_relabel_max_flow(g, s, f) << "\n";
    }
}
