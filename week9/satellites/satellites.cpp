/**
 * BGL-Minimum Vertex Cover problem. The graph is bipartite between the ground stations and the satellites. We just need to compute the vertex cover.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>	graph;
typedef	boost::graph_traits<graph>::edge_descriptor			    edge_desc;
typedef	boost::graph_traits<graph>::vertex_descriptor			vertex_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
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
        int g, s, l;
        cin >> g >> s >> l;

        graph G(g+s);
        edge_adder adder(G);
        for(int i = 0; i < l; i++) {
            int from, to;
            cin >> from >> to;
            to += g;
            adder.add_edge(from, to, 1);
        }
        
        const vertex_desc source = boost::add_vertex(G); 
        const vertex_desc sink = boost::add_vertex(G);
        for(int i = 0; i < g; i++) adder.add_edge(source, i, 1);
        for(int i = 0; i < s; i++) adder.add_edge(i+g, sink, 1);

        boost::push_relabel_max_flow(G, source, sink);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        std::vector<int> vis(g+s+2, false); 
	    std::queue<int> Q; 
	    vis[source] = true;
	    Q.push(source);
	    while (!Q.empty()) {
    		const int u = Q.front();
		    Q.pop();
		    out_edge_it ebeg, eend;
		    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
    			const int v = boost::target(*ebeg, G);
			    if (rc_map[*ebeg] == 0 || vis[v]) continue;
			    vis[v] = true;
			    Q.push(v);
		    }
	    }
        
        int _g = 0, _s = 0;
        for(int i = 0; i < g; i++) if(!vis[i]) _g++;
        for(int i = 0; i < s; i++) if(vis[i+g]) _s++;
        cout << _g << " " << _s << "\n";
        for(int i = 0; i < g+s; i++) {
            if(i < g && !vis[i]) cout << i << " ";
            else if(i >= g && vis[i]) cout << i-g << " ";
        }
        if(_g != 0 || _s != 0) cout << "\n";
    }
}
