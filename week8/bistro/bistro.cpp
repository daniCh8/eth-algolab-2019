/**
 * CGAL-Proximity problem: we just need to build the triangulation from the positions of the existing restaurants.
 * The answer to the problem will be the squared distance from every new position to its nearest vertex of the triangulation.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    size_t n;
    cin >> n;

    while(n > 0) {
        vector<K::Point_2> pts;
        pts.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            K::Point_2 p;
            cin >> p;
            pts.push_back(p);
        }
        
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        int m;
        cin >> m;
        for(int i = 0; i < m; i++) {
            K::Point_2 new_location;
            cin >> new_location;
            cout << fixed << setprecision(0) << CGAL::squared_distance(new_location, t.nearest_vertex(new_location)->point()) << "\n";
        }

        cin >> n;
    }
}
