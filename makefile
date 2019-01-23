build:
	g++ divvy_graph_analysis.cpp graph.cpp -o divvy_graph_analysis -std=c++11 -Wall
clean:
	rm divvy_graph_analysis

run:
	clear
	./divvy_graph_analysis