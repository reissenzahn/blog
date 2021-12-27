Using Pointers to Support Data Structures

typedef struct _employee{
 char name[32];
 unsigned char age;
} Employee;

Two comparison functions will be developed. The first one compares two employees
and returns an integer. This function is modeled after the strcmp function. A return
value of 0 means the two employee structures are considered to be equal to each other.
A return value of –1 means the first employee precedes the second employee alphabet‐
ically. A return value of 1 means the first employee follows the second employee. The
second function displays a single employee:
int compareEmployee(Employee *e1, Employee *e2) {
 return strcmp(e1->name, e2->name);
}
void displayEmployee(Employee* employee) {
 printf("%s\t%d\n", employee->name, employee->age);
}
In addition, two function pointers will be used as defined below. The DISPLAY function
pointer designates a function that is passed void and returns void. Its intent is to display
data. The second pointer, COMPARE, represents a function used to compare data refer‐
enced by two pointers. The function should compare the data and return either a 0,
–1, or 1, as explained with the compareEmployee function:
typedef void(*DISPLAY)(void*);
typedef int(*COMPARE)(void*, void*);
Single-Linked List
A linked list is a data structure that consists of a series of nodes interconnected with
links. Typically, one node is called the head node and subsequent nodes follow the head,
one after another. The last node is called the tail. The links connecting the nodes are
easily implemented using a pointer. Each node can be dynamically allocated as needed.
This approach is preferable to an array of nodes. Using an array results in the creation
of a fixed number of nodes regardless of how many nodes may be needed. Links are

implemented using the index of the array’s elements. Using an array is not as flexible as
using dynamic memory allocation and pointers.
For example, if we wanted to change the order of elements of the array, it would be
necessary to copy both elements, and that can be large for a structure. In addition, adding
or removing an element may require moving large portions of the array to make room
for the new element or to remove an existing element.
There are several types of linked lists. The simplest is a single-linked list where there is
a single link from one node to the next. The links start at the head and eventually leads
to the tail. A circular-linked list has no tail. The linked list’s last node points back to the
head. A doubly linked list uses two links, one pointing forward and one pointing back‐
ward so that you can navigate through the list in both directions. This type of linked list
is more flexible but is more difficult to implement. Figure 6-5 conceptually illustrates
these types of linked lists.

In this section, we will illustrate the construction and use of a single-linked list. The
following shows the structure used to support the linked list. A Node structure is defined
to represent a node. It consists of two pointers. The first one, a pointer to void, holds
an arbitrary data type. The second is a pointer to the next node. The LinkedList struc‐
ture represents the linked list and holds a pointer to the head and the tail. The current
pointer will help traverse the linked list:
typedef struct _node {
 void *data;
 struct _node *next;
} Node;
typedef struct _linkedList {
 Node *head;
 Node *tail;
 Node *current;
} LinkedList;
We will develop several functions that use these structures to support linked list
functionality:
void initializeList(LinkedList*) Initializes the linked list
void addHead(LinkedList*, void*) Adds data to the linked list’s head
void addTail(LinkedList*, void*) Adds data to the linked list’s tail
void delete(LinkedList*, Node*) Removes a node from the linked list
Node *getNode(LinkedList*, COMPARE, void*) Returns a pointer to the node containing a specific data item
void displayLinkedList(LinkedList*, DISPLAY) Displays the linked list
Before the linked list can be used it needs to be initialized. The initializeList func‐
tion, shown below, performs this task. A pointer to the LinkedList object is passed to
the function where each pointer in the structure is set to NULL:
void initializeList(LinkedList *list) {
 list->head = NULL;
 list->tail = NULL;
 list->current = NULL;
}
The addHead and addTail functions add data to the linked list’s head and tail, respec‐
tively. In this linked list implementation, the add and delete functions are responsible
for allocating and freeing memory used by the linked list’s nodes. This removes this
responsibility from the user of the linked list.
In the addHead function listed below, memory is first allocated for the node and the data
passed to the function is assigned to the structure’s data field. By passing the data as a
pointer to void, the linked list is able to hold any type of data the user wants to use.

