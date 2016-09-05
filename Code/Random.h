/*
 * Random.h
 *
 *  Created on: Apr 18, 2011
 *      Author: Administrator
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

class Random
{
private:
	static long int idum;
public:
	Random();
	virtual ~Random();
	float ran3_(long *idum);
	long int integer(long int n);
	float real();
};

#endif /* RANDOM_H_ */
