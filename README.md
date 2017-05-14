# Recursive-SBDS
Recursive SBDS

This is the implementation of the paper "J. Lee, Z. Zhu, Boosting SBDS for partial symmetry breaking in constraint programming, in: Proceeding of the 28th AAAI Conference on Artificial Intelligence, 2014, pp. 2695–2702."

Please use Gecode Solver 4.2.0 to run these files.
Put all hpp files into gecode/mytest.
Note that we set opt.c_d(1) in our experiment, please do not change this.


To run the N-Queens problem (e.g. N=10) using one of the symmetry breaking methods SBDS/ParSBDS/ReSBDS/LDSB:

./queens -search sbds 10
./queens -search parsbds 10
./queens -search resbds 10
./queens -search ldsb 10
 