Next, we check to see whether the linked list is empty. If so, we assign the tail pointer to
the node and assign NULL to the node’s next field. If not, the node’s next pointer is
assigned to the list’s head. Regardless, the list’s head is assigned to the node:
void addHead(LinkedList *list, void* data) {
 Node *node = (Node*) malloc(sizeof(Node));
 node->data = data;
 if (list->head == NULL) {
 list->tail = node;
 node->next = NULL;
 } else {
 node->next = list->head;
 }
 list->head = node;
}
The following code sequence illustrates using the initializeList and addHead func‐
tions. Three employees are added to the list. Figure 6-6 shows how memory is allocated
after these statements execute. Some arrows have been removed to simplify the diagram.
In addition, the Employee structure’s name array has been simplified:
 LinkedList linkedList;
 Employee *samuel = (Employee*) malloc(sizeof(Employee));
 strcpy(samuel->name, "Samuel");
 samuel->age = 32;
 Employee *sally = (Employee*) malloc(sizeof(Employee));
 strcpy(sally->name, "Sally");
 sally->age = 28;
 Employee *susan = (Employee*) malloc(sizeof(Employee));
 strcpy(susan->name, "Susan");
 susan->age = 45;
 initializeList(&linkedList);
 addHead(&linkedList, samuel);
 addHead(&linkedList, sally);
 addHead(&linkedList, susan);
The addTail function is shown below. It starts by allocating memory for a new node
and assigning the data to the data field. Since the node will always be added to the tail,
the node’s next field is assigned to NULL. If the linked list is empty, then the head pointer
will be NULL and head can be assigned to the new node. If it is not NULL, then the tail’s
next pointer is assigned to the new node. Regardless, the linked list’s tail pointer is
assigned to the node:

void addTail(LinkedList *list, void* data) {
 Node *node = (Node*) malloc(sizeof(Node));
 node->data = data;
 node->next = NULL;
 if (list->head == NULL) {
 list->head = node;
 } else {
 list->tail->next = node;
 }
 list->tail = node;
}
In the following sequence, the addTail function is illustrated. The creation of the em‐
ployee objects has not been duplicated here. The employees have been added in the
opposite order from the previous example using the addTail function. This results in
the memory allocation being the same as shown in Figure 6-6:
 initializeList(&linkedList);
 addTail(&linkedList, susan);
 addTail(&linkedList, sally);
 addTail(&linkedList, samuel);


 The delete function will remove a node from the linked list. To simplify this function,
a pointer to the node to be deleted is passed to it. The function’s user probably has a
pointer to the data but not to the node holding the data. To aid in identifying the node,
a helper function has been provided to return a pointer to the node: getNode. The
getNode function is passed three parameters:
• A pointer to the linked list
• A pointer to a comparison function
• A pointer to the data to be found
The code for the getNode function follows. The variable node initially points to the list’s
head and traverses the list until either a match is found or the linked list’s end is en‐
countered. The compare function is invoked to determine whether a match is found.
When the two data items are equal, it returns a zero.
Node *getNode(LinkedList *list, COMPARE compare , void* data) {
 Node *node = list->head;
 while (node != NULL) {
 if (compare(node->data, data) == 0) {
 return node;
 }
 node = node->next;
 }
 return NULL;
}
The compare function illustrates using a function pointer at runtime to determine which
function to use to perform a comparison. This adds considerable flexibility to the linked
list implementation because we do not need to hard code the comparison function’s
name in the getNode function.
The delete function follows. To keep the function simple, it does not always check for
null values in the linked list or the node passed. The first if statement handles a node
to be deleted from the head. If the head node is the only node, then the head and tail
are assigned null values. Otherwise, the head is assigned to the node following the head.
The else statement traverses the list from head to tail using a tmp pointer. The while
loop will terminate if either tmp is assigned NULL, indicating the node does not exist in
the list, or the node following tmp is the node we are looking for. Since this is a singlelinked list, we need to know which node precedes the target node to be deleted. This
knowledge is needed to assign the node following the target node to the preceding node’s
next field. At the end of the delete function, the node is freed. The user is responsible
for deleting the data pointed to by this node before the delete function is called.
void delete(LinkedList *list, Node *node) {
 if (node == list->head) {
   if (list->head->next == NULL) {
 list->head = list->tail = NULL;
 } else {
 list->head = list->head->next;
 }
 } else {
 Node *tmp = list->head;
 while (tmp != NULL && tmp->next != node) {
 tmp = tmp->next;
 }
 if (tmp != NULL) {
 tmp->next = node->next;
 }
 }
 free(node);
}
The next sequence demonstrates the use of this function. The three employees are added
to the linked list’s head. We will use the compareEmployee function as described in the
section “Using Pointers to Support Data Structures” on page 141 to perform
comparisons:
 addHead(&linkedList, samuel);
 addHead(&linkedList, sally);
 addHead(&linkedList, susan);
 Node *node = getNode(&linkedList,
 (int (*)(void*, void*))compareEmployee, sally);
 delete(&linkedList, node);
