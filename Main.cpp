#include "Hamiltonian Circuit.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

    //for (int i = 1; i < 3; i++) {

        Graph graph("P2_test1.txt");

        cout << "Ok" << endl;

        vector<int> path;
        path.push_back(0);
        vector<int> bestPath;

        tuple<vector<int>, int> initialTuple = make_tuple(vector<int>(), -1);

        vector<vector<tuple<vector<int>, int>>> memo(graph.numCities, vector<tuple<vector<int>, int>>(1 << graph.numCities, initialTuple));

        tuple<vector<int>, int> info = findHamiltonianCircuit(graph, 1, 0, path, bestPath, memo);

        cout << "Test Case " << to_string(1) << ":" << endl;

        if (get<1>(info) == 10000) {

            cout << "NO FEASIBLE CIRCUIT" << endl;

        }

        else {

            for (int i : get<0>(info)) {

                cout << graph.cityNames[i] << " -> ";

            }

        }
        
        cout << endl << endl;

//    }


}
