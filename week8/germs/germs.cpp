/**
 * CGAL-Proximity problem: we build a triangulation of the positions of the germs, and for each germ we iterate over its incident edges of the triangulation.
 * We also check whether every germ will reach the plate before hitting any other germ.
 * To find the time when a germ dies is then trivial, given the distance to the first thing it its.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KS;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef K::Point_2 P;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    size_t n;
    cin >> n;

    while(n > 0) {
        int l, b, r, t;
        cin >> l >> b >> r >> t;

        vector<P> points(n);
        for(size_t i = 0; i < n; i++) cin >> points[i];

        Triangulation tri;
        tri.insert(points.begin(), points.end());

        vector<K::RT> times;
        for(auto v = tri.vertices_begin(); v != tri.vertices_end(); v++) {
            K::FT best_time = -1;
            Triangulation::Edge_circulator c = tri.incident_edges(v->handle());
            int best_dist = INT32_MAX;
            best_dist = min(best_dist, (int)(v->point().x()-l));
            best_dist = min(best_dist, (int)(r-v->point().x()));
            best_dist = min(best_dist, (int)(v->point().y()-b));
            best_dist = min(best_dist, (int)(t-v->point().y()));
            best_time = pow(best_dist, 2)*4;
            if(c != 0) do {
                if (!tri.is_infinite(c) && (best_time == -1 || best_time > (tri.segment(c).squared_length()))) best_time = tri.segment(c).squared_length();
            } while (++c != tri.incident_edges(v->handle()));
            times.push_back(best_time);
        }
        sort(times.begin(), times.end());
        assert(times.size() == n);

        cout << fixed << setprecision(0) << ceil(CGAL::to_double(CGAL::sqrt(CGAL::sqrt(times[0]/4)-0.5))) << " ";
        cout << fixed << setprecision(0) << ceil(CGAL::to_double(CGAL::sqrt(CGAL::sqrt(times[n/2]/4)-0.5))) << " ";
        cout << fixed << setprecision(0) << ceil(CGAL::to_double(CGAL::sqrt(CGAL::sqrt(times[n-1]/4)-0.5))) << "\n";

        cin >> n;
    }
}
 