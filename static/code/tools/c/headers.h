// include guards prevent a header file from being included twice
#ifndef FILENAME_H
#define FILENAME_H

// as a general rule the following belong in header (.h) files:

// - macro defintions (i.e. #defines)
// - structure, union and enumeration definitions
// - typedef declarations
// - external function declarations
// - global variable declarations

// it is especially important to put a declaration or definition in a header file when it will be shared between several other files

/// when a definition or declaration should remain private to one source (.c) file then it is fine to leave it there and declare it as static

// also put a declaration or macro definition in a header file and #include it wherever needed rather than repeating it at the top of two or more source files

// you should not put function bodies or global variable definitions (that is, defining or initializing instances) in header files

// the <> #include syntax is typically used with standard or system-supplied headers while "" is used for your own header files

#endif