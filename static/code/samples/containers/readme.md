
3
4
6
7
8
9
10
11
12
13
14
16
17

1 1.Introduction

Algorithms are well-defined procedures for solving problems.

Data structures are conceptual organizations of information.

Three reasons for using data structures are efficiency, abstraction, and reusability.


When one thinks of data structures, one normally thinks of certain actions, or operations, one would like to perform with them as well.

A data
structure together with basic operations like these is called an abstract datatype.

The operations of an abstract datatype constitute its public interface.


Randomized algorithms rely on the statistical properties of random numbers.

One example of a randomized algorithm is quicksort where we randomly select a pivot around which to partition. Quicksort performs well on
average because the normal distribution of random numbers leads to relatively balanced partitioning overall.

Divide-and-conquer algorithms revolve around three steps: divide, conquer, and combine. In the divide step, we divide the data into smaller, more manageable pieces. In the conquer step, we process each division by performing some operation on it. In the combine step, we recombine the processed divisions. One example of a divide-and-conquer algorithm is merge sort.

Dynamic-programming involves breaking a larger problem into overlapping sub-problems and then recombining the solutions of these sub-problems while ensuring common sub-problems are solved only once.

Greedy algorithms make decisions that are locally optimal in the hope that they will lead to globally optimal solutions. Unfortunately, decisions that look best at the moment
are not always the best in the long run. Therefore, greedy algorithms do not always produce optimal results; however, in some cases they do.


Approximation algorithms are algorithms that do not compute optimal solutions; instead, they compute solutions that are “good enough.” Often we use approximation algorithms to solve problems that are computationally expensive but are too significant to give up on altogether. The traveling-salesman problem  is one example of a problem usually solved using an approximation algorithm.


3
4
5
6
7
8
9
10
11
12
13
14
15
16
17



2.Pointer Manipulation

A pointer is simply a variable that stores the address where a piece of data resides in memory.

Nothing prevents a pointer from pointing to an invalid address. Pointers that point to invalid addresses are sometimes called dangling pointers.

Some examples of programming errors that can lead to dangling pointers include casting arbitrary  integers to pointers, adjusting pointers beyond the bounds of arrays, and deallocating storage that one or more pointers still reference.


---


Automatic variables are those for which storage is allocated and deallocated automatically when entering and leaving a block or function.

Memory leaks are blocks of storage that are allocated but never freed by a program, even when no longer in use.

structures are not permitted to contain instances of themselves, but they may contain pointers to instances of themselves

For example, if an array or pointer contains the address 0x10000000, at which a sequence of five 4-byte integers is stored, a[3] accesses the integer at address 0x1000000c. This address is obtained by adding (3)(4) = 1210 = c16 to the address 0x10000000. On the other hand, for an array or pointer referencing twenty characters (a string), a[3] accesses the character at address 0x10000003. This address is obtained by adding (3)(1) = 310 = 316 to the address 0x10000000.

In call-by-value parameter passing, private copies of a function’s calling parameters are made for the function to use as it executes. However, we can simulate call-by-reference parameter passing by passing pointers to parameters instead of passing the parameters


---

void swap1(int x, int y) {
  int tmp;
  tmp = x; x = y; y = tmp;
  return;
}

void swap2(int *x, int *y) {
  int tmp;
  tmp = *x; *x = *y; *y = tmp;
  return;
}

When defining a function that accepts a multidimensional array, all but the first dimension must be specified so that pointer arithmetic can be performed when elements are accessed, as shown in the following code:

int g(int a[][2]) {
a[2][0] = 5;
return 0;
}

To understand why we must include all but the first dimension, imagine a twodimensional array of integers with three rows and two columns. In C, elements are
stored in row-major order at increasing addresses in memory. This means that the
two integers in the first row are stored first, followed by the two integers in the
second row, followed by the two integers of the third row. Therefore, to access an
element in any row but the first, we must know exactly how many elements to
skip in each row to get to elements in successive rows (see Figure 2-6).
Pointers to Pointers as Parameters
One situation in which pointers are used as parameters to functions a great deal in
this book is when a function must modify a pointer passed into it. To do this, the
function is passed a pointer to the pointer to be modified. Consider the operation
list_rem_next, which Chapter 5 defines for removing an element from a linked list.
Upon return, data points to the data removed from the list:
int list_rem_next(List *list, ListElmt *element, void **data);
Since the operation must modify the pointer data to make it point to the data
removed, we must pass the address of the pointer data in order to simulate callby-reference parameter passing (see Figure 2-7). Thus, the operation takes a
pointer to a pointer as its third parameter. This is typical of how data is removed
from most of the data structures presented in this book.
Figure 2-6. Writing 5 to row 2, column 0, in a 2 × 3 array of integers (a) conceptually and
(b) as viewed in memory
0
1
2
0 1
5
5
[0][0] [0][1] [1][0] [1][1] [2][0] [2][1]
2 x sizeof(int)
2 x sizeof(int)
2 x sizeof(int) increasing addresses
a b


---


Casting is a mechanism in C that lets us temporarily treat a variable of one type as a variable of another type.

fptr = (float *)iptr;

After the assignment, iptr and fptr both contain the same address. However, the interpretation of the data at this address depends on which pointer we use to access it.


When casting pointers, one issue we need to be particularly sensitive to is the way
data is aligned in memory. Specifically, we need to be aware that applying casts to
pointers can undermine the alignment a computer expects. Often computers have
alignment requirements so that certain hardware optimizations can make accessing memory more efficient. For example, a system may insist that all integers be
aligned on word boundaries. Thus, given a void pointer that is not word aligned,
if we cast the void pointer to an integer pointer and dereference it, we can expect
an exception to occur at runtime.


---

5. Linked Lists

Singly-linked lists: The simplest linked lists, in which elements are linked by a single pointer. This structure allows the list to be traversed from its first element to its last.

Doubly-linked lists: Linked lists in which elements are linked by two pointers instead of one. This structure allows the list to be traversed both forward and backward.

Circular lists: Linked lists in which the last element is linked to the first instead of being set to NULL. This structure allows the list to be traversed in a circular fashion.


Singly-linked list, usually simply called linked lists, are composed of individual elements, each linked by a single pointer. Each element consists of two parts: a data member and a pointer, called the next pointer. Using this two-member structure, a linked list is formed by setting the next pointer of each element to point to the element that follows it. The next pointer of the last element is set to NULL, a convenient sentinel marking the end of the list. The element at the start of the list is its head; the element at the end of the list is its tail.

To access an element in a linked list, we start at the head of the list and use the next pointers of successive elements to move from element to element until the desired element is reached. With singly-linked lists, the list can be traversed in
only one direction—from head to tail—because each element contains no link to its predecessor.

