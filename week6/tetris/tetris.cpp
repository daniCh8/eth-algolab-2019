/**
 * BGL-Flow problem: the nodes are the positions between 0 (the source) and w (the sink).
 * All those nodes must have capacity one (so, every node will have two nodes - in and out - and an edge of capacity one connecting them).
 * The flow between 0 and w will be the answer.
 * Note that the left end does not necessarily come first - so when reading the bricks it's important to check which endpoint is which.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, long, boost::property<boost::edge_residual_capacity_t, long, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

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

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int w, n;
        cin >> w >> n;

        graph g(2*w);
        edge_adder adder(g);
        for(int i = 1; i < w; i++) adder.add_edge(i, i+w, 1);
        for(int i = 0; i < n; i++) {
            int a, b, from, to;
            cin >> a >> b;
            from = min(a, b);
            to = max(a, b);
            if(from != 0) adder.add_edge(from+w, to, 1);
            else adder.add_edge(from, to, 1);
        }

        long flow = boost::push_relabel_max_flow(g, 0, w);
        cout << flow << "\n";
    }
}
