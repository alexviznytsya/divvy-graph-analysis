// main.cpp
//
// Divvy Graph Analysis
//
// Alex Viznytsya
// macOS Sierra 10.12.4, Xcode 8.3
// Spring 2017
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

#include "graph.h"

using namespace std;

// Ignore C-related security warnings in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS


//
// Defines a Divvy Station:
//
class Station {
    public:
    string Name;
    int    ID;
    double Latitude;
    double Longitude;
    int    Capacity;
    
    Station(string name, int id, double latitude, double longitude, int capacity) {
        Name = name;
        ID = id;
        Latitude = latitude;
        Longitude = longitude;
        Capacity = capacity;
    }
};

//
// FindStationNameById:
//
// Find Station element in vector by station ID using linear search.
//
Station FindStationNameById(vector<Station>& s, int id) {
    for (int i = 0 ; i < (int)s.size(); i++) {
        if (s[i].ID == id) {
            return s[i];
        }
    }
    
    return Station("empty", 0, 0, 0, 0);
}

//
// FindStationNameByName:
//
// Find Station element in vector by station name using linear search.
//
Station FindStationNameByName(vector<Station>& s, string n) {
    for (int i = 0 ; i < (int)s.size(); i++) {
        if (s[i].Name == n) {
            return s[i];
        }
    }
    
    return Station("empty", 0, 0, 0, 0);
}

//
// InputStations:
//
// Inputs the stations from the given file, adding each station name as a
// vertex to the graph, and storing a new Station object into the vector.
// The graph is passed by reference --- note the & --- so that the changes
// made by the function are returned back.  The vector of Station objects is
// returned by the function.
//
vector<Station> InputStations(Graph& G, string filename) {
    string line;
    vector<Station> V;
    
    ifstream input(filename);
    
    // Failed to open:
    if (!input.good()) {
        return V;
    }
    
    // Skip first line: column headers
    getline(input, line);
    
    // First line of real data
    getline(input, line);
    
    
    // For each line of input:
    while (!input.eof()) {
        stringstream  ss(line);
        
        // Format:
        //   id,name,latitude,longitude,dpcapacity,online_date
        //
        // Sample:
        //   456,2112 W Peterson Ave,41.991178,-87.683593,15,5/12/2015
        //   101,63rd St Beach,41.78101637,-87.57611976,23,4/20/2015
        //
        string  stationID, name, latitude, longitude, capacity;
        
        // Parse the line:
        getline(ss, stationID, ',');
        getline(ss, name, ',');
        getline(ss, latitude, ',');
        getline(ss, longitude, ',');
        getline(ss, capacity, ',');
        
        // Create a new station object:
        Station S(name, stoi(stationID), stod(latitude), stod(longitude), stoi(capacity));
        
        // Add station object to vector:
        V.push_back(S);
        
        // Add vertex to graph
        G.AddVertex(name);
        
        getline(input, line);
    }
    
    return V;
}

//
// ProcessTrips:
//
// Inputs the trips, adding / updating the edges in the graph.  The graph is
// passed by reference --- note the & --- so that the changes made by the
// function are returned back.  The vector of stations is needed so that
// station ids can be mapped to names; it is passed by reference only for
// efficiency (so that a copy is not made).
//
void ProcessTrips(string filename, Graph& G, vector<Station>& stations) {
    string line;
    
    ifstream input(filename);
    
    // Failed to open:
    if (!input.good()) {
        return;
    }
    
    // Skip first line: column headers
    getline(input, line);
    
    // First line of real data
    getline(input, line);
    
    
    // For each line of input:
    while (!input.eof()) {
        stringstream  ss(line);
        
        //
        // Format:
        //   trip_id,starttime,stoptime,bikeid,tripduration,from_station_id,
        //   from_station_name,to_station_id,to_station_name,usertype,gender,
        //   birthyear
        //
        // Sample:
        //   10426561,6/30/2016 23:35,7/1/2016 0:02,5229,1620,329,
        //   Lake Shore Dr & Diversey Pkwy,307,Southport Ave & Clybourn Ave,
        //   Subscriber,Male,1968
        //
        string  tripID, fromID, toID, skip;
        
        //
        // parse the input line:
        //
        getline(ss, tripID, ',');
        getline(ss, skip, ',');
        getline(ss, skip, ',');
        getline(ss, skip, ',');
        getline(ss, skip, ',');
        getline(ss, fromID, ',');
        getline(ss, skip, ',');
        getline(ss, toID, ',');
        getline(ss, skip, ',');
        
        //
        // NOTE: don't trust the names in the trips file, not always accurate.  Trust the
        // from and to station ids, and then lookup in our vector of stations:
        //
        
        Station S = FindStationNameById(stations, stoi(fromID));
        Station D = FindStationNameById(stations, stoi(toID));
        
        
        // Add new edge or update existing edge for this trip:
        if (G.IsEdgeExists(S.Name, D.Name)) {
            vector<int> V = G.GetEdgeWeights(S.Name, D.Name);
            G.UpdateEdgeWeight(S.Name, D.Name, V[0], V[0] + 1);
        } else {
            G.AddEdge(S.Name, D.Name, 1);
        }
        
        getline(input, line);
    }
}

