import java.util.*;

public class CycleDetection {

  public boolean hasCycle(int[][] graph) {
    Set<Integer> whiteSet = new HashSet<>();
    Set<Integer> graySet = new HashSet<>();
    Set<Integer> blackSet = new HashSet<>();

    // add all vertices to white set
    for (int i = 0; i < graph.length; i++) {
      whiteSet.add(i);
    }

    // choose arbitrary node from white set and start dfs
    while (whiteSet.size() > 0) {
      int v = whiteSet.iterator().next();

      if (this.dfs(graph, whiteSet, graySet, blackSet, v)) {
        return true;
      }
    }

    return false;
  }

  private boolean dfs(int[][] graph, Set<Integer> whiteSet, Set<Integer> graySet, Set<Integer> blackSet, int v) {
    this.move(v, whiteSet, graySet);

    for (int u : graph[v]) {
      if (blackSet.contains(u)) continue;

      if (graySet.contains(u)) return true;

      if (this.dfs(graph, whiteSet, graySet, blackSet, u)) return true;
    }

    this.move(v, graySet, blackSet);

    return false;
  }

  private void move(int v, Set<Integer> sourceSet, Set<Integer> destSet) {
    sourceSet.remove(v);
    destSet.add(v);
  }

  public static void main(String[] args) {
    int[][] graph = new int[5][];

    graph[0] = new int[]{};
    graph[1] = new int[]{0};
    graph[2] = new int[]{1, 3};
    graph[3] = new int[]{4};
    graph[4] = new int[]{2};

    System.out.println(new CycleDetection().hasCycle(graph));

    graph[2] = new int[]{1};
    
    System.out.println(new CycleDetection().hasCycle(graph));
  }
}
