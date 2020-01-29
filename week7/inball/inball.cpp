/**
 * CGAL-LP problem - master solution.
 **/ 

#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <math.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, d;
    cin >> n;
    while(n != 0) {
        cin >> d;
        Program lp (CGAL::SMALLER, false, 0, false, 0);

        for(int i = 0; i < n; i++) {
            int squared_sum = 0;
            for(int j = 0; j < d; j++) {
                int coefficient;
                cin >> coefficient;
                lp.set_a(j, i, coefficient);
                squared_sum += coefficient*coefficient;
            }
            squared_sum = (int)sqrt(squared_sum);
            lp.set_a(d, i, squared_sum);
            int c;
            cin >> c;
            lp.set_b(i, c);
        }
        lp.set_c(d, -1);
        lp.set_l(d, true, 0);
        Solution s = CGAL::solve_linear_program(lp, ET());

        if(s.is_infeasible()) cout << "none\n";
        else if(s.is_unbounded()) cout << "inf\n";
        else cout << floor(CGAL::to_double(-s.objective_value())) << "\n";

        cin >> n;
    }
}
