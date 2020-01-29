/**
 * This solution involves a binary search on the capacities and a min-cost-max-flow analysis.
 * First of all, we create a graph as given by the problem, and we compute the max flow between Kholby and Allahabad (that is the maximum number of suitcases we could bring if we had no budget limits).
 * Then, we perform a binary search on the capacities to give to the edge from the source to Kholby, and we stop when we find
 * a capacity such that we can afford the flow cost, but increasing the capacity only by one we would go out of budget.
 * That is the answer to the problem.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
    return e;
  }
};

using namespace std;

int log_search(int a, int b, graph &g, const vertex_desc &source, const vertex_desc &sink, int budget, edge_desc &e) {
    int middle = (b+a)/2;
    auto c_map = boost::get(boost::edge_capacity, g);
    c_map[e] = middle;
    boost::successive_shortest_path_nonnegative_weights(g, source, sink);
    int cost = boost::find_flow_cost(g);
    if(cost < budget) {
        if(middle == b) return middle;
        c_map[e] = middle+1;
        boost::successive_shortest_path_nonnegative_weights(g, source, sink);
        int cost_1 = boost::find_flow_cost(g);
        if(cost_1 > budget) return middle;
        if(cost_1 == budget) return middle+1;
        return log_search(middle+1, b, g, source, sink, budget, e);
    }
    if(cost > budget) {
        if(middle == 1) return 0;
        c_map[e] = middle-1;
        boost::successive_shortest_path_nonnegative_weights(g, source, sink);
        int cost_1 = boost::find_flow_cost(g);
        if(cost_1 <= budget) return middle-1;
        return log_search(a, middle-1, g, source, sink, budget, e);
    }
    return middle;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        int c, guides, b, k, a, infinite_suitcases = 0;
        cin >> c >> guides >> b >> k >> a;

        graph g(c);
        edge_adder adder(g);
        for(int i = 0; i < guides; i++) {
            int x, y, d, e;
            cin >> x >> y >> d >> e;
            adder.add_edge(x, y, e, d);
            infinite_suitcases += e;
        }
        const vertex_desc source = boost::add_vertex(g), sink = boost::add_vertex(g);
        adder.add_edge(a, sink, infinite_suitcases, 0);
        edge_desc edge = adder.add_edge(source, k, infinite_suitcases, 0);
        auto c_map = boost::get(boost::edge_capacity, g);
        auto rc_map = boost::get(boost::edge_residual_capacity, g);
        int max_flow = 0;
        out_edge_it e, eend;
        boost::successive_shortest_path_nonnegative_weights(g, source, sink);
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, g), g); e != eend; ++e) max_flow += c_map[*e] - rc_map[*e];
        cout << log_search(1, max_flow, g, source, sink, b, edge) << "\n";
    }
}
