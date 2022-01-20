



// a preprocessor #define gives you a true compile-time constant compared to const which gives you a readonly run-time object
#define BUFSIZE 1024


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