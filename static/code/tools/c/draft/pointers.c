#include <stdio.h>
#include <stddef.h>

// a pointer is a variable that contains the address of a memory location

int main() {
  int i = 1;

  // a pointer is typically declared with a type corresponding to what it points to though nothing inherent in a pointer indicates what type of data the pointer is referencing as it only contains an address
  int *ptr1;

  // an uninitialized pointer will contain garbage and should not be used

  // be careful when declaring multiple pointers on a single line as ptr2 is an int here
  // int *ptr1, ptr2;

  // here both ptr1 and ptr2 are pointers
  // int *ptr1, *ptr2;

  // compare this to declaring multiple pointres using a typdef
  // typedef int* PINT;
  // PINT ptr1, ptr2;

  // the size of a pointer depends on the machine in use and the compiler though on modern systems the size of a pointer to data is normally the same regardless of the pointer type
  printf("%p\n", sizeof(char *));    // 
  printf("%p\n", sizeof(int *));     // 
  printf("%p\n", sizeof(double *));  // 

  // the address-of operator gives the address of an object in memory
  ptr1 = &i;

  // one approach for printing pointers in a cross-platform way is to cast the pointer as a pointer to a void pointer and then use the %p format specifier
  printf("%p\n", (void *) ptr1);  // 0x7ffd77107c50

  // the indirection operator returns the value pointed to by the pointer (i.e. dereferences the pointer)
  *ptr1 = *ptr1 + 1;
  printf("%d\n", *ptr1);  // 2


  // a pointer set to NULL is called a null pointer and does not point to anything
  ptr1 = NULL;

  // zero is overloaded and refers to the null pointer
  int j = 2;
  int *ptr2 = &j;
  *ptr2 = 0;          // integer zero
  ptr2 = 0;           // null pointer
  printf("%d\n", j);  // 0

  // two null pointers are always equal
  printf("%d\n", ptr1 == ptr2);  // 1

  



  // pointers can use different levels of indirection and so a pointer may point to another pointer
  char *arr1[] = {"a", "b", "c", "d"};

  // array of pointers to pointers
  char **arr2[3];

  arr2[0] = &arr1[1];
  arr2[1] = &arr1[2];
  arr2[2] = &arr1[3;

  printf("%s\n",*arr2[1]);  // TODO


  // pointer to an array of length 5
  int (*a)[5];

  typedef int vec5[5];
  vec5 *a;

}

// SOME STUFF ABOUT NULL POINTERS VVV

// // Q: What is this infamous null pointer, anyway?

// A: The language definition states that for each pointer type, there is a special value--the ``null pointer''--which is distinguishable from all other pointer values and which is ``guaranteed to compare unequal to a pointer to any object or function.''

// A null pointer is known not to point to any object or function; an uninitialized pointer might point anywhere.


// // Q: How do I get a null pointer in my programs?

// A: With a null pointer constant.

// In an initialization, assignment, or comparison when one side is a variable or expression of pointer type, the compiler can tell that a constant 0 on the other side requests a null pointer, and generate the correctly-typed null pointer value.

// However, an argument being passed to a function is not necessarily recognizable as a pointer context, and the compiler may not be able to tell that an unadorned 0 ``means'' a null pointer. To generate a null pointer in a function call context, an explicit cast may be required, to force the 0 to be recognized as a pointer.


// // Q: What is NULL and how is it defined?

// Therefore, the preprocessor macro NULL is defined (by several headers, including <stdio.h> and <stddef.h>) as a null pointer constant, typically 0 or ((void *)0). A programmer who wishes to make explicit the distinction between 0 the integer and 0 the null pointer constant can then use NULL whenever a null pointer is required.

// A constant 0 is a null pointer constant; NULL is just a convenient name for it.


// // Q: Is the abbreviated pointer comparison ``if(p)'' to test for non-null pointers valid? What if the internal representation for null pointers is nonzero?

// A: It is always valid.

// When C requires the Boolean value of an expression, a false value is inferred when the expression compares equal to zero, and a true value otherwise.

// 	if(p)	is equivalent to	if(p != 0)
// and this is a comparison context, so the compiler can tell that the (implicit) 0 is actually a null pointer constant, and use the correct null pointer value.



// // Q: This is strange. NULL is guaranteed to be 0, but the null pointer is not?

// A: When the term ``null'' or ``NULL'' is casually used, one of several things may be meant:

// The conceptual null pointer, the abstract language concept defined in question 5.1. It is implemented with...
// The internal (or run-time) representation of a null pointer, which may or may not be all-bits-0 and which may be different for different pointer types. The actual values should be of concern only to compiler writers. Authors of C programs never see them, since they use...
// The null pointer constant, which is a constant integer 0 [footnote] (see question 5.2). It is often hidden behind...
// The NULL macro, which is #defined to be 0 (see question 5.4). 


// // Q: I'm confused. I just can't understand all this null pointer stuff.

// A: Here are two simple rules you can follow:

// When you want a null pointer constant in source code, use ``0'' or ``NULL''.
// If the usage of ``0'' or ``NULL'' is an argument in a function call, cast it to the pointer type expected by the function being called.


// // Q: If NULL and 0 are equivalent as null pointer constants, which should I use?

// A: Many programmers believe that NULL should be used in all pointer contexts, as a reminder that the value is to be thought of as a pointer. Others feel that the confusion surrounding NULL and 0 is only compounded by hiding 0 behind a macro, and prefer to use unadorned 0 instead. There is no one right answer.

// C programmers must understand that NULL and 0 are interchangeable in pointer contexts, and that an uncast 0 is perfectly acceptable. Any usage of NULL (as opposed to 0) should be considered a gentle reminder that a pointer is involved; programmers should not depend on it (either for their own understanding or the compiler's) for distinguishing pointer 0's from integer 0's.

// It is only in pointer contexts that NULL and 0 are equivalent. NULL should not be used when another kind of 0 is required, even though it might work, because doing so sends the wrong stylistic message. (Furthermore, ANSI allows the definition of NULL to be ((void *)0), which will not work at all in non-pointer contexts.) In particular, do not use NULL when the ASCII null character (NUL) is desired. Provide your own definition

// 	#define NUL '\0'
// if you must.