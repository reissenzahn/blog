class Solution {
  public ListNode mergeTwoLists(ListNode head1, ListNode head2) {
    ListNode dummy = new ListNode(-1);
    
    ListNode curr = dummy;
    
    ListNode curr1 = head1;
    ListNode curr2 = head2;
    
    while (curr1 != null || curr2 != null) {      
      if (curr1 == null) {
        curr.next = curr2;
        break;
      } else if (curr2 == null) {
        curr.next = curr1;
        break;
      } else {
        if (curr1.val < curr2.val) {
          curr.next = curr1;
          curr = curr.next;
          curr1 = curr1.next;
        } else {
          curr.next = curr2;
          curr = curr.next;
          curr2 = curr2.next; 
        }
      }
    }
    
    return dummy.next;
  }
}