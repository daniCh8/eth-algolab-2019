/**
 * BGL-Minimum Cut problem: there will always be a solution where I keep only a figure between one of those pairs: [0, 1], [1, 2], ..., [n-1, 0].
 * Therefore, I just need to try all the minimum cut between those pairs - and keep the one that costs less.
 * After finding the one that costs less, I'll need to find the mincut vertices as explained in the lectures' sample code.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <queue>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

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
        int n, m;
        cin >> n >> m;

        graph g(n);
        edge_adder adder(g);
        for(int i = 0; i < m; i++) {
            int from, to, capacity;
            cin >> from >> to >> capacity;
            adder.add_edge(from, to, capacity);
        }

        int best_mine = 0, best_yours = 1;
        long min_cut = boost::push_relabel_max_flow(g, best_mine, best_yours);
        for(int i = 1; i < n; i++) {
            int mine = i, yours = i+1;
            if(yours == n) yours = 0;
            long flow = boost::push_relabel_max_flow(g, mine, yours);
            if(flow < min_cut) {
                min_cut = flow;
                best_mine = mine;
                best_yours = yours;
            }
        }

        min_cut = boost::push_relabel_max_flow(g, best_mine, best_yours);

        auto rc_map = boost::get(boost::edge_residual_capacity, g);
        int total_visited = 1;
        vector<int> vis(n, false); 
	    queue<int> Q;
	    vis[best_mine] = true;
	    Q.push(best_mine);
	    while (!Q.empty()) {
    		const int u = Q.front();
		    Q.pop();
		    out_edge_it ebeg, eend;
		    for (boost::tie(ebeg, eend) = boost::out_edges(u, g); ebeg != eend; ++ebeg) {
    			const int v = boost::target(*ebeg, g);
			    if (rc_map[*ebeg] == 0 || vis[v]) continue;
			    vis[v] = true;
                total_visited++;
			    Q.push(v);
		    }
	    }

        cout << min_cut << "\n" << total_visited;
        for(int i = 0; i < n; i++)
            if(vis[i]) cout << " " << i;
        cout << "\n";
    }
}
