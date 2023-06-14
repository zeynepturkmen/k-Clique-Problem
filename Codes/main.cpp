#include <iostream>
#include <cmath>
#include "randgen.h"
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
 
const int MAX = 100;
 
// Stores the vertices
int store[MAX], n;
// Graph
int graph[MAX][MAX];
// Degree of the vertices
int d[MAX];
//Stores the value coming from the brute force algorithm
int brute_force_value = 0;
//Stores the value coming from the heuristic algorithm
int heuristic_value = 0;
 
// Function to check if the given set of vertices
// in store array is a clique or not
bool is_clique(int b)
{
    // Run a loop for all the set of edges for the selected vertices
    for (int i = 0; i < b; i++) {
        for (int j = i + 1; j < b; j++) {
            // If any edge is missing, it's not a clique
            if (graph[store[i]][store[j]] == 0)
                return false;
        }
    }
    return true;
}

void print(int l)
{
    for (int i = 0; i < l; i++)
       cout << store[i] << " ";
    cout << ", ";
}

// Function to find all the cliques of size s
void findCliques(int i, int l, int s)
{
    // Check if any vertices from i+1 can be inserted
    for (int j = i; j <= n - (s - l) + 1; j++) {
        // If the degree of the graph is sufficient
        if (d[j] >= s - 1) {
            // Add the vertex to store
            store[l] = j;
            
            // If the graph is not a clique of size k, then it cannot be a clique by adding another edge
            if (is_clique(l + 1)) {
                // If the length of the clique is still less than the desired size
                if (l + 1 < s)
                    findCliques(j, l + 1, s); // Recursion to add vertices
                // Size is met
                else {
                    print(l + 1);
                    brute_force_value = 1;
                }
            }
        }
    }
}

