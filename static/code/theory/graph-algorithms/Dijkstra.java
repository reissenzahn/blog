import java.util.*;

public class Dijkstra {

  public static void solve(List<List<Edge>> graph, int start) {
    int n = graph.size();

    boolean[] vis = new boolean[n];
    double[] dist = new double[n];
    int[] prev = new int[n];

    for (int i = 0; i < n; i++) {
      dist[i] = Double.POSITIVE_INFINITY;
      prev[i] = -1;
    }

    PriorityQueue<Pair> pq = new PriorityQueue<>();

    dist[start] = 0;
    pq.offer(new Pair(start, 0));

    while (!pq.isEmpty()) {
      Pair pair = pq.poll();

      vis[pair.index] = true;

      // we already found a better path before we got to processing this node so we can ignore it
      if (dist[pair.index] < pair.dist) {
        continue;
      }

      for (Edge edge : graph.get(pair.index)) {
        // you cannot get a shorter path by revisiting a node you have already visited before
        if (vis[edge.to]) {
          continue;
        }

        double newDist = dist[edge.from] + edge.weight;
        
        if (newDist < dist[edge.to]) {
          dist[edge.to] = newDist;
          prev[edge.to] = edge.from;
          pq.offer(new Pair(edge.to, dist[edge.to]));
        }
      }

      // here we would stop early if we just wanted the shortest path to some target vertex end
      // if (pair.index == end) return dist[end];
    }

    for (int i = 0; i < n; i++) {
      if (dist[i] == Double.POSITIVE_INFINITY) {
        System.out.print(i + ": unreachable");
        continue;
      }

      // reconstruct path
      List<Integer> path = new ArrayList<>();
      
      for (int at = i; at != -1; at = prev[at]) {
        path.add(at);
      }

      Collections.reverse(path);

      System.out.println(i + ": " + Arrays.toString(path.toArray()) + " (" + dist[i] + ")");
    }
  }

  private static class Pair implements Comparable<Pair> {
    private static final double EPS = 1e-6;

    private int index;
    private double dist;

    public Pair(int index, double dist) {
      this.index = index;
      this.dist = dist;
    }

    @Override
    public int compareTo(Pair other) {
      if (Math.abs(this.dist - other.dist) < EPS) {
        return 0;
      }
      
      return (this.dist - other.dist) > 0 ? 1 : -1;
    }
  }

  private static class Edge {
    private double weight;
    private int from;
    private int to;

    public Edge(int from, int to, double weight) {
      this.from = from;
      this.to = to;
      this.weight = weight;
    }
  }

  // $ javac Dijkstra.java && java Dijkstra
  public static void main(String[] args) {
    int n = 6;

    List<List<Edge>> graph = new ArrayList<>(n);
    for (int i = 0; i < n; i++) {
      graph.add(new ArrayList<>());
    }

    graph.get(0).add(new Edge(0, 1, 5));
    graph.get(0).add(new Edge(0, 2, 1));
    graph.get(1).add(new Edge(1, 2, 2));
    graph.get(1).add(new Edge(1, 3, 3));
    graph.get(1).add(new Edge(1, 4, 20));
    graph.get(2).add(new Edge(2, 1, 3));
    graph.get(2).add(new Edge(2, 4, 12));
    graph.get(3).add(new Edge(3, 2, 3));
    graph.get(3).add(new Edge(3, 4, 2));
    graph.get(3).add(new Edge(3, 5, 6));
    graph.get(4).add(new Edge(4, 5, 1));

    Dijkstra.solve(graph, 0);
  }
}
