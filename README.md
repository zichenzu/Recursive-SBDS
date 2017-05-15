# Recursive-SBDS
Recursive SBDS

This is the implementation of the paper "J. Lee, Z. Zhu, Boosting SBDS for partial symmetry breaking in constraint programming, in: Proceeding of the 28th AAAI Conference on Artificial Intelligence, 2014, pp. 2695–2702."

Please use Gecode Solver 4.2.0 to run these files.

Note that we set opt.c_d(1) in our experiment, please do not change this.

Put ReSBDS folder into gecode folder and queens_resbds.cpp file into example folder.

To run the N-Queens problem (e.g. N=10) using one of the symmetry breaking methods SBDS/ParSBDS/ReSBDS/LDSB:

./queens_resbds -search sbds 10
./queens_resbds -search parsbds 10
./queens_resbds -search resbds 10
./queens_resbds -search ldsb 10
 
