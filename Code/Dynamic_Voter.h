/*
 * Dynamic_Voter.h
 *
 *  Created on: Apr 18, 2011
 *      Author: Administrator
 */

#ifndef DYNAMIC_VOTER_H_
#define DYNAMIC_VOTER_H_

#include "Node.h"
#include "Edge.h"
#include <fstream>
#include <string>
//#include <gsl/gsl_matrix.h>
//#include <gsl/gsl_vector.h>
//#include <bct/bct.h>
#include <igraph.h>

class Dynamic_Voter{
public:
	long int max_edge_ID;
	vector<Node> population; // list of nodes
	vector<Edge> edges; // list of edges
	float degree;
	vector<vector<vector<Node>::iterator> > sites; // list of pointers to i-nodes
	vector<vector<Edge>::iterator> active_edge_boundary; // list of discordant edges
	vector<vector<Edge>::iterator> inactive_edge_boundary; // list of discordant edges
	vector<int> comp; // component ID for each node
	vector<long int> comp_size; // number of nodes in each component
//	igraph is used to calculate graph centralities
//	gsl_matrix* igraph;
	igraph_t igraph;

public:
	Dynamic_Voter(long int number_of_nodes, int number_of_opinions, float p); // Generate Watts-Strogatz network
	Dynamic_Voter(long int number_of_nodes, int degree, int number_of_opinions); // Generate regular graph
	Dynamic_Voter(long int number_of_nodes, long int number_of_edges, int number_of_opinions); // Generate ER network
	virtual ~Dynamic_Voter();
	bool is_neighbor(long int i1, long int i2); // test if nodes i1 and i2 are neighbors
	vector<Edge>::iterator add_edge(long int i1, long int i2); // add an edge between node i1 and i2
	bool swap_delete(vector<Node>::iterator person_it, vector<vector<Node>::iterator> & sites);
	bool swap_delete(vector<Edge>::iterator edge_it, vector<vector<Edge>::iterator> & sites);
	void assign_states(vector<float> &initial_density);
    void activate_edges(float lambda);
    bool delete_edge(vector<Edge>::iterator new_edge);
	double simulate(float alpha, int dt, string process, string comp, string ddist, string edgelist, string opinion);
	void adopt_state(vector<Edge>::iterator edge_it);
	void component(vector<int> &comp, vector<int> &comp_size); // determine the component ID for each node and the size of each component
	void component(); // determine the component ID for each node and the size of each component
	void printEdgelist(string edgelist, string opinion); // print the edge list and node states to file at give time point
	void print_statistics_simple(ofstream &pFile_process); //print simulation statistics at given time point
	void print_statistics_igraph(ofstream &pFile_process); //print simulation statistics at given time point with clustering
	void degree_dist(vector<long int> &d); // calculate the degree distribution of all nodes and of 1-nodes
	float average_path_len(); //calculate average path length of the network
	float clutering(); //calculate clustering coefficient of the network
};

#endif /* DYNAMIC_VOTER_H_ */
