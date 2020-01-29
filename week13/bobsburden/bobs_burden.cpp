/**
 * The difficulties of this problem are two: representing all those present weights in a graph and find the minimum sum of weights.
 * I've represented the graph with every vertex being a present, and all the incoming edges to a present have weight equals to the weight of the present.
 * Then, I've saved the dijkstra distances from the three presents that we want to take, and found the minimum sum between the three path weights 
 * starting at some present inside the triangle. Indeed, in every solution there will be a present that will be taken in all the paths.
 * The answer to the problem is that minimum sum.
 **/ 

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

std::vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);

	int t;
	cin >> t;
	while(t--) {
		int k;
		cin >> k;
		int tot_num = (k*(k+1))/2, counter = 0;
		vector<vector<pair<int, int>>> tree(k);
		for(int i = 0; i < k; i++)
			for(int j = 0; j <= i; j++) {
				int gift;
				cin >> gift;
				tree[i].push_back(make_pair(gift, counter++)); 
			}

		weighted_graph g(tot_num);
		auto weights = boost::get(boost::edge_weight, g);
		for(int i = 0; i < k; i++) {
			for(int j = 0; j < (int)tree[i].size(); j++) {
				if(i != k-1) {
					auto me = tree[i][j], other1 = tree[i+1][j], other2 = tree[i+1][j+1];
					edge_desc e = boost::add_edge(me.second, other1.second, g).first;
					weights[e] = other1.first;
					e = boost::add_edge(me.second, other2.second, g).first;
					weights[e] = other2.first;
					e = boost::add_edge(other1.second, me.second, g).first;
					weights[e] = me.first;
					e = boost::add_edge(other2.second, me.second, g).first;
					weights[e] = me.first;
				}
				if(j != (int)tree[i].size() - 1) {
					auto me = tree[i][j], other = tree[i][j+1];
					edge_desc e = boost::add_edge(me.second, other.second, g).first;
					weights[e] = other.first;
					e = boost::add_edge(other.second, me.second, g).first;
					weights[e] = me.first;
				}
			}
		}

		auto dist_map_v0 = dijkstra_dist(g, tree[0][0].second), dist_map_v1 = dijkstra_dist(g, tree[k-1][0].second), dist_map_v2 = dijkstra_dist(g, tree[k-1][k-1].second);
		int best_weight = INT32_MAX;
		for(int i = 0; i < k; i++) {
			for(int j = 0; j < (int)tree[i].size(); j++) {
				if(j != 0 && j != (int)tree[i].size()-1) {
					best_weight = min(best_weight, dist_map_v0[tree[i][j].second] + dist_map_v1[tree[i][j].second] + dist_map_v2[tree[i][j].second] - 2*tree[i][j].first);
				}
			}
		}
		best_weight = min(best_weight, dist_map_v0[tree[0][0].second] + dist_map_v1[tree[0][0].second] + dist_map_v2[tree[0][0].second] - 2*tree[0][0].first);
		best_weight = min(best_weight, dist_map_v0[tree[k-1][0].second] + dist_map_v1[tree[k-1][0].second] + dist_map_v2[tree[k-1][0].second] - 2*tree[k-1][0].first);
		best_weight = min(best_weight, dist_map_v0[tree[k-1][k-1].second] + dist_map_v1[tree[k-1][k-1].second] + dist_map_v2[tree[k-1][k-1].second] - 2*tree[k-1][k-1].first);
		cout << best_weight << endl;
	}
}