Conceptually, one thinks of a linked list as a series of contiguous elements. However, because these elements are allocated dynamically (using malloc in C), it is important to remember that, in actuality, they are usually scattered about in memory.


Doubly-linked lists, as their name implies, are composed of elements linked by
two pointers. Each element of a doubly-linked list consists of three parts: in addition to the data and the next pointer, each element includes a pointer to the previous element, called the prev pointer. A doubly-linked list is formed by composing
a number of elements so that the next pointer of each element points to the element that follows it, and the prev pointer points to the element preceding it. To
mark the head and tail of the list, we set the prev pointer of the first element and
the next pointer of the last element to NULL.
To traverse backward through a doubly-linked list, we use the prev pointers of
consecutive elements in the tail-to-head direction.


The circular list is another form of linked list that provides additional flexibility in
traversing elements. A circular list may be singly-linked or doubly-linked, but its
distinguishing feature is that it has no tail. In a circular list, the next pointer of the
last element points back to its first element rather than to NULL. In the case of a
doubly-linked circular list, the prev pointer of the first element is set to point to the
last element as well.


Linked lists present advantages over arrays when we expect to insert and
remove elements frequently. However, arrays themselves offer some advantages when we expect the number of random accesses to overshadow the
number of insertions and deletions. Arrays are strong in this case because their
elements are arranged contiguously in memory. This contiguous arrangement
allows any element to be accessed in O(1) time by using its index. Recall that
to access an element of a linked list, we must have a pointer to the element
itself. Getting a pointer to an element can be expensive if we do not know a
great deal about the pattern in which the elements will be accessed. In
practice, for many applications, we end up traversing at least part of the list.


Q: Insertion before the head of a list using NULL for the element argument is used
only in the singly-linked list implementation. Why is this not necessary for
doubly-linked lists or circular lists?
A: Insertion before the head element of a doubly-linked list is possible using the
prev pointer of the head element itself. In a circular list, an element is inserted
before the head by inserting the element after the last element using clist_ins_
next. Remember, in a circular list, the last element points back to the first
element.


---


8.Hash Tables

The primary idea behind a hash table is to establish a mapping between the set of all possible keys and positions in the array using a hash function.

A hash function accepts a key and returns its hash coding, or hash value. Keys vary in type, but hash codings are always integers.

Since both computing a hash value and indexing into an array can be performed in constant time, the beauty of hashing is that we can use it to perform constant time searches.

When a hash function can guarantee that no two keys will generate the same hash coding, the resulting hash table is said to be directly addressed. This is ideal, but direct addressing is rarely possible in practice. Typically, the number of entries in a hash table is small relative to the universe of possible keys. Consequently, most hash functions map some keys to the same position in the table. When two keys map to the same position, they collide. A good hash function minimizes collisions, but we must still be prepared to deal with them. This chapter presents two types of hash tables that resolve collisions in different ways.


Chained hash tables: Hash tables that store data in buckets. Each bucket is a linked list that can grow as large as necessary to accommodate collisions.

Open-addressed hash tables: Hash tables that store data in the table itself instead of in buckets. Collisions are resolved using various methods of probing the table.

Selecting a hash function: The crux of hashing. By distributing keys in a random manner about the table,
collisions are minimized. Thus, it is important to select a hash function that
accomplishes this.

Collision resolution: Methods of managing when several keys map to the same index. Chained hash tables have an inherent way to resolve collisions. Open-addressed hash tables use various forms of probing.


A chained hash table fundamentally consists of an array of linked lists. Each list forms a bucket in which we place all elements hashing to a specific position in the array. To insert an element, we first pass its key to a hash function in a process called hashing the key. This tells us in which bucket the element belongs. We then insert the element at the head of the appropriate list. To look up or remove an element, we hash its key again to find its bucket, then traverse the appropriate list until we find the element we are looking for. Because each bucket is a linked list, a chained hash table is not limited to a fixed number of elements. However, performance degrades if the table becomes too full.

When two keys hash to the same position in a hash table, they collide. Chained hash tables have a simple solution for resolving collisions: elements are simply placed in the bucket where the collision occurs. One problem with this, however, is that if an excessive number of collisions occur at a specific position, a bucket becomes longer and longer. Thus, accessing its elements takes more and more time.

Ideally, we would like all buckets to grow at the same rate so that they remain nearly the same size and as small as possible. In other words, the goal is to distribute elements about the table in as uniform and random a manner as possible. This theoretically perfect situation is known as uniform hashing; however, in practice it
usually can only be approximated.

Even assuming uniform hashing, performance degrades significantly if we make the number of buckets in the table small relative to the number of elements we plan to insert. In this situation, all of the buckets become longer and longer. Thus, it is important to pay close attention to a hash table’s load factor.


The load factor of a hash table is defined as:

α = n/m

where n is the number of elements in the table and m is the number of positions into which elements may be hashed. The load factor of a chained hash table indicates the maximum number of elements we can expect to encounter in a bucket, assuming uniform hashing.

For example, in a chained hash table with m = 1699 buckets and a total of n =
3198 elements, the load factor of the table is α = 3198/1699 = 2. Therefore, in this
case, we can expect to encounter no more than two elements while searching any
one bucket. When the load factor of a table drops below 1, each position will
probably contain no more than one element. Of course, since uniform hashing is
only approximated, in actuality we end up encountering somewhat more or less
than what the load factor suggests. How close we come to uniform hashing ultimately depends on how well we select our hash function.



