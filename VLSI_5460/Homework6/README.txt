Calvin Passmore
A02107892
ECE 5460

1. The purpose of the three stages is to take advantage of each of the individual strengths of the various routing algorithms.
    a. The first initial routing is to get a good starting position, but this will be overridden.
    b. The second stage, rip up and re-route, tries to find the best order to put the net routing. This part will take the input from the previous stage and try to improve it while being able to take some 'bad' moves.
    c. The third stage, greedyImprovement, is to get any last improvements that can be obtained. This part takes the route from the last part and makes as many positive changes it can.

2. SimpleGr makes a framework for the 3 stages of routing by providing convenience functions (hiding complexity) and passing the data back to the other stages.