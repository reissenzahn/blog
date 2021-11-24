import java.util.*;

public class DepthFirstSearch {

  public int search(int[][] graph, int start, int size) {
    boolean[] visited = new boolean[size];
    Stack<Integer> stack = new Stack<>();
    int count = 0;

    stack.push(start);
    visited[start] = true;

    while(!stack.isEmpty()) {
      int node = stack.pop();
      count++;

      System.out.println("visited: " + node);

      for (int to : graph[node]) {
        if (!visited[to]) {
          stack.push(to);
          visited[to] = true;
        }
      }
    }
  
    return count;
  }

  public static void main(String[] args) {
    int[][] graph = new int[5][];

    graph[0] = new int[]{1, 4};
    graph[1] = new int[]{1, 2, 3};
    graph[2] = new int[]{3};
    graph[3] = new int[]{};
    graph[4] = new int[]{1, 3};

    DepthFirstSearch dfs = new DepthFirstSearch();
    dfs.search(graph, 0, 5);
  }
}