Selecting a Hash Function
The goal of a good hash function is to approximate uniform hashing, that is, to
spread elements about a hash table in as uniform and random a manner as possible. A hash function h is a function we define to map a key k to some position x
in a hash table. x is called the hash coding of k. Formally stated:
Generally, most hashing methods assume k to be an integer so that it may be easily altered mathematically to make h distribute elements throughout the table more
uniformly. When k is not an integer, we can usually coerce it into one without
much difficulty.
Precisely how to coerce a set of keys depends a great deal on the characteristics of
the keys themselves. Therefore, it is important to gain as much of a qualitative
understanding of them in a particular application as we can. For example, if we
were to hash the identifiers found in a program, we might observe that many have
similar prefixes and suffixes since developers tend to gravitate toward variables
such as sampleptr, simpleptr, and sentryptr. A poor way to coerce these
keys would be any method depending strictly on characters at the beginning and
end of the keys, since this would result in many of the same integers for k. On the
other hand, we might try selecting characters from four positions that have the
propensity to be somewhat random, permute them in a way that randomizes them
further, and stuff them into specific bytes of a four-byte integer. Whatever
h k( ) = x
Description of Chained Hash Tables 145
approach we choose for coercing keys, the most important thing to remember,
again, is that a hash function should distribute a set of keys about a hash table in a
uniform and random manner.
Division method
Once we have a key k represented as an integer, one of the simplest hashing
methods is to map it into one of m positions in a table by taking the remainder of
k divided by m. This is called the division method. Formally stated:
Using this method, if the table has m = 1699 positions, and we hash the key k =
25,657, the hash coding is 25,657 mod 1699 = 172. Typically, we should avoid values for m that are powers of 2. This is because if m = 2p, h becomes just the p
lowest-order bits of k. Usually we choose m to be a prime number not too close to
a power of 2, while considering storage constraints and load factor.
For example, if we expect to insert around n = 4500 elements into a chained hash
table, we might choose m = 1699, a good prime number between 210 and 211.
This results in a load factor of α = 4500/1699 ≈ 2.6, which indicates that generally
two or three elements will reside in each bucket, assuming uniform hashing.
Multiplication method
An alternative to the division method is to multiply the integer key k by a constant A in the range 0 < A < 1; extract the fractional part; multiply this value by the
number of positions in the table, m; and take the floor of the result. Typically, A is
chosen to be 0.618, which is the square root of 5, minus 1, all divided by 2. This
method is called the multiplication method. Formally stated:
An advantage to this method is that m, the number of positions in the table, is not
as critical as in the division method. For example, if the table contains m = 2000
positions, and we hash the key k = 6341, the hash coding is (2000)((6341)(0.618)
mod 1) = (2000)(3918.738 mod 1) = (2000)(0.738) = 1476.
In a chained hash table, if we expect to insert no more than n = 4500 elements,
we might let m = 2250. This results in a load factor of α = 4500/2250 = 2, which
indicates that no more than two traversals should be required to locate an element in any bucket, assuming uniform hashing. Again, notice how this method of
hashing allows more flexibility in choosing m to suit the maximum number of traversals acceptable to us.
Example 8-1 presents a hash function that performs particularly well for strings. It
coerces a key into a permuted integer through a series of bit operations. The
h k( ) = k mod m
h k( ) = m kA mod 1( ) , where A ≈ ( ) 5 1 – ⁄ 2 = 0.618
146 Chapter 8: Hash Tables
resulting integer is mapped using the division method. The function was adapted
from Compilers: Principles, Techniques, and Tools (Reading, MA: Addison-Wesley,
1986), by Alfred V. Aho, Ravi Sethi, and Jeffrey D. Ullman, who attributed it to P. J.
Weinberger as a hash function that performed well in hashing strings for his
compiler.
Example 8-1. A Hash Function That Performs Well for Strings



typedef struct CHTbl_ {
  int buckets;
  int (*h)(const void *key);
  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  int size;
  List *table;
} CHTbl;

int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key), int
 (*match)(const void *key1, const void *key2), void (*destroy)(void *data));
