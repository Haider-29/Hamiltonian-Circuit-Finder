#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

int allVisited;

struct Graph {

	string* cityNames;
	int** adjacencyMatrix;
	int* deliveryTimes;
	int numCities = 0;
	int totalBattery = 0;

	Graph(string filename) {

		ifstream fin;
		fin.open(filename);

		string temp;

		getline(fin, temp);

		string V[10];
		int n = 0;
		bool reading = false;

		for (int i = 0; i < temp.length(); i++) {

			if (!reading) {

				if (temp[i] == '{') {

					reading = true;

				}

			}

			else if (reading) {

				if (temp[i] != ',' && temp[i] != '}') {

					V[n++] = temp[i];

				}

			}

		}

		cityNames = new string[n];
		numCities = n;

		allVisited = (1 << n) - 1;

		for (int i = 0; i < n; i++) {

			cityNames[i] = V[i];

		}

		getline(fin, temp);

		string edge1[10];
		string edge2[10];
		int numEdges = 0;

		int i = 0;

		while (temp[i] != '{') {

			i++;

		}

		for (i; i < temp.length(); i++) {

			if (temp[i] == '(') {

				i++;
				edge1[numEdges] = temp[i];
				i++;
				i++;
				edge2[numEdges++] = temp[i];

			}

		}

		getline(fin, temp);

		int weights[10];
		int nw = 0;

		int number = 0;

		cout << "OK" << endl;

		for (int i = 0; i < temp.length(); i++) {

			//if (temp[i] == ',') {

			//	number = 0;

			//}

			while (temp[i] >= '0' && temp[i] <= '9') {

				//number *= 10;
				//number += temp[i] - 48;
				//i++;
				weights[nw++] = temp[i] - 48;

			}

			//i--;
			/*weights[nw++] = number;*/

		}


		adjacencyMatrix = new int* [numCities];

		for (int i = 0; i < numCities; i++) {

			adjacencyMatrix[i] = new int[numCities];

		}

		for (int i = 0; i < numCities; i++) {

			for (int j = 0; j < numCities; j++) {

				adjacencyMatrix[i][j] = 0;

			}

		}

		for (int i = 0; i < nw; i++) {

			int src;
			int dst;

			for (int j = 0; j < n; j++) {

				if (edge1[i] == cityNames[j]) {

					src = j;

				}

				if (edge2[i] == cityNames[j]) {

					dst = j;

				}

			}

			adjacencyMatrix[src][dst] = weights[i];
			adjacencyMatrix[dst][src] = weights[i];

		}

		getline(fin, temp);

		string VTT[10];
		int TT[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int nTT = 0;

		deliveryTimes = new int[numCities];

		for (int i = 0; i < numCities; i++) {

			deliveryTimes[i] = 0;

		}

		for (int i = 0; i < temp.length(); i++) {

			if (temp[i] >= 'A' && temp[i] <= 'Z') {

				VTT[nTT] = temp[i];
				i++;
				i++;
				i++;

				while (temp[i] >= '0' && temp[i] <= '9') {

					TT[nTT] *= 10;
					TT[nTT] += temp[i] - 48;
					i++;

				}

				i--;
				nTT++;

			}

		}

		for (int i = 0; i < nTT; i++) {

			for (int j = 0; j < n; j++) {

				if (VTT[i] == cityNames[j]) {

					deliveryTimes[j] = TT[i];

				}

			}

		}

		getline(fin, temp);

		totalBattery = 0;

		for (int i = 0; i < temp.length(); i++) {

			if (temp[i] >= '0' && temp[i] <= '9') {

				totalBattery *= 10;
				totalBattery += temp[i] - 48;

			}

		}

	}

};

tuple<vector<int>, int> findHamiltonianCircuit(Graph& g, int mask, int currentCity, vector<int>& path, vector<int>& bestPath, vector<vector<tuple<vector<int>,int>>>& memo) {

	if (mask == allVisited) {
		if (g.adjacencyMatrix[currentCity][0] == 0) {

			return make_tuple(vector<int>(), INT_MAX);

		}

		path.push_back(0);
		bestPath = path;
		path.pop_back();

		return make_tuple(bestPath, g.adjacencyMatrix[currentCity][0]);
	}

	if (get<1>(memo[currentCity][mask]) != -1) {

		return memo[currentCity][mask];

	}

	tuple<vector<int>, int> currentDistance = make_tuple(vector<int>(), 10000);

	for (int i = 1; i < g.numCities; i++) {

		if ((mask & (1 << i)) == 0 && i != currentCity) {

			path.push_back(i);

			tuple<vector<int>, int> newDistance = findHamiltonianCircuit(g, mask | 1 << i, i, path, bestPath, memo);
			get<1>(newDistance) = g.adjacencyMatrix[currentCity][i] + get<1>(newDistance);

			int currBattery = g.totalBattery;

			for (int i = 0; i < get<0>(newDistance).size(); i++) {

				currBattery -= g.deliveryTimes[get<0>(newDistance)[i]];

			}

			if (get<0>(newDistance).size() != 0) {

				for (int i = 0; i < get<0>(newDistance).size() - 1; i++) {

					currBattery -= g.adjacencyMatrix[get<0>(newDistance)[i]][get<0>(newDistance)[i + 1]];

				}

			}

			if (get<1>(newDistance) < get<1>(currentDistance) && currBattery >= 0) {

				currentDistance = newDistance;

			}

			path.pop_back();
		}
	}

	memo[currentCity][mask] = currentDistance;
	return currentDistance;
}
