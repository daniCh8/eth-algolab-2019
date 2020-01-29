/**
 * BGL-MinCostMaxFlow problem: the nodes are the days in the canteen. 
 * The edges between the days have capacity equals to the amount of food I can store per night and cost equals to the cost of storing a meal per night.
 * The edges between the source and the days have capacity equals to the amount of food cooked that day and cost equals to the cost of cooking a meal.
 * The edges between the days and the sink have capacity equals to the number of students that will eat in the mensa each day and the cost equals to the negative cost of a lunch.
 * The costs above are the theoretical ones - they are actually normalized to use the successive_shortes_path_nonnegative_weight.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
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
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; 

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
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
  }
};


using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        graph g(n);
        edge_adder adder(g);
        const auto source = boost::add_vertex(g), sink = boost::add_vertex(g);

        for(int i = 0; i < n; i++) {
            int a, c;
            cin >> a >> c;
            adder.add_edge(source, i, a, c+20);
        }

        long tot_students = 0;
        for(int i = 0; i < n; i++) {
            int s, p;
            cin >> s >> p;
            tot_students += s;
            adder.add_edge(i, sink, s, 20-p);
        }

        for(int i = 0; i < n-1; i++) {
            int v, e;
            cin >> v >> e;
            adder.add_edge(i, i+1, v, e);
        }

        auto c_map = boost::get(boost::edge_capacity, g);
        auto rc_map = boost::get(boost::edge_residual_capacity, g);
        boost::successive_shortest_path_nonnegative_weights(g, source, sink);
        int cost = boost::find_flow_cost(g);
        long flow = 0;
        out_edge_it e, eend;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, g), g); e != eend; ++e)
            flow += c_map[*e] - rc_map[*e];
        cost -= 40*flow;

        flow == tot_students ? cout << "possible " : cout << "impossible ";
        cout << flow << " " << -cost << "\n";
    }
}
