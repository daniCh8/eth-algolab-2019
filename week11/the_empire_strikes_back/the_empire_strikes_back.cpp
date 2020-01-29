/**
 * The solution is made through a LP and a triangulation on the bounty hunters.
 * The LP has as variables every energy given to a shot, and as constraints the sum of damages that every asteroid takes (that must be larger than its explosion treshold).
 * The last constraint is that the sum of the energies given to the shots must be lower than the maximum energy we are given.
 * The triangulation is needed to find the nearest bounty hunter for every shot and therefore the maximum radius that can be given to the explosion.
 * This maximum radius will then be used to see whether an asteroid is hit by a shot or not.
 * The answer to the problem is its feasibility.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>


typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2  P;

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int a, s, b, e;
        cin >> a >> s >> b >> e;

        Program lp (CGAL::LARGER, true, 0, false, 0);
        vector<P> asteroids(a), shooting_points(s), bounty_hunters(b);
        for(int i = 0; i < a; i++) {
            int a;
            cin >> asteroids[i] >> a;
            lp.set_b(i, a);
        }
        for(int i = 0; i < s; i++) cin >> shooting_points[i];
        for(int i = 0; i < b; i++) cin >> bounty_hunters[i];

        Triangulation tri;
        tri.insert(bounty_hunters.begin(), bounty_hunters.end());

        for(int i = 0; i < s; i++) {
            long radius;
            if(b == 0) radius = LONG_MAX;
            else radius = CGAL::squared_distance(shooting_points[i], tri.nearest_vertex(shooting_points[i])->point());
            for(int j = 0; j < a; j++) {
                long distance = CGAL::squared_distance(shooting_points[i], asteroids[j]);
                ET deno = 1;
                if(distance == 0) deno = 1;
                else deno = (ET)pow(distance, -1);
                if(distance <= radius) lp.set_a(i, j, deno);
            }
            lp.set_a(i, a, -1);
        }
        lp.set_b(a, -e);

        Solution sol = CGAL::solve_linear_program(lp, ET());
        if(sol.is_infeasible()) cout << "n\n";
        else cout << "y\n";

    }
}
