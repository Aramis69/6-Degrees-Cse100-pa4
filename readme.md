File:readme.md

1)You can verify a undirected graph is connected by applying the DFS algorithm
to a node you have and marking a node as seen whenever a new node is encountered. when
done looking through al the connected components you can keep a count to show 
we have found one connected component. we can try to do DFS again
on a node that had not been marked as seen to repeat the process.
if we end up with a count of 1 we know all our nodes are connected.
The runtime for this algorithm is O(V+E)

2)start by running DFS on a node if it had not been marked as visited.
if a node has not been visited then mark ask visited and add to a count.
do bfs for each of this nodes neighbors. Once it is done going through all
neighbors and neighbors neighbors. save count and reset. The final count will
tell you how many connected nodes there is for this connected component. if it
is not equal to the total number of nodes we keep a connected component count 
and repeat the search adding to out connected component count when needed.
The runtime for this algorithm is O(V+E)

3) We can use BFS algorithm to go from our starting actor to kevin bacon.
BFS will give us the shortest path by disregarding weights. By going
level by level and terminating the search when kevin bacon is encountered
we can easily determine if the level where we encounter kevin bacon is 6 or 
under. The runtime of this algirithm is O(V+E)


