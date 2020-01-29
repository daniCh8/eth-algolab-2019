/**
 * This solution gets 100 points. The algorithm creates a triangulation on the infected people, and pre-computes for all the faces the maximum distance with 
 * which it's possible to escape from that face. This process goes this way: we add to a set<pair<Triangulation::Face_handle, K::FT>>
 * which is ordered by decreasing distance (pair.second) all the faces that have an infinite face as neighbor, with the face as the first element
 * of the pair, and the squared length of the segment through which we should pass to go to the infinite face as the second element of the pair.
 * Then, we start visiting the triangulation faces checking the first element of the set every time. If that first element was not visited before, we 
 * can set its maximum distance and add to the set all its neighbor faces that were not visited before, with as distance the minimum distance between
 * the one I need to escape from the face I'm visiting now and the one that's needed to go from this face to that neighbor face (because to escape from
 * this direction from that neighbor face, I need to come through that edge first).
 * This way, at every iteration of the visit, we are sure that the face that was on top of the stack, if it hasn't been already visited, has the distance that's
 * bound with her in the pair as maximum distance from which it's possible to escape from that face.
 * After this precomputation, the rest of the problem is almost trivial: we just need to check if the starting point is already too near to an infected person.
 * If it's not, we check whether or not the maximum distance needed to escape from the face the non-infected person is located on is enough to escape for that person.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;

using namespace std;

struct pair_sorter {
    bool operator() (const pair<Triangulation::Face_handle, K::FT> &i, const pair<Triangulation::Face_handle, K::FT> &j) const {
        return i.second > j.second;
    }
};

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
        multiset<pair<Triangulation::Face_handle, K::FT>, pair_sorter> handles_edges;
        for(Triangulation::Face_handle f = t.all_faces_begin(); f != t.all_faces_end(); f++) {
            if(!t.is_infinite(f)) {
                f->info() = 0;
                continue;
            }

            f->info() = -1;
            for(int i = 0; i < 3; i++) 
                if(!t.is_infinite(f->neighbor(i))) handles_edges.insert({f->neighbor(i), t.segment(f, i).squared_length()});
        }

        while(handles_edges.size() > 0) {
            Triangulation::Face_handle face = (*(handles_edges.begin())).first;
            K::FT dist = (*(handles_edges.begin())).second;
            handles_edges.erase(handles_edges.begin());
            if(face->info() != 0) continue;
            face->info() = dist;

            for(int i = 0; i < 3; i++) 
                if(!t.is_infinite(face->neighbor(i)) && !face->neighbor(i)->info()) 
                    handles_edges.insert({face->neighbor(i), min(dist, t.segment(face, i).squared_length())});
        }

        int m;
        cin >> m;
        for(int i = 0; i < m; i++) {
            P starting_point;
            long long int d;
            cin >> starting_point >> d;
            auto nearest_vertex = t.nearest_vertex(starting_point);
            if(CGAL::squared_distance(nearest_vertex->point(), starting_point) < d) {
                cout << "n";
                continue;
            }
            Triangulation::Face_handle face = t.locate(starting_point);
            if(t.is_infinite(face) || face->info() >= 4*d) cout << "y";
            else cout << "n";
        }
        cout << "\n";
        cin >> n;
    }
}
