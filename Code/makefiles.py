import numpy as np
import subprocess

count=0;
node=10000;
edge=20000;
g=2;
U0=1.0/g*np.ones(g,);
U0[-1]=1-sum(U0[:-1]);
#system('g++ main.cpp Dynamic_Voter.h Dynamic_Voter.cpp Node.cpp Edge.cpp Random1.cpp -O3 -Wall -I/usr/local/include/igraph -L/usr/local/lib -ligraph -o DynamicVoter');
for realization in [1]:
    for alpha in [0.4]:
        for lambda in [0.4]:
            cmd='LD_PRELOAD=/usr/local/lib/libigraph.so.0 ./DynamicVoter -n {} -m {} -a {} -l {} -u {}'.format(node,edge,alpha,lambda,g);
            for i in xrange(g):
                cmd=cmd+' '+str(U0[i]);
            print cmd
            subprocess.call(cmd, shell=True);
            count=count+1;

print str(count)+' files submitted.';
print 'Mission Complete!';
