


// MISC

// on some machine architectures, the bytes used to hold an object must have proper alignment for the hardware to access it efficiently

// e.g. a 4-byte int often has to aligned on a word boundary

// this is most visible in cases where structs contain "holes" to improve alignment

// the alignof() operator returns the alignment of its argument type
auto ac = alignof(char);
auto ai = alignof(int);

int a[20];
auto aa = alignof(decltype(a));

// TODO: 151



