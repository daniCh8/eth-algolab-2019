/**
 * Just 80 points: the algorithm creates a triangulation on the infected people and sorts all the queries by decreasing distance.
 * If a non-infected people can escape, all the non-infected people after it that are able to reach a face from which the first non-infected people could escape, can escape aswell.
 * First of all, a non-infected people can escape if  its distance from the nearest vertex is greater than the distance allowed.
 * Then, it can escape if, moving between the edges, it can reach an infinite face.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<std::pair<int, bool>,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;
typedef std::tuple<P, long long int, int> point_radius_index;

using namespace std;

bool can_you_escape(Triangulation &t, const Triangulation::Face_handle &face, long long int d, int i) {
    if(t.is_infinite(face) || face->info().second) return true;
    face->info().first = i;
    bool escaped = false;
    if(face->neighbor(0)->info().first != i && t.segment(face, 0).squared_length() >= 4*d && can_you_escape(t, face->neighbor(0), d, i)) escaped = true;
    else if(face->neighbor(1)->info().first != i && t.segment(face, 1).squared_length() >= 4*d && can_you_escape(t, face->neighbor(1), d, i)) escaped = true;
    else if(face->neighbor(2)->info().first != i && t.segment(face, 2).squared_length() >= 4*d && can_you_escape(t, face->neighbor(2), d, i)) escaped = true;
    face->info().second = escaped;
    return escaped;
}

bool mysort(const point_radius_index &i, const point_radius_index &j) {
    return get<1>(i) > get<1>(j);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    size_t n;
    cin >> n;

    while(n != 0) {
        vector<P> infected_ppl(n);
        for(size_t i = 0; i < n; i++) cin >> infected_ppl[i];

        Triangulation t;
        t.insert(infected_ppl.begin(), infected_ppl.end());
        for(auto f = t.all_faces_begin(); f != t.all_faces_end(); f++)
            f->info() = make_pair(-1, false);

        int m;
        cin >> m;

        vector<point_radius_index> queries(m);
        for(int i = 0; i < m; i++) {
            P starting_point;
            long long int d;
            cin >> starting_point >> d;
            queries[i] = {starting_point, d, i};
        }

        sort(queries.begin(), queries.end(), mysort);
        vector<bool> answers(m);
        for(int i = 0; i < m; i++) {
            auto nearest_vertex = t.nearest_vertex(get<0>(queries[i]));
            if(CGAL::squared_distance(get<0>(queries[i]), nearest_vertex->point()) < get<1>(queries[i])) {
                answers[get<2>(queries[i])] = false;
                continue;
            }
            Triangulation::Face_handle face = t.locate(get<0>(queries[i]));
            if(t.is_infinite(face) || face->info().second) {
                answers[get<2>(queries[i])] = true;
                continue;
            }
            answers[get<2>(queries[i])] = can_you_escape(t, face, get<1>(queries[i]), i);
        }
        for(int i = 0; i < m; i++)
            answers[i] ? cout << "y" : cout << "n";

        cout << "\n";
        cin >> n;
    }
}
