/**
 * This solution uses the max-flow to solve the problem. It makes sure that the flow will always be the total number of foods that it's possible
 * to bring between the path, and then assigns the costs to the edges in a way such that the max-flow algorithm will choose the path that maximizes the
 * achieved significance. Indeed if the flow will always be maximal, all that remains to the max-flow-min-cost algorithm is to compute the best cost path.
 * Edges costs are normalized to use the @successive_shortest_path_nonnegative_weights.
 * One last thing is that not all of the edges will be used, and to get a more efficient solution (and the last 10 points on the judge), 
 * the edges that we'll surely not use should not be added.
 **/ 

#include <iostream>
#include <vector>
#include <set>
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

bool decreasing_sort(const int &i, const int &j) {
    return i > j;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        int n, m;
        cin >> n >> m;
        
        graph g(n);
        edge_adder adder(g);
        const vertex_desc src = boost::add_vertex(g), sink = boost::add_vertex(g);
        int inf_cost = pow(2, 7);

        vector<int> cap_per_day(n, 0);
        int total_flow = 0;
        for(int i = 0; i < n-1; i++) {
            cin >> cap_per_day[i];
            total_flow += cap_per_day[i];
            adder.add_edge(src, i, cap_per_day[i], 0); //here I make sure that every day I can bring all the food that I will be able to transport
            adder.add_edge(i+1, sink, cap_per_day[i], 0); //here I make sure that the next day I will leave all the food that I was able to transport
            adder.add_edge(i, i+1, cap_per_day[i], inf_cost);
        }

        vector<vector<vector<int>>> edges(n, vector<vector<int>>(n));
        for(int i = 0; i < m; i++) {
            int a, b, d;
            cin >> a >> b >> d;
            edges[a][b].push_back(d);
        }
        
        for(int i = 0; i < n; i++) 
            for(int j = i+1; j < n; j++) {
                if(edges[i][j].size() > 0) {
                    sort(edges[i][j].begin(), edges[i][j].end(), decreasing_sort);
                    for(int k = 0; k < (int)edges[i][j].size() && k < cap_per_day[i]; k++)
                        adder.add_edge(i, j, 1, inf_cost*(j-i) - edges[i][j][k]); //I only add the interesting edges
                }
            }

        boost::successive_shortest_path_nonnegative_weights(g, src, sink);
        int cost = boost::find_flow_cost(g);
        cout << total_flow*inf_cost - cost << "\n";
    }
}
