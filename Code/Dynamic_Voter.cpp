/*
 * Dynamic_Voter.cpp
 *
 *  Created on: Apr 18, 2011
 *      Author: Bill
 */

#include "Dynamic_Voter.h"
#include "global_variables.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <math.h>
#include <algorithm>

using namespace std;

//Generate a regular random graph of the given degree
Dynamic_Voter::Dynamic_Voter(long int number_of_nodes, int degree, int number_of_opinions)
: max_edge_ID(0), population(number_of_nodes), sites(number_of_opinions) {
	bool need_new_neigh,need_new_edge;
	long int i,j,k,i1,i2,p,q,r,s,u,v,e1;
	long int number_of_edges=number_of_nodes*degree/2;
	vector<Node>::iterator person_it;
	vector<Edge>::iterator new_edge;
	vector<long int> nodequeue(number_of_nodes,0); // all the nodes of degree less than d
	edges.reserve(number_of_edges);
	for(i=0;i<number_of_opinions;i++)
		sites[i].reserve(number_of_nodes);
	active_edge_boundary.reserve(number_of_edges);
    inactive_edge_boundary.reserve(number_of_edges);

	for(person_it=population.begin(),j=0; person_it!=population.end(); person_it++,j++)
	{
		person_it->ID=j;
		person_it->state=0;
		person_it->myself=person_it;
	}

	for (i=0; i<number_of_nodes; i++)
		nodequeue[i] = i;
	if ((degree % 2) != 0)
		if ((number_of_nodes % 2) != 0)
		{
			cout<<"if degree is odd then number_of_nodes must be even"<<endl;
			return;
		}
	if (number_of_nodes <= degree)
	{
		cout<<"number_of_nodes should be greater than degree"<<endl;
		return;
	}

	while (nodequeue.empty()==false)
	{
		u=-1;
		v=-1;
		if (nodequeue.size()==1)
		{
			// node r has less than the required degree,
			// find two adjacent nodes p and q non-adjacent to r.
			r=nodequeue[0];
			need_new_edge=true;
			while (need_new_edge)
			{
				e1 = random_number.integer(edges.size());
				new_edge = edges[e1].myself;
				need_new_edge = (new_edge->person1->ID==r || new_edge->person2->ID==r);
				if (need_new_edge==false)
				{
					p = new_edge->person1->ID;
					q = new_edge->person2->ID;
					need_new_edge = is_neighbor(p, r) || is_neighbor(q, r);
				}
			}
			delete_edge(new_edge);
			add_edge(p, r);
			add_edge(q, r);
			if ((int)population[r].edge_list.size()>=degree)
			{
				nodequeue.pop_back();
			}
		}

		// find two non-adjacent nodes each has less than required degree
		if ((int)nodequeue.size()>degree)
		{
			need_new_neigh=true;
			while (need_new_neigh)
			{
				i1=random_number.integer(nodequeue.size());
				i2 = i1;
				while (i2 == i1)
				{
					i2=random_number.integer(nodequeue.size());
				}
				need_new_neigh=is_neighbor(nodequeue[i1],nodequeue[i2]);
			}
			add_edge(nodequeue[i1],nodequeue[i2]); // add this edge
			if ((int)population[nodequeue[i1]].edge_list.size()>=degree && (int)population[nodequeue[i2]].edge_list.size()>=degree)
			{
				if (i1==(int)nodequeue.size()-1)
				{
					nodequeue.pop_back();
					nodequeue[i2]=nodequeue.back();
					nodequeue.pop_back();
				}
				else
				{
					nodequeue[i2]=nodequeue.back();
					nodequeue.pop_back();
					nodequeue[i1]=nodequeue.back();
					nodequeue.pop_back();
				}
			}
			else if((int)population[nodequeue[i1]].edge_list.size()>=degree)
			{
				nodequeue[i1]=nodequeue.back();
				nodequeue.pop_back();
			}
			else if ((int)population[nodequeue[i2]].edge_list.size()>=degree)
			{
				nodequeue[i2]=nodequeue.back();
				nodequeue.pop_back();
			}
		}
		else
		{
			for (i=0; i<(int)nodequeue.size(); i++)  // randomly permute node queue
			{
				j = (int)(i + random_number.real() * (nodequeue.size() - i));
				k = nodequeue[i];
				nodequeue[i] = nodequeue[j];
				nodequeue[j] = k;
			}
			for (i1=0; i1<(int)nodequeue.size()-1; i1++)
			{
				for (i2=i1+1; i2<(int)nodequeue.size(); i2++)
				{
					if (!is_neighbor(nodequeue[i1],nodequeue[i2]))
					{
						add_edge(nodequeue[i1],nodequeue[i2]);  // add this edge
						if ((int)population[nodequeue[i1]].edge_list.size()>=degree && (int)population[nodequeue[i2]].edge_list.size()>=degree)
						{
							if (i1==(int)nodequeue.size()-1)
							{
								nodequeue.pop_back();
								nodequeue[i2]=nodequeue.back();
								nodequeue.pop_back();
							}
							else
							{
								nodequeue[i2]=nodequeue.back();
								nodequeue.pop_back();
								nodequeue[i1]=nodequeue.back();
								nodequeue.pop_back();
							}
						}
						else if((int)population[nodequeue[i1]].edge_list.size()>=degree)
						{
							nodequeue[i1]=nodequeue.back();
							nodequeue.pop_back();
						}
						else if ((int)population[nodequeue[i2]].edge_list.size()>=degree)
						{
							nodequeue[i2]=nodequeue.back();
							nodequeue.pop_back();
						}
						v=-1;
						break;
					}
					else
					{
						u=i1;
						v=i2;
						r = nodequeue[i1];
						s = nodequeue[i2];
					}
				}
				if (v==-1)
					break;
			}
		}
		if (v!=-1)
		{
			// nodes r and s of less than required degree, find two
			// adjacent nodes p & q such that (p,r) & (q,s) are not edges.
			need_new_edge=true;
			while (need_new_edge)
			{
				e1 = random_number.integer(edges.size());
				new_edge = edges[e1].myself;
				need_new_edge = (new_edge->person1->ID==r || new_edge->person2->ID==r || new_edge->person1->ID==s || new_edge->person2->ID==s);
				if (need_new_edge==false)
				{
					p = new_edge->person1->ID;
					q = new_edge->person2->ID;
					need_new_edge = is_neighbor(p, r) || is_neighbor(q, s);
					if (need_new_edge==true)
					{
						q = new_edge->person1->ID;
						p = new_edge->person2->ID;
						need_new_edge = is_neighbor(p, r) || is_neighbor(q, s);
					}
				}
			}
			delete_edge(new_edge);
			add_edge(p, r);
			if((int)population[r].edge_list.size()>=degree)
			{
				nodequeue[u]=nodequeue.back();
				nodequeue.pop_back();
			}
			add_edge(q, s);
			if((int)population[s].edge_list.size()>=degree)
			{
				nodequeue[v]=nodequeue.back();
				nodequeue.pop_back();
			}
		}
	}
	return;
}

