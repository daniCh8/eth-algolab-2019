/**
 * This problem can be solved using a LP built the following way:
 * The variables are just the time that Asterix and Panoramix will have, and the (x, y) coordinates of the point P they'll choose.
 * The constraints are all such that the variable time must be <= than the distance(P, legion)/legion_speed.
 * That distance has a sign: it's used to save the position of Asterix and Panoramix with respect to the legion (they can't trepass the legions to go to their point).
 * The objective function will be the time, negated (we want to maximize it).
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

IT sign(IT a, IT b, IT c, IT x, IT y) {
    if(a*x+b*y+c >= 0) return 1;
    return -1;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        IT x_ao, y_ao, n;
        cin >> x_ao >> y_ao >> n;

        Program lp (CGAL::SMALLER, false, 0, false, 0); 
        const int time = 0, x_p = 1, y_p = 2;
        for(int i = 0; i < n; i++) {
            IT a, b, c, v;
            cin >> a >> b >> c >> v;
            IT a_l = pow(a, 2), b_l = pow(b, 2), deno = sqrt(a_l + b_l), l_sign = sign(a, b, c, x_ao, y_ao);
            lp.set_a(time, i, v*deno);
            lp.set_a(x_p, i, -a*l_sign);
            lp.set_a(y_p, i, -b*l_sign);
            lp.set_b(i, c*l_sign);
        }
        lp.set_l(time, true, 0);
        lp.set_c(time, -1);
        lp.set_c(x_p, 0);
        lp.set_c(y_p, 0);
        lp.set_c0(0);

        Solution s = CGAL::solve_linear_program(lp, ET());
        ET result = s.objective_value_numerator()/s.objective_value_denominator();
        cout << fixed << setprecision(0) << floor(CGAL::to_double(-1*result)) << "\n";
    }
}
