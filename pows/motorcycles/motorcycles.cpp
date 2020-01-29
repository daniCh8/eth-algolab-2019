/**
 * The solution has a greedy flavour: if we order the rays by increasing height of their starting position, we are then able to compare them all efficiently.
 * If a ray has a "better" slope than another that it crosses (the absolute value of the slope is lower), it wins.
 * If a ray has a "worse" slope than another, it can proceed to east only if it starts at a higher position than every ray with a better slope than it.
 * The rest of the problem's difficulty is all about saving the indices of the rays and also computing each slope just once to not occur in a time limit.
 **/ 

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef std::tuple<K::Ray_2, int, K::FT> ray_index_slope;

using namespace std;

bool mysort_position(const ray_index_slope &i, const ray_index_slope &j) {
    return get<0>(i).source().y() > get<0>(j).source().y();
}

bool mysort_index(const ray_index_slope &i, const ray_index_slope &j) {
    return get<1>(i) < get<1>(j);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<ray_index_slope> bikers(n);
        for(int i = 0; i < n; i++) {
            long y0, x1, y1;
            cin >> y0 >> x1 >> y1;

            K::Ray_2 biker(K::Point_2(0, y0), K::Point_2(x1, y1));
            K::FT slope = (K::FT(y1-y0))/(K::FT(x1));
            bikers[i] = {biker, i, slope};
        }

        sort(bikers.begin(), bikers.end(), mysort_position);
        vector<ray_index_slope> into_the_sunshine;
        ray_index_slope best_slope = bikers[0];
        into_the_sunshine.push_back(bikers[0]);
        for(int i = 1; i < n; i++) {
            if(get<2>(bikers[i]) <= get<2>(best_slope)) {
                into_the_sunshine.push_back(bikers[i]);
                best_slope = bikers[i];
                continue;
            }
            if(abs(get<2>(bikers[i])) <= abs(get<2>(best_slope))) {
                int start_to_delete = (int)into_the_sunshine.size();
                while(start_to_delete > 0 && CGAL::do_intersect(get<0>(bikers[i]), get<0>(into_the_sunshine[start_to_delete-1])) && abs(get<2>(bikers[i])) <= abs(get<2>(into_the_sunshine[start_to_delete-1])))
                    start_to_delete--;
                into_the_sunshine.erase(into_the_sunshine.begin()+start_to_delete, into_the_sunshine.end());
                if(start_to_delete == 0 || get<2>(bikers[i]) <= get<2>(into_the_sunshine[start_to_delete-1])) {
                    into_the_sunshine.push_back(bikers[i]);
                    best_slope = bikers[i];
                }
            }
        }

        sort(into_the_sunshine.begin(), into_the_sunshine.end(), mysort_index);
        for(int i = 0; i < (int)into_the_sunshine.size(); i++)
            cout << get<1>(into_the_sunshine[i]) << " ";
        cout << "\n";
    }
}
