/**
 * Flow problem: we'll use a graph with n + m nodes - one per every round and one per every player.
 * Every round will have an edge coming from the source with capacity 1.
 * If the result of a round is known, the node corresponding to that round will have just one outcoming edge going to the winner of the round with capacity 1.
 * If the result of a round is unknown, there will be two outcoming edges, both with capacity one, directed to the two possible winners of the round.
 * From every player there will be an edge going to the sink with its given result as capacity.
 * If the flow from the source to the sink is the same as the sum of all the points of every player
 * and if it's also equal to the number of rounds (this is a corner case that needs to be checked), the answer is yes: otherwise, it's no.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
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

    while(t > 0) {
        int n, m;
        cin >> n >> m;

        graph g(n+m);
        edge_adder adder(g);
        const vertex_desc v_source = boost::add_vertex(g);
        const vertex_desc v_target = boost::add_vertex(g);

        for(int i = 0; i < m; i++) {
            int a, b, c;
            cin >> a >> b >> c;
            adder.add_edge(v_source, n+i, 1);
            switch (c)
            {
            case 1:
                adder.add_edge(n+i, a, 1);
                break;
            case 2:
                adder.add_edge(n+i, b, 1);
                break;
            case 0:
                adder.add_edge(n+i, a, 1);
                adder.add_edge(n+i, b, 1);
                break;
            }
        }

        int total_score = 0;
        for(int i = 0; i < n; i++) {
            int score;
            cin >> score;
            total_score += score;
            adder.add_edge(i, v_target, score);
        }

        long flow = boost::push_relabel_max_flow(g, v_source, v_target);
        if(flow == total_score && total_score == m) cout << "yes\n";
        else cout << "no\n";

        t--;
    }
}