void GenRandomGraphs(int NOEdge, int NOVertex)
{
    RandGen gen;
    int i, j, edge[NOEdge][2], count;
    
    // Initialize the adjacency matrix to false
    for (i = 0; i < NOVertex; i++) {
        for (j = 0; j < NOVertex; j++) {
            graph[i][j] = 0;
            d[i] = 0;
        }
    }

    i = 0;
    // Assign random values to the number
    // of vertex and edges of the graph,
    // Using rand().
    while (i < NOEdge) {
        edge[i][0] = gen.RandInt(1,INT_MAX) % NOVertex + 1;
        edge[i][1] = gen.RandInt(1,INT_MAX) % NOVertex + 1;

        // Check if edge already exists
        if (edge[i][0] == edge[i][1]) {
            continue;
        } else if (graph[edge[i][0]-1][edge[i][1]-1] || graph[edge[i][1]-1][edge[i][0]-1]) {
            continue;
        } else {
            // Add edge to adjacency matrix
            graph[edge[i][0]-1][edge[i][1]-1] = true;
            graph[edge[i][1]-1][edge[i][0]-1] = true;
            d[edge[i][0]-1] += 1; // increment degree of first vertex of the edge
            d[edge[i][1]-1] += 1; // increment degree of first vertex of the edge
            i++;
        }
    }
    // Print the adjacency matrix
    cout << "The adjacency matrix for the generated random graph is:" << endl;
    for (i = 0; i < NOVertex; i++) {
        for (j = 0; j < NOVertex; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    cout << "-----------------------" << endl;
    for (i = 0; i < NOVertex; i++) {
            count = 0;
            cout << "\t" << i<< "-> { ";
            for (j = 0; j < NOEdge; j++) {
                if (edge[j][0] == i + 1) {
                    cout << edge[j][1]-1 << " ";
                    count++;
                }
                else if (edge[j][1] == i + 1) {
                    cout << edge[j][0]-1 << " ";
                    count++;
                }
                else if (j == NOEdge - 1 && count == 0)

                    // Print “Isolated vertex”
                    // for the vertex having
                    // no degree.
                    cout << "Isolated Vertex!";
            }
            cout << " }" << endl;
        }
}

bool greedy_clique(int k) {
    
    if (k == 1) {
        heuristic_value = 1;
        return true;
    }
    vector<int> clique;
    vector<int> vertices;
    for (int i = 0; i < n; ++i) {
        vertices.push_back(i);
    }
    
    for (int i = 0; i < vertices.size(); ++i) {
        clique.clear();
        clique.push_back(vertices[i]);

        for (const auto& v : vertices) {
            if (find(clique.begin(), clique.end(), v) != clique.end()) {
                continue;
            }

            bool isNext = true;
            for (const auto& u : clique) {
                if (!graph[u][v]) {
                    isNext = false;
                    break;
                }
            }
            if (isNext) {
                clique.push_back(v);
                if (k <= clique.size()) {
                    for(int x=0; x < clique.size(); x++){
                        cout << clique[x] << " ";
                    }
                    cout << endl;
                    heuristic_value = 1;
                    return true;
                }
            }
        }
    }
    return false;
}

// Driver code
int main()
{
    int e;
    RandGen gen;
    cout << "Random graph generation: " << endl;

    n = gen.RandInt(7,20); //random between 7 and 20 simply
    cout << "The graph has " << n << " vertices" << endl;
    
    e = gen.RandInt(0,((n * (n - 1)) / 2));
    
    cout << "and has " << e << " edges." << endl;

  
    // Function call
    GenRandomGraphs(e, n);
    
    int k = gen.RandInt(2,8); //random between 2 and 15 simply
    cout << "Looking for cliques of size: " << k << "........" << endl;
    
    cout << endl << "Brute Force Algorithm: " << endl;
    cout << "-------------------------------------------" << endl;
    findCliques(0, 0, k);
    cout << endl;
    
    cout << endl << "Greedy heuristic algorithm: " << endl;
    cout << "-------------------------------------------" << endl;
    
    if(greedy_clique(k)){
        cout << "Yes, there is a clique of size " << k << endl;
    }
    else{
        cout << "No clique of size " << k << endl;
    }
    cout << endl;

    /*
    //For (SECTION 7) Quality testing of the greedy heuristic algorithm the below code is used
    ofstream pathDoc;
	string pathFile = "quality_of_k_5_size_25_in_250_iterations.txt";
	pathDoc.open(pathFile.c_str());
    const int iterator = 250; //iteration number is changed between trials --- 50/100/200/250
    double total = 0;
	for (int i = 0; i < iterator; i++){
	    brute_force_value = 0;
        heuristic_value = 0;
	    n = 25; // 10/15/20/25 //The graph has n vertices
        e = gen.RandInt(0,((n * (n - 1)) / 2)); //The graph has e edges
        GenRandomGraphs(e, n); //Generate a new graph
        int k = 5; // 3/4/5 //looking for cliques of size: k
        
        findCliques(0, 0, k);
        cout << endl;
        greedy_clique(k);
        
        if(brute_force_value == heuristic_value){
            pathDoc << "1" << endl;
            total++;
        }
        else{
            pathDoc << "0" << endl;
        }
	}
	cout << "\n--------\n RATE = " << total/250 << endl;
    */
    
    /*
    //For (SECTION 6) Performance testing of the greedy heuristic algorithm the below code is used
    ofstream pathDoc;
	string pathFile = "time_of_size_25_in_250_iterations.txt"; //time_of_size_SIZE_in_ITERATOR_iterations
	pathDoc.open(pathFile.c_str());
    const int iterator = 250; //iteration number is changed between trials --- 50/100/200/250
	vector<double> durations;
	for (int i = 0; i < iterator; i++){
	    n = 30; //also 15/20/25 //The graph has n vertices
        e = gen.RandInt(0,((n * (n - 1)) / 2)); //The graph has e edges
        GenRandomGraphs(e, n); //Generate a new graph
        int k = 4; //clique size is determined as 4 in all cases and looking for cliques of size: k
        
        auto start = std::chrono::high_resolution_clock::now();
        greedy_clique(k);
		auto findDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
		durations.push_back(findDuration.count());
		pathDoc << findDuration.count() << endl; //writing all the coordinates to the .txt file
	}
	double total = 0;
	for(int i=0; i<durations.size(); i++){
	    cout << i+1 << " th iteration time is --> " << durations[i] << endl;
	    total += durations[i];
	}
	cout << "-----\nTotal iteration time in for loop is -->" << total << endl;
	cout << "-----\nMean iteration time in for loop is -->" << total/250 << endl; 
	*/
    return 0;
}
