// graph.h
//
// Divvy Graph Analysis
//
// Alex Viznytsya
// macOS Sierra 10.12.4, Xcode 8.3
// Spring 2017
//

#pragma once

#include <string>
#include <vector>
#include <set>
#include <queue>

using namespace std;

class Graph {
    private:
        class Edge {
            public:
                int  Src, Dest, Weight;
                Edge *Next;
        };
    
        Edge    **Vertices;
        string  *Names;
        int     NumVertices;
        int     NumEdges;
        int     Capacity;
    
        int FindVertexByName(string name);
        vector<int> iNeighbors(string name);
    
    public:
        Graph(int N);
        ~Graph();
    
        bool AddVertex(string v);
        bool AddEdge(string src, string dest, int weight);
        bool UpdateEdgeWeight(string src, string dest, int value, int newValue);
        bool IsEdgeExists(string src, string dest);
        int GetNumVertices();
        set<string> GetVertices();
        set<string> GetNeighbors(string v);
        int GetNumEdges();
        vector<int> GetEdgeWeights(string src, string dest);
        vector<string> BFS(string v);
        void PrintGraph(string title);
};
