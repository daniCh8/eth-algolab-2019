/**
 * CGAL-LP problem: the variables are the scaling factors of each new poster;
 *                  the constraints are the relationship between the new posters and themselves and the new posters and the old ones;
 * The constraints are such that no two posters intersect between each other.
 * The objective function is the total perimeter of the posters.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n, m, h, w;
        cin >> n >> m >> h >> w;
        
        vector<pair<int, int>> free_nails(n);
        Program lp (CGAL::SMALLER, true, 1, false, 0);
        for(int i = 0; i < n; i++) {
            int x, y;
            cin >> x >> y;
            free_nails[i] = pair<int, int>(x, y);
            lp.set_c(i, -2*(h+w));
        }

        vector<pair<int, int>> old_posters(m);
        for(int i = 0; i < m; i++) {
            int x, y;
            cin >> x >> y;
            old_posters[i] = pair<int, int>(x, y);
        }

        int row_counter = 0;
        for(int i = 0; i < n; i++) {
            for(int j = i+1; j < n; j++) {
                ET delta_x = abs(free_nails[i].first - free_nails[j].first), delta_y = abs(free_nails[i].second - free_nails[j].second);
                lp.set_a(i, row_counter, 1);
                lp.set_a(j, row_counter, 1);
                lp.set_b(row_counter, max(2*(delta_x/w), 2*(delta_y/h)));
                row_counter++;
            }
            ET best_coeff = -1;
            for(int j = 0; j < m; j++) {
                ET delta_x = abs(free_nails[i].first - old_posters[j].first), delta_y = abs(free_nails[i].second - old_posters[j].second), coeff = max(2*(delta_x/w)-1, 2*(delta_y/h)-1);
                if(best_coeff == -1 || coeff < best_coeff) best_coeff = coeff;
            }
            if(m > 0) {
                lp.set_a(i, row_counter, 1);
                lp.set_b(row_counter, best_coeff);
                row_counter++;
            }
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        cout << fixed << setprecision(0) << ceil(CGAL::to_double(-s.objective_value())) << "\n";
    }
}