//Generate a Watts Strogaz network
Dynamic_Voter::Dynamic_Voter(long int number_of_nodes, int number_of_opinions, float p)
: max_edge_ID(0), population(number_of_nodes),sites(number_of_opinions) {
	long int j,number_of_edges;
	int i;
	vector<Node>::iterator person_it;
	igraph_eit_t eit;
	igraph_integer_t from, to;
	degree=4.0;
	for(i=0;i<number_of_opinions;i++)
		sites[i].reserve(number_of_nodes);

	igraph_watts_strogatz_game(&igraph, 1, number_of_nodes, 2, p, 0, 0);
	igraph_simplify(&igraph,1,1,0);
	number_of_edges=(long int)igraph_ecount(&igraph);
	edges.reserve(number_of_edges);
	active_edge_boundary.reserve(number_of_edges);
	inactive_edge_boundary.reserve(number_of_edges);

	for(person_it=population.begin(),j=0; person_it!=population.end(); person_it++,j++){
		person_it->ID=j;
		person_it->state=0;
		person_it->myself=person_it;}

	igraph_eit_create(&igraph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);
	while (!IGRAPH_EIT_END(eit)) {
		igraph_edge(&igraph, IGRAPH_EIT_GET(eit), &from, &to);
		add_edge((long int)from, (long int)to);
		IGRAPH_EIT_NEXT(eit); }

	igraph_eit_destroy(&eit);
}

