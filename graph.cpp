// graph.cpp
//
// Divvy Graph Analysis
//
// Alex Viznytsya
// macOS Sierra 10.12.4, Xcode 8.3
// Spring 2017
//

#include <iostream>

#include "graph.h"

using namespace std;


//
// Constructor:
//
Graph::Graph(int N) {
    
    this->NumVertices = 0;
    this->NumEdges = 0;
    this->Capacity = N;
    this->Vertices = new Edge*[N];
    this->Names = new string[N];
}


//
// Destructor:
//
Graph::~Graph() {
    
    // Deallocate LL:
    
    for (int i = 0 ; i < this->GetNumVertices(); i++) {
        Edge *prev = nullptr;
        Edge *curr = this->Vertices[i];
        while (curr != nullptr) {
            prev = curr;
            curr = curr->Next;
            delete prev;
        }
    }
    
    delete [] this->Vertices;
    delete [] this->Names;
}

//
// GetNumEdges:
//
//
//
int Graph::GetNumEdges() {
    return this->NumEdges;
}

//
// GetNumVertices:
//
//
//
int Graph::GetNumVertices() {
    return this->NumVertices;
}

//
// AddVertex:
//
// Adds the given vertex v to the graph, returning true if
// successful and false if not.  The add fails if (1) the
// graph is full, or (2) the vertex already exists in the
// graph.
//
bool Graph::AddVertex(string v) {
    
    // Check if we can add new vertex with current capacity:
    if (this->Capacity == this->NumVertices) {
        return false;
    }
    
    // Check if vertex already exists:
    if (this->FindVertexByName(v) != -1) {
        return false;
    }
    
    // Next vertex index:
    int i = this->NumVertices;
    
    // Head of LL: null
    this->Vertices[i] = nullptr;
    
    // Copy vertex string:
    this->Names[i] = v;
    
    // Increment vertex count:
    this->NumVertices += 1;
    
    return true;
}

//
// isEdgeExist:
//
bool Graph::IsEdgeExists(string src, string dest) {
    
    // Ceck if source and destination vertices exists:
    int S = this->FindVertexByName(src);
    if (S == -1) { return false; }
    int D = this->FindVertexByName(dest);
    if (D == -1) { return false; }
    
    Edge *curr = this->Vertices[S];
    while (curr != nullptr) {
        if (curr->Dest == D) {
            return true;
        }
        curr = curr->Next;
    }
    
    return false;
}


//
// AddEdge:
//
// Adds an edge from src to dest, with the given weight, returning
// true if successful and false if not.  The add fails if the src
// or dest vertex do not exist.
//
bool Graph::AddEdge(string src, string dest, int weight) {
    
    // Ceck if source and destination vertices exists:
    int S = this->FindVertexByName(src);
    if (S == -1) { return false; }
    int D = this->FindVertexByName(dest);
    if (D == -1) { return false; }
    
    // Create new graph edge:
    Edge *newEdge = new Edge;
    newEdge->Src = S;
    newEdge->Dest = D;
    newEdge->Weight = weight;
    
    Edge *prev = nullptr;
    Edge *curr = this->Vertices[S];
    
    while (curr != nullptr) {
        if (newEdge->Dest < curr->Dest) {
            break;
        } else if (newEdge->Dest == curr->Dest) {
            if (newEdge->Weight <= curr->Weight) {
                break;
            }
        }
        prev = curr;
        curr = curr->Next;
    }

    newEdge->Next = curr;
    if (prev == nullptr) {
        this->Vertices[S] = newEdge;
    } else {
        prev->Next = newEdge;
    }
    
    // Increment the # of edges and return true:
    this->NumEdges += 1;
    
    return true;
}


//
// UpdateEdgeWeight:
//
// If graphs's edge already exists, then update the edge with incrementing
// its weight by passed value.
//
bool Graph::UpdateEdgeWeight(string src, string dest, int value, int newValue) {
    
    // Ceck if source and destination vertices exists:
    int S = this->FindVertexByName(src);
    if (S == -1) { return false; }
    int D = this->FindVertexByName(dest);
    if (D == -1) { return false; }
    
    // Find and increment weigth by 1:
    Edge *curr = this->Vertices[S];
    while (curr != nullptr) {
        if (curr->Dest == D && curr->Weight == value) {
            curr->Weight = newValue;
            return true;
        }
        curr = curr->Next;
    }
    
    return false;
}

