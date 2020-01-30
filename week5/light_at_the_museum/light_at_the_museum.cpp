/**
 * Super ugly and long solution - it's a mix of different tries to solve the problem. It works, but it's super ugly. DP mixed to Split & List.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>

using namespace std;

const int maxint = INT_MAX;

void brute_force(int number_of_switches, int m, int index, vector<int> lights_in_the_room, vector<vector<int>> &switches, vector<pair<vector<int>, int>> &subset) {
	if(index < 0) return;

	vector<int> newLights(m);
	for(int i = 0; i < m; i++)
		newLights[i] = lights_in_the_room[i] + switches[index][i];

	pair<vector<int>, int> tmp(newLights, number_of_switches+1);
	subset.push_back(tmp);

	brute_force(number_of_switches, m, index-1, lights_in_the_room, switches, subset);
	brute_force(number_of_switches+1, m, index-1, newLights, switches, subset);
}

bool mysort1(pair<vector<int>, int> i, pair<vector<int>, int> j) {
	for(int index = 0; index < (int)(i.first.size()); index++) {
		if(i.first[index] < j.first[index]) return true;
		else if(i.first[index] > j.first[index]) return false;
	}
	return i.second < j.second;
}

bool mysort2(pair<vector<int>, int> i, pair<vector<int>, int> j) {
	for(int index = 0; index < (int)(i.first.size()); index++) {
		if(i.first[index] > j.first[index]) return true;
		else if(i.first[index] < j.first[index]) return false;
	}
	return i.second < j.second;
}

int compare(vector<pair<vector<int>, int>> &subset1, vector<pair<vector<int>, int>> &subset2, vector<int> &target, int m, int found) {
	int index1 = 0, index2 = 0;
	while(index1 < (int)subset1.size() && index2 < (int)subset2.size()) {
		bool mix = true;
		for(int i = 0; i < m; i++) {
			int comparison = target[i] - (subset1[index1].first)[i] - (subset2[index2].first)[i];
			if(comparison > 0) {
				mix = false;
				i = m;
				index1++;
			} else if(comparison < 0) {
				mix = false;
				i = m;
				index2++;
			}
		}
		if(mix) {
			int tmp = subset1[index1].second + subset2[index2].second;
			if(tmp < found)
				found = tmp;

			int index1_backup = index1;
			bool check1 = true;
			index1++;
			while(check1 && index1 < subset1.size()) {
				for(int i = 0; i < m; i++) {
					int comparison = target[i] - (subset1[index1].first)[i] - (subset2[index2].first)[i];
					if(comparison != 0) {
						check1 = false;
						i = m;
					}
				}
				if(check1) {
					tmp = subset1[index1].second + subset2[index2].second;
					if(tmp < found)
						found = tmp;
				}
				index1++;
			}
			index1 = index1_backup;
			index2++;
		}
	}
	return found;
}

int findmin1(vector<pair<vector<int>, int>> &subset, vector<int> &target, int m) {
	int index = 0;
	while(index < (int)subset.size()) {
		bool mix = true;
		for(int i = 0; i < m; i++) {
			int comparison = target[i] - (subset[index].first)[i];
			if(comparison > 0) {
				mix = false;
				i = m;
				index++;
			} else if(comparison < 0) {
				return maxint;
			}
		}
		if(mix) return subset[index].second; 
	}
	return maxint;
}

int findmin2(vector<pair<vector<int>, int>> &subset, vector<int> &target, int m) {
	int index = 0;
	while(index < (int)subset.size()) {
		bool mix = true;
		for(int i = 0; i < m; i++) {
			int comparison = target[i] - (subset[index].first)[i];
			if(comparison < 0) {
				mix = false;
				i = m;
				index++;
			} else if(comparison > 0) {
				return maxint;
			}
		}
		if(mix) return subset[index].second; 
	}
	return maxint;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int num_of_tests;
    cin >> num_of_tests;

    while(num_of_tests > 0) {
    	int n, m;
    	cin >> n >> m;

    	vector<int> target(m);
    	for(int i = 0; i < m; i++)
    		cin >> target[i];

    	int half = n/2, remaining = n-half;

    	vector<int> lights_in_the_room(m, 0);
    	vector<vector<int>> switches1(half, vector<int>(m, 0)), switches2(remaining, vector<int>(m, 0));
    	for(int i = 0; i < n; i++) {
    		for(int j = 0; j < m; j++) {
    			int tmp = 0;
    			for(int k = 0; k < 2; k++) {
    				if(k == 0) {
                        int first;
    					cin >> first;
    					target[j] -= first;
                        tmp -= first;
    				}
    				else {
                        int second;
    					cin >> second;
                        tmp += second;
    					if(i < half) switches1[i][j] = tmp;
    					else switches2[i-half][j] = tmp;
    				}
    			}
    		}
    	}

    	vector<pair<vector<int>, int>> subset1, subset2;
    	brute_force(0, m, half-1, lights_in_the_room, switches1, subset1);
    	brute_force(0, m, remaining-1, lights_in_the_room, switches2, subset2);

    	sort(subset1.begin(), subset1.end(), mysort1);
    	sort(subset2.begin(), subset2.end(), mysort2);

    	int result = min(compare(subset1, subset2, target, m, maxint), findmin1(subset1, target, m));
    	result = min(result, findmin2(subset2, target, m));
    	if(result == maxint) cout << "impossible\n";
    	else cout << result << "\n";

    	num_of_tests--;
    }
}
