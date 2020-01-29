/**
 * This time we're looking for the first intersection of a ray with a vector of segments.
 * The way to achieve this efficiently is to resize my segment every time that it intersects a ray, in order to not check and look for unnecessary intersections.
 * Moreover, a random shuffle of the array of segments is needed (if they're given such that they're ordered by decreasing intersections, all the process above would be pointless and time-wasting).
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

using namespace std;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    while (n != 0) {
        long int a, b, c ,d;
        cin >> a >> b >> c >> d;
        P start(a, b), point(c, d);
        R philip(start, point);
        S philip_seg;

        vector<S> segments;
        for(int i = 0; i < n; i++) {
            cin >> a >> b >> c >> d;
            S s(P(a, b), P(c, d));
            segments.push_back(s);
        }
        random_shuffle(segments.begin(), segments.end());

        bool intersect = false;
        P best_intersection;
        K::FT best_distance = -1;
        for(int i = 0; i < n; i++) {
            S s = segments[i];
            bool this_intersect;
            if(intersect) this_intersect = CGAL::do_intersect(philip_seg, s);
            else this_intersect = CGAL::do_intersect(philip, s);
            if(this_intersect) {
                intersect = true;
                P intersection;
                auto o = CGAL::intersection(philip,s);
                if (const P* op = boost::get<P>(&*o))
                    intersection = *op;
                else if (const S* os = boost::get<S>(&*o)) {
                    if(CGAL::squared_distance(start, os->source()) < CGAL::squared_distance(start, os->target())) intersection = os->source();
                    else intersection = os->target();
                }
                if(CGAL::squared_distance(intersection, start) < best_distance || best_distance == -1) {
                    best_distance = CGAL::squared_distance(intersection, start);
                    S segm(start, intersection);
                    philip_seg = segm;
                }
            }
        }

        if(!intersect) cout << "no\n";
        else {
            cout << setiosflags(ios::fixed) << setprecision(0) << floor_to_double(philip_seg.target().x()) << " " << floor_to_double(philip_seg.target().y()) << "\n";
        }

        cin >> n;
    }
    
}