void chtbl_destroy(CHTbl *htbl);
int chtbl_insert(CHTbl *htbl, const void *data);
int chtbl_remove(CHTbl *htbl, void **data);
int chtbl_lookup(const CHTbl *htbl, void **data);
#define chtbl_size(htbl) ((htbl)->size)
#endif
Example 8-2. Header for the Chained Hash Table Abstract Datatype (continued)
Implementation and Analysis of Chained Hash Tables 151
chtbl_insert
The chtbl_insert operation inserts an element into a chained hash table (see
Example 8-3). Since a key is not allowed to be inserted into the hash table more
than once, chtbl_lookup is called to make sure that the table does not already contain the new element. If no element with the same key already exists in the hash
table, we hash the key for the new element and insert it into the bucket at the
position in the hash table that corresponds to the hash coding. If this is successful, we increment the table size.
Assuming we approximate uniform hashing well, the runtime complexity of chtbl_
insert is O(1), since chtbl_lookup, hashing a key, and inserting an element at the
head of a linked list all run in a constant amount of time.
chtbl_remove
The chtbl_remove operation removes an element from a chained hash table (see
Example 8-3). To remove the element, we hash its key, search the appropriate
bucket for an element with a key that matches, and call list_rem_next to remove it.
The pointer prev maintains a pointer to the element before the one to be
removed since list_rem_next requires this. Recall that list_rem_next sets data to
point to the data removed from the table. If a matching key is not found in the
bucket, the element is not in the table. If removing the element is successful, we
decrease the table size by 1.
Assuming we approximate uniform hashing well, the runtime complexity of chtbl_
remove is O(1). This is because we expect to search a number of elements equal
to the load factor of the hash table, which is treated as a small constant.
chtbl_lookup
The chtbl_lookup operation searches for an element in a chained hash table and
returns a pointer to it (see Example 8-3). This operation works much like chtbl_
remove, except that once the element is found, it is not removed from the table.
Assuming we approximate uniform hashing well, the runtime complexity of chtbl_
lookup is O(1). This is because we expect to search a number of elements equal to
the load factor of the hash table, which is treated as a small constant.
chtbl_size
This macro evaluates to the number of elements in a chained hash table (see
Example 8-2). It works by accessing the size member of the CHTbl structure.
The runtime complexity of chtbl_size is O(1) because accessing a member of a
structure is a simple task that runs in a constant amount of time.
152 Chapter 8: Hash Tables
Example 8-3. Implementation of the Chained Hash Table Abstract Datatype
/*****************************************************************************
* *
* ------------------------------- chtbl.c -------------------------------- *
* *
*****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "chtbl.h"
/*****************************************************************************
* *
* ------------------------------ chtbl_init ------------------------------ *
* *
*****************************************************************************/
int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key), int
 (*match)(const void *key1, const void *key2), void (*destroy)(void*data)) {
int i;
/*****************************************************************************
* *
* Allocate space for the hash table. *
* *
*****************************************************************************/
if ((htbl->table = (List *)malloc(buckets * sizeof(List))) == NULL)
 return -1;
/*****************************************************************************
* *
* Initialize the buckets. *
* *
*****************************************************************************/
htbl->buckets = buckets;
for (i = 0; i < htbl->buckets; i++)
 list_init(&htbl->table[i], destroy);
/*****************************************************************************
* *
* Encapsulate the functions. *
* *
*****************************************************************************/
htbl->h = h;
htbl->match = match;
htbl->destroy = destroy;
Implementation and Analysis of Chained Hash Tables 153
/*****************************************************************************
* *
* Initialize the number of elements in the table. *
* *
*****************************************************************************/
htbl->size = 0;
return 0;
}
/*****************************************************************************
* *
* ---------------------------- chtbl_destroy ----------------------------- *
* *
*****************************************************************************/
void chtbl_destroy(CHTbl *htbl) {
int i;
/*****************************************************************************
* *
* Destroy each bucket. *
* *
*****************************************************************************/
for (i = 0; i < htbl->buckets; i++) {
 list_destroy(&htbl->table[i]);
}
/*****************************************************************************
* *
* Free the storage allocated for the hash table. *
* *
*****************************************************************************/
free(htbl->table);
/*****************************************************************************
* *
* No operations are allowed now, but clear the structure as a precaution. *
* *
*****************************************************************************/
memset(htbl, 0, sizeof(CHTbl));
return;
}
Example 8-3. Implementation of the Chained Hash Table Abstract Datatype (continued)
154 Chapter 8: Hash Tables
/*****************************************************************************
* *
* ----------------------------- chtbl_insert ----------------------------- *
* *
*****************************************************************************/
int chtbl_insert(CHTbl *htbl, const void *data) {
void *temp;
int bucket,
 retval;
/*****************************************************************************
* *
* Do nothing if the data is already in the table. *
* *
*****************************************************************************/
temp = (void *)data;
if (chtbl_lookup(htbl, &temp) == 0)
 return 1;
/*****************************************************************************
* *
* Hash the key. *
* *
*****************************************************************************/
bucket = htbl->h(data) % htbl->buckets;
/*****************************************************************************
* *
* Insert the data into the bucket. *
* *
*****************************************************************************/
if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
 htbl->size++;
return retval;
}
/*****************************************************************************
* *
* ----------------------------- chtbl_remove ----------------------------- *
* *
*****************************************************************************/
int chtbl_remove(CHTbl *htbl, void **data) {
Example 8-3. Implementation of the Chained Hash Table Abstract Datatype (continued)
Implementation and Analysis of Chained Hash Tables 155
ListElmt *element,
 *prev;
int bucket;
/*****************************************************************************
* *
* Hash the key. *
* *
*****************************************************************************/
bucket = htbl->h(*data) % htbl->buckets;
/*****************************************************************************
* *
* Search for the data in the bucket. *
* *
*****************************************************************************/
prev = NULL;
for (element = list_head(&htbl->table[bucket]); element != NULL; element =
 list_next(element)) {
 if (htbl->match(*data, list_data(element))) {
 /***********************************************************************
 * *
 * Remove the data from the bucket. *
 * *
 ***********************************************************************/
 if (list_rem_next(&htbl->table[bucket], prev, data) == 0) {
 htbl->size--;
 return 0;
 }
 else {
 return -1;
 }
 }
 prev = element;
}
Example 8-3. Implementation of the Chained Hash Table Abstract Datatype (continued)
156 Chapter 8: Hash Tables
/*****************************************************************************
* *
* Return that the data was not found. *
* *
*****************************************************************************/
return -1;
}
/*****************************************************************************
* *
* ----------------------------- chtbl_lookup ----------------------------- *
* *
*****************************************************************************/
int chtbl_lookup(const CHTbl *htbl, void **data) {
ListElmt *element;
int bucket;
/*****************************************************************************
* *
* Hash the key. *
* *
*****************************************************************************/
bucket = htbl->h(*data) % htbl->buckets;
/*****************************************************************************
* *
* Search for the data in the bucket. *
* *
*****************************************************************************/
for (element = list_head(&htbl->table[bucket]); element != NULL; element =
 list_next(element)) {
 if (htbl->match(*data, list_data(element))) {
 /***********************************************************************
 * *
 * Pass back the data from the table. *
 * *
 ***********************************************************************/
 *data = list_data(element);
 return 0;
 }
}
Example 8-3. Implementation of the Chained Hash Table Abstract Datatype (continued)
Chained Hash Table Example: Symbol Tables 157
Chained Hash Table Example:
Symbol Tables
An important application of hash tables is the way compilers maintain information
about symbols encountered in a program. Formally, a compiler translates a program written in one language, a source language such as C, into another language, which is a set of instructions for the machine on which the program will
run. In order to maintain information about the symbols in a program, compilers
make use of a data structure called a symbol table. Symbol tables are often implemented as hash tables because a compiler must be able to store and retrieve information about symbols very quickly.
Several parts of a compiler access the symbol table during various phases of the
compilation process. One part, the lexical analyzer, inserts symbols. The lexical
analyzer is the part of a compiler charged with grouping characters from the
source code into meaningful strings, called lexemes. These are translated into syntactic elements, called tokens, that are passed on to the parser. The parser performs syntactical analysis. As the lexical analyzer encounters symbols in its input
stream, it stores information about them into the symbol table. Two important
attributes stored by the lexical analyzer are a symbol’s lexeme and the type of
token the lexeme constitutes (e.g., an identifier or an operator).
The example presented here is a very simple lexical analyzer that analyzes a string
of characters and then groups the characters into one of two types of tokens: a
token consisting only of digits or a token consisting of something other than digits
alone. For simplicity, we assume that tokens are separated in the input stream by a
single blank. The lexical analyzer is implemented as a function, lex (see Examples
8-4 and 8-5), which a parser calls each time it requires another token.
The function works by first calling the next_token function (whose implementation is not shown) to get the next blank-delimited string from the input stream
istream. If next_token returns NULL, there are no more tokens in the input
stream. In this case, the function returns lexit, which tells the parser that there
are no more tokens to be processed. If next_token finds a string, some simple
/*****************************************************************************
* *
* Return that the data was not found. *
* *
*****************************************************************************/
return -1;
}
Example 8-3. Implementation of the Chained Hash Table Abstract Datatype (continued)
158 Chapter 8: Hash Tables
analysis is performed to determine what type of token the string represents. Next,
the function inserts the lexeme and token type together as a Symbol structure into
the symbol table, symtbl, and returns the token type to the parser. The type
Symbol is defined in symbol.h, which is not included in this example.
A chained hash table is a good way to implement a symbol table because, in addition to being an efficient way to store and retrieve information, we can use it to
store a virtually unlimited amount of data. This is important for a compiler since it
is difficult to know how many symbols a program will contain before lexical
analysis.
The runtime complexity of lex is O(1), assuming next_token runs in a constant
amount of time. This is because lex simply calls chtbl_insert, which is an O(1)
operation.
Example 8-4. Header for a Simple Lexical Analyzer
/*****************************************************************************
* *
* --------------------------------- lex.h -------------------------------- *
* *
*****************************************************************************/
#ifndef LEX_H
#define LEX_H
#include "chtbl.h"
/*****************************************************************************
* *
* Define the token types recognized by the lexical analyzer. *
* *
*****************************************************************************/
typedef enum Token_ {lexit, error, digit, other} Token;
/*****************************************************************************
* *
* --------------------------- Public Interface --------------------------- *
* *
*****************************************************************************/
Token lex(const char *istream, CHTbl *symtbl);
#endif
Example 8-5. Implementation of a Simple Lexical Analyzer
/*****************************************************************************
* *
* --------------------------------- lex.c -------------------------------- *
* *
*****************************************************************************/
Chained Hash Table Example: Symbol Tables 159
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "chtbl.h"
#include "lex.h"
#include "symbol.h"
/*****************************************************************************
* *
* ---------------------------------- lex --------------------------------- *
* *
*****************************************************************************/
Token lex(const char *istream, CHTbl *symtbl) {
Token token;
Symbol *symbol;
int length,
 retval,
 i;
/*****************************************************************************
* *
* Allocate space for a symbol. *
* *
*****************************************************************************/
if ((symbol = (Symbol *)malloc(sizeof(Symbol))) == NULL)
 return error;
/*****************************************************************************
* *
* Process the next token. *
* *
*****************************************************************************/
if ((symbol->lexeme = next_token(istream)) == NULL) {
 /**************************************************************************
 * *
 * Return that there is no more input. *
 * *
 **************************************************************************/
 free(symbol);
 return lexit;
 }
else {
Example 8-5. Implementation of a Simple Lexical Analyzer (continued)
160 Chapter 8: Hash Tables
 /**************************************************************************
 * *
 * Determine the token type. *
 * *
 **************************************************************************/
 symbol->token = digit;
 length = strlen(symbol->lexeme);
 for (i = 0; i < length; i++) {
 if (!isdigit(symbol->lexeme[i]))
 symbol->token = other;
 }
 memcpy(&token, &symbol->token, sizeof(Token));
 /**************************************************************************
 * *
 * Insert the symbol into the symbol table. *
 * *
 **************************************************************************/
 if ((retval = chtbl_insert(symtbl, symbol)) < 0) {
 free(symbol);
 return error;
 }
 else if (retval == 1) {
 /***********************************************************************
 * *
 * The symbol is already in the symbol table. *
 * *
 ***********************************************************************/
 free(symbol);
 }
}
/*****************************************************************************
* *
* Return the token for the parser. *
* *
*****************************************************************************/
return token ;
}
Example 8-5. Implementation of a Simple Lexical Analyzer (continued)
Description of Open-Addressed Hash Tables 161
Description of Open-Addressed
Hash Tables
In a chained hash table, elements reside in buckets extending from each position.
In an open-addressed hash table, on the other hand, all elements reside in the table
itself. This may be important for some applications that rely on the table being a
fixed size. Without a way to extend the number of elements at each position, however, an open-addressed hash table needs another way to resolve collisions.
Collision Resolution
Whereas chained hash tables have an inherent means of resolving collisions, openaddressed hash tables must handle them in a different way. The way to resolve
collisions in an open-addressed hash table is to probe the table. To insert an element, for example, we probe positions until we find an unoccupied one, and
insert the element there. To remove or look up an element, we probe positions
until the element is located or until we encounter an unoccupied position. If we
encounter an unoccupied position before finding the element, or if we end up traversing all of the positions, the element is not in the table.
Of course, the goal is to minimize how many probes we have to perform. Exactly
how many positions we end up probing depends primarily on two things: the load
factor of the hash table and the degree to which elements are distributed uniformly. Recall that the load factor of a hash table is α = n/m, where n is the number of elements and m is the number of positions into which the elements may be
hashed. Notice that since an open-addressed hash table cannot contain more elements than the number of positions in the table (n > m), its load factor is always
less than or equal to 1. This makes sense, since no position can ever contain more
than one element.
Assuming uniform hashing, the number of positions we can expect to probe in an
open-addressed hash table is:
For an open-addressed hash table that is half full (whose load factor is 0.5), for
example, the number of positions we can expect to probe is 1/(1 – 0.5) = 2.
Table 8-1 illustrates how dramatically the expected number of probes increases as
the load factor of an open-addressed hash table approaches 1 (or 100%), at which
point the table is completely full. In a particularly time-sensitive application, it may
be advantageous to increase the size of the hash table to allow extra space for
probing.
1 1 ⁄ ( ) – α
162 Chapter 8: Hash Tables
How close we come to the figures presented in Table 8-1 depends on how closely
we approximate uniform hashing. Just as in a chained hash table, this depends on
how well we select our hash function. In an open-addressed hash table, however,
this also depends on how we probe subsequent positions in the table when collisions occur. Generally, a hash function for probing positions in an open-addressed
hash table is defined by:
where k is a key, i is the number of times the table has been probed thus far, and
x is the resulting hash coding. Typically, h makes use of one or more auxiliary
hash functions selected for the same properties as presented for chained hash
tables. However, for an open-addressed hash table, h must possess an additional
property: as i increases from 0 to m – 1, where m is the number of positions in the
hash table, all positions in the table must be visited before any position is visited
twice; otherwise, not all positions will be probed.
Linear probing
One simple approach to probing an open-addressed hash table is to probe successive positions in the table. Formally stated, if we let i go between 0 and m – 1,
where m is the number of positions in the table, a hash function for linear probing is defined as:
The function h' is an auxiliary hash function, which is selected like any hash function; that is, so that elements are distributed in a uniform and random manner. For
example, we might choose to use the division method of hashing and let h'(k) = k
mod m. In this case, if we hash an element with key k = 2998 into a table of size
m = 1000, the hash codings produced are (998 + 0) mod 1000 = 998 when i = 0,
(998 + 1) mod 1000 = 999 when i = 1, (998 + 2) mod 1000 = 0 when i = 2, and so
on. Therefore, to insert an element with key k = 2998, we would look for an unoccupied position first at position 998, then 999, then 0, and so on.
The advantage of linear probing is that it is simple and there are no constraints on
m to ensure that all positions will eventually be probed. Unfortunately, linear
probing does not approximate uniform hashing very well. In particular, linear
Table 8-1. Expected Probes as a Result of Load Factor, Assuming Uniform Hashing
Load Factor (%) Expected Probes
< 50 < 1 / (1 – 0.50) = 2
 80 1 / (1 – 0.80) = 5
 90 1 / (1 – 0.90) = 10
 95 1 / (1 – 0.95) = 20
