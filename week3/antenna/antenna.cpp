/**
 * The whole problem is solved by a CGAL construct - the Min_Circle, which computes the minimum enclosing circle of a set of points.
 */ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  K::Point_2                      P;

using namespace std;

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    while(n != 0) {
        long int x, y;
        vector<P> points(n);
        for(int i = 0; i < n; i++) {
            cin >> x >> y;
            P p(x, y);
            points[i] = p;
        }

        Min_circle mc(points.begin(), points.end(), true);
        cout << fixed << setprecision(0) << ceil_to_double(sqrt(mc.circle().squared_radius())) << "\n";

        cin >> n;
    }
}
