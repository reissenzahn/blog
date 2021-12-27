
Strings can be allocated to different regions of memory and pointers are commonly
used to support string operations. Pointers support the dynamic allocation of strings
and passing strings to a function. A good understanding of pointers and their use with
strings enables programmers to develop valid and efficient applications.
Strings are a common component of many applications and are a complex topic. In this
chapter, we will explore the various ways of declaring and initializing strings. We will
examine the use of literal pools in C applications and their impact. In addition, we will
look at common string operations, such as comparing, copying, and concatenating
strings.
Strings are regularly passed and returned to functions as pointers to char. When we
pass a string, we can do so either as a pointer to a char or a pointer to a constant char.
The latter approach protects the string from modification within the function. Many
examples used in this chapter provide additional illustrations of the concepts developed
in the function chapter. They differ as they do not need to pass their size to a function.
A string may also be returned from a function to fulfill a request. This string may be
passed to the function to be modified or allocated from within the function. We could
also return a statically allocated string. Each of these approaches will be examined.
We will also examine the use of function pointers and how they can assist sorting op‐
erations. Understanding how pointers work in these situations is the primary focus of
this chapter.
String Fundamentals
A string is a sequence of characters terminated with the ASCII NUL character. The ASCII
character NUL is represented as \0. Strings are commonly stored in arrays or in memory
allocated from the heap. However, not all arrays of characters are strings. An array of

char may not contain the NUL character. Arrays of char have been used to represent
smaller integer units, such as boolean, to conserve memory space in an application.
There are two types of strings in C:
Byte string
Consists of a sequence of char data type
Wide string
Consists of a sequence of wchar_t data type
The wchar_t data type is used for wide characters and may be either 16 or 32 bits in
width. Both of these strings are terminated by the NUL character. Byte string functions
are found in the string.h file. Wide string functions are found in the wchar.h file. Unless
otherwise noted, we will be using byte strings in this chapter. Wide chars were created
to support non-Latin character sets and are useful in applications that support foreign
languages.
The length of a string is the number of characters in the string. This does not include
the NUL character. When memory is allocated for a string, remember to allocate enough
memory for all of the characters plus the NUL character.
Remember that NULL and NUL are different. NULL is used as a special
pointer and is typically defined as ((void*)0). NUL is a char and is
defined as '\0'. They should not be used interchangeably.
Character constants are character sequences enclosed in single quotes. Normally, they
consist of a single character but can contain more than one character, as found with
escape sequences. In C, they are of type int. This is demonstrated as follows:
 printf("%d\n",sizeof(char));
 printf("%d\n",sizeof('a'));
When executed, the size of char will be 1 while the character literal’s size will be 4. This
anomaly is an artifact of the language design.
String Declaration
String declarations are supported in one of three ways: either as a literal, as an array of
characters, or using a pointer to a character. The string literal is a sequence of characters
enclosed in double quotes. String literals are frequently used for initialization purposes.
They are located in a string literal pool discussed in the next section.
String literals are not to be confused with characters enclosed in single quotes—these
are character literals. As we will see in later sections, when used in place of string literals,
character literals can cause problems.

An array of characters is illustrated below where we declare a header array whose size
may hold up to 31 characters. Since a string requires the NUL termination character, an
array declared to have 32 characters can only use 31 elements for the actual string’s text.
The string’s location depends on where the declaration is placed. We will explore this
issue in the section “String Initialization” on page 110.
 char header[32];
A pointer to a character is illustrated below. Since it has not been initialized, it does not
reference a string. The string’s length and location are not specified at this time.
 char *header;
The String Literal Pool
When literals are defined they are frequently assigned to a literal pool. This area of
memory holds the character sequences making up a string. When a literal is used more
than once, there is normally only a single copy of the string in the string literal pool.
This will reduce the amount of space needed for the application. Since a literal is nor‐
mally considered to be immutable, it does not hurt to have a single copy of it. However,
it is not a good practice to assume there will only be a single copy or that literals are
immutable. Most compilers provide an option to turn off string pooling. When this
happens, literals may be duplicated, each having their own address.
GCC uses a -fwritable-strings option to turn off string pooling. In
Microsoft Visual Studio, the /GF option will turn on string pooling.
Figure 5-1 illustrates how memory may be allocated for a literal pool.
Figure 5-1. String literal pool


