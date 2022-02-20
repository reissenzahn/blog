class Solution {
  public ListNode reverseBetween(ListNode head, int m, int n) {
    ListNode dummy = new ListNode(0);
    dummy.next = head;
      
    ListNode tail1 = dummy;
    
    for (int i = 0; i < m - 1; i++) {
      tail1 = tail1.next;
    }
    
    ListNode tail2 = tail1.next;
    
    ListNode curr = tail2;
    ListNode prev = null;
    
    for (int i = 0; i <= (n - m); i++){
      ListNode next = curr.next;
      curr.next = prev;
      prev = curr;
      curr = next;
    }
    
    ListNode head2 = prev;
    ListNode head3 = curr;
        
    tail1.next = head2;
    tail2.next = head3;

    return dummy.next;
  }
}