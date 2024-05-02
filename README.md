## Porto Navigation with A* Algorithm 
This program implements the A* algorithm to find the shortest path between ports on a map grid. It utilizes two-dimensional vectors to track visited ports and store accumulated costs, along with a priority queue to select the next port to explore. The accumulated fuel cost to the starting point and the estimated value to the destination are calculated using the Manhattan distance function, which calculates the absolute differences of coordinates between points on the horizontal and vertical axes.

The ```A_STAR``` function iterates over all possible pairs of ports, calculating the distance between each pair using the A* algorithm. If a destination is obstructed, it prints a message indicating this. After calculating the distances between all pairs of ports, it sums the distances traveled to obtain the total distance traveled on the map. The runtime of the algorithm is measured using the ```<chrono>``` library, which records the start and end time of the program's execution. Finally, the total execution time is displayed in seconds.


### Compilation
To compile the project, it is necessary to have g++ installed and compile using the following command line in the terminal:

```
g++ ReversedPhoenicians.cpp -o ReversedPhoenicians
./ReversedPhoenicians
```