//Generate a ER random graph with given number of nodes and edges
Dynamic_Voter::Dynamic_Voter(long int number_of_nodes, long int number_of_edges, int number_of_opinions)
: max_edge_ID(0), population(number_of_nodes),sites(number_of_opinions) {
	bool need_new_neigh;
	long int j,i1,i2;
	int i;
	vector<Node>::iterator person_it;
	degree=2.0*number_of_edges/number_of_nodes;
	edges.reserve(number_of_edges);
	for(i=0;i<number_of_opinions;i++)
		sites[i].reserve(number_of_nodes);
	active_edge_boundary.reserve(number_of_edges);
	inactive_edge_boundary.reserve(number_of_edges);
	// igraph = gsl_matrix_calloc(number_of_nodes, number_of_nodes);
	// igraph_vector_t edgelist;
	// igraph_vector_init(&edgelist,2*number_of_edges);

	for(person_it=population.begin(),j=0; person_it!=population.end(); person_it++,j++){
		person_it->ID=j;
		person_it->state=0;
		person_it->myself=person_it;}

	for(j=0; j<number_of_edges; j++){
		// choose two nodes
		need_new_neigh=true;
		while (need_new_neigh){
			i1=random_number.integer(number_of_nodes);
			i2 = i1;
			while (i2 == i1){
				i2=random_number.integer(number_of_nodes);}
			need_new_neigh=is_neighbor(i1,i2);}

		add_edge(i1, i2);  // add this edge
		// VECTOR(edgelist)[2*j]=i1;
		// VECTOR(edgelist)[2*j+1]=i2;
		// gsl_matrix_set(igraph,i1,i2,1);
		// gsl_matrix_set(igraph,i2,i1,1);
	}
	// igraph_create(&igraph, &edgelist, number_of_nodes, IGRAPH_UNDIRECTED);
	// igraph_vector_destroy(&edgelist);

	return;
}

Dynamic_Voter::~Dynamic_Voter() {
	// TODO Auto-generated destructor stub
}

bool Dynamic_Voter::is_neighbor(long int i1, long int i2) {
	list<vector<Edge>::iterator>::iterator neigh_edge_it;

	for (neigh_edge_it=population[i1].edge_list.begin(); neigh_edge_it!=population[i1].edge_list.end(); neigh_edge_it++)
	{
		if ((*neigh_edge_it)->person1->ID == i2)
			return true;
		if ((*neigh_edge_it)->person2->ID == i2)
			return true;
	}
	return false;
}

vector<Edge>::iterator Dynamic_Voter::add_edge(long int i1, long int i2) {
	Edge new_edge;
    vector<Edge>::iterator edge_it;
	new_edge.ID = max_edge_ID++;
	edge_it = edges.insert(edges.end(), new_edge);
	edge_it->person1 = population[i1].myself;
	edge_it->person2 = population[i2].myself;
	edge_it->link1 = population[i1].edge_list.insert(population[i1].edge_list.begin(), edge_it);
	edge_it->link2 = population[i2].edge_list.insert(population[i2].edge_list.begin(), edge_it);
	edge_it->myself=edge_it;
	edge_it->boundary_place = NULL_EDGE_BOUNDARY_ITERATOR;
    edge_it->state=0;
    edge_it->active=0;

    return edge_it;
}

void Dynamic_Voter::assign_states(vector<float> &initial_density) {
	vector<Node>::iterator person_it;
	double u;
	int i;
	vector<float> cmf;
	cmf.reserve(initial_density.size()+1);
	cmf.push_back(0);

	for (i=0;i<(int)initial_density.size();i++) {
		cmf.push_back(initial_density[i]+cmf[i]);
	}

	for (person_it = population.begin(); person_it != population.end(); person_it++) {
		u=random_number.real();
		for (i=0;i<(int)cmf.size()-1;i++)
			if (u > cmf[i] && u<=cmf[i+1]) {
				person_it->state = i;
				person_it->sites_place = (sites[i]).insert((sites[i]).end(),person_it);
				break;
			}
	}
	return;
}

void Dynamic_Voter::activate_edges(float lambda) {
    vector<Edge>::iterator edge_it;

    for (edge_it = edges.begin(); edge_it != edges.end(); edge_it++) {
        if (random_number.real()<=lambda) {
            edge_it->active=1;}

		if (edge_it->person1->state != edge_it->person2->state) {
			edge_it->state=1;
            if (edge_it->active==1) {
			    edge_it->boundary_place=active_edge_boundary.insert(active_edge_boundary.end(),edge_it);}
            else {
                edge_it->boundary_place=inactive_edge_boundary.insert(inactive_edge_boundary.end(),edge_it);}}
        else {
    		edge_it->state=0;
    		edge_it->boundary_place = NULL_EDGE_BOUNDARY_ITERATOR;}}

    return;
}

