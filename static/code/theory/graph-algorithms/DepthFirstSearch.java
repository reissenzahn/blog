import java.util.*;

public class DepthFirstSearch {

  public static void search(int[][] adjacencyList, int start) {
    int n = adjacencyList.length;

    boolean[] visited = new boolean[n];
    Stack<Integer> stack = new Stack<>();

    System.out.println("visited: " + start);
    stack.push(start);
    visited[start] = true;

    while (!stack.isEmpty()) {
      int at = stack.pop();
      
      for (int to : adjacencyList[at]) {
        if (!visited[to]) {
          System.out.println("visited: " + to);
          stack.push(to);
          visited[to] = true;
        }
      }
    }
  }

  // javac DepthFirstSearch.java && java DepthFirstSearch
  public static void main(String[] args) {
    int[][] adjacencyList = new int[4][];

    adjacencyList[0] = new int[]{1, 2};
    adjacencyList[1] = new int[]{2, 3};
    adjacencyList[2] = new int[]{2, 3};
    adjacencyList[3] = new int[]{};

    DepthFirstSearch.search(adjacencyList, 0);
  }
}

