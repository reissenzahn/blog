
// Stack is a collection of objects that are inserted and removed according to the last-in, first-out principle.
public interface Stack<T> {

  // size returns the number of elements in the stack.
  int size();

  // isEmpty tests whether the stack is empty.
  boolean isEmpty();

  // push inserts an element at the top of the stack.
  void push(T element);

  // top returns the element at the top of the stack.
  T top();

  // pop removes and returns the top element from the stack.
  T pop();
}