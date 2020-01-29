/**
 * This solution uses the disjoint sets to solve the problem. 
 * It creates a triangulation on the jammers and stores all the edges of this triangulation (and their length) in an array.
 * Then, it sorts this array by increasing length and populates the disjoint sets of the p-mission linking all the vertices pairwise reachable under the p-umbrella.
 * After that, it iterates over all the missions: if one mission is doable under the p-umbrella,
 * which means that the distance between the starting and ending point and the nearest vertices is covered and that those nearest vertices have the same set in the p-disjoint sets,
 * it populates the disjoint sets of the min-power-missions with edges of increasing length until that mission is possible (and keeps track of the energy needed).
 * The disjoint sets of the all-missions is always populated the same way of above until every mission is possible (and the energy needed here is also stored in a variable).
 **/ 

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;
typedef boost::disjoint_sets_with_storage<> ds;

using namespace std;

bool mysort(const pair<Triangulation::Edge_iterator, double> &i, const pair<Triangulation::Edge_iterator, double> &j) {
    return i.second < j.second;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;

    while(tests--) {
        int n, m;
        double p;
        cin >> n >> m >> p;

        vector<P> pts(n);
        for(int i = 0; i < n; i++) cin >> pts[i];
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        vector<pair<Triangulation::Edge_iterator, double>> edges;
        for(auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) edges.push_back(make_pair(it, (t.segment(it).squared_length())));
        map<Triangulation::Vertex_handle, int> vertices;
        int counter = 0;
        for(auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); it++) vertices[it->handle()] = counter++;
        sort(edges.begin(), edges.end(), mysort);

        ds sets(n);
        for(int i = 0; i < (int)edges.size(); i++) {
            if(edges[i].second > p) i = edges.size();
            else sets.union_set(vertices[edges[i].first->first->vertex((edges[i].first->second+1)%3)], vertices[edges[i].first->first->vertex((edges[i].first->second+2)%3)]);
        }

        int index_p = 0, index_all = 0;
        ds sets_p_missions(n), sets_all_missions(n);
        double min_power_all_missions = 0, min_power_p_missions = 0;
        for(int i = 0; i < m; i++) {
            P mission_start, mission_end;
            cin >> mission_start >> mission_end;
            double power_from_endpoints = 4*max(CGAL::squared_distance(t.nearest_vertex(mission_start)->point(), mission_start), CGAL::squared_distance(t.nearest_vertex(mission_end)->point(), mission_end));
            Triangulation::Vertex_handle v1 = t.nearest_vertex(mission_start)->handle(), v2 = t.nearest_vertex(mission_end)->handle();
            if(power_from_endpoints <= p && sets.find_set(vertices[v1]) == sets.find_set(vertices[v2])) {
                cout << 'y';
                min_power_p_missions = max(min_power_p_missions, power_from_endpoints);
                while(sets_p_missions.find_set(vertices[v1]) != sets_p_missions.find_set(vertices[v2])) {
                    sets_p_missions.union_set(vertices[edges[index_p].first->first->vertex((edges[index_p].first->second+1)%3)], vertices[edges[index_p].first->first->vertex((edges[index_p].first->second+2)%3)]);
                    index_p++;
                }
            } else cout << 'n';
            min_power_all_missions = max(min_power_all_missions, power_from_endpoints);
            while(sets_all_missions.find_set(vertices[v1]) != sets_all_missions.find_set(vertices[v2])) {
                sets_all_missions.union_set(vertices[edges[index_all].first->first->vertex((edges[index_all].first->second+1)%3)], vertices[edges[index_all].first->first->vertex((edges[index_all].first->second+2)%3)]);
                index_all++;
            }
        }

        min_power_p_missions = max(min_power_p_missions, edges[index_p-1].second);
        min_power_all_missions = max(min_power_all_missions, edges[index_all-1].second);
        cout << fixed << setprecision(0) << "\n" << min_power_all_missions << "\n" << min_power_p_missions << "\n";
    }
}
