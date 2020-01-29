/**
 * BGL-Flow problem: the solution is to take all the jobs - and constraint the flow given by this jobs to the cost of the tickets.
 * The final result will then be the difference between the sum of the reward of all the jobs and the flow of that graph.
 * Indeed that flow will correspond to the money spent on tickets - and also to the jobs I've not done because they were not convenient.
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
        int z, j;
        cin >> z >> j;

        graph g(z+j);
        edge_adder adder(g);
        const vertex_desc source = boost::add_vertex(g), target = boost::add_vertex(g);

        for(int i = 0; i < z; i++) {
            int cost;
            cin >> cost;
            adder.add_edge(i+j, target, cost);
        }

        vector<int> rewards(j);
        int total_reward = 0;
        for(int i = 0; i < j; i++) {
            cin >> rewards[i];
            total_reward += rewards[i];
            adder.add_edge(source, i, rewards[i]);
        }

        for(int i = 0; i < j; i++) {
            int N;
            cin >> N;
            for(int k = 0; k < N; k++) {
                int zone;
                cin >> zone;
                adder.add_edge(i, zone+j, rewards[i]);
            }
        }

        long flow = boost::push_relabel_max_flow(g, source, target);
        cout << total_reward-flow << "\n";
    }
}
