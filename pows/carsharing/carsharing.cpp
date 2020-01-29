/**
 * The solution to this problem creates a graph that has as vertices all the times when a car can get taken or left in a station
 * For instance, if we only have three bookings: 
 *      2 1 60 90 7
 *      1 2  0 90 10
 *      1 2 30 60 5
 * and if we call the two station a and b, the graph will have the nodes: 0a, 30a, 90a, 0b, 60b, 90b (+ the source and the sink).
 * We don't care about the flow and we make the graph such that it will always be the total number of cards.
 * All the nodes of a car station will be connected from the beginning to the end, with cost 0 along the path.
 * The bookings will connect nodes of different stations with negative cost (because we want to maximize the incomes - and we can only minimize the flow).
 * The reward will then be the cost of the flow from the source to the sink.
 * Note that we'll normalize the costs in order to use the @successive_shortest_path_nonnegative_weights.
 **/

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; 

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
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

typedef tuple<int, int, int, int, int> trip;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    while(T--) {
        int N, S;
        cin >> N >> S;

        int tot_cars = 0;
        vector<int> starting_cars(S);
        for(int i = 0; i < S; i++) {
            cin >> starting_cars[i];
            tot_cars += starting_cars[i];
        }

        vector<set<int>> times_per_station(S, set<int>());
        vector<trip> trips(N);
        set<int> times_set;
        for(int i = 0; i < N; i++) {
            int from, to, start, end, cost;
            cin >> from >> to >> start >> end >> cost;
            from--;
            to--;
            times_set.insert(start);
            times_set.insert(end);
            times_per_station[from].insert(start);
            times_per_station[to].insert(end);
            trips.push_back({from, to, start, end, cost});
        }
        times_set.insert(0);
        auto max_time = times_set.end();
        max_time--;
        map<int, int> times_index;
        int counter = 0;
        for(auto it = times_set.begin(); it != times_set.end(); it++) times_index[*it] = counter++;

        vector<vector<vertex_desc>> station_time(S, vector<vertex_desc>(times_set.size(), -1));
        graph G(0);
        edge_adder adder(G);
        const vertex_desc src = boost::add_vertex(G), sink = boost::add_vertex(G);
        for(int i = 0; i < S; i++) {
            times_per_station[i].insert(0);
            times_per_station[i].insert(*max_time);
            for(auto it = times_per_station[i].begin(); it != times_per_station[i].end(); it++)
                station_time[i][times_index[*it]] = boost::add_vertex(G);
        }

        for(int i = 0; i < trips.size(); i++) {
            vertex_desc from = station_time[get<0>(trips[i])][times_index[get<2>(trips[i])]];
            vertex_desc to = station_time[get<1>(trips[i])][times_index[get<3>(trips[i])]];
            int cost = 100*(get<3>(trips[i])-get<2>(trips[i])) - get<4>(trips[i]);
            adder.add_edge(from, to, 1, cost);
        }

        for(int i = 0; i < S; i++) {
            for(auto it = times_per_station[i].begin(); it != times_per_station[i].end(); it++) {
                if(*it == 0) {
                    adder.add_edge(src, station_time[i][times_index[*it]], starting_cars[i], 0);
                    auto itplus = it;
                    itplus++;
                    adder.add_edge(station_time[i][times_index[*it]], station_time[i][times_index[*itplus]], INT32_MAX, 100*(*itplus - *it));
                }
                else if(*it == *max_time) adder.add_edge(station_time[i][times_index[*it]], sink, INT32_MAX, 0);
                else {
                    auto itplus = it;
                    itplus++;
                    adder.add_edge(station_time[i][times_index[*it]], station_time[i][times_index[*itplus]], INT32_MAX, 100*(*itplus - *it));
                }
            }
        }

        boost::successive_shortest_path_nonnegative_weights(G, src, sink);
        int cost = boost::find_flow_cost(G);
        cout << 100*(*max_time)*tot_cars - cost << "\n";
    }
}
