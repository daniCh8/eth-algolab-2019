/**
 * As in Antenna, the minimum enclosing circle is the construct used to solve this problem.
 * This time, we don't need to cover all the points: we can leave one of them out.
 * The solution is therefore to compute the min_circle with all the points, and then check for the points that lay on the boundary of this min_circle:
 *      each of those boundary points may be left out from the circle; therefore, we leave out one of them per time and look for the smallest circle.
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
        vector<int> on_boundary_points;
        for(int i = 0; i < n; i++) {
            if(mc.has_on_boundary(points[i]))
                on_boundary_points.push_back(i);
        }

        K::FT squared_radius = mc.circle().squared_radius();
        for(int i = 0; i < on_boundary_points.size(); i++) {
            P box = points[on_boundary_points[i]];
            points[on_boundary_points[i]] = points[n-1];
            points[n-1] = box;
            Min_circle m_tmp(points.begin(), points.end()-1, true);
            if(m_tmp.circle().squared_radius() < squared_radius)
                squared_radius = m_tmp.circle().squared_radius();
            points[n-1] = points[on_boundary_points[i]];
            points[on_boundary_points[i]] = box;
        }


        cout << fixed << setprecision(0) << ceil_to_double(sqrt(squared_radius)) << "\n";

        cin >> n;
    }
}
