/**
 * The solution to this problem is a min cut: every planet is given a vertex capacity of one,
 * every source planet has an incoming edge from the source of capacity one and every destination planet has an outcoming edge to the sink of capacity one.
 * The solution is just the flow from the source to the sink.
 **/ 

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
    c_map[rev_e] = 0; // reverse edge has no capacity!
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
        int n, m, s, d;
        cin >> n >> m >> s >> d;

        graph g(2*n);
        edge_adder adder(g);
        const vertex_desc src = boost::add_vertex(g), sink = boost::add_vertex(g);
        for(int i = 0; i < m; i++) {
            int from, to;
            cin >> from >> to;
            adder.add_edge(from+n, to, 1);
        }
        for(int i = 0; i < n; i++) adder.add_edge(i, i+n, 1);
        for(int i = 0; i < s+d; i++) {
            int node;
            cin >> node;
            if(i < s) adder.add_edge(src, node, 1);
            else adder.add_edge(node+n, sink, 1);
        }

        cout << boost::push_relabel_max_flow(g, src, sink) << "\n";
    }
}
