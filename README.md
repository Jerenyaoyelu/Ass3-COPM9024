% This repository is for the purpose of completing the third assignment of COMP9024 (Data structure and Algorithms).
% All works are written by YAOYE LU (Jeren) From China
=============================================================================================================================================================
% This assignment is asking to implement binomial-heaps based priority queue, and use this ADT to solve a classically practical problem -- TaskScheduler
=============================================================================================================================================================
% Highlight:
    1. Using two priority queues --- one is to store all tasks (PQ1 with key being the release time) and the other one is to store all ready tasks (PQ2 with key being the deadline time) .
        1.1 With the time flies, unready tasks become ready. So when time updates, removeMin from PQ1 and insert to PQ2
    2. Using a third priorit queue (PQ3) to store cores with key being the completion time of last task in this core.
    3. When PQ2 is not empty and Min(PQ3) no larger than time, then removeMin(PQ2), and schedule it.
    4. When merging two BH, add a new function -- adding binomial trees to a binomial heap, and the premise of this function is all binomial trees added to the BH must in a incresing degree.

% More details seen the code.
=============================================================================================================================================================
% Github is Awesome!! Coding is fun!!