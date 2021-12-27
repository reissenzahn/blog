
public class BreadthFirstSearch {

  public BFS() {
  }

  public int[] bfs(List<List<Integer>> graph, int start) {
    boolean[] visited = new boolean[n];
    int[] prev = new int[n];
    Deque<Integer> queue = new ArrayDeque<>(n);

    queue.offer(start);
    visited[start] = true;

    while (!queue.isEmpty()) {
      int u = queue.poll();
      
      for (Integer v : graph.get(u)) {
        if (!visited[v]) {
          visited[v] = true;
          prev[v] = u;
          queue.offer(v);
        }
      }
    }

    return prev;
  }

  public List<Integer> shortestPath(int start, int end) {
    int[] prev = bfs(start);

    List<Integer> path = new ArrayList<>();

    for (Integer u = end; u != null; u = prev[u]) {
      path.add(u);
    }

    Collections.reverse(path);

    if (path.get(0) == start) {
      return path;
    }

    return null;
  }
}