When this sequence executes, the program stack and heap will appear as illustrated in
Figure 6-7.
The displayLinkedList function illustrates how to traverse a linked list as shown be‐
low. It starts at the head and displays each element using the function passed as the
second argument. The node pointer is assigned the next field’s value and will terminate
when the last node is displayed:
void displayLinkedList(LinkedList *list, DISPLAY display) {
 printf("\nLinked List\n");
 Node *current = list->head;
 while (current != NULL) {
 display(current->data);
 current = current->next;
 }
}
The following illustrates this function using the displayEmployee function developed
in the section “Using Pointers to Support Data Structures” on page 141:

addHead(&linkedList, samuel);
 addHead(&linkedList, sally);
 addHead(&linkedList, susan);
 displayLinkedList(&linkedList, (DISPLAY)displayEmployee);
The output of this sequence follows:
Linked List
Susan 45
Sally 28
Samuel 32
Using Pointers to Support a Queue
A queue is a linear data structure whose behavior is similar to a waiting line. It typically
supports two primary operations: enqueue and dequeue. The enqueue operation adds
an element to the queue. The dequeue operation removes an element from the queue.
Normally, the first element added to a queue is the first element dequeued from a queue.
This behavior is referred to as First-In-First-Out (FIFO).
A linked list is frequently used to implement a queue. The enqueue operation will add
a node to the linked list’s head and the dequeue operation will remove a node from the

tail. To illustrate the queue, we will use the linked list developed in the “Single-Linked
List” on page 142.
Let’s start by using a type definition statement to define a term for queue. It will be based
on a linked list as shown below. We can now use Queue to clearly designate our intent:
typedef LinkedList Queue;
To implement the initialization operation, all we need to do is use the function
initializeList. Instead of calling this function directly, we will use the following
initializeQueue function:
void initializeQueue(Queue *queue) {
 initializeList(queue);
}
In a similar manner, the following will add a node to a queue using the addHead function:
void enqueue(Queue *queue, void *node) {
 addHead(queue, node);
}
The previous linked list implementation does not have an explicit function to remove
the tail node. The dequeue function that follows removes the last node. Three conditions
are handled:
An empty queue
NULL is returned
A single node queue
Handled by the else if statement
A multiple node queue
Handled by the else clause
In the latter case, the tmp pointer is advanced node by node until it points to the node
immediately preceding the tail node. Three operations are then performed in the fol‐
lowing sequence:
1. The tail is assigned to the tmp node
2. The tmp pointer is advanced to the next node
3. The tail’s next field is set to NULL to indicate there are no more nodes in the queue
This order is necessary to ensure the list’s integrity, as illustrated conceptually in
Figure 6-8. The circled numbers correspond to the three steps listed above:
void *dequeue(Queue *queue) {
 Node *tmp = queue->head;
 void *data;
 if (queue->head == NULL) {
 data = NULL;
 } else if (queue->head == queue->tail) {
 queue->head = queue->tail = NULL;
 data = tmp->data;
 free(tmp);
 } else {
 while (tmp->next != queue->tail) {
 tmp = tmp->next;
 }
 queue->tail = tmp;
 tmp = tmp->next;
 queue->tail->next = NULL;
 data = tmp->data;
 free(tmp);
 }
 return data;
}