// swap the node pointer with the last node pointer in the list and remove it
bool Dynamic_Voter::swap_delete(vector<Node>::iterator person_it, vector<vector<Node>::iterator> & site) {
	if(person_it->sites_place == NULL_SITES_ITERATOR)
		return false;
	if (site.size()==1 || person_it == site.back())
		;
	else {
		*(person_it->sites_place) = site.back();
		(*site.back()).sites_place = person_it->sites_place;
	}
	site.pop_back();
	person_it->sites_place = NULL_SITES_ITERATOR;
	return true;
}

// swap the edge pointer with the last edge pointer in the list and remove it
bool Dynamic_Voter::swap_delete(vector<Edge>::iterator edge_it, vector<vector<Edge>::iterator> & site) {
	if (edge_it->boundary_place == NULL_EDGE_BOUNDARY_ITERATOR)
		return false;
	if (site.size()==1 || edge_it == site.back())
		;
	else {
		*(edge_it->boundary_place) = site.back();
		(*site.back()).boundary_place = edge_it->boundary_place;}
	site.pop_back();
	edge_it->boundary_place=NULL_EDGE_BOUNDARY_ITERATOR;
	return true;
}

bool Dynamic_Voter::delete_edge(vector<Edge>::iterator new_edge){
    return true;
}

double Dynamic_Voter::simulate(float alpha, int dt, string process, string comp, string ddist, string edgelist, string opinion) {
	long int e1,e2;
	double step=0;
	int j;
    dt=1000;
	vector<Edge>::iterator edge_it,edge_it2;
	vector<long int>::iterator comp_it;
	vector<long int> degreedist;
	int action=-1; //0: opinion change, 1: deavtivation
	ofstream pFile_process, pFile_comp, pFile_degree;

	if (!process.empty()) {
		pFile_process.open(process.c_str());
		pFile_process<<"step action ";
		for (j=0; j< (int)sites.size(); j++)
			pFile_process<<"N"<<j<<" ";
		pFile_process<<"N_neq_active N_neq_inactive";
		pFile_process<<endl;
		pFile_process<<step<<" "<<action<<" ";
		print_statistics_simple(pFile_process);}

	if (!comp.empty()) {
		pFile_comp.open(comp.c_str());
		component();
		for(comp_it=comp_size.begin()+1;comp_it!=comp_size.end();comp_it++)
			pFile_comp<<*comp_it<<" ";
		pFile_comp<<endl;}

	if (!ddist.empty()) {
		pFile_degree.open(ddist.c_str());
		degree_dist(degreedist);
		for(comp_it=degreedist.begin();comp_it!=degreedist.end();comp_it++)
			pFile_degree<<*comp_it<<" ";
		pFile_degree<<endl;}

	while (active_edge_boundary.empty() == false) {
		e1 = random_number.integer(active_edge_boundary.size());
		edge_it=active_edge_boundary[e1];
		if (random_number.real()<alpha) {
            action=1;
            edge_it->active=0;
            swap_delete(edge_it, active_edge_boundary);
            if (inactive_edge_boundary.empty() == false) {
                e2 = random_number.integer(inactive_edge_boundary.size());
                edge_it2=inactive_edge_boundary[e2];
                swap_delete(edge_it2, inactive_edge_boundary);
                edge_it2->active=1;
                edge_it2->boundary_place=active_edge_boundary.insert(active_edge_boundary.end(),edge_it);}
            edge_it->boundary_place=inactive_edge_boundary.insert(inactive_edge_boundary.end(),edge_it);}
		else {
            action=0;
			adopt_state(edge_it);}
		step++;

		if ((long int)step%dt==0){
			pFile_process<<step<<" "<<action<<" ";
			print_statistics_simple(pFile_process);
			// component();
			// for(comp_it=comp_size.begin()+1;comp_it!=comp_size.end();comp_it++)
			// {
			// 	pFile_comp<<*comp_it<<" ";
			// }
			// pFile_comp<<endl;
			// degree_dist(degreedist);
			// for(comp_it=degreedist.begin();comp_it!=degreedist.end();comp_it++)
			// {
			// 	pFile_degree<<*comp_it<<" ";
			// }
			// pFile_degree<<endl;
			// printEdgelist();
		}
	}

	if (pFile_process.is_open()) {
		pFile_process<<step<<" "<<action<<" ";
		print_statistics_simple(pFile_process);
		pFile_process.close(); }

	if (pFile_comp.is_open()) {
		component();
		for(comp_it=comp_size.begin()+1;comp_it!=comp_size.end();comp_it++)
			pFile_comp<<*comp_it<<" ";
		pFile_comp<<endl;
		pFile_comp.close();}

	if (pFile_degree.is_open()) {
		degree_dist(degreedist);
		for(comp_it=degreedist.begin();comp_it!=degreedist.end();comp_it++)
			pFile_degree<<*comp_it<<" ";
		pFile_degree<<endl;
		pFile_degree.close();}

	if (!edgelist.empty())
		printEdgelist(edgelist,opinion);

	return step;
}