String literals are frequently allocated to read-only memory. This makes them immut‐
able. It doesn’t matter where a string literal is used or whether it is global, static, or local.
In this sense, string literals do not have scope.
When a string literal is not a constant
In most compilers, a string literal is treated as a constant. It is not possible to modify
the string. However, in some compilers, such as GCC, modification of the string literal
is possible. Consider the following example:
 char *tabHeader = "Sound";
 *tabHeader = 'L';
 printf("%s\n",tabHeader); // Displays "Lound"
This will modify the literal to “Lound.” Normally, this is not desirable and should be
avoided. Making the variable a constant as follows will provide a partial solution to this
problem. Any attempt to modify the string will result in a compile-time error.
 const char *tabHeader = "Sound";
String Initialization
When we initialize a string, the approach we use depends on whether the variable is
declared as an array of characters or as a pointer to a character. The memory used for
a string will be either an array or a memory pointed to by a pointer. When a string is
initialized, we can use a string literal or a series of characters, or obtain the characters
from a different source such as standard input. We will examine these approaches.
Initializing an array of char
An array of char can be initialized using the initialization operator. In the following
example, a header array is initialized to the character contained in a string literal:
 char header[] = "Media Player";
Since the literal “Media Player” is 12 characters in length, 13 bytes are required to rep‐
resent the literal. The array is allocated 13 bytes to hold the string. The initialization will
copy these characters to the array terminated by the NUL character, as illustrated in
Figure 5-2, assuming the declaration is located in the main function.
An array can also be initialized using strcpy function, which is discussed in detail in
the section “Copying Strings” on page 116. In the following sequence, the string literal is
copied to the array.
 char header[13];
 strcpy(header,"Media Player");

Figure 5-2. Initializing an array of char
A more tedious technique assigns individual characters as follows:
 header[0] = 'M';
 header[1] = 'e';
 ...
 header[12] = '\0';
The following assignment is invalid. We cannot assign the address of a
string literal to an array name.
 char header2[];
 header2 = "Media Player";
Initializing a pointer to a char
Using dynamic memory allocation provides flexibility and potentially allows the
memory to stay around longer. The following declaration will be used to illustrate this
technique:
 char *header;
A common way to initialize this string is to use the malloc and strcpy functions to
allocate and copy a literal to the string, as illustrated below:
 char *header = (char*) malloc(strlen("Media Player")+1);
 strcpy(header,"Media Player");
Assuming that the code is located in the main function, Figure 5-3 shows the state of
the program stack.

Figure 5-3. Initializing a pointer to a char
In the previous use of the malloc function, we used the strlen function with a string
literal argument. We could have declared its size explicitly as shown below:
 char *header = (char*) malloc(13);
When determining the length of a string to be used with the malloc
function:
• Always remember to add one for the NUL terminator.
• Don’t use sizeof operator. Instead, use the strlen function to de‐
termine the length of an existing string. The sizeof operator will
return the size of an array or pointer, not the length of the string.
Instead of using a string literal and strcpy function to initialize the string, we can use
the following:
 *(header + 0) = 'M';
 *(header + 1) = 'e';
 ...
 *(header + 12) = '\0';
The address of a string literal can be assigned directly to a character pointer as shown
below. However, this does not create a new copy of the string as illustrated in Figure 5-4:
 char *header = "Media Player";

Figure 5-4. Copying a string literal’s address to a pointer
Attempting to initialize a pointer to a char with a character literal will
not work. Since a character literal is of type int, we would be trying to
assign an integer to a character pointer. This will frequently cause the
application to terminate when the pointer is dereferenced:
 char* prefix = '+'; // Illegal
A valid approach using the malloc function follows:
 prefix = (char*)malloc(2);
 *prefix = '+';
 *(prefix+1) = 0;
Initializing a string from standard input
A string can also be initialized from some external source such as standard input. How‐
ever, potential initialization errors can occur when reading in a string from standard
input, as shown below. The problem exists because we have not assigned memory to
the command variable before attempting to use it:
 char *command;
 printf("Enter a Command: ");
 scanf("%s",command);
To address this problem, we should first allocate memory for the pointer or use a fixed
size array instead of a pointer. However, the user may enter more data than can be held
by these approaches. A more robust approach is illustrated in Chapter 4.
Summary of string placement
Strings can be allocated in several potential locations. The following example illustrates
possible variations with Figure 5-5 illustrates how these strings are laid out in memory:
char* globalHeader = "Chapter";
char globalArrayHeader[] = "Chapter";
void displayHeader() {
static char* staticHeader = "Chapter";
 char* localHeader = "Chapter";
 static char staticArrayHeader[] = "Chapter";
 char localArrayHeader[] = "Chapter";
 char* heapHeader = (char*)malloc(strlen("Chapter")+1);
 strcpy(heapHeader,"Chapter");
}
Knowing where a string is located is useful when attempting to understand how a pro‐
gram works and when using pointers to access the strings. A string’s location determines
how long it will persist and which parts of an application can access it. For example,
strings allocated to global memory will always be available and are accessible by multiple
functions. Static strings will always be available but are accessible only to their defining
function. Strings allocated to the heap will persist until they are released and may be
used in multiple functions. Understanding these issues allows you to make informed
decisions.
Figure 5-5. String allocation in memory
Standard String Operations
In this section, we will examine the use of pointers in common string operations. This
includes comparing, copying, and concatenating strings.

Comparing Strings
String comparisons can be an integral part of an application. We will examine the details
of how string comparisons are made, as incorrect comparisons can result in misleading
or invalid results. Understanding how comparisons are made will help you avoid in‐
correct operations. This understanding will transfer to similar situations.
The standard way to compare strings is to use the strcmp function. Its prototype follows:
int strcmp(const char *s1, const char *s2);
Both of the strings being compared are passed as pointers to constant chars. This allows
us to use the function without fear of it modifying the strings passed. This function
returns one of three values:
Negative
If s1 precedes s2 lexicographically (alphabetically)
Zero
If the two strings are equal
Positive
If s1 follows s2 lexicographically
The positive and negative return values are useful for sorting strings in alphabetical
order. The use of this function to test equality is illustrated below. The user’s entry will
be stored in command. This is then compared to the literal string:
 char command[16];
 printf("Enter a Command: ");
 scanf("%s", command);
 if (strcmp(command, "Quit") == 0) {
 printf("The command was Quit");
 } else {
 printf("The command was not Quit");
 }
Memory for this example is allocated as shown in Figure 5-6.
There are a couple of incorrect ways to compare two strings. The first approach shown
below attempts to use the assignment operator to perform the comparison:
 char command[16];

 printf("Enter a Command: ");
 scanf("%s",command);
 if(command = "Quit") {
 ...
First, it does not perform a comparison, and second, this will result in a syntax error
message complaining about incompatible types. We cannot assign the address of a string

Figure 5-6. strcmp example
literal to the array name. In this example, we tried to assign the string literal’s address,
600, to command. Since command is an array, it is not possible to assign a value to this
variable without using array subscripts.
The second approach is to use the equality operator:
 char command[16];

 printf("Enter a Command: ");
 scanf("%s",command);
 if(command == "Quit") {
 ...
This should evaluate false since we are comparing the address of command, 300, with the
string literal’s address, 600. The equality operator compares the addresses, not what is
stored at the addresses. Using an array name or a string literal by themselves will return
their addresses.
Copying Strings
Copying strings is a common operation and is normally accomplished using the strcpy
function whose prototype follows:
char* strcpy(char *s1, const char *s2);
In this section, we will cover the basic copying process and identify common pitfalls.
We will assume there is a need to copy an existing string to a new dynamically allocated
buffer, though we could also have used an array of characters.
A common application is to read in a series of strings and store each of them in an
array using a minimum amount of memory. This can be accomplished by creating an
array sized to handle the largest string that the user might enter and then reading it into
this array. On the basis of the string read in, we can then allocate just the right amount
of memory. The basic approach is to:

1. Read in the string using a large array of char
2. Use malloc to allocate just the right amount of memory
3. Use strcpy to copy the string into the dynamically allocated memory
The following sequence illustrates this technique. The names array will hold pointers to
each name read in. The count variable specifies the next available array element. The
name array is used to hold a string that is read in and is reused for each name read. The
malloc function allocates the memory needed for each string and is assigned to the next
available element of names. The name is then copied into the allocated memory:
 char name[32];
 char *names[30];
 size_t count = 0;

 printf("Enter a name: ");
 scanf("%s",name);
 names[count] = (char*)malloc(strlen(name)+1);
 strcpy(names[count],name);
 count++;
We can repeat the operation within a loop, incrementing count with each iteration.
Figure 5-7 illustrates how memory is laid out for this process after reading in a single
name: “Sam.”
Figure 5-7. Copying a string
Two pointers can reference the same string. When two pointers reference the same
location, this is called aliasing. This topic is covered in Chapter 8. While this is not
necessarily a problem, realize that the assignment of one pointer to another does not
result in the string being copied. Instead, we simply copied the string’s address.

To illustrate this, an array of pointers to page headers is declared below. The
page with index 12 is assigned the address of a string literal. Next, the pointer in
pageHeaders[12] is copied to pageHeaders[13]. Both of these pointers now reference
the same string literal. The pointer is copied, not the string:
 char *pageHeaders[300];

 pageHeaders[12] = "Amorphous Compounds";
 pageHeaders[13] = pageHeaders[12];
These assignments are illustrated in Figure 5-8.
Figure 5-8. Effects of copying pointers
Concatenating Strings
String concatenation involves the merging of two strings. The strcat function is fre‐
quently used for this operation. This function takes pointers to the two strings to be
concatenated and returns a pointer to the concatenated results. The prototype for the
function follows:
char *strcat(char *s1, const char *s2);
The function concatenates the second string to the end of the first string. The second
string is passed as a pointer to a constant char. The function does not allocate memory.
This means the first string must be large enough to hold the concatenated results or it
may write past the end of the string, resulting in unpredictable behavior. The return
value of the function is the same address as its first argument. This can be convenient
in some situations such as when the function is used as an argument of the printf
function.

To illustrate the use of this function, we will combine two error message strings. The
first one is a prefix and the second one is a specific error message. As shown below, we
first need to allocate enough memory for both strings in a buffer, then copy the first
string to the buffer, and finally concatenate the second string with the buffer:
 char* error = "ERROR: ";
 char* errorMessage = "Not enough memory";
 char* buffer = (char*)malloc(strlen(error)+strlen(errorMessage)+1);
 strcpy(buffer,error);
 strcat(buffer, errorMessage);

 printf("%s\n", buffer);
 printf("%s\n", error);
 printf("%s\n", errorMessage);
We added one to the malloc function’s argument to accommodate the NUL character. If
we assume the first literal immediately precedes the second literal in memory, the output
of this sequence will be as follows. Figure 5-9 illustrates how memory is allocated:
ERROR: Not enough memory
ERROR:
Not enough memory
Figure 5-9. Correct copy operation
If we had not allocated a separate memory location for the concatenated string, we would
overwrite the first string. This is illustrated in the following example, where a buffer is
not used. We also assume the first literal immediately precedes the second literal in
memory:
 char* error = "ERROR: ";
 char* errorMessage = "Not enough memory";

 strcat(error, errorMessage);

printf("%s\n", error);
 printf("%s\n", errorMessage);
The output of this sequence follows:
ERROR: Not enough memory
ot enough memory
The errorMessage string has been shifted one character to the left. This is because the
resulting concatenated string is written over errorMessage. Since the literal “Not
enough memory” follows the first literal, the second literal is overwritten. This is illus‐
trated in Figure 5-10, where the literal pool’s state is displayed before and after the copy
operation.
Figure 5-10. Improper string copy operation
We could have used a char array instead of a pointer for the messages, as shown below.
However, this will not always work:
 char error[] = "ERROR: ";
 char errorMessage[] = "Not enough memory";
If we used the following strcpy call, we would get a syntax error. This is because we are
attempting to assign the pointer returned by the function to the name of an array. This
type of operation is illegal:
 error = strcat(error, errorMessage);
If we remove the assignment, as follows, we would likely get a memory access violation,
since the copy operation is overwriting a part of the stack frame. This assumes the array
declarations are in a function, as illustrated in Figure 5-11. Whether the source strings
are stored in the string literal pool or on the stack frame, they should not be used to
directly hold the concatenated result. Always allocate dedicated memory for the
concatenation:
 strcat(error, errorMessage);

Figure 5-11. Overwriting the stack frame
Another simple mistake made when concatenating strings is using a character literal
instead of a string literal. In the following example, we concatenate a string to a path
string. This will work as expected:
 char* path = "C:";
 char* currentPath = (char*) malloc(strlen(path)+2);
 currentPath = strcat(currentPath,"\\");
We add two to the string length in the malloc call because we need space for the extra
character and the NUL character. We are concatenating a single character, the backslash,
since we used an escape sequence in the string literal.
However, if we used a character literal instead, as shown below, we will get a runtime
error when the second argument is mistakenly interpreted as the address of a char:
 currentPath = strcat(path,'\\');
Passing Strings
Passing a string is simple enough. In the function call, use an expression that evaluates
to the address of a char. In the parameter list, declare the parameter as a pointer to a
char. The interesting issues occur when using the string within the function. We will
first examine how to pass a simple string in the first two subsections and then how to
pass a string requiring initialization in the third section. Passing strings as arguments
to an application is covered in the section “Passing Arguments to an Application” on
page 125.
Passing a Simple String
There are several ways of passing the address of a string to a function, depending on
how the string is declared. In this section, we will demonstrate these techniques using
a function that mimics the strlen function as implemented below. We used parentheses
to force the post increment operator to execute first, incrementing the pointer.

Otherwise, the character referenced by string would have been incremented, which is
not what is desired:
size_t stringLength(char* string) {
 size_t length = 0;
 while(*(string++)) {
 length++;
 }
 return length;
}
The string should actually be passed as a pointer to a constant char, as
discussed in the section “Passing a Pointer to a Constant char” on page
123.
Let’s start with the following declarations:
 char simpleArray[] = "simple string";
 char *simplePtr = (char*)malloc(strlen("simple string")+1);
 strcpy(simplePtr, "simple string");
To invoke the function with the pointer, we simply use the pointer’s name:
 printf("%d\n",stringLength(simplePtr));
To invoke the function using the array, we have three choices, as shown below. In the
first statement, we use the array’s name. This will return its address. In the second
statement, the address-of operator is used explicitly. This is redundant and unnecessary.
In addition, it will often generate a warning. In the third statement, we used the addressof operator with the array’s first element. While this works, it is somewhat verbose:
 printf("%d\n",stringLength(simpleArray));
 printf("%d\n",stringLength(&simpleArray));
 printf("%d\n",stringLength(&simpleArray[0]));
Figure 5-12 illustrates how memory will be allocated for the stringLength function.
Figure 5-12. Passing a string

Now let’s turn our attention to how we declare the formal parameter. In the previous
implementation of stringLength, we declared the parameter as a pointer to a char. We
could have also used array notation as shown below:
size_t stringLength(char string[]) { ... }
The function’s body will stay the same. This change will have no effect on how the
function is invoked or its behavior.
Passing a Pointer to a Constant char
Passing a pointer to a string as a constant char is a very common and useful technique.
It passes the string using a pointer, and at the same time prevents the string being passed
from being modified. A better implementation of the stringLength function developed
in the section “Passing Strings” on page 121 incorporates this declaration as follows:
size_t stringLength(const char* string) {
 size_t length = 0;
 while(*(string++)) {
 length++;
 }
 return length;
}
If we attempt to modify the original string as follows, then a compile-time error message
will be generated:
size_t stringLength(const char* string) {
 ...
 *string = 'A';
 ...
}
Passing a String to Be Initialized
There are situations where we want a function to return a string initialized by the func‐
tion. For example, we may want to pass information about a part, such as its name and
quantity, and then have a formatted string representing this information returned. By
keeping the formatting process in a function we can reuse it in different sections of our
program.
However, we need to decide whether we want to pass the function an empty buffer to
be filled and returned by the function, or whether the buffer should be dynamically
allocated by the function and then returned to us.
When a buffer is passed:

• The buffer’s address and its size must be passed
• The caller is responsible for deallocating the buffer
• The function normally returns a pointer to this buffer
This approach keeps the buffer’s allocation and deallocation responsibility with the
caller. Returning a pointer to the buffer is common, even if it is unnecessary, as typified
by strcpy and similar functions. The following format function illustrates this
approach:
char* format(char *buffer, size_t size,
 const char* name, size_t quantity, size_t weight) {
 snprintf(buffer, size, "Item: %s Quantity: %u Weight: %u",
 name, quantity, weight);
 return buffer;
}
The snprintf function was used as a simple way of formatting the string. This function
writes to the buffer provided by the first parameter. The second argument specifies the
buffer’s size. This function will not write past the end of the buffer. Otherwise, the
function behaves the same way as printf.
The following demonstrates the use of the function:
printf("%s\n",format(buffer,sizeof(buffer),"Axle",25,45));
The output of this sequence is as follows:
Item: Axle Quantity: 25 Weight: 45
By returning a pointer to buffer, we are able to use the function as a parameter of the
printf function.
An alternative to this approach is to pass NULL as the buffer’s address. This implies the
caller does not want to provide the buffer or is unsure how large the buffer should be.
This version of the function can be implemented as follows. When length is calculated
the subexpression 10 + 10 represents the largest width anticipated for the quantity and
weight. The one allows space for the NUL termination character:
char* format(char *buffer, size_t size,
 const char* name, size_t quantity, size_t weight) {
 char *formatString = "Item: %s Quantity: %u Weight: %u";
 size_t formatStringLength = strlen(formatString)-6;
 size_t nameLength = strlen(name);
 size_t length = formatStringLength + nameLength +
 10 + 10 + 1;

 if(buffer == NULL) {
 buffer = (char*)malloc(length);
 size = length;
}
 snprintf(buffer, size, formatString, name, quantity, weight);
 return buffer;
}
The function variation to use depends on the needs of the application. The chief draw‐
back of the second approach is that the caller is now responsible for freeing the memory
allocated. The caller needs to be fully aware of how this function should be used; other‐
wise, a memory leak can easily occur.
Passing Arguments to an Application
The main function is normally the first function in an application to be executed. With
console-based programs it is common to pass information to the program to enable or
otherwise control the application’s behavior. These parameters may be used to specify
which files to process or to configure the application’s output. For example, the ls Linux
command will list the files in the current directory based on parameters used with the
command.
C supports command line arguments using the traditionally named argc and argv
parameters. The first parameter, argc, is an integer that indicates how many parameters
are passed. At least one parameter is always passed. This parameter is the name of the
executable. The second parameter, argv, is normally viewed as a one-dimensional
array of string pointers. Each pointer references a command line argument.
The following main function will simply list its arguments one per line. In this version,
argv is declared as a pointer to a pointer to a char:
int main(int argc, char** argv) {
 for(int i=0; i<argc; i++) {
 printf("argv[%d] %s\n",i,argv[i]);
 }
 ...
}
The program is executed with the following command line:
process.exe -f names.txt limit=12 -verbose
The output will be as follows:
argv[0] c:/process.exe
argv[1] -f
argv[2] names.txt
argv[3] limit=12
argv[4] -verbose
Each command line parameter is delineated by whitespace. The memory allocated for
the program is illustrated in Figure 5-13.

Figure 5-13. Using argc/argv
The declaration of argv can be simplified as follows:
int main(int argc, char* argv[]) {
This is equivalent to char** argv. A more detailed explanation of this notation is found
in “Multiple Levels of Indirection” on page 25.
Returning Strings
When a function returns a string, it returns the address of the string. The main concern
is to return a valid string address. To do this, we can return a reference to either:
• A literal
• Dynamically allocated memory
• A local string variable
Returning the Address of a Literal
An example of returning a literal is shown below. An integer code selects from one of
four different processing centers. The function’s purpose is to return the processing
center’s name as a string. In this example, it simply returns the literal’s address:
char* returnALiteral(int code) {
 switch(code) {
 case 100:
 return "Boston Processing Center";
 case 200:
 return "Denver Processing Center";
 case 300:
 return "Atlanta Processing Center";

case 400:
 return "San Jose Processing Center";
 }
}
This will work fine. Just keep in mind that string literals are not always treated as con‐
stants, as discussed in the section “When a string literal is not a constant” on page 110.
We can also declare static literals as in the following example. A subCode field has been
added and selects between different centers. The advantage of this approach is not hav‐
ing to use the same literal in more than one place and possibly introducing errors by
mistyping the literal:
char* returnAStaticLiteral(int code, int subCode) {
 static char* bpCenter = "Boston Processing Center";
 static char* dpCenter = "Denver Processing Center";
 static char* apCenter = "Atlanta Processing Center";
 static char* sjpCenter = "San Jose Processing Center";

 switch(code) {
 case 100:
 return bpCenter;
 case 135:
 if(subCode <35) {
 return dpCenter;
 } else {
 return bpCenter;
 }
 case 200:
 return dpCenter;
 case 300:
 return apCenter;
 case 400:
 return sjpCenter;
 }
}
Returning a pointer to a static string used for multiple purposes can be a problem.
Consider the following variation of the format function developed in the section “Pass‐
ing a String to Be Initialized” on page 123. Information about a part is passed to the
function and a formatted string representing the string is returned:
char* staticFormat(const char* name, size_t quantity, size_t weight) {
 static char buffer[64]; // Assume to be large enough
 sprintf(buffer, "Item: %s Quantity: %u Weight: %u",
 name, quantity, weight);
 return buffer;
}
The buffer is allocated 64 bytes, which may or may not be enough. For purposes of this
example, we will ignore this potential problem. The main problem with this approach
is illustrated with the following sequence:
char* part1 = staticFormat("Axle",25,45);
 char* part2 = staticFormat("Piston",55,5);
 printf("%s\n",part1);
 printf("%s\n",part2);
When executed, we get the following output:
Item: Piston Quantity: 55 Weight: 5
Item: Piston Quantity: 55 Weight: 5
Since the staticFormat method used the same static buffer for both calls, the last call
overwrote the first call’s results.
Returning the Address of Dynamically Allocated Memory
If a string needs to be returned from a function, the memory for the string can be
allocated from the heap and then its address can be returned. We will demonstrate this
technique by developing a blanks function. This function returns a string containing
a series of blanks representing a “tab,” as shown below. The function is passed an integer
specifying the tab sequence’s length:
char* blanks(int number) {
 char* spaces = (char*) malloc(number + 1);
 int i;
 for (i = 0; i<number; i++) {
 spaces[i] = ' ';
 }
 spaces[number] = '\0';
 return spaces;
}
 ...
 char *tmp = blanks(5);
The NUL termination character is assigned to the last element of the array indexed by
number. Figure 5-14 illustrates the allocation of memory for this example. It shows the
application’s state just before and after the blanks function returns.
It is the function’s caller’s responsibility to deallocate the memory returned. Failure to
deallocate it when it is no longer needed will result in a memory leak. The following is
an example of when a memory leak can occur. The string is used within the printf
function and its address is subsequently lost because it was not saved:
 printf("[%s]\n",blanks(5));
A safer approach is demonstrated below:
 char *tmp = blanks(5);
 printf("[%s]\n",tmp);
 free(tmp);

 Figure 5-14. Returning dynamically allocated string
Returning the address of a local string
Returning the address of a local string will be a problem since the memory will be
corrupted when it is overwritten by another stack frame. This approach should be
avoided; it is explained here to demonstrate the potential problems with the course of
action.
We rewrite the blanks function from the previous section as shown below. Instead of
dynamically allocating memory, an array is declared within the function and will sub‐
sequently be located in a stack frame. The function returns the array’s address:
#define MAX_TAB_LENGTH 32
char* blanks(int number) {
 char spaces[MAX_TAB_LENGTH];
 int i;
 for (i = 0; i < number && i < MAX_TAB_LENGTH; i++) {
 spaces[i] = ' ';
 }
 spaces[i] = '\0';
 return spaces;
}
When the function executes it will return the string’s address, but that memory area will
subsequently be overwritten by the next function called. When this pointer is derefer‐
enced, the contents of this memory location may have been changed. The program
stack’s state is illustrated in Figure 5-15.

Figure 5-15. Returning the address of a local string
Function Pointers and Strings
Function pointers are discussed in depth in “Function Pointers” on page 71. They can
be a flexible means of controlling how a program executes. In this section, we will
demonstrate this capability by passing a comparison function to a sort function. Within
a sort function, comparison of the array’s elements are made to determine whether the
array’s elements need to be swapped. The comparison determines whether the array is
sorted in ascending or descending order, or by some other sorting criteria. By passing
a function to control the comparison, the function is more flexible. By passing different
comparison functions, we can have the same sort function perform in different ways.
The comparison functions we will use determine the sorting order based on the case of
the array’s elements. The following two functions, compare and compareIgnoreCase,
compare two strings based on the case of the strings. The compareIgnoreCase function
converts the strings to lower case before it uses the strcmp function to compare the
strings. The strcmp function was discussed in the section “Comparing Strings” on page
115. The stringToLower function returns a pointer to dynamically allocated memory.
This means we need to free it when we no longer need it:
int compare(const char* s1, const char* s2) {
 return strcmp(s1,s2);
}
int compareIgnoreCase(const char* s1, const char* s2) {
 char* t1 = stringToLower(s1);
 char* t2 = stringToLower(s2);
 int result = strcmp(t1, t2);
 free(t1);
 free(t2);
 return result;
}
The stringToLower function is shown below. It returns a lowercase equivalent of the
string passed to it:

char* stringToLower(const char* string) {
 char *tmp = (char*) malloc(strlen(string) + 1);
 char *start = tmp;
 while (*string != 0) {
 *tmp++ = tolower(*string++);
 }
 *tmp = 0;
 return start;
}
The function pointer to be used is declared using a type definition as shown below:
typedef int (fptrOperation)(const char*, const char*);
The following sort function’s implementation is based on the bubble sort algorithm. It
is passed the array’s address, its size, and a pointer to the function controlling the sort.
In the if statement, the function passed is invoked with two elements of the array. It
determines whether the array’s two elements will be swapped.
void sort(char *array[], int size, fptrOperation operation) {
 int swap = 1;
 while(swap) {
 swap = 0;
 for(int i=0; i<size-1; i++) {
 if(operation(array[i],array[i+1]) > 0){
 swap = 1;
 char *tmp = array[i];
 array[i] = array[i+1];
 array[i+1] = tmp;
 }
 }
 }
}
A display function will show the array’s contents:
void displayNames(char* names[], int size) {
 for(int i=0; i<size; i++) {
 printf("%s ",names[i]);
 }
 printf("\n");
}
We can invoke the sort function using either of the two comparison functions. The
following uses the compare function to perform a case-sensitive sort:
 char* names[] = {"Bob", "Ted", "Carol", "Alice", "alice"};
 sort(names,5,compare);
 displayNames(names,5);
The output of this sequence follows:
Alice Bob Carol Ted alice

If we had used the compareIgnoreCase function instead, then our output would appear
as shown below:
Alice alice Bob Carol Ted
This makes the sort function much more flexible. We can now devise and pass as simple
or complex an operation as we want to control the sort without having to write different
sort functions for different sorting needs.
Summary
In this chapter, we focused on string operations and the use of pointers. The structure
of strings and where they are located in memory impacts their use. Pointers provide a
flexible tool for working with strings but also offer numerous opportunities to misuse
strings.
String literals and the use of a literal pool were covered. Understanding literals helps
explain why certain string assignment operations do not always behave as expected.
This is closely related to string initialization, which was addressed in detail. Several
standard string operations were examined and potential problems were identified.
Passing and returning strings to functions are common operations. The issues and po‐
tential problems with these type of operations were detailed, including the problems
potentially occurring when returning a local string. The use of a pointer to a constant
character was also discussed.
Finally, function pointers were used to demonstrate a powerful approach for writing
sort functions. The approach is not limited to the sort operation but can be applied to
other areas.


