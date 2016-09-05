/*
 * Random.cpp
 *
 *  Created on: Apr 18, 2011
 *      Author: Administrator
 */

#include "Random.h"
#include <sys/time.h>

long int Random::idum=0;

Random::Random()
{
	struct timeval timetest;
	struct timezone tz;
	float v;
	if (idum == 0)
	{
		idum = gettimeofday(&timetest, &tz);
		idum = timetest.tv_usec;
	}
	v = ran3_(&idum);
}

Random::~Random() {
	// TODO Auto-generated destructor stub
}

float Random::ran3_(long *idum)
{
	/* (C) Copr. 1986-92 Numerical Recipes Software %6<5@)1.#109. */
	static int inext,inextp;
	static long ma[56];
	static int iff=0;
	long mj,mk;
	int i,ii,k;
	float val;

	mj=0;
	while ((val=mj*FAC)==0) {
	if (*idum < 0 || iff == 0) {
		iff=1;
		mj=MSEED-(*idum < 0 ? -*idum : *idum);
		mj %= MBIG;
		ma[55]=mj;
		mk=1;
		for (i=1;i<=54;i++) {
			ii=(21*i) % 55;
			ma[ii]=mk;
			mk=mj-mk;
			if (mk < MZ) mk += MBIG;
			mj=ma[ii];
		}
		for (k=1;k<=4;k++)
			for (i=1;i<=55;i++) {
				ma[i] -= ma[1+(i+30) % 55];
				if (ma[i] < MZ) ma[i] += MBIG;
			}
		inext=0;
		inextp=31;
		*idum=1;
	}
	if (++inext == 56) inext=1;
	if (++inextp == 56) inextp=1;
	mj=ma[inext]-ma[inextp];
	if (mj < MZ) mj += MBIG;
	ma[inext]=mj;
	}

	return val;
}

long int Random::integer(long int n) // generate a integer within 0,...,n-1 at random
{
	long int i1;
	while ((i1=(int)(ran3_(&idum)*n))==n)
		;
	return i1;
}

float Random::real() // generate a real number in [0,1]
{
	return ran3_(&idum);
}