//
// GetFileName:
//
// Inputs a filename from the keyboard, checks that the file can be
// opened, and returns the filename if so.  If the file cannot be
// opened, an error message is output and the program is exited.
//
string GetFileName() {
    string filename;
    
    // Input filename from the keyboard:
    getline(cin, filename);
    
    // Make sure filename exists and can be opened:
    ifstream file(filename);
    if (!file.good()) {
        cout << "**Error: unable to open '" << filename << "'" << endl << endl;
        terminate();
    }
    
    return filename;
}

//
// PrintStationInfo:
//
// Print information about specific station, such as: name, coordinates,
// # of neighbor stations, # of trips to this station, and names of all its
// neighbor stations.
//
void PrintStationInfo(Graph& G, vector<Station>& ST, int ID) {
    
    Station S = FindStationNameById(ST, ID);
    if (S.Name == "empty") {
        cout << "** No such station..." << endl;
        return;
    }
    
    cout << S.Name << endl;
    cout << "(" << S.Latitude << "," << S.Longitude <<")" << endl;
    cout << "Capacity: " << S.Capacity << endl;
    set<string> trips = G.GetNeighbors(S.Name);
    vector<string> neighborsOutput;
    string tString;
    int numOfTrips = 0;
    
    for (string t : trips) {
        Station D = FindStationNameByName(ST, t);
        vector<int> W = G.GetEdgeWeights(S.Name, D.Name);
        tString = "  ";
        tString.append(t);
        tString.append(" (");
        tString.append(to_string(D.ID));
        tString.append(") : ");
        tString.append(to_string(W[0]));
        numOfTrips += W[0];
        neighborsOutput.push_back(tString);
    }

    printf("%-29s %d\n", "# of destination stations:", (int)trips.size());
    printf("%-29s %d\n", "# of trips to those stations:", numOfTrips);
    cout << "Station: trips" << endl;
    
    for (string n : neighborsOutput) {
        cout << n << endl;
    }
}

//
// PrintTrips:
//
// Print from station A to station B information, such as station names and
// number of trips from A to B.
//
void PrintTrips(Graph& G, vector<Station>& ST, int FID, int TID) {
    
    Station S = FindStationNameById(ST, FID);
    Station D = FindStationNameById(ST, TID);
    
    if (S.Name == "empty" || D.Name == "empty") {
        cout << "** One of those stations doesn't exist..." << endl;
        return;
    }
    
    vector<int> W = G.GetEdgeWeights(S.Name, D.Name);
    cout << S.Name << " -> " << D.Name << endl;
    cout << "# of trips: ";
    if (W.empty()) {
        cout << "0" << endl;
    } else {
        cout << W[0] << endl;
    }
}

//
// PrintBFS:
//
// Print BFS hraph traversal from requested vertex.
//
void PrintBFS(Graph& G, vector<Station>& ST, int ID) {
    
    Station S = FindStationNameById(ST, ID);
    if (S.Name == "empty") {
        cout << "** No such station..." << endl;
        return;
    }
    
    vector<string> R = G.BFS(S.Name);
    cout << "# of stations: " << R.size() << endl;
    for (string r : R) {
        cout << FindStationNameByName(ST, r).ID << ", ";
    }
    cout << "#" << endl;
}

//
// ProcessUserInput:
//
// Get user inpout and run valid commands.
//
void ProcessUserInput(Graph& DivvyGraph, vector<Station>& stations) {
    
    cout << ">> Graph:" << endl;
    cout << "   # of vertices: " << DivvyGraph.GetNumVertices() << endl;
    cout << "   # of edges:    " << DivvyGraph.GetNumEdges() << endl;
    cout << ">> Ready:" << endl;
    
    string cmd;
    int    fromID, toID;
    
    cout << ">> ";
    cin >> cmd;
    
    while (cmd != "exit") {
        if (cmd == "info") {
            cin >> fromID;
            PrintStationInfo(DivvyGraph, stations, fromID);
        } else if (cmd == "trips") {
            cin >> fromID;
            cin >> toID;
            PrintTrips(DivvyGraph, stations, fromID, toID);
        } else if (cmd == "bfs") {
            cin >> fromID;
            PrintBFS(DivvyGraph, stations, fromID);
        } else if (cmd == "debug") {
            // Print graph:
            DivvyGraph.PrintGraph("Divvy Graph");
        } else {
            cout << "**Invalid command, try again..." << endl;
        }
        
        cout << ">> ";
        cin >> cmd;
    }
}


int main() {
    int    N = 1000;
    Graph  DivvyGraph(N);
    
    cout << "** Divvy Graph Analysis **" << endl;
    
    string stationsFilename = GetFileName();
    string tripsFilename = GetFileName();
    
    vector<Station> stations = InputStations(DivvyGraph, stationsFilename);
    ProcessTrips(tripsFilename, DivvyGraph, stations);
    
    ProcessUserInput(DivvyGraph, stations);

    cout << "**Done**" << endl;
    return 0;
}
