#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

enum Channel {None = -1, One = 0, Other = 1};
typedef std::tuple<Channel, bool, int, int> channel_visited_index_counter;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<channel_visited_index_counter, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef K::Point_2 P;
typedef boost::disjoint_sets_with_storage<> Dis_set;

using namespace std;

bool reachable(Dis_set &uf, const long int &squared_r, int index, Triangulation &t, const Triangulation::Vertex_handle &v, const Triangulation::Vertex_handle &target) {
    if(uf.find_set(get<3>(v->info())) == uf.find_set(get<3>(target->info()))) return true;
    get<2>(v->info()) = index;
    if(v == target) return true;
    Triangulation::Vertex_circulator c = t.incident_vertices(v);
    if(c != 0) do {     
        if (!t.is_infinite(c) && get<2>(c->info()) < index && uf.find_set(get<3>(v->info())) == uf.find_set(get<3>(c->info())) && reachable(uf, squared_r, index, t, c->handle(), target)) {
            uf.link(get<3>(v->info()), get<3>(target->info()));
            return true;
        }
    } while (++c != t.incident_vertices(v));
    return false;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;
    while(tests--) {
        int n, m, r;
        cin >> n >> m >> r;

        long int squared_r = pow(r, 2);
        vector<P> pts(n), pts_one, pts_other;
        for(int i = 0; i < n; i++) cin >> pts[i];
        Dis_set uf(n);
        Triangulation t;
        t.insert(pts.begin(), pts.end());
        bool interferences = false;
        int counter = 0;
        for(auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); it++) it->info() = make_tuple(None, false, -1, counter++);
        queue<Triangulation::Vertex_handle> to_visit;
        for(auto it = t.finite_vertices_begin(); !interferences && it != t.finite_vertices_end(); it++) {
            if(get<1>(it->info())) continue;
            get<0>(it->info()) = Other;
            pts_other.push_back(it->point());
            Channel to_assign = One;
            get<1>(it->info()) = true;
            Triangulation::Vertex_circulator c = t.incident_vertices(it);
            if(c != 0) do {
                if(!t.is_infinite(c) && CGAL::squared_distance(c->point(), it->point()) <= squared_r) {
                    uf.link(get<3>(it->info()), get<3>(c->info()));
                    get<0>(c->info()) = to_assign;
                    pts_one.push_back(c->point());
                    to_visit.push(c->handle());
                }
            } while(++c != t.incident_vertices(it));

            while(!interferences && !to_visit.empty()) {
                Triangulation::Vertex_handle v = to_visit.front();
                to_visit.pop();
                if(get<1>(v->info())) continue;
                get<1>(v->info()) = true;
                if(get<0>(v->info()) == One) to_assign = Other;
                else to_assign = One;
                c = t.incident_vertices(v);
                if(c != 0) do {
                    if(!t.is_infinite(c) && CGAL::squared_distance(c->point(), v->point()) <= squared_r) {
                        uf.link(get<3>(v->info()), get<3>(c->info()));
                        if(get<0>(c->info()) == None) {
                            get<0>(c->info()) = to_assign;
                            if(to_assign == One) pts_one.push_back(c->point());
                            else pts_other.push_back(c->point());
                        }
                        if(get<0>(c->info()) != to_assign) {
                            interferences = true;
                            it = t.finite_vertices_end();
                        }
                        if(!get<1>(c->info())) to_visit.push(c->handle());
                    }
                } while(!interferences && ++c != t.incident_vertices(v));
            }
        }

        if(!interferences) {
            Triangulation t_one, t_other;
            t_one.insert(pts_one.begin(), pts_one.end());
            t_other.insert(pts_other.begin(), pts_other.end());
            for(auto s = t_one.finite_edges_begin(); !interferences && s != t_one.finite_edges_end(); s++)
                if(t_one.segment(s).squared_length() <= squared_r)
                    interferences = true;
            for(auto s = t_other.finite_edges_begin(); !interferences && s != t_other.finite_edges_end(); s++)
                if(t_other.segment(s).squared_length() <= squared_r)
                    interferences = true;
        }

        for(int i = 0; i < m; i++) {
            P a, b;
            cin >> a >> b;
            if(interferences) {
                cout << "n";
                continue;
            }

            if(CGAL::squared_distance(a, b) <= squared_r) {
                cout << "y";
                continue;
            }

            Triangulation::Vertex_handle nearest_a = t.nearest_vertex(a)->handle(), nearest_b = t.nearest_vertex(b)->handle();
            if(CGAL::squared_distance(a, nearest_a->point()) > squared_r || CGAL::squared_distance(b, nearest_b->point()) > squared_r || !reachable(uf, squared_r, i, t, nearest_a, nearest_b)) cout << "n";
            else cout << "y";
        }
        cout << "\n";
    }
}
