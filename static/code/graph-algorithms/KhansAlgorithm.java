import java.util.*;

public class Khans {

  public static int[] solve(List<List<Integer>> graph) {
    int n = graph.size();

    // calculate the in-degree of each node
    int[] inDegrees = new int[n];
    for (List<Integer> edges : graph) {
      for (int to : edges) {
        inDegrees[to]++;
      }
    }

    // queue of nodes with no incoming edges
    Queue<Integer> queue = new ArrayDeque<>();

    for (int i = 0; i < n; i++) {
      if (inDegrees[i] == 0) {
        queue.offer(i);
      }
    }

    int index = 0;
    int[] ordering = new int[n];

    while(!queue.isEmpty()) {
      int at = queue.poll();
      
      ordering[index] = at;
      index++; 

      for (int to : graph.get(at)) {
        inDegrees[to]--;
        if (inDegrees[to] == 0) {
          queue.offer(to);
        }
      }
    }

    if (index != n) {
      return null;
    }

    return ordering;
  }

  public static void main(String[] args) {
    List<List<Integer>> graph = new ArrayList<>();

    for (int i = 0; i < 14; i++) {
      graph.add(new ArrayList<>());
    }

    addDirectedEdge(graph, 0, 2);
    addDirectedEdge(graph, 0, 3);
    addDirectedEdge(graph, 0, 6);
    addDirectedEdge(graph, 1, 4);
    addDirectedEdge(graph, 2, 6);
    addDirectedEdge(graph, 3, 1);
    addDirectedEdge(graph, 3, 4);
    addDirectedEdge(graph, 4, 5);
    addDirectedEdge(graph, 4, 8);
    addDirectedEdge(graph, 6, 7);
    addDirectedEdge(graph, 6, 11);
    addDirectedEdge(graph, 7, 4);
    addDirectedEdge(graph, 7, 12);
    addDirectedEdge(graph, 9, 2);
    addDirectedEdge(graph, 9, 10);
    addDirectedEdge(graph, 10, 6);
    addDirectedEdge(graph, 11, 12);
    addDirectedEdge(graph, 12, 8);

    // [0, 9, 13, 3, 2, 10, 1, 6, 7, 11, 4, 12, 5, 8]
    System.out.println(Arrays.toString(Khans.solve(graph)));
  }

  private static void addDirectedEdge(List<List<Integer>> graph, int from, int to) {
   graph.get(from).add(to);
  }
}

