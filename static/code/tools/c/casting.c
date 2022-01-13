
int main() {

  // casting an integer to a pointer allows access to an arbitrary address and should only be done when we know this address is valid
  int i = 8;
  int *ptr = (int *) i;
  // printf("%d\n", *ptr);  // error

  // casting can be used to determine the endianess by showing the order of bytes in memory
  int i = 0x12345678;
  char* ptr = (char *) &i;
  for (int i = 0; i < 4; i++) {
    printf("%p: %02x \n", (void *) ptr, (unsigned char) *ptr++);
  }


}






// one pointer is said to alias another pointer if they both reference the same memory location

int num = 5;
int* p1 = &num;
int* p2 = &num;


// strict aliasing is a form of aliasing that does not allow a pointer of one data type to alias a pointer of a different data type

// strict aliasing does not apply to pointers differing only by sign or qualifier so this is valid
int i;
const int *ptr1 = &i;
int *ptr2 = &i;
int volatile ptr3 = &i;


// compilers will assume that pointers may be aliased though this can result in inefficient code generation

// the restrict keyword allows the compiler to perform better optimization

// The GCC compiler
// has the following compiler options:
// • -fno-strict-aliasing to turn it off
// • -fstrict-aliasing to turn it on
// • -Wstrict-aliasing to warn of strict aliasing-related problems



// Using a Union to Represent a Value in Multiple Ways

To illustrate this
technique, we will use three different functions. These will determine whether a floating point number is positive.

The first function shown below uses a union of a float and an unsigned integer. The
function first assigns the floating point value to the union and then extracts the integer
to perform the test:

typedef union _conversion {
 float fNum;
 unsigned int uiNum;
} Conversion;

int isPositive1(float number) {
 Conversion conversion = { .fNum =number};
 return (conversion.uiNum & 0x80000000) == 0;
}

This will work correctly and does not involve aliasing because no pointers are involved.
The next version uses a union that contains pointers to the two data types. The floating
point number’s address is assigned to the first pointer. The integer’s pointer is then
dereferenced to perform the test. This violates the strict aliasing rule:
typedef union _conversion2 {
 float *fNum;
 unsigned int *uiNum;
} Conversion2;
int isPositive2(float number) {
 Conversion2 conversion;
 conversion.fNum =&number;
 return (*conversion.uiNum & 0x80000000) == 0;
}
The following function does not use a union and violates the strict aliasing rule since
the ptrValue pointer shares the same address as number:
int isPositive3(float number) {
 unsigned int *ptrValue = (unsigned int *)&number;
 return (*ptrValue & 0x80000000) == 0;
}
The approach used by these functions assumes:
• The IEEE-754 floating point standard is used to represent a floating point number
• The floating number is laid out in a particular manner
• Integer and floating point pointers are aligned correctly
However, these assumptions are not always valid. While approaches such as this can
optimize operations, they are not always portable. When portability is important, per‐
forming a floating point comparison is a better approach.


Strict Aliasing

A compiler does not enforce strict aliasing. It will only generate warnings. The compiler
assumes that two or more pointers of different types will never reference the same object.
This includes pointers to structures with different names but that are otherwise iden‐
tical. With strict aliasing, the compiler is able to perform certain optimizations. If the
assumption is incorrect, then unexpected results may occur.
Even if two structures have the same field but different names, two pointers to these
structures should never reference the same object. In the following example, it is as‐
sumed the person and employee pointers will never reference the same object:
typedef struct _person {
 char* firstName;
 char* lastName;
 unsigned int age;
} Person;
typedef struct _employee {
 char* firstName;
 char* lastName;
 unsigned int age;
} Employee;
Person* person;
Employee* employee;

However, the pointers can reference the same object if the structure definitions differ
only by their name, as illustrated below:
typedef struct _person {
 char* firstName;
 char* lastName;
 unsigned int age;
} Person;
typedef Person Employee;
Person* person;
Employee* employee;
Using the restrict Keyword
C compilers assume pointers are aliased by default. Using the restrict keyword when
declaring a pointer tells the compiler that the pointer is not aliased. This allows the
compiler to generate more efficient code. Frequently, this is achieved by caching the
pointer. Bear in mind that this is only a recommendation. The compiler may decide not
to optimize the code. If aliases are used, then the code’s execution will result in undefined
behavior. The compiler will not provide any warning when the assumption is violated.
New code development should use the restrict keyword with most
pointer declarations. This will enable better code optimization. Modi‐
fying existing code may not be worth the effort.
The following function illustrates the definition and use of the restrict keyword. The
function adds two vectors together and stores the result in the first vector:
void add(int size, double * restrict arr1, const double * restrict arr2) {
 for (int i = 0; i < size; i++) {
 arr1[i] += arr2[i];
 }
}
The restrict keyword is used with both array parameters, but they should not both
reference the same block of memory. The following shows the correct usage of the
function:
 double vector1[] = {1.1, 2.2, 3.3, 4.4};
 double vector2[] = {1.1, 2.2, 3.3, 4.4};

 add(4,vector1,vector2);
In the following sequence, the function is called improperly with the same vector being
passed as both parameters. The first statement uses an alias while the second statement
uses the same vector twice:

double vector1[] = {1.1, 2.2, 3.3, 4.4};
 double *vector3 = vector1;
 add(4,vector1,vector3);
 add(4,vector1,vector1);
Though it may sometimes work correctly, the results of the function invocation may
not be reliable.
Several standard C functions use the restrict keyword, including:
• void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
• char *strcpy(char * restrict s1, const char * restrict s2);
• char *strncpy(char * restrict s1, const char * restrict s2, size_t n);
• int printf(const char * restrict format, ... );
• int sprintf(char * restrict s, const char * restrict format, ... );
• int snprintf(char * restrict s, size_t n, const char * restrict for
mat, ... );
• int scanf(const char * restrict format, ...);
The restrict keyword has two implications:
1. To the compiler it means it can perform certain code optimizations
2. To the programmer it means these pointers should not be aliased; otherwise, the
results of the operation are undefined.