//
// PrintGraph:
//
// Prints the graph, typically for debugging purposes.
//
void Graph::PrintGraph(string title) {
    
    cout << ">>Graph: " << title << endl;
    cout << "  # of vertices: " << this->NumVertices << endl;
    cout << "  # of edges:    " << this->NumEdges << endl;
    cout << "  Capacity:      " << this->Capacity << endl;
    
    cout << "  Vertices:" << endl;
    
    for (int v = 0; v < this->NumVertices; ++v) {
        cout << "    " << v << " (" << this->Names[v] << "): ";
        
        Edge *edge = this->Vertices[v];
        while (edge != nullptr) {
            cout << "(" << edge->Src << "," << edge->Dest << "," << edge->Weight << ")";
            
            edge = edge->Next;
            if (edge != nullptr)
            cout << ", ";
        }
        cout << endl;
    }
}

//
// FindVertexByName:
//
// Find vertex index by name.
//
int Graph::FindVertexByName(string name) {
    
    for (int i = 0; i < this->NumVertices; i++) {
        if (this->Names[i] == name ) {
            return i;
        }
    }
    
    return -1;
}

//
// GetVertices:
//
// Returns a set containing all vertices in the graph.
//
set<string> Graph::GetVertices() {
    
    set<string> vertices;
    
    for (int i = 0; i < this->NumVertices; i++) {
        vertices.insert(this->Names[i]);
    }
    
    return vertices;
}

//
// GetNeighbors:
//
// Returns a set containing all the vertices adjacent to v.
// If v does not exist in the graph, the returned set is
// empty.
//
set<string> Graph::GetNeighbors(string v) {
    
    set<string>  neighbors;
    
    int num = this->FindVertexByName(v);
    Edge *temp = this->Vertices[num];
    while(temp != nullptr) {
        neighbors.insert(this->Names[temp->Dest]);
        temp = temp->Next;
    }
    
    return neighbors;
}

//
// GetEdgeWeights:
//
// Returns a vector containing all the weights for edges
// from src to dest.  The weights are in ascending order,
// and may contain duplicates (since it is legal for there
// to be 2 different edges from src to dest with the same
// weight).  If src or dest do not exist in the graph, the
// returned vector is empty.
//
vector<int> Graph::GetEdgeWeights(string src, string dest) {
    
    vector<int> weights;
    
    int S = this->FindVertexByName(src);
    int D = this->FindVertexByName(dest);
    
    Edge *curr = this->Vertices[S];
    while (curr != nullptr) {
        if (curr->Dest == D) {
            weights.push_back(curr->Weight);
        }
        curr = curr->Next;
    }
    
    return weights;
}

//
// iNeighbors:
//
//
//
vector<int> Graph::iNeighbors(string v) {
    
    vector<int> neighbors;
    int source = this->FindVertexByName(v);
    Edge *curr = this->Vertices[source];
    
    while (curr != nullptr) {
        neighbors.push_back(curr->Dest);
        curr = curr->Next;
    }
    
    return neighbors;
}

//
// BFS:
//
// Performs a breadth-first search starting from vertex v, returning
// a vector that denotes the order in vertices were visited.  If v
// does not exist in the graph, an empty vector is returned.
//
// NOTE that a vertex will appear at most once in the returned vector,
// since once a vertex is visited, it is never visited again.
//
vector<string> Graph::BFS(string v) {

    queue<int> frontierQueue;
    set<int> discoveredSet;
    vector<string> visited;
    int currentV = -1;
    
    int startV = this->FindVertexByName(v);
    
    if (startV == -1) {
        cout << "**Error: Invalid vertex name." << endl;
        return visited;
    }
    
    frontierQueue.push(startV);
    discoveredSet.insert(startV);
    
    while (!frontierQueue.empty()) {
        currentV = frontierQueue.front();
        frontierQueue.pop();
        visited.push_back(this->Names[currentV]);
        
        vector<int> neighbors = this->iNeighbors(this->Names[currentV]);
        
        for (int adjV : neighbors) {
            if (discoveredSet.find(adjV) == discoveredSet.end() ) {
                frontierQueue.push(adjV);
                discoveredSet.insert(adjV);
            }
        }
    }
    
    return visited;
}

