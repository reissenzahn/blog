import java.util.*;

public class BellmanFord {

  public static double[] solve(List<List<Edge>> graph, int n, int start) {
    double[] dist = new double[n];

    for (int i = 0; i < n; i++) {
      dist[i] = Double.POSITIVE_INFINITY;
    }

    dist[start] = 0;

    // loop over each edge in the graph n - 1 times
    for (int i = 0; i < n - 1; i++) {
      for (List<Edge> edges : graph) {
        for (Edge edge : edges) {
          if (dist[edge.from] + edge.weight < dist[edge.to]) {
            dist[edge.to] = dist[edge.from] + edge.weight;
          }
        }
      }
    }

    // check for negative cycles
    for (int i = 0; i < n - 1; i++) {
      for (List<Edge> edges : graph) {
        for (Edge edge : edges) {
          if (dist[edge.from] + edge.weight < dist[edge.to]) {
            System.out.println("negative cycle detected!");
            dist[edge.to] = dist[edge.from] + edge.weight;
            // return;
          }
        }
      }
    }

    return dist;
  }

  private static class Edge {
    private int from;
    private int to;
    private double weight;

    public Edge(int from, int to, int weight) {
      this.from = from;
      this.to = to;
      this.weight = weight;
    }
  }

  // $ javac BellmanFord.java && java BellmanFord
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


    double[] dist = BellmanFord.solve(graph, n, 0);

    for (int i = 0; i < n; i++) {
      System.out.println(i + ": " + dist[i]);
    }
  }
}


