# Cutwidth Minimization Problem (CMP)

The CMP is a NP-Hard problem<sup>1</sup> that consists on finding a linear setup of vertices of a given graph in such a way that we minimize the maximum cutwidth in it. A **cutwidth** is defined as the amount of points intercepted by a imaginary *vertical* line crossing edges between each node in the graph.  An analogy for the problem is trying to have the least amount of wires as possible between eletronic components - in a device that has a linear arrangement of its internal components, meaning only 1 dimension is considered here.

## Example
Note in the example below that we have a lot of outgoing edges from **1** and that the maximum cutwidth is **4**.
<p align="center">
<img src="https://raw.githubusercontent.com/lucas-t-reis/MinimumCutwidth/master/assets/initial-placement.svg">
</p>
One possible solution, and in that case the optimal, is to position the most influential node in the middle and group nodes with one edge between them, as seen below:
<p align="center">
<img src="https://raw.githubusercontent.com/lucas-t-reis/MinimumCutwidth/master/assets/final-placement.svg">
</p>

For a graph of this size it may seem a trivial problem to solve with a program, but as the size scales up one has no option but to rely on Metaheuristics, since that even for pure Bruteforce methods there is a point in which the amount of nodes makes it infeasible to solve with current technology - a.k.a. it takes too long. So a smart way to approach the problem is to aproximate solutions. Hence the need for Metaheuristics, which were **Simulated Annealing** and **GRASP** for this project.

## Problem Definition
- *G* = (*V*,*E*) is a Graph with *V* nodes and *E* edges
- u,v,w are arbitrary edges in *E*
- **CutWidth**(*G*) is the the function that returns the minimum cutwidth for graph *G*
- An Edge 'e' is calculated only if **f**(u) <= **f**(v) < **f**(w)

## CMPLIB2010
The folder CMPLIB2010 contains benchmarks for the problem. They're  graphs with various sizes, undirected and represented by **edges list** - harwell and small - and **adjacency matrices** - grid. Note that some graphs are dense and have many vertices, meaning it will take a considerable longer time to get a solution for them when compared to small or medium graph that is sparse. To mitigate runtime, it is also possible to run in parallel using the OpenMP compilation flag.

### Reference
**1** - Gavril, F. Some NP-complete problems on graphs. In Proceedings of the 11th conference on Information Sciences and Systems, 91-95, 1977.
