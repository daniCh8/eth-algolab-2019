/**
 * CGAL-LP problem: the variables are the quantity of each food that the people should eat.
 * The constraints are that the quantity of each nutrient must be <= max and >= min.
 * The objective function is the sum of every variable multiplied for the cost of the respective food.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    while(n != 0 || m != 0) {
        Program lp (CGAL::SMALLER, true, 0, false, 0);
        for(int i = 0; i < n; i++) {
            int min, max;
            cin >> min >> max;
            lp.set_b(i, max);
            lp.set_b(n+i, -min);
        }

        for(int i = 0; i < m; i++) {
            int cost;
            cin >> cost;
            lp.set_c(i, cost);
            for(int j = 0; j < n; j++) {
                int nutrient;
                cin >> nutrient;
                lp.set_a(i, j, nutrient);
                lp.set_a(i, n+j, -nutrient);
            }
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        if(s.is_infeasible()) cout << "No such diet.\n";
        else cout << floor(CGAL::to_double(s.objective_value())) << "\n";
        
        cin >> n >> m;
    }
}
