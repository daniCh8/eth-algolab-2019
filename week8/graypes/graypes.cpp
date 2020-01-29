/**
 * CGAL-Proximity problem: I'll need to find those two graypes that are at the minimum distance - those are the two that will reach each other first.
 * Therefore, I just need to iterate over the finite edges of the triangulation to find the smallest one.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
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
        vector<P> points(n);
        for(size_t i = 0; i < n; i++) cin >> points[i];

        Triangulation t;
        t.insert(points.begin(), points.end());

        K::FT best_seg = -1;
        for(auto it = t.edges_begin(); it != t.edges_end(); it++) 
            if(best_seg == -1 || t.segment(it).squared_length() < best_seg)
                best_seg = t.segment(it).squared_length();

        cout << fixed << setprecision(0) << ceil(CGAL::to_double((sqrt(best_seg)*50))) << "\n";
        cin >> n;
    }
}
