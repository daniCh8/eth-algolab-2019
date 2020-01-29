/**
 * This solution to this problem computes the triangulation from all the lights.
 * For every player, it first sees whether that player is hit by some lamp or not using the triangulation's nearest vertex.
 * If the player is hit, it looks for the first hit in the lights vector (which is ordered by default).
 * It keeps track of the best result and prints all the players with that result.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;

    while(tests--) {
        int m, n;
        cin >> m >> n;

        vector<pair<P, int>> players(m);
        for(int i = 0; i < m; i++) {
            int r;
            P p;
            cin >> p >> r;
            players[i] = make_pair(p, r);
        }

        int h;
        cin >> h;

        vector<P> lights(n);
        for(int i = 0; i < n; i++) cin >> lights[i];
        
        Triangulation t;
        t.insert(lights.begin(), lights.end());

        vector<int> players_result(m, INT32_MAX);
        int best_result = -1;
        for(int i = 0; i < m; i++) {
            Triangulation::Vertex_handle nearest = t.nearest_vertex(players[i].first)->handle();
            long int treshold = pow(players[i].second + h, 2);
            if(CGAL::squared_distance(nearest->point(), players[i].first) >= treshold) {
                best_result = INT32_MAX;
                continue;
            } else {
                for(int j = 0; j < n; j++)
                    if(CGAL::squared_distance(lights[j], players[i].first) < treshold) {
                        players_result[i] = j;
                        j = n;
                    }
                best_result = max(best_result, players_result[i]);
            }
        }

        for(int i = 0; i < m; i++)
            if(players_result[i] == best_result) cout << i << " ";
        cout << "\n";
    }
}
