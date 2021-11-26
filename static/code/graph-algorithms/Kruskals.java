// status: done
// source: https://github.com/williamfiset/Algorithms/blob/master/src/main/java/com/williamfiset/algorithms/graphtheory/KruskalsEdgeList.java

import java.util.*;

public class Kruskals {

  public static int solve(Edge[] edges, int n) {
    UnionFind uf = new UnionFind(n);
    int sum = 0;
    
    Arrays.sort(edges);
    
    for (Edge edge : edges) {
      // avoid creating cycles
      if (uf.connected(edge.from, edge.to)) {
        continue;
      }

      // include the edge
      uf.union(edge.from, edge.to);
      sum += edge.cost;

      // stop early if we have found an MST
      if (uf.size(0) == n) {
        break;
      }
    }

    // ensure we have an MST
    if (uf.size(0) != n) {
      return -1;
    }

    return sum;
  }

  private static class Edge implements Comparable<Edge> {
    int from;
    int to;
    int cost;

    public Edge(int from, int to, int cost) {
      this.from = from;
      this.to = to;
      this.cost = cost;
    }

    public int compareTo(Edge other) {
      return this.cost - other.cost;
    }
  }

  private static class UnionFind {
    private int[] parents, componentSizes;

    public UnionFind(int n) {
      this.parents = new int[n];
      this.componentSizes = new int[n];
      
      for (int i = 0; i < n; i++) {
        parents[i] = i;
        componentSizes[i] = 1;
      }
    }

    public int find(int p) {
      int root = p;
      while (root != parents[root]) {
        root = parents[root];
      }

      // path compression
      int curr = p;
      while (curr != root) {
        int next = parents[curr];
        parents[curr] = root;
        curr = next;
      }

      return root;
    }

    public void union(int p, int q) {
      int root1 = find(p);
      int root2 = find(q);

      if (root1 == root2) {
        return;
      }

      if (componentSizes[root1] < componentSizes[root2]) {
        componentSizes[root2] += componentSizes[root1];
        parents[root1] = root2;
      } else {
        componentSizes[root1] += componentSizes[root2];
        parents[root2] = root1;
      }
    }

    public boolean connected(int p, int q) {
      return find(p) == find(q);
    }

    public int size(int p) {
      return componentSizes[find(p)];
    }
  }

  public static void main(String[] args) {
    Edge[] edges = new Edge[18];

    edges[0] = new Edge(0, 1, 5);
    edges[1] = new Edge(1, 2, 4);
    edges[2] = new Edge(2, 9, 2);
    edges[3] = new Edge(0, 4, 1);
    edges[4] = new Edge(0, 3, 4);
    edges[5] = new Edge(1, 3, 2);
    edges[6] = new Edge(2, 7, 4);
    edges[7] = new Edge(2, 8, 1);
    edges[8] = new Edge(9, 8, 0);
    edges[9] = new Edge(4, 5, 1);
    edges[10] = new Edge(5, 6, 7);
    edges[11] = new Edge(6, 8, 4);
    edges[12] = new Edge(4, 3, 2);
    edges[13] = new Edge(5, 3, 5);
    edges[14] = new Edge(3, 6, 11);
    edges[15] = new Edge(6, 7, 1);
    edges[16] = new Edge(3, 7, 2);
    edges[17] = new Edge(7, 8, 6);

    System.out.println(Kruskals.solve(edges, 10));
  }
}

