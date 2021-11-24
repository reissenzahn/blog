// Status: DONE
// Source: https://github.com/williamfiset/Algorithms/blob/master/src/main/java/com/williamfiset/algorithms/graphtheory/DepthFirstSearchAdjacencyListIterative.java

import java.util.*;

public class DFS {

  public static void search(List<List<Integer>> graph, int start) {
    int n = graph.size();

    boolean[] visited = new boolean[n];
    Stack<Integer> stack = new Stack<>();

    System.out.println("visited " + start);
    stack.push(start);
    visited[start] = true;

    while (!stack.isEmpty()) {
      int at = stack.pop();
      
      for (int to : graph.get(at)) {
        if (!visited[to]) {
          System.out.println("visited " + to);
          stack.push(to);
          visited[to] = true;
        }
      }
    }
  }

  public static void main(String[] args) {
    List<List<Integer>> graph = new ArrayList<>();

    for (int i = 0; i < 4; i++) {
      graph.add(new ArrayList<>());
    }

    graph.get(0).add(1);
    graph.get(0).add(2);
    graph.get(1).add(2);
    graph.get(1).add(3);
    graph.get(2).add(3);
    graph.get(2).add(2);

    DFS.search(graph, 0);
  }
}

