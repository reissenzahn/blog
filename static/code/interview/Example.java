import java.util.*;

public class Example {

  public static void main(String[] args) {

    // String Builder
    StringBuilder builder = new StringBuilder();
    builder.append("ab");
    builder.append("cde");
    System.out.println(builder.toString());  // abcde
    builder.reverse();
    System.out.println(builder.toString());  // edcba

    // Dynamic Array
    List<Integer> array = new ArrayList<>();
    array.add(3);
    array.add(0, 1);
    array.add(array.size() - 1, 2);
    System.out.println(Arrays.toString(array.toArray()));   // [1, 2, 3]
    array.remove(1);
    System.out.println(Arrays.toString(array.toArray()));   // [1, 3]

    // Linked List
    LinkedList<Integer> list = new LinkedList<>();
    list.addFirst(1);
    list.addLast(2);
    list.addLast(3);
    System.out.println(Arrays.toString(list.toArray()));   // [1, 2, 3]
    list.removeFirst();
    list.removeLast();
    System.out.println(Arrays.toString(list.toArray()));   // [2]

    // Stack
    Stack<Integer> stack = new Stack<>();
    stack.push(1);
    System.out.println(stack.peek());  // 1
    System.out.println(stack.pop());   // 1
    // System.out.println(stack.pop());   // Exception

    // Queue
    Queue<Integer> queue = new LinkedList<>();
    queue.offer(1);
    System.out.println(queue.poll());  // 1
    System.out.println(queue.poll());  // null

    // Hash Table
    Map<String, Integer> table = new HashMap<>();
    table.put("a", 1);
    System.out.println(table.get("a"));  // 1
    System.out.println(table.keySet());  // [a]
    System.out.println(table.values());  // [1]
    table.remove("a");
    System.out.println(table.get("a"));              // null
    System.out.println(table.getOrDefault("a", 1));  // 1

    // Set
    Set<Integer> set = new HashSet<>();
    set.add(1);
    System.out.println(set.contains(1));  // true
    set.remove(1);
    System.out.println(set.contains(1));  // false

    // Deque
    Deque<Integer> deque = new LinkedList<>();
    deque.offerFirst(1);
    System.out.println(deque.peekFirst());  // 1
    System.out.println(deque.pollFirst());  // 1
    deque.offerLast(2);
    System.out.println(deque.peekLast());  // 2
    System.out.println(deque.pollLast());  // 2

    // PriorityQueue
    PriorityQueue<Integer> pq = new PriorityQueue<>((x, y) -> {
      if (x == y) return 0;
      
      return x > y ? 1 : -1;
    });
    pq.offer(3);
    pq.offer(2);
    pq.offer(1);
    System.out.println(pq.peek());  // 1
    System.out.println(pq.poll());  // 1

  }
}

