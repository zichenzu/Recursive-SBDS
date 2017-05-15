# Recursive-SBDS
Recursive SBDS

This is the implementation of the paper "Jimmy H.M. Lee and Zichen Zhu.  Boosting SBDS for Partial Symmetry Breaking in Constraint Programming, Proceedings of the 28th AAAI Conference on Artificial Intelligence (AAAI 2014), pages 2695-2702, Quebec City, Quebec, Canada, July, 2014"

Please use Gecode Solver 4.2.0 to run these files.

Note that we set opt.c_d(1) in our experiment, please do not change this.

Put ReSBDS folder into gecode folder and queens_resbds.cpp file into example folder.

To run the N-Queens problem (e.g. N=10) using one of the symmetry breaking methods SBDS/ParSBDS/ReSBDS/LDSB:

./queens_resbds -search sbds 10

./queens_resbds -search parsbds 10

./queens_resbds -search resbds 10

./queens_resbds -search ldsb 10
 