hki ( ) , = x
hki ( ) , = ( ) h′( ) k + i mod m
Description of Open-Addressed Hash Tables 163
probing suffers from a phenomenon known as primary clustering, in which large
chains of occupied positions begin to develop as the table becomes more and
more full. This results in excessive probing (see Figure 8-2).
Double hashing
One of the most effective approaches for probing an open-addressed hash table
focuses on adding the hash codings of two auxiliary hash functions. Formally
stated, if we let i go between 0 and m – 1, where m is the number of positions in
the table, a hash function for double hashing is defined as:
The functions h1 and h2 are auxiliary hash functions, which are selected like any
hash function: so that elements are distributed in a uniform and random manner.
However, in order to ensure that all positions in the table are visited before any
position is visited twice, we must adhere to one of the following procedures: we
must select m to be a power of 2 and make h2 always return an odd value, or we
must make m prime and design h2 so that it always returns a positive integer less
than m.
Typically, we let h1(k) = k mod m and h2(k)=1+(k mod m'), where m' is slightly
less than m, say, m – 1 or m – 2. Using this approach, for example, if the hash table
contains m = 1699 positions (a prime number) and we hash the key k = 15,385, the
Figure 8-2. Linear probing with h(k, i) = (k mod 11 + i) mod 11
78
1 After inserting k = 37, 83, 97, 78, 14 with no collisions
14 37 83 97
0 10
78
2 Inserting an element with k = 59; i = 0, 1
14 37 83 97
78
3 Inserting an element with k = 25; i = 0, 1, 2, 3, 4
14 37 83 97
78
4 Inserting an element with k = 72; i = 0, 1, 2
14 37 83 97
59
59
59 25
25
59 25 72
72
hki ( ) , h1( ) k ih2 = ( ) + ( ) k mod m
164 Chapter 8: Hash Tables
positions probed are (94 + (0)(113)) mod 1699 = 94 when i = 0, and every 113th
position after this as i increases.
The advantage of double hashing is that it is one of the best forms of probing,
producing a good distribution of elements throughout a hash table (see
Figure 8-3). The disadvantage is that m is constrained in order to ensure that all
positions in the table will be visited in a series of probes before any position is
probed twice.
Interface for Open-Addressed
Hash Tables
ohtbl_init
int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key)
 int (*h2)(const void *key), int (*match)(const void *key1
 const void *key2), void (*destroy)(void *data));
