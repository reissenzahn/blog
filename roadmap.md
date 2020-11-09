
# Introduction to Algorithms

https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-006-introduction-to-algorithms-fall-2011/

Lecture 1: Algorithmic Thinking, Peak Finding
Lecture 2: Models of Computation, Document Distance
Lecture 3: Insertion Sort, Merge Sort
Lecture 4: Heaps and Heap Sort
Lecture 5: Binary Search Trees, BST Sort
Lecture 6: AVL Trees, AVL Sort
Lecture 7: Counting Sort, Radix Sort, Lower Bounds for Sorting
Lecture 8: Hashing with Chaining
Lecture 9: Table Doubling, Karp-Rabin
Lecture 10: Open Addressing, Cryptographic Hashing
Lecture 11: Integer Arithmetic, Karatsuba Multiplication
Lecture 12: Square Roots, Newton's Method
Lecture 14: Depth-First Search (DFS), Topological Sort
Lecture 15: Single-Source Shortest Paths Problem
Lecture 16: Dijkstra
Lecture 17: Bellman-Ford
Lecture 18: Speeding up Dijkstra
Lecture 19: Dynamic Programming I: Fibonacci, Shortest Paths
Lecture 20: Dynamic Programming II: Text Justification, Blackjack
Lecture 21: Dynamic Programming III: Parenthesization, Edit Distance, Knapsack
Lecture 22: Dynamic Programming IV: Guitar Fingering, Tetris, Super Mario Bros.
Lecture 23: Computational Complexity
Lecture 24: Topics in Algorithms Research



# Algorithms I

8 1 Randomized Selection Algorithm
8 3 Deterministic Selection Algorithm Advanced Optional
8 4 Deterministic Selection Analysis I Advanced Optional
8 5 Deterministic Selection Analysis II Advanced Optional
8 6 Omega n log n Lower Bound for Comparison Based Sorting Advanced

9 4 Analysis of Contraction Algorithm
9 5 Counting Minimum Cuts
 
11 3 Correctness of Dijkstra 's Algorithm Advanced Optional
11 4 Dijkstra's Algorithm Implementation and Running Time

13 3 Binary Search Tree Basics, Part II
13 4 Red Black Trees
13 6 Insertion in a Red Black Tree Advanced

15 1 Pathological Data Sets and Universal Hashing Motivation
15 2 Universal Hashing Definition and Example Advanced Optional
15 3 Universal Hashing Analysis of Chaining Advanced Optional
15 4 Hash Table Performance with Open Addressing Advanced Optional 

16 1 Bloom Filters The Basics
16 2 Bloom Filters Heuristic Analysis



# Plate

https://en.wikipedia.org/wiki/Timsort

https://github.com/williamfiset/Algorithms/tree/master/src/main/java/com/williamfiset/algorithms/graphtheory

http://www.cs.cornell.edu/~bwong/Cubit/



# Template

```html
---
title: ""
date: 2020-09-29
draft: false
---

<ul class="contents">
	<li>
		<ul>
      <li>
        <a href="#introduction">Introduction</a>
      </li>
      <li>
        <a href="#resources">Resources</a>
      </li>
		</ul>
	</li>
</ul>

<h3 id="introduction">Introduction</h3>



<figure>
  <img src="/img/raft/previous-terms.svg" style="max-width: 230px;" alt="">
  <figcaption>
    <i>figure 1</i>
  </figcaption>
</figure>


<h3 id="resources">Resources</h3>

<ul>
  <li>
    <a href=""></a>
  </li>
</ul>
```


<!-- 
<h4 id="doubly-linked-list">Doubly Linked List</h4>

<p>
  In a doubly linked list, each node contains a reference to the previous node in the sequence, a reference to the next node in the sequence and a reference to an element of the list. The previous reference of the head node and the next reference of the tail point to null.
</p>

<figure>
  <img src="/img/data-structures/doubly-linked-list.svg" style="max-width: 550px;">
  <figcaption>
    <i>figure 1</i>
  </figcaption>
</figure>


<h4 id="circularly-linked-list">Circularly Linked List</h4>

<p>
  In a circularly linked list, each node contains a reference to the next node in the sequence and a reference to an element of the list. The next reference of the tail points to the head.
</p>

<figure>
  <img src="/img/data-structures/circularly-linked-list.svg" style="max-width: 330px;">
  <figcaption>
    <i>figure 1</i>
  </figcaption>
</figure> -->



```java

```




TOPOLOGICAL SORT DFS

<h3 id="depth-first-search">Depth-First Search</h3>

<!-- 

-->

<!-- An alternative algorithm for topological sorting is based on depth-first search. The algorithm loops through each node of the graph, in an arbitrary order, initiating a depth-first search that terminates when it hits any node that has already been visited since the beginning of the topological sort or the node has no outgoing edges (i.e. a leaf node):

L ← Empty list that will contain the sorted nodes
while exists nodes without a permanent mark do
    select an unmarked node n
    visit(n)

function visit(node n)
    if n has a permanent mark then
        return
    if n has a temporary mark then
        stop   (not a DAG)

    mark n with a temporary mark

    for each node m with an edge from n to m do
        visit(m)

    remove temporary mark from n
    mark n with a permanent mark
    add n to head of L

Each node n gets prepended to the output list L only after considering all other nodes which depend on n (all descendants of n in the graph). Specifically, when the algorithm adds node n, we are guaranteed that all nodes which depend on n are already in the output list L: they were added to L either by the recursive call to visit() which ended before the call to visit n, or by a call to visit() which started even before the call to visit n. Since each edge and node is visited once, the algorithm runs in linear time. This depth-first-search-based algorithm is the one described by Cormen et al. (2001); it seems to have been first described in print by Tarjan (1976). -->




<h3 class="">Shortest Paths</h3>

<!-- 
  Breath-first search can be augmented to compute shortest-path distances.

  In a graph G, we use the notation dist(v,w) for the fewest number of edges in a path from v to w (or +1, if G contains no path from v to w).
 -->