/**
 * This solution uses both the triangulation and the LP.
 * In the LP, the variables are the quantity of beer of a given warehouse given to a stadium (n*m variables).
 * The constraints are: every beer from a warehouse must be less than its supply;
 *                      the quantity of beer in every stadium must be equal to (greater-equal + less-equal) the quantity of beer given;
 *                      the quantity of alcohol per stadium must not exceed the treshold of every stadium;
 * The objective function is the sum of the variables multiplied to their respecrive revenues.
 * Finally, the triangulation is used to see whether or not a track between a warehouse and a stadium trepasses a contour line:
 * the triangulation is done on the locations of the stadiums and the warehouses; iterating over the contour lines, we need to check whether or not there's 
 * a stadium or a warehouse inside its circle. If there is, we check all the tracks between a stadium and a warehouse whether or not the track steps
 * on a contour line: if it does, the revenue of that track is decreased.
 * The solution to the problem is the objective value of the solution of the linear problem.
 **/ 

#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double floor_to_double(const ET& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while(t--) {
        int n, m, c;
        cin >> n >> m >> c;
        vector<P> s_w(n+m);
        vector<IT> warehouses(n);

        Program lp (CGAL::SMALLER, true, 0, false, 0);
        for(int i = 0; i < n; i++) {
            IT s;
            cin >> s_w[i] >> s >> warehouses[i];
            lp.set_b(i, s);
        }

        for(int i = 0; i < m; i++) {
            IT d, u;
            cin >> s_w[i+n] >> d >> u;
            u *= 100;
            lp.set_b(i+n, d);
            lp.set_b(i+n+m, -d);
            lp.set_b(i+n+2*m, u);
        }

        int counter = -1;
        vector<vector<pair<int, double>>> revenues(n, vector<pair<int, double>>(m));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                counter++;
                double revenue;
                cin >> revenue;

                revenues[i][j] = make_pair(counter, -revenue);

                lp.set_a(counter, i, 1);
                lp.set_a(counter, j+n, 1);
                lp.set_a(counter, j+n+m, -1);
                lp.set_a(counter, j+n+2*m, warehouses[i]);
            }
        }

        Triangulation tri;
        tri.insert(s_w.begin(), s_w.end());
        for(int i = 0; i < c; i++) {
            P contour;
            long radius;
            cin >> contour >> radius;
            radius *= radius;
            if(CGAL::squared_distance(tri.nearest_vertex(contour)->point(), contour) <= radius) {
                for(int i = 0; i < n; i++) {
                    bool sign1 = (CGAL::squared_distance(contour, s_w[i]) <= radius);
                    for(int j = 0; j < m; j++) {
                        bool sign2 = (CGAL::squared_distance(contour, s_w[j+n]) <= radius);
                        if(sign1 != sign2)
                            revenues[i][j].second += 0.01;
                    }
                }
            }
        }

        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                lp.set_c(revenues[i][j].first, ET(revenues[i][j].second));

        Solution s = CGAL::solve_linear_program(lp, ET());
        if(s.is_infeasible()) cout << "RIOT!\n";
        else cout << fixed << setprecision(0) << floor_to_double((-1*ET(s.objective_value_numerator()/s.objective_value_denominator()))) << "\n";
    }
}