void Dynamic_Voter::adopt_state(vector<Edge>::iterator edge_it) {
	vector<Node>::iterator person1_it, person2_it, person3_it;
	list<vector<Edge>::iterator>::iterator neigh_edge_it;

	person1_it = edge_it->person1;
	person2_it = edge_it->person2;
	// swap roles of the two people
	if (random_number.real() < 0.5) {
		person1_it = edge_it->person2;
		person2_it = edge_it->person1;}

	// person 1 adopts the state of person 2.
	swap_delete(person1_it, sites[person1_it->state]);
	person1_it->sites_place = sites[person2_it->state].insert(sites[person2_it->state].end(),person1_it);
	person1_it->state = person2_it->state;

	// need to go through all of person 1's neighbors:
	// each edge that was concordant is now discordant and vice-versa
	for (neigh_edge_it = person1_it->edge_list.begin(); neigh_edge_it!=person1_it->edge_list.end(); neigh_edge_it++) {
		edge_it = *neigh_edge_it;
		if (edge_it->state==0) { // "0" means that this edge was concordant
			edge_it->state=1;
            if (edge_it->active==1)
			    edge_it->boundary_place=active_edge_boundary.insert(active_edge_boundary.end(),edge_it);
            else
                edge_it->boundary_place=inactive_edge_boundary.insert(inactive_edge_boundary.end(),edge_it); }
		else { // edge was discordant, it may be concordant now
			if (edge_it->person1 == person1_it)
				person3_it=edge_it->person2;
			else
				person3_it=edge_it->person1;
			if (person1_it->state == person3_it->state){
				edge_it->state=0;
                if (edge_it->active==1)
				    swap_delete(edge_it, active_edge_boundary);
                else
                    swap_delete(edge_it, inactive_edge_boundary);
            }
        }
    }
	return;
}

void Dynamic_Voter::component(vector<int> &comp, vector<int> &comp_size)
{
	queue<int> que;
	vector<Node>::iterator person_it;
	vector<Edge>::iterator edge_it;
	list<vector<Edge>::iterator>::iterator neigh_edge_it;
	long int num_comp=0, n1, n2;

	comp_size.push_back(0);
	for (person_it = population.begin(); person_it != population.end(); person_it++){
		if(comp[person_it->ID]==0){
			que.push(person_it->ID);
			while(que.empty()==false){
				n1=que.front();
				que.pop();
				if(comp[n1]==0){
					comp[n1]=++num_comp;
					comp_size.push_back(1);}
				for (neigh_edge_it = population[n1].edge_list.begin(); neigh_edge_it!=population[n1].edge_list.end(); neigh_edge_it++){
					edge_it = *neigh_edge_it;
					n2=edge_it->person1->ID;
					if(n2==n1)
						n2=edge_it->person2->ID;
					if(comp[n2]==0){
						comp[n2]=comp[n1];
						que.push(n2);
						(comp_size[num_comp])++;}}}}}
}