Return Value 0 if initializing the hash table is successful, or –1 otherwise.
Description Initializes the open-addressed hash table specified by htbl. This
operation must be called for an open-addressed hash table before the hash table
can be used with any other operation. The number of positions to be allocated in
the hash table is specified by positions. The function pointers h1 and h2 specify
Figure 8-3. Hashing the same keys as Figure 8-2 but with double hashing, where h(k, i) =
(k mod 11 + i(1 + k mod 9)) mod 11
78
1 After inserting k = 37, 83, 97, 78, 14 with no collisions
14 37 83 97
0 10
78
2 Inserting an element with k = 59; i = 0, 1
14 37 83 97
78
3 Inserting an element with k = 25; i = 0, 1
14 37 83 97
78
4 Inserting an element with k = 72; i = 0, 1
14 37 83 97
59
25
72
72
59
25
25
59
59
Interface for Open-Addressed Hash Tables 165
user-defined auxiliary hash functions for double hashing. The function pointer
match specifies a user-defined function to determine if two keys match. It should
perform in a manner similar to that described for chtbl_init. The destroy argument provides a way to free dynamically allocated data when ohtbl_destroy is
called. It works in a manner similar to that described for chtbl_destroy. For an
open-addressed hash table containing data that should not be freed, destroy
should be set to NULL.
Complexity O(m), where m is the number of positions in the hash table.
ohtbl_destroy
void ohtbl_destroy(OHTbl *htbl);
Return Value None.
Description Destroys the open-addressed hash table specified by htbl. No
other operations are permitted after calling ohtbl_destroy unless ohtbl_init is called
again. The ohtbl_destroy operation removes all elements from a hash table and
calls the function passed as destroy to ohtbl_init once for each element as it is
removed, provided destroy was not set to NULL.
Complexity O(m), where m is the number of positions in the hash table.
ohtbl_insert
int ohtbl_insert(OHTbl *htbl, const void *data);
Return Value 0 if inserting the element is successful, 1 if the element is already
in the hash table, or –1 otherwise.
Description Inserts an element into the open-addressed hash table specified
by htbl. The new element contains a pointer to data, so the memory referenced
by data should remain valid as long as the element remains in the hash table. It is
the responsibility of the caller to manage the storage associated with data.
Complexity O(1)
ohtbl_remove
int ohtbl_remove(OHTbl *htbl, void **data);
Return Value 0 if removing the element is successful, or –1 otherwise.
Description Removes the element matching data from the open-addressed
hash table specified by htbl. Upon return, data points to the data stored in the
element that was removed. It is the responsibility of the caller to manage the storage associated with the data.
Complexity O(1)
166 Chapter 8: Hash Tables
ohtbl_lookup
int ohtbl_lookup(const OHTbl *htbl, void **data);
Return Value 0 if the element is found in the hash table, or –1 otherwise.
Description Determines whether an element matches data in the openaddressed hash table specified by htbl. If a match is found, upon return data
points to the matching data in the hash table.
Complexity O(1)
ohtbl_size
int ohtbl_size(const OHTbl *htbl);
Return Value Number of elements in the hash table.
Description Macro that evaluates to the number of elements in the openaddressed hash table specified by htbl.
Complexity O(1)
Implementation and Analysis
of Open Addressed Hash Tables
An open-addressed hash table fundamentally consists of a single array. The structure OHTbl is the open-addressed hash table data structure (see Example 8-6). This
structure consists of eight members: positions is the number of positions allocated in the hash table; vacated is a pointer that will be initialized to a special
storage location to indicate that a particular position in the table has had an
element removed from it; h1, h2, match, and destroy are members used to
encapsulate the functions passed to ohtbl_init; size is the number of elements
currently in the table; and table is the array in which the elements are stored.
The vacated member requires a bit of discussion. Its purpose is to support the
removal of elements. An unoccupied position in an open-addressed hash table
usually contains a NULL pointer. However, when we remove an element, we cannot set its data pointer back to NULL because when probing to look up a subsequent element, NULL would indicate that the position is unoccupied and no more
probes should be performed. In actuality, one or more elements may have been
inserted by probing past the removed element while it was still in the table.
Considering this, we set the data pointer to the vacated member of the hash table
data structure when we remove an element. The address of vacated serves as a
special sentinel to indicate that a new element may be inserted at the position.
This way, when probing to look up an element, we are assured that a NULL really
means to stop probing.
Implementation and Analysis of Open Addressed Hash Tables 167
Example 8-6. Header for the Open-Addressed Hash Table Abstract Datatype
/*****************************************************************************
* *
* ------------------------------- ohtbl.h -------------------------------- *
* *
*****************************************************************************/
#ifndef OHTBL_H
#define OHTBL_H
#include <stdlib.h>
/*****************************************************************************
* *
* Define a structure for open-addressed hash tables. *
* *
*****************************************************************************/
typedef struct OHTbl_ {
int positions;
void *vacated;
int (*h1)(const void *key);
int (*h2)(const void *key);
int (*match)(const void *key1, const void *key2);
void (*destroy)(void *data);
int size;
void **table;
} OHTbl;
/*****************************************************************************
* *
* --------------------------- Public Interface --------------------------- *
* *
*****************************************************************************/
int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key), int
 (*h2)(const void *key), int (*match)(const void *key1, const void *key2),
 void (*destroy)(void *data));