The data assigned to the node is returned, and the node is freed. These functions are
illustrated in the following code sequence using the employees created earlier:
 Queue queue;
 initializeQueue(&queue);
 enqueue(&queue, samuel);
 enqueue(&queue, sally);
 enqueue(&queue, susan);

 void *data = dequeue(&queue);
 printf("Dequeued %s\n", ((Employee*) data)->name);
 data = dequeue(&queue);
 printf("Dequeued %s\n", ((Employee*) data)->name);
 data = dequeue(&queue);
 printf("Dequeued %s\n", ((Employee*) data)->name);
The output of this sequence follows:
Dequeued Samuel
Dequeued Sally
Dequeued Susan
Using Pointers to Support a Stack
The stack data structure is also a type of list. In this case, elements are pushed onto the
stack’s top and then popped off. When multiple elements are pushed and then popped,
the stack exhibits First-In-Last-Out (FILO) behavior. The first element pushed on to
the stack is the last element popped off.
Like the queue’s implementation, we can use a linked list to support stack operations.
The two most common operations are the push and pop operations. The push operation
is effected using the addHead function. The pop operation requires adding a new func‐
tion to remove the head node. We start by defining a stack in terms of a linked list as
follows:
typedef LinkedList Stack;
To initialize the stack, we add an initializeStack function. This function calls the
initializeList function:
void initializeStack(Stack *stack) {
 initializeList(stack);
}
The push operation calls the addHead function as shown below:
void push(Stack *stack, void* data) {
 addHead(stack, data);
}

The pop operation’s implementation follows. We start by assigning the stack’s head to a
node pointer. It involves handling three conditions:
The stack is empty
The function returns NULL
The stack contains a single element
If the node points to the tail then the head and tail are the same element. The head
and tail are assigned NULL, and the data is returned.
The stack contains more than one element
In this case, the head is assigned to the next element in the list, and the data is
returned.
In the latter two cases, the node is freed:
void *pop(Stack *stack) {
 Node *node = stack->head;
 if (node == NULL) {
 return NULL;
 } else if (node == stack->tail) {
 stack->head = stack->tail = NULL;
 void *data = node->data;
 free(node);
 return data;
 } else {
 stack->head = stack->head->next;
 void *data = node->data;
 free(node);
 return data;
 }
}
We will reuse the employees’ instances created in the section “Single-Linked List” on
page 142 to demonstrate the stack. The following code sequence will push three em‐
ployees and then pop them off the stack:
 Stack stack;
 initializeStack(&stack);
 push(&stack, samuel);
 push(&stack, sally);
 push(&stack, susan);
 Employee *employee;

 for(int i=0; i<4; i++) {
 employee = (Employee*) pop(&stack);
 printf("Popped %s\n", employee->name);
 }

 When executed, we get the following output. Because we used the pop function four
times, NULL was returned the last time:
Popped Susan
Popped Sally
Popped Samuel
Popped (null)
Other stack operations sometime include a peek operation where the top element is
returned but is not popped off the stack.
Using Pointers to Support a Tree
The tree is a very useful data structure whose name is derived from the relationship
between its elements. Typically, child nodes are attached to a parent node. The overall
form is an inverted tree where a root node represents the data structure’s starting ele‐
ment.
A tree can have any number of children nodes. However, binary trees are more common
where each node possesses zero, one, or two children nodes. The children are designated
as either the left child or the right child. Nodes with no children are called leaf nodes,
similar to the leaves of a tree. The examples presented in this section will illustrate a
binary tree.
Pointers provide an obvious and dynamic way of maintaining the relationship between
tree nodes. Nodes can be dynamically allocated and added to a tree as needed. We will
use the following structure for a node. Using a pointer to void allows us to handle any
type of data that we need:
typedef struct _tree {
 void *data;
 struct _tree *left;
 struct _tree *right;
} TreeNode;
When we add nodes to a tree, it makes sense to add them in a particular order. This will
make many operations, such as searching, easier. A common ordering is to add a new
node to a tree such that all of the node’s children possess a value less than the parent
node and all of the children on the right possess a value greater than the parent node.
This is called a binary search tree.
The following insertNode function will insert a node into a binary search tree. However,
to insert a node, a comparison needs to be performed between the new node and the
tree’s existing nodes. We use the COMPARE function pointer to pass the comparison func‐
tion’s address. The first part of the function allocates memory for a new node and assigns
the data to the node. The left and right children are set to NULL since new nodes are
always added as leaves to a tree:

