/**
 * Master solution - Planks
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

void back_track(int id, int ubound, VVI &F, VVI &assignment, const VI &planks) {
    if (id >= ubound) {
        VI tuple(4, 0);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < (int)assignment[i].size(); ++j)
                tuple[i] += planks[assignment[i][j]];
        }
        F.push_back(tuple);
        return;
    }

    for (int i = 0; i < 4; ++i) {
        assignment[i].push_back(id);
        back_track(id + 1, ubound, F, assignment, planks);
        assignment[i].pop_back();
    }
}

void testcase() {
    int n; 
    std::cin >> n;
    std::vector<int> planks;
    for (int i = 0; i < n; ++i) {
        int plank;
        std::cin >> plank;
        planks.push_back(plank);
    }
    
    int sum = 0;
    long long result = 0;
    
    for (int i = 0 ; i < (int)planks.size(); ++i)
        sum += planks[i];
        
    if (sum % 4 != 0) {
        std::cout << 0 << std::endl;
        return;
    }

    VVI F1, assignment(4);
    
    back_track(0, n/2, F1, assignment, planks);
    
    VVI F2, assignment2(4);
    
    back_track(n/2, n, F2, assignment2, planks);
    std::sort(F2.begin(), F2.end());
    
    for (int idx = 0; idx < (int)F1.size(); ++idx) {
        std::vector<int> member = F1[idx];
        for (int i = 0; i < 4; ++i)
            member[i] = sum/4 - member[i];
            
        std::pair<VVI::iterator, VVI::iterator> bounds;
        bounds = std::equal_range(F2.begin(), F2.end(), member);
        long long counter = std::distance(bounds.first, bounds.second);
        result += counter;
    }
    
    std::cout << result / 24 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    
    int t; 
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}