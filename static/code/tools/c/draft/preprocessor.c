
// as a general rule the following belong in header (.h) files:

// - macro defintions (i.e. #defines)
// - structure, union and enumeration definitions
// - typedef declarations
// - external function declarations
// - global variable declarations

// it is especially important to put a declaration or definition in a header file when it will be shared between several other files; when a definition or declaration should remain private to one source (.c) file then it is fine to leave it there and declare it as static

// also put a declaration or macro definition in a header file and #include it wherever needed rather than repeating it at the top of two or more source files

// you should not put function bodies or global variable definitions (that is, defining or initializing instances) in header files.


// the <> #include syntax is typically used with standard or system-supplied headers while "" is used for your own header files

// include guards prevent a header file from being included twice
#ifndef FILENAME_H
#define FILENAME_H

// a preprocessor #define gives you a true compile-time constant compared to const which gives you a readonly run-time object
#define BUFSIZE 1024

// a #define can also be used to define function-like behavior
#define SQUARE(x) x * x

// there are three rules to follow when defining function-like macros:

// 1. the macro expansion must always be parenthesized to protect any lower-precedence operators from the surrounding expression (e.g. with 1 / SQUARE(n) we get 1 / n * n instead of 1 / (n * n))

// 2. in the macro definition all occurrences of the parameters must be parenthesized to protect any low-precedence operators in the actual arguments from the rest of the macro expansion (e.g. with SQUARE(n + 1) we get n + 1 * n + 1 instead of (n + 1) * (n + 1))

// 3. if a parameter appears several times in the expansion then the macro may not work properly if the actual argument is an expression with side effects (e.g. SQUARE(i++) would expand to i++ * i++ which is undefined)

// accounting for rule 3 can be difficult so we might just mark the macro as unsafe to remember not to use it on arguments with side effects
#define CUBE(x) ((x) * (x) * (x))

// a multi-statement macro must be defined carefully without a trailing semicolon
#define COUNT(to) do { \
  for (int i = 1; i <= to; i++) { \
    printf("%d ", i); \
  } \
  printf("\n"); \
} while(0)


// the # operator is the stringizing operator
#define STR(x) #x

// there is no charizing operator so the best solution is probably to move the single quotes to the invocation
#define CTRL(c) ((c) & 037)

// the ## operator performs token-pasting
#define PASTE(a, b) a##b

// macro parameters are not expanded inside string literals and character constants so we need to use the stringizing operator and concatenation
#define TRACE(var, fmt) printf("TRACE: " #var " = " #fmt "\n", var)


// #ifdef checks if 


// preprocessor #if arithmetic uses only integers but we can use some macros with symbolic names and distinct integer values in conditionals
#define RED	1
#define BLUE	2
#define GREEN	3

#if COLOR == RED
// red case
#elif COLOR == BLUE
// blue case
#elif COLOR == GREEN
// green case
#else
// default case
#endif

int main(void) {

  printf("%c\n", CTRL('A'));
}

#endif