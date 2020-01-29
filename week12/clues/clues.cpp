/**
 * First we need to see whether or not there are interferences. During this process, we can also save which vertices are pairwise reachable.
 * To check for interferences, we iterate over each set of connected vertices, and we populate their info with its component and its frequence.
 * We populate the two different triangulations of the frequences and check whether there are edges with distance lower than the radius.
 * The rest is then trivial.
 **/ 

#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<std::pair<int, bool>, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef K::Point_2 P;

using namespace std;

bool interferences(Triangulation &tri, long int radius) {
    for(auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); e++)
        if(tri.segment(e).squared_length() <= radius) return false;
    return true;
}

bool doable(Triangulation &tri, long int radius) {
    vector<P> pts1, pts2;
    Triangulation tri1, tri2;
    int component = -1;
    for(auto v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); v++) {
        if(v->info().first != -1) continue;
        v->info() = make_pair(++component, false);
        queue<Triangulation::Vertex_handle> stack;
        stack.push(v);
        do {
            Triangulation::Vertex_handle handle = stack.front();
            stack.pop();
            Triangulation::Vertex_circulator c = tri.incident_vertices(handle);
            if(c != 0) do {     
                if (!tri.is_infinite(c) && CGAL::squared_distance(c->point(), handle->point()) <= radius) {
                    if(c->info() == handle->info()) return false;
                    if(c->info().first == -1) {
                        stack.push(c->handle());
                        c->info() = make_pair(component, !handle->info().second);
                    }
                } 
            } while (++c != tri.incident_vertices(handle));
            if(handle->info().second) pts1.push_back(handle->point());
            else pts2.push_back(handle->point());
        } while (!stack.empty());
    }
    tri1.insert(pts1.begin(), pts1.end());
    tri2.insert(pts2.begin(), pts2.end());
    return interferences(tri1, radius) && interferences(tri2, radius);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        long int n, m, r;
        cin >> n >> m >> r;
        r *= r;

        vector<P> pts(n);
        for(int i = 0; i < n; i++) cin >> pts[i];

        Triangulation tri;
        tri.insert(pts.begin(), pts.end());
        for(auto v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); v++) v->info() = make_pair(-1, false);
        
        bool network = doable(tri, r);
        for(int i = 0; i < m; i++) {
            P start, end;
            cin >> start >> end;
            if(!network) cout << "n";
            else {
                Triangulation::Vertex_handle v1 = tri.nearest_vertex(start), v2 = tri.nearest_vertex(end);
                if(CGAL::squared_distance(start, end) <= r ||
                    (v1->info().first == v2->info().first && 
                    CGAL::squared_distance(v1->point(), start) <= r &&
                    CGAL::squared_distance(v2->point(), end) <= r)) cout << "y";
                else cout << "n";
            }
        }
        cout << "\n";
    }
}