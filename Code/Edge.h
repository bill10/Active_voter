/*
 * Edge.h
 *
 *  Created on: Apr 17, 2011
 *      Author: Bill
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <list>
#include <vector>
class Node;
#include "Node.h"
using namespace std;

class Edge
{
public:
	long int ID;
	bool state; //0-concordant, 1-discordant
    bool active; //0-No, 1-Yes
	vector<Edge>::iterator myself;
	list<vector<Edge>::iterator>::iterator link1; //iterators to where this edge appears in edge lists
	list<vector<Edge>::iterator>::iterator link2; //iterators to where this edge appears in edge lists
	vector<Node>::iterator person1; //iterators to the individuals that this edge joins
	vector<Node>::iterator person2; //iterators to the individuals that this edge joins
	vector<vector<Edge>::iterator>::iterator boundary_place; // iterator to where it is in the list of discordant edges

public:
	Edge();
	virtual ~Edge();
};

#endif /* EDGE_H_ */