void ohtbl_destroy(OHTbl *htbl);
int ohtbl_insert(OHTbl *htbl, const void *data);
int ohtbl_remove(OHTbl *htbl, void **data);
int ohtbl_lookup(const OHTbl *htbl, void **data);
#define ohtbl_size(htbl) ((htbl)->size)
#endif
168 Chapter 8: Hash Tables
ohtbl_init
The ohtbl_init operation initializes an open-addressed hash table so that it can be
used in other operations (see Example 8-7). Initializing an open-addressed hash
table is a simple operation in which we allocate space for the table; initialize the
pointer in each position to NULL; encapsulate the h1, h2, match and destroy
functions; initialize vacated to its sentinel address; and set the size member to 0.
The runtime complexity of ohtbl_init is O(m), where m is the number of positions in the table. This is because the data pointer in each of the m positions must
be initialized to NULL, and all other parts of the operation run in a constant
amount of time.
ohtbl_destroy
The ohtbl_destroy operation destroys an open-addressed hash table (see
Example 8-7). Primarily this means freeing the memory ohtbl_init allocated for the
table. The function passed as destroy to ohtbl_init is called once for each element as it is removed, provided destroy was not set to NULL.
The runtime complexity of ohtbl_destroy is O(m), where m is the number of positions in the hash table. This is because we must traverse all positions in the hash
table to determine which are occupied. If destroy is NULL, ohtbl_destroy runs in
O(1) time.
ohtbl_insert
The ohtbl_insert operation inserts an element into an open-addressed hash table
(see Example 8-7). Since an open-addressed hash table has a fixed size, we first
ensure that there is room for the new element to be inserted. Also, since a key is
not allowed to be inserted into the hash table more than once, we call ohtbl_
lookup to make sure the table does not already contain the new element.
Once these conditions are met, we use double hashing to probe the table for an
unoccupied position. A position in the table is unoccupied if it points either to
NULL or the address in vacated, a special member of the hash table data structure that indicates that a position has had an element removed from it. Once we
find an unoccupied position in the table, we set the pointer at that position to
point to the data we wish to insert. After this, we increment the table size.
Assuming we approximate uniform hashing well and the load factor of the hash
table is relatively small, the runtime complexity of ohtbl_insert is O(1). This is
because in order to find an unoccupied position at which to insert the element, we
expect to probe 1/(1 – α) positions, a number treated as a small constant, where α
is the load factor of the hash table.
Implementation and Analysis of Open Addressed Hash Tables 169
ohtbl_remove
The ohtbl_remove operation removes an element from an open-addressed hash
table (see Example 8-7). To remove the element, we use double hashing as in
ohtbl_insert to locate the position at which the element resides. We continue
searching until we locate the element or NULL is found. If we find the element, we
set data to the data being removed and decrease the table size by 1. Also, we set
the position in the table to the vacated member of the hash table data structure.
Assuming we approximate uniform hashing well, the runtime complexity of ohtbl_
remove is O(1). This is because we expect to probe 1/(1 – α) positions, a number
treated as a small constant, where α is the largest load factor of the hash table
since calling ohtbl_init. The reason that the performance of this operation depends
on the largest load factor and thus does not improve as elements are removed is
that we must still probe past vacated positions. The use of the vacated member
only improves the performance of ohtbl_insert.
ohtbl_lookup
The ohtbl_lookup operation searches for an element in an open-addressed hash
table and returns a pointer to it (see Example 8-7). This operation works similarly
to ohtbl_remove, except that the element is not removed from the table.
Assuming we approximate uniform hashing well, the runtime complexity of ohtbl_
lookup is the same as ohtbl_remove, or O(1). This is because we expect to probe
1/(1 – α) positions, a number treated as a small constant, where α is the largest
load factor of the hash table since calling ohtbl_init. The reason that performance
depends on the largest load factor since calling ohtbl_init is the same as described
for ohtbl_remove.
ohtbl_size
This macro evaluates to the number of elements in an open-addressed hash table
(see Example 8-6). It works by accessing the size member of the OHTbl structure.
The runtime complexity of ohtbl_size is O(1) because accessing a member of a
structure is a simple task that runs in a constant amount of time.
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype
/*****************************************************************************
* *
* ------------------------------- ohtbl.c -------------------------------- *
* *
*****************************************************************************/
170 Chapter 8: Hash Tables
#include <stdlib.h>
#include <string.h>
#include "ohtbl.h"
/*****************************************************************************
* *
* Reserve a sentinel memory address for vacated elements. *
* *
*****************************************************************************/
static char vacated;
/*****************************************************************************
* *
* ------------------------------ ohtbl_init ------------------------------ *
* *
*****************************************************************************/
int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key), int
 (*h2)(const void *key), int (*match)(const void *key1, const void *key2),
 void (*destroy)(void *data)) {
int i;
/*****************************************************************************
* *
* Allocate space for the hash table. *
* *
*****************************************************************************/
if ((htbl->table = (void **)malloc(positions * sizeof(void *))) == NULL)
 return -1;
/*****************************************************************************
* *
* Initialize each position. *
* *
*****************************************************************************/
htbl->positions = positions;
for (i = 0; i < htbl->positions; i++)
 htbl->table[i] = NULL;
/*****************************************************************************
* *
* Set the vacated member to the sentinel memory address reserved for this. *
* *
*****************************************************************************/
htbl->vacated = &vacated;
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
Implementation and Analysis of Open Addressed Hash Tables 171
/*****************************************************************************
* *
* Encapsulate the functions. *
* *
*****************************************************************************/
htbl->h1 = h1;
htbl->h2 = h2;
htbl->match = match;
htbl->destroy = destroy;
/*****************************************************************************
* *
* Initialize the number of elements in the table. *
* *
*****************************************************************************/
htbl->size = 0;
return 0;
}
/*****************************************************************************
* *
* ---------------------------- ohtbl_destroy ----------------------------- *
* *
*****************************************************************************/
void ohtbl_destroy(OHTbl *htbl) {
int i;
if (htbl->destroy != NULL) {
 /**************************************************************************
 * *
 * Call a user-defined function to free dynamically allocated data. *
 * *
 **************************************************************************/
 for (i = 0; i < htbl->positions; i++) {
 if (htbl->table[i] != NULL && htbl->table[i] != htbl->vacated)
 htbl->destroy(htbl->table[i]);
 }
}
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
172 Chapter 8: Hash Tables
/*****************************************************************************
* *
* Free the storage allocated for the hash table. *
* *
*****************************************************************************/
free(htbl->table);
/*****************************************************************************
* *
* No operations are allowed now, but clear the structure as a precaution. *
* *
*****************************************************************************/
memset(htbl, 0, sizeof(OHTbl));
return;
}
/*****************************************************************************
* *
* ----------------------------- ohtbl_insert ----------------------------- *
* *
*****************************************************************************/
int ohtbl_insert(OHTbl *htbl, const void *data) {
void *temp;
int position,
 i;
/*****************************************************************************
* *
* Do not exceed the number of positions in the table. *
* *
*****************************************************************************/
if (htbl->size == htbl->positions)
 return -1;
/*****************************************************************************
* *
* Do nothing if the data is already in the table. *
* *
*****************************************************************************/
temp = (void *)data;
if (ohtbl_lookup(htbl, &temp) == 0)
 return 1;
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
Implementation and Analysis of Open Addressed Hash Tables 173
/*****************************************************************************
* *
* Use double hashing to hash the key. *
* *
*****************************************************************************/
for (i = 0; i < htbl->positions; i++) {
 position = (htbl->h1(data) + (i * htbl->h2(data))) % htbl->positions;
 if (htbl->table[position] == NULL || htbl->table[position] == htbl->
 vacated) {
 /***********************************************************************
 * *
 * Insert the data into the table. *
 * *
 ***********************************************************************/
 htbl->table[position] = (void *)data;
 htbl->size++;
 return 0;
 }
}
/*****************************************************************************
* *
* Return that the hash functions were selected incorrectly. *
* *
*****************************************************************************/
return -1;
}
/*****************************************************************************
* *
* ----------------------------- ohtbl_remove ----------------------------- *
* *
*****************************************************************************/
int ohtbl_remove(OHTbl *htbl, void **data) {
int position,
 i;
/*****************************************************************************
* *
* Use double hashing to hash the key. *
* *
*****************************************************************************/
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
174 Chapter 8: Hash Tables
for (i = 0; i < htbl->positions; i++) {
 position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
 if (htbl->table[position] == NULL) {
 /***********************************************************************
 * *
 * Return that the data was not found. *
 * *
 ***********************************************************************/
 return -1;
 }
 else if (htbl->table[position] == htbl->vacated) {
 /***********************************************************************
 * *
 * Search beyond vacated positions. *
 * *
 ***********************************************************************/
 continue;
 }
 else if (htbl->match(htbl->table[position], *data)) {
 /***********************************************************************
 * *
 * Pass back the data from the table. *
 * *
 ***********************************************************************/
 *data = htbl->table[position];
 htbl->table[position] = htbl->vacated;
 htbl->size--;
 return 0;
 }
}
/*****************************************************************************
* *
* Return that the data was not found. *
* *
*****************************************************************************/
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
Implementation and Analysis of Open Addressed Hash Tables 175
return -1;
}
/*****************************************************************************
* *
* ----------------------------- ohtbl_lookup ----------------------------- *
* *
*****************************************************************************/
int ohtbl_lookup(const OHTbl *htbl, void **data) {
int position,
 i;
/*****************************************************************************
* *
* Use double hashing to hash the key. *
* *
*****************************************************************************/
for (i = 0; i < htbl->positions; i++) {
 position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
 if (htbl->table[position] == NULL) {
 /***********************************************************************
 * *
 * Return that the data was not found. *
 * *
 ***********************************************************************/
 return -1;
 }
 else if (htbl->match(htbl->table[position], *data)) {
 /***********************************************************************
 * *
 * Pass back the data from the table. *
 * *
 ***********************************************************************/
 *data = htbl->table[position];
 return 0;
 }
}
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
176 Chapter 8: Hash Tables
Questions and Answers
Q: In the implementation of chained hash tables presented in this chapter, the
actual hash code used for accessing the table is the hash code modulo the table
size. Why is this?
A: This transformation ensures that the hash coding does not position us past the
end of the table. Although the hash function should ensure this itself, it is
worthwhile for the hash table implementation to provide the guarantee as
well, especially since the hash function is provided by the caller. However,
this is not the same reason that the modulo is performed when double hashing a key in an open-addressed hash table. In this case, the process of double
hashing may produce a hash coding that falls outside of the bounds of the
table, even for two auxiliary hash functions each producing hash codings
within the table. This is because the two hash codings are added together.
Q: Why are hash tables good for random access but not sequential access? For
example, in a database system in which records are to be accessed in a sequential fashion, what is the problem with hashing?
A: Hash tables are excellent for random access because each key hashes us precisely to where we need to be in the table to access the data, or at least within
a few steps when a collision occurs. However, hash tables do not support
sequential access. After hashing to some position, we have no way to determine where the next smallest or largest key resides. Compare this with a
linked list containing elements that are sorted. Assuming some initial position
in the list, the next key is easy to determine: we simply look at the next element in the list.
Q: What is the worst-case performance of searching for an element in a chained
hash table? How do we ensure that this case will not occur?
A: A chained hash table performs the worst when all elements hash into a single
bucket. In this case, searching for an element is O(n), where n is the number
of elements in the table. A ridiculous hash function that would result in this
/*****************************************************************************
* *
* Return that the data was not found. *
* *
*****************************************************************************/
return -1;
}
Example 8-7. Implementation of the Open-Addressed Hash Table
Abstract Datatype (continued)
Related Topics 177
performance is h(k) = c, where c is some constant within the bounds of the
hash table. Selecting a good hash function ensures that this case will not
occur. If the hash function approximates uniform hashing well, we can expect
to locate an element in constant time.
Q: What is the worst-case performance of searching for an element in an openaddressed hash table? How do we ensure that this case will not occur?
A: The worst-case performance of searching for an element in an open-addressed
hash table occurs once the hash table is completely full and the element we
are searching for is not in the table. In this case, searching for an element is an
O(m) operation, where m is the number of positions in the table. This case
can occur with any hash function. To ensure reasonable performance in an
open-addressed hash table, we should not let the table become more than
80% full. If we choose a hash function that approximates uniform hashing
well, we can expect performance consistent with what is presented in
Table 8-1.
Related Topics
Direct-address tables
A simple type of hash table in which there is a one-to-one mapping between
all possible keys and positions in the table. Since no two keys map to the
same position, there is no need for collision resolution. However, if there are
many possible keys, the table will be large. Generally, direct addressing works
well when the universe of possible keys is small.
Linear congruential generators
A common class of random number generators. Understanding the principles
behind random number generators can help in devising good hash functions.
Quadratic probing
An alternative to linear probing and double hashing for probing an openaddressed hash table. In quadratic probing, the sequence of positions probed
is determined using a quadratic-form hash function. In general, quadratic
probing performs better than linear probing, but it does not perform as well as
double hashing. Quadratic probing results in secondary clustering, a form of
clustering that is less severe than the primary clustering of linear probing.
Universal hashing
A hashing method in which hashing functions are generated randomly at runtime so that no particular set of keys is likely to produce a bad distribution of
elements in the hash table. Because the hash functions are generated randomly, even hashing the same set of keys during different executions may
result in different measures of performance.

