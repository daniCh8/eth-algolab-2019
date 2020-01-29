/**
 * BGL-Flow problem: the nodes are all the possible combinations between the letters of the alphabet that occurs in the nodes and all those letters.
 * Every note has a back and a front: if we cut every note in pieces corresponding to a single letter each, we can always take either the front letter or the back letter.
 * Therefore, we compute how many times every pair of letter occurs, and we add a dummy node per pair of letter that occurs.
 * This node has an incoming edge from the source (capacity = number of times that this pair occurs) and 
 * two outcoming edges to the nodes corresponding to the letters of the pair, each with capacity = number of times that the pair occurs.
 * Finally, every letter has an outcoming edge to the sink with capacity equals to how many times that letter occurs in the crime scene note.
 * If the flow from the source to the sink is equal to the length of the crime scene note, the answer is yes: otherwise, it's no.
 **/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, long, boost::property<boost::edge_residual_capacity_t, long, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
    graph &G;
    public:
        explicit edge_adder(graph &G) : G(G) {}
        void add_edge(int from, int to, long capacity) {
            auto c_map = boost::get(boost::edge_capacity, G);
            auto r_map = boost::get(boost::edge_reverse, G);
            const auto e = boost::add_edge(from, to, G).first;
            const auto rev_e = boost::add_edge(to, from, G).first;
            c_map[e] = capacity;
            c_map[rev_e] = 0; // reverse edge has no capacity!
            r_map[e] = rev_e;
            r_map[rev_e] = e;
        }
};

using namespace std;

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--) {
        int h, w;
        cin >> h >> w;

        string crime_scene;
        cin >> crime_scene;
        vector<int> crime_scene_letters(26, 0);
        for(int i = 0; i < (int)crime_scene.length(); i++) crime_scene_letters[(int)crime_scene[i]-65]++;

        vector<string> front(h), back(h);
        for(int i = 0; i < 2*h; i++) i < h ? cin >> front[i] : cin >> back[i-h];

        vector<vector<int>> letter_matrix(26, vector<int>(26, 0));
        int tuple_counter = 0;
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < w; j++) {
                int front_letter = min((int)front[i][j]-65, (int)back[i][w-1-j]-65), back_letter = max((int)front[i][j]-65, (int)back[i][w-1-j]-65);
                if(letter_matrix[front_letter][back_letter] == 0) tuple_counter++;
                letter_matrix[front_letter][back_letter]++;
            }
        }

        graph g(26 + tuple_counter);
        tuple_counter = 26;
        edge_adder adder(g);
        const vertex_desc source = boost::add_vertex(g), target = boost::add_vertex(g);
        for(int i = 0; i < 26; i++) {
            if(crime_scene_letters[i] > 0) adder.add_edge(i, target, crime_scene_letters[i]);
            for(int j = 0; j < 26; j++) {
                if(letter_matrix[i][j] > 0) {
                    adder.add_edge(source, tuple_counter, letter_matrix[i][j]);
                    adder.add_edge(tuple_counter, i, letter_matrix[i][j]);
                    adder.add_edge(tuple_counter, j, letter_matrix[i][j]);
                    tuple_counter++;
                }
            }
        }

        if((int)boost::push_relabel_max_flow(g, source, target) == (int)crime_scene.length()) cout << "Yes\n";
        else cout << "No\n";
    }
}
