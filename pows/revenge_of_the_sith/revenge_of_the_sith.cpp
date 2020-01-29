/**
 * This solution to this problem uses a triangulation on the planets positions and a binary search on the possible number of planets that can be conquered. 
 * The answer varies between 0 and n/2: therefore, the algorithm performs a binary search between those two endpoints.
 * At every iteration, the algorithm looks for how many planets it's possible to conquer between planet (iteration) and planet (n) using the disjoint sets.
 * The answer to the problem is the result of the binary search.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef K::Point_2 P;
typedef boost::disjoint_sets_with_storage<> uf;

using namespace std;

int max_num(Triangulation &t, vector<P> &pts, int to_jump, int n, long r) {
    assert(to_jump < n && to_jump > 0);
    t.insert(pts.begin() + to_jump, pts.end());
    int count = 0;
    for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) v->info() = count++;
    uf set(n-to_jump);
    for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++)
        if(t.segment(e).squared_length() <= r) set.link(e->first->vertex((e->second + 1) % 3)->info(), e->first->vertex((e->second + 2) % 3)->info());
    int num_of_sets = -1;
    for(int i = 0; i < n-to_jump; i++) num_of_sets = max(num_of_sets, set.find_set(i));
    vector<int> sets(num_of_sets+1, 0);
    for(int i = 0; i < n-to_jump; i++) sets[set.find_set(i)]++;
    int maxn = -1;
    for(int i = 0; i <= num_of_sets; i++) maxn = max(maxn, sets[i]);
    return maxn;
}

int log_search(Triangulation &t, vector<P> &pts, int start, int stop, int n, long r) {
    if(start >= stop) return start;
    int middle = (start + stop + 1)/2, num = max_num(t, pts, middle, n, r);
    t.clear();
    if(num >= middle) return log_search(t, pts, middle, stop, n, r);
    return log_search(t, pts, start, middle-1, n, r);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        long n, r;
        cin >> n >> r;
        r *= r;
        vector<P> pts(n);
        for(int i = 0; i < n; i++) cin >> pts[i];
        Triangulation tri;
        cout << log_search(tri, pts, 0, n/2, n, r) << "\n";
    }
}
