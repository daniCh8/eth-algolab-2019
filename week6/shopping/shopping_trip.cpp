/**
 * BGL-Flow problem: the nodes are the intersections and to every street is given a capacity of one (in both the directions, as streets are undirected).
 * The capacity from the source to the intersection 0 is equal to the number of shops and from every shop there's an outcoming edge with capacity 1 directed to the sink.
 * If the flow equals the number of shops, the answer is yes: otherwise, it's no.
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
        int n, m, s;
        cin >> n >> m >> s;
        
        graph g(n);
        const vertex_desc source = boost::add_vertex(g);
        const vertex_desc target = boost::add_vertex(g);
        edge_adder adder(g);

        adder.add_edge(source, 0, s);
        for(int i = 0; i < s; i++) {
            int store;
            cin >> store;
            adder.add_edge(store, target, 1);
        }
        for(int i = 0; i < m; i++) {
            int from, to;
            cin >> from >> to;
            adder.add_edge(from, to, 1);
            adder.add_edge(to, from, 1);
        }

        long flow = boost::push_relabel_max_flow(g, source, target);
        flow == s ? cout << "yes\n" : cout << "no\n";
    }
}