void Dynamic_Voter::component() {
	queue<long int> que;
	vector<Node>::iterator person_it;
	vector<Edge>::iterator edge_it;
	list<vector<Edge>::iterator>::iterator neigh_edge_it;
	long int num_comp=0, n1, n2;
	comp.assign(population.size(),0);
	comp_size.clear();
	comp_size.reserve((int)(population.size()/log((float)population.size())));
	comp_size.push_back(0);

	for (person_it = population.begin(); person_it != population.end(); person_it++) {
		if(comp[person_it->ID]==0) {
			que.push(person_it->ID);
			while(que.empty()==false) {
				n1=que.front();
				que.pop();
				if(comp[n1]==0) {
					comp[n1]=++num_comp;
					comp_size.push_back(1);
                }
				for (neigh_edge_it = population[n1].edge_list.begin(); neigh_edge_it!=population[n1].edge_list.end(); neigh_edge_it++){
					edge_it = *neigh_edge_it;
					n2=edge_it->person1->ID;
					if(n2==n1)
						n2=edge_it->person2->ID;
					if(comp[n2]==0){
						comp[n2]=comp[n1];
						que.push(n2);
						(comp_size[num_comp])++;
					}
                }
            }
        }
    }
}

void Dynamic_Voter::printEdgelist(string edgelist, string opinion) {
	vector<Edge>::iterator edge_it;
	vector<Node>::iterator person_it;
	ofstream pFile_edgelist;
	ofstream pFile_opinion;

	pFile_edgelist.open(edgelist.c_str(),ios_base::app);
	for (edge_it = edges.begin(); edge_it != edges.end(); edge_it++) {
		pFile_edgelist<<edge_it->person1->ID<<" "<<edge_it->person2->ID<<endl; }
	pFile_edgelist.close();

	pFile_opinion.open(opinion.c_str(),ios_base::app);
	for (person_it = population.begin(); person_it!=population.end(); person_it++) {
		pFile_opinion << person_it->state <<endl; }
	pFile_opinion.close();
}

void Dynamic_Voter::print_statistics_simple(ofstream &pFile_process) {
	int j;
	for (j=0; j<(int)sites.size(); j++)
		pFile_process<<sites[j].size()<<" ";
	pFile_process<<active_edge_boundary.size()<<" "<<inactive_edge_boundary.size()<<endl;
}

void Dynamic_Voter::print_statistics_igraph(ofstream &pFile_process) {
	igraph_real_t avg_path;
	igraph_real_t clustering;
	int j;

	igraph_average_path_length(&igraph, &avg_path, IGRAPH_UNDIRECTED, 1);
	igraph_transitivity_undirected(&igraph, &clustering, IGRAPH_TRANSITIVITY_ZERO);

	for (j=0; j<(int)sites.size(); j++)
		pFile_process<<sites[j].size()<<" ";
	pFile_process<<active_edge_boundary.size()<<" "<<(float)avg_path<<" "<<(float)clustering<<endl;}

void Dynamic_Voter::degree_dist(vector<long int> &d) {
	vector<Node>::iterator person_it;
	d.clear();
	d.reserve(population.size());

	for (person_it = population.begin(); person_it != population.end(); person_it++) {
		if(d.size()<=person_it->edge_list.size()) {
			d.resize(person_it->edge_list.size()+1);}
		d[(person_it->edge_list.size())]++;}

    return;
}

float Dynamic_Voter::average_path_len() {
	igraph_real_t avg_path;
	igraph_vector_t edgelist;
	igraph_vector_init(&edgelist,2*(edges.size()));
	vector<Edge>::iterator edge_it;
	int j;

	for(edge_it=edges.begin(),j=0; edge_it!=edges.end(); edge_it++,j++) {
		VECTOR(edgelist)[2*j]=edge_it->person1->ID;
		VECTOR(edgelist)[2*j+1]=edge_it->person2->ID; }

	igraph_create(&igraph, &edgelist, population.size(), IGRAPH_UNDIRECTED);
	igraph_vector_destroy(&edgelist);

	igraph_average_path_length(&igraph, &avg_path, IGRAPH_UNDIRECTED, 1);
	return (float)avg_path;
}

float Dynamic_Voter::clutering() {
	igraph_real_t c;
	igraph_vector_t edgelist;
	igraph_vector_init(&edgelist,2*(edges.size()));
	vector<Edge>::iterator edge_it;
	int j;

	for(edge_it=edges.begin(),j=0; edge_it!=edges.end(); edge_it++,j++) {
		VECTOR(edgelist)[2*j]=edge_it->person1->ID;
		VECTOR(edgelist)[2*j+1]=edge_it->person2->ID; }

	igraph_create(&igraph, &edgelist, population.size(), IGRAPH_UNDIRECTED);
	igraph_vector_destroy(&edgelist);

	igraph_transitivity_undirected(&igraph, &c, IGRAPH_TRANSITIVITY_ZERO);
	return (float)c;
}
