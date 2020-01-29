/**
 * It's just a test for the intersection of a ray with some segments.
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

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    while (n != 0) {
        long a, b, c ,d;
        cin >> a >> b >> c >> d;
        P start(a, b), point(c, d);
        R philip(start, point);

        bool intersect = false;
        for(int i = 0; i < n; i++) {
            P p1, p2;
            cin >> p1 >> p2;
            S s(p1, p2);
            if(CGAL::do_intersect(philip, s)) {
                intersect = true;
                i++;
                long w, x, y, z;
                while(i < n) {
                    cin >> w >> x >> y >> z;
                    i++;
                }
            }
        }

        if(intersect) cout << "yes\n";
        else cout << "no\n";

        cin >> n;
    }
    
}