void insertNode(TreeNode **root, COMPARE compare, void* data) {
 TreeNode *node = (TreeNode*) malloc(sizeof(TreeNode));
 node->data = data;
 node->left = NULL;
 node->right = NULL;
 if (*root == NULL) {
 *root = node;
 return;
 }
 while (1) {
 if (compare((*root)->data, data) > 0) {
 if ((*root)->left != NULL) {
 *root = (*root)->left;
 } else {
 (*root)->left = node;
 break;
 }
 } else {
 if ((*root)->right != NULL) {
 *root = (*root)->right;
 } else {
 (*root)->right = node;
 break;
 }
 }
 }
}
First, the root is checked to determine whether the tree is empty. If it is, then a new node
is assigned to the root and the function returns. The root is passed as a pointer to a
pointer to a TreeNode. This is necessary because we want to modify the pointer passed
to the function, not what the pointer points to. This use of two levels of indirection is
detailed in “Multiple Levels of Indirection” on page 25.
If the tree is not empty, then an infinite loop is entered and will terminate when the new
node has been added to the tree. With each loop’s iteration, the new node and current
parent node are compared. On the basis of this comparison, the local root pointer will
be reassigned to either the left or right child. This root pointer points to the current
node in the tree. If the left or right child is NULL, then the node is added as a child and
the loop terminates.
To demonstrate insertNode function, we will reuse the employee instances created in
the section “Using Pointers to Support Data Structures” on page 141. The following
sequence initializes an empty TreeNode and then inserts the three employees. The re‐
sulting program stack’s and heap’s state is illustrated in Figure 6-9. Some lines pointing
to the employees have been removed to simplify the diagram. The nodes’ placement in
the heap have also been arranged to reflect the tree structure’s order:

TreeNode *tree = NULL;
 insertNode(&tree, (COMPARE) compareEmployee, samuel);
 insertNode(&tree, (COMPARE) compareEmployee, sally);
 insertNode(&tree, (COMPARE) compareEmployee, susan);

 Binary trees are used for a number of purposes and can be traversed in three different
ways: pre-order, in-order, and post-order. The three techniques use the same steps, but
they are performed in different orders. The three steps are:
Visit the node
Process the node
Go left
Transfer to the left node

Go right
Transfer to the right node
For our purposes, visiting a node means we will display its contents. The three orders
are:
In-order
Go left, visit the node, go right
Pre-order
Visit the node, go left, go right
Post-order
Go left, go right, visit the node
The functions’ implementations are shown below. Each passes the tree’s root and a
function pointer for the display function. They are recursive and will call themselves as
long as the root node passed to it is not null. They only differ in the order the three steps
are executed:
void inOrder(TreeNode *root, DISPLAY display) {
 if (root != NULL) {
 inOrder(root->left, display);
 display(root->data);
 inOrder(root->right, display);
 }
}
void postOrder(TreeNode *root, DISPLAY display) {
 if (root != NULL) {
 postOrder(root->left, display);
 postOrder(root->right, display);
 display(root->data);
 }
}
void preOrder(TreeNode *root, DISPLAY display) {
 if (root != NULL) {
 display(root->data);
 preOrder(root->left, display);
 preOrder(root->right, display);
 }
}
The following code sequence invokes these functions:
 preOrder(tree, (DISPLAY) displayEmployee);
 inOrder(tree, (DISPLAY) displayEmployee);
 postOrder(tree, (DISPLAY) displayEmployee);
Table 6-1 shows the output of each function call based on the previous initialization of
the tree.

Table 6-1. Traversal techniques
pre-order Samuel 32 Sally 28 Susan 45
in-order Sally 28 Samuel 32 Susan 45
post-order Sally 28 Susan 45 Samuel 32
The in-order traversal will return a sorted list of the tree’s members. The pre-order and
post-order traversal can be used to evaluate arithmetic expressions when used in con‐
junction with a stack and queue.
