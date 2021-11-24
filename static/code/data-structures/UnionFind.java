public class UnionFind {

  private int size;              // number of items in set
  private int[] parents;         // parents[i] is the parent of i
  private int numComponents;     // number of components
  private int[] componentSizes;  // contains sizes

  public UnionFind(int size) {
    this.size = size;
    this.numComponents = size;
    
    this.parents = new int[size];
    this.componentSizes = new int[size];

    // initially, each node is a root
    for (int i = 0; i < size; i++) {
      parents[i] = i;
      componentSizes[i]  = 1;
    }
  }

  // find root of p
  public int find(int p) {
    // find the root by following parents until self loop
    int root = p;
    while (root != parents[root]) {
      root = parents[root];
    }

    // compress the path leading to the root
    int curr = p;
    while (curr != root) {
      int next = parents[curr];
      parents[curr] = root;
      curr = next;
    }

    return root;
  }

  // merge the components containing p and q
  public void unify(int p, int q) {
    if (connected(p, q)) {
      return;
    }

    int root1 = find(p);
    int root2 = find(q);

    // merge smaller component into the larger one
    if (componentSizes[root1] < componentSizes[root2]) {
      componentSizes[root2] += componentSizes[root1];
      componentSizes[root1] = 0;
      parents[root1] = root2;
    } else {
      componentSizes[root1] += componentSizes[root2];
      componentSizes[root2] = 0;
      parents[root2] = root1;
    }

    numComponents--;
  }

  // check if p and q are in the same component
  public boolean connected(int p, int q) {
    return find(p) == find(q);
  }

  // get number of items in set
  public int size() {
    return size;
  }

  // return number of components
  public int numComponents() {
    return numComponents;
  }

  // get the size of the component containing p
  public int componentSize(int p) {
    return componentSizes[find(p)];
  }

  public static void main(String[] args) {
    UnionFind uf = new UnionFind(10);

    uf.unify(0, 1);
    uf.unify(2, 3);
    uf.unify(4, 5);
    uf.unify(6, 7);
    uf.unify(8, 9);
    uf.unify(9, 6);
    uf.unify(7, 5);
    uf.unify(0, 2);
    uf.unify(3, 4);
    uf.unify(6, 1);
    uf.unify(8, 9);

    System.out.println(uf.find(0));
    System.out.println(uf.find(1));
    System.out.println(uf.find(2));
    System.out.println(uf.find(3));
    System.out.println(uf.find(4));
    System.out.println(uf.find(5));
    System.out.println(uf.find(6));
    System.out.println(uf.find(7));
    System.out.println(uf.find(8));
    System.out.println(uf.find(9));
  }
}