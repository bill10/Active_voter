/*
 * main.cpp
 *
 *  Created on: Apr 18, 2011
 *      Author: Bill
 */
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>
#include "Dynamic_Voter.h"
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc, char* argv[]){ // -n number_of_nodes -m number_of_edges -d degree -u number_of_opinions initial_densities -a alpha -l lambda -p rewiring_watts_strogatz
	long int number_of_nodes, number_of_edges;
	double steps;
	float alpha,lambda,p;
	int realization, i,j, degree, number_of_opinions;
	vector<float> initial_density;
	clock_t t_start;
	stringstream filename;
	string s,process,component,ddist,edgelist,opinion;
	size_t start_pos;

//	Initialize parameters
	number_of_nodes=1000;
	number_of_edges=2000;
	degree=4;
	number_of_opinions=2;
	initial_density.push_back(1.0/2);
	initial_density.push_back(1.0/2);
	alpha=0.3;
	lambda=0.5;
	p=0.5;
	if(argc>1){
		for (i = 1; i < argc; i++){
			if (!strcmp(argv[i],"-n")){
				i++;
				number_of_nodes=atol(argv[i]);}
			else if (!strcmp(argv[i],"-m")){
				i++;
				number_of_edges=atol(argv[i]);
				degree=number_of_edges*2/number_of_nodes;}
			else if (!strcmp(argv[i],"-d")){
				i++;
				degree=atoi(argv[i]);
				number_of_edges=number_of_nodes*degree/2;}
			else if (!strcmp(argv[i],"-u")){
				i++;
				number_of_opinions=atoi(argv[i]);
				initial_density.clear();
				for (j=0; j<number_of_opinions; j++){
					i++;
					initial_density.push_back(atof(argv[i]));}}
			else if (!strcmp(argv[i],"-a")){
				i++;
				alpha=atof(argv[i]);}
			else if (!strcmp(argv[i],"-l")){
				i++;
				lambda=atof(argv[i]);}
			else if (!strcmp(argv[i],"-p")){
				i++;
				p=atof(argv[i]);}
			else {
				cout<<"unknown parameter: "<<argv[i]<<endl;
				exit(1);}}}

//	summary of the simulation
	filename<<"n"<<number_of_nodes<<"k"<<degree<<"a"<< setiosflags(ios::fixed) << setprecision(2)<<alpha<<"l"<<lambda<<"u";
	//filename<<"mode"<<mode<<"n"<<number_of_nodes<<"p"<< setiosflags(ios::fixed) << setprecision(2)<<p<<"a"<<alpha<<"g"<<gamma<<"u";
	for (j=0; j<number_of_opinions-1; j++)
		filename<< initial_density[j]<<"_";
	filename<< initial_density[j]<<".summary";

//	test if the file already exists
	ifstream testfile(filename.str().c_str());
	i=1;
	while (testfile.is_open()) {
		i++;
		filename.str("");
		filename<<"n"<<number_of_nodes<<"k"<<degree<<"a"<<alpha<<"l"<<lambda<<"u";
		for (j=0; j<number_of_opinions-1; j++)
			filename <<initial_density[j] <<"_";
		filename<<initial_density[j] <<"("<< i <<").summary";
		testfile.close();
		testfile.open(filename.str().c_str());
	}
	ofstream outfile(filename.str().c_str());
	outfile<<"realization "<<"alpha "<<"lambda ";
	for (i=0; i<number_of_opinions; i++)
		outfile<<"u"<<i<<" ";
	outfile<< "steps " << "time " << "discordant_edges " << "number_of_components "<<"largest_comp_size "<<"average_path_len "<<"clustering"<< endl;

// create file to save the frequencies of opinions throughout the process
	s="summary";
	process=filename.str();
	start_pos = process.find(s);
	process.replace(start_pos, s.length(), "process");
//	create file to save the component sizes throughout the process
	component=filename.str();
	component.replace(start_pos, s.length(), "component");
//	create file to save the degree distributions throughout the process
	ddist=filename.str();
	ddist.replace(start_pos, s.length(), "ddist");
//	create file to save the edgelist in the end
	edgelist=filename.str();
	edgelist.replace(start_pos, s.length(), "edgelist");
//	create file to save node opinions in the end
	opinion=filename.str();
	opinion.replace(start_pos, s.length(), "opinion");

	for(realization=1;realization<=1;realization++){
		t_start=clock();
		//Dynamic_Voter *dv = new Dynamic_Voter(number_of_nodes,degree,number_of_opinions);
		Dynamic_Voter *dv = new Dynamic_Voter(number_of_nodes,number_of_edges,number_of_opinions);
		//Dynamic_Voter *dv = new Dynamic_Voter(number_of_nodes,number_of_opinions,p);
		dv->assign_states(initial_density);
        dv->activate_edges(lambda);
		//steps = dv->simulate(alpha,beta,mode,gamma,process,component,ddist);
		steps = dv->simulate(alpha,100,process,"","","","");
		dv->component();
		outfile <<realization<<" "<< alpha <<" "<<lambda<<" ";
		for (i=0; i<number_of_opinions; i++)
			outfile << dv->sites[i].size()/(float)number_of_nodes <<" ";
		outfile << steps <<" "<< (float)(clock()-t_start)/CLOCKS_PER_SEC <<" "<< dv->active_edge_boundary.size()<<" "<< dv->inactive_edge_boundary.size()<<" "<<dv->comp_size.size()-1<<" "<<*max_element(dv->comp_size.begin(),dv->comp_size.end())<<" "<<dv->average_path_len()<<" "<<dv->clutering()<< endl;
		delete dv;
	}

	outfile.close();
	return 0;
}
