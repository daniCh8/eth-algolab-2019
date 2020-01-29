/**
 * x + y <= 1 equals to -x -y >= -1.
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

    int p;
    cin >> p;

    while(p != 0) {
        int a, b;
        cin >> a >> b;

        Program lp1(CGAL::SMALLER, true, 0, false, 0), lp2(CGAL::SMALLER, false, 0, true, 0);
        const int X = 0;
        const int Y = 1;
        const int Z = 2;
        Solution s;

        switch (p) {
        case 1:
            lp1.set_a(X, 0, 1); lp1.set_a(Y, 0, 1); lp1.set_b(0, 4);
            lp1.set_a(X, 1, 4); lp1.set_a(Y, 1, 2); lp1.set_b(1, a*b);
            lp1.set_a(X, 2, -1); lp1.set_a(Y, 2, 1); lp1.set_b(2, 1);
            lp1.set_c(X, a); lp1.set_c(Y, -b);
            s = CGAL::solve_linear_program(lp1, ET());
            break;
        
        case 2:
            lp2.set_a(X, 0, -1); lp2.set_a(Y, 0, -1); lp2.set_b(0, 4);
            lp2.set_a(X, 1, -4); lp2.set_a(Y, 1, -2); lp2.set_a(Z, 1, -1); lp2.set_b(1, a*b);
            lp2.set_a(X, 2, 1); lp2.set_a(Y, 2, -1); lp2.set_b(2, 1);
            lp2.set_c(X, a); lp2.set_c(Y, b); lp2.set_c(Z, 1);
            s = CGAL::solve_linear_program(lp2, ET());
            break;
        }

        if(s.is_infeasible()) cout << "no\n";
        else {
            int value = ceil(CGAL::to_double(s.objective_value()));
            if(p == 1) value *= -1;
            cout << fixed << setprecision(0) << value << "\n";
        }

        cin >> p;
    }
    
}
