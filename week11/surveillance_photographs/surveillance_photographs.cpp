/**
 * This solution creates a graph where all the nodes (the intersections) and all the edges (the streets) are doubled.
 * It creates a bipartite graph: the left side of the bipartite graph is the starting situation: all the police stations are connected to the source
 * with capacity one and all the edges have maximum capacity. The right side of the graph is the situation when some police man has taken a photograph;
 * all the edges have capacity one and all the police stations are connected to the sink with capacity one.
 * The partition relays between the two nodes corresponding to every photograph: the left-node of a photograph is connected to the right-node of a photograph
 * with capacity one (they have a vertex capacity of one).
 * The answer to the problem is then the flow flowing from the source to the sink.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        int n, m, k, l;
        cin >> n >> m >> k >> l;

        graph g(2*n);
        edge_adder adder(g);
        const vertex_desc source = boost::add_vertex(g), sink = boost::add_vertex(g);
        for(int i = 0; i < k; i++) {
            int ps;
            cin >> ps;
            adder.add_edge(ps+n, sink, 1);
            adder.add_edge(source, ps, 1);
        }
        for(int i = 0; i < l; i++) {
            int ph;
            cin >> ph;
            adder.add_edge(ph, ph+n, 1);
        }
        for(int i = 0; i < m; i++) {
            int from, to;
            cin >> from >> to;
            adder.add_edge(from, to, l+k);
            adder.add_edge(from+n, to+n, 1);
        }

        cout << boost::push_relabel_max_flow(g, source, sink) << "\n";
    }

}
