
2
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
18
19


1.

```c
#include <stdlib.h>
#include <stdio.h>

int main(void) {
double A[5] = {
[0] = 9.0,
[1] = 2.9,
[4] = 3.E+25,
[3] = .00007,
};
for (size_t i = 0; i < 5; ++i) {
printf(”element %zu is %g, \tits square is %g\n”,
i,
A[i],
A[i]*A[i]);
}
return EXIT_SUCCESS;
}
```

16. More involved processing and IO


int main(void) {
  char lbuf[256];

  while (1) {
    if (fgetline(sizeof(lbuf), lbuf, stdin)) {
      size_t n;
      
    }
  }
}

248 for (;;) {
249 if (fgetline(sizeof lbuf, lbuf, stdin)) {
250 size_t n;
251 size_t* nums = numberline(strlen(lbuf)+1, lbuf, &n, 0);
252 int ret = fprintnumbers(stdout, ”%#zX”, ”,\t”, n, nums);
253 if (ret < 0) return EXIT_FAILURE;
254 free(nums);
255 } else {
256 if (lbuf[0]) { /* a partial line has been read */
257 for (;;) {
258 int c = getc(stdin);
259 if (c == EOF) return EXIT_FAILURE;
260 if (c == '\n') {
261 fprintf(stderr, ”line too long: %s\n”, lbuf);
262 break;
263 }
264 }
265 } else break; /* regular end of input */
266 }
267 }
268 }

This program splits the job in three different tasks:
fgetline to read a line of text
numberline to split such a line in a series of numbers of type size_t
fprintnumbers to print them
At the heart is the function numberline. It splits the lbuf string that it receives into
numbers, allocates an array to store them, and also returns the count of these numbers through
the pointer argument np if that is provided:
numberline.c
numberline: interpret string lbuf as a sequence of numbers represented with base
Returns: a newly allocated array of numbers as found in lbuf
Parameters:
lbuf is supposed to be a string
np if non-null, the count of numbers is stored in *np
base value from 0 to 36, with the same interpretation as for strtoul
Remarks: The caller of this function is responsible to free the array that is returned.
size_t* numberline(size_t size, char const lbuf[restrict size],
size_t*restrict np, int base);
Text processing 233
That function itself is split into two parts, which perform quite different tasks. One performs the task of interpreting the line, numberline_inner. The other, numberline
itself, is just a wrapper around the first that verifies or ensures the prerequisites for the first.
Function numberline_inner puts the C library function strtoull in a loop that collects the numbers and returns a count of them.
Now we see the use of the second parameter of strtoull. Here, it is the address of
the variable next, and next is used to keep track of the position in the string that ends
the number. Since next is a pointer to char, the argument to strtoull is a pointer to a
pointer to char:
numberline.c
97 static
98 size_t numberline_inner(char const*restrict act,
99 size_t numb[restrict], int base){
100 size_t n = 0;
101 for (char* next = 0; act[0]; act = next) {
102 numb[n] = strtoull(act, &next, base);
103 if (act == next) break;
104 ++n;
105 }
106 return n;
107 }
Suppose strtoull is called as strtoull(”0789a”, &next, base). According
to the value of the parameter base, that string is interpreted differently. If, for example,
base has the value 10, the first non-digit is the character 'a' at the end:
Base Digits Number *next
8 2 7 '8'
10 4 789 'a'
16 5 30874 '\0'
0 2 7 '8'
Remember the special rules for base 0. The effective base is deduced from the first (or first
two) characters in the string. Here, the first character is a '0', so the string is interpreted as
being octal, and parsing stops at the first non-digit for that base: '8'.
There are two conditions that may end the parsing of the line that numberline_inner
receives:
act points to a string termination: to a 0 character.
Function strtoull doesn’t find a number, in which case next is set to the value
of act.
These two conditions are found as the controlling expression of the for loop and as ifbreak condition inside.
Note that the C library function strtoull has a historical weakness: the first argument
has type char const*, whereas the second has type char**, without const qualification. This is why we had to type next as char* and couldn’t use char const*. As a
234 More involved processing and IO
result of a call to strtoull, we could inadvertently modify a read-only string and crash the
program.
TAKEAWAY 14.1 The string strto... conversion functions are not const-safe.
Now, the function numberline itself provides the glue around numberline_inner:
If np is null, it is set to point to an auxiliary.
The input string is checked for validity.
An array with enough elements to store the values is allocated and tailored to the
appropriate size, once the correct length is known.
We use three functions from the C library: memchr, malloc, and realloc. As in
previous examples, a combination of malloc and realloc ensures that we have an array
of the necessary length:
numberline.c
109 size_t* numberline(size_t size, char const lbuf[restrict size],
110 size_t*restrict np, int base){
111 size_t* ret = 0;
112 size_t n = 0;
113 /* Check for validity of the string, first. */
114 if (memchr(lbuf, 0, size)) {
115 /* The maximum number of integers encoded.
116 To see that this may be as much look at
117 the sequence 08 08 08 08 ... and suppose
118 that base is 0. */
119 ret = malloc(sizeof(size_t[1+(2*size)/3]));
120
121 n = numberline_inner(lbuf, ret, base);
122
123 /* Supposes that shrinking realloc will always succeed. */
124 size_t len = n ? n : 1;
125 ret = realloc(ret, sizeof(size_t[len]));
126 }
127 if (np) *np = n;
128 return ret;
129 }
The call to memchr returns the address of the first byte that has value 0, if there is any, or
(void*)0 if there is none. Here, this is just used to check that within the first size bytes
there effectively is a 0 character. That way, it guarantees that all the string functions used
underneath (in particular, strtoull) operate on a 0-terminated string.
With memchr, we encounter another problematic interface. It returns a void* that potentially points into a read-only object.
TAKEAWAY 14.2 The memchr and strchr search functions are not const-safe.
In contrast, functions that return an index position within the string would be safe.
TAKEAWAY 14.3 The strspn and strcspn search functions are const-safe.
Text processing 235
Unfortunately, they have the disadvantage that they can’t be used to check whether a chararray is in fact a string. So they can’t be used here.
Now, let us look at the second function in our example:
numberline.c
fgetline: read one text line of at most size-1 bytes.
The '\n' character is replaced by 0.
Returns: s if an entire line was read successfully. Otherwise, 0 is returned and s contains
a maximal partial line that could be read. s is null terminated.
char* fgetline(size_t size, char s[restrict size],
FILE*restrict stream);
This is quite similar to the C library function fgets. The first difference is the interface:
the parameter order is different, and the size parameter is a size_t instead of an int.
Like fgets, it returns a null pointer if the read from the stream failed. Thus the end-of-file
condition is easily detected on stream.
More important is that fgetline handles another critical case more gracefully. It detects
whether the next input line is too long or whether the last line of the stream ends without a
'\n' character:
numberline.c
131 char* fgetline(size_t size, char s[restrict size],
132 FILE*restrict stream){
133 s[0] = 0;
134 char* ret = fgets(s, size, stream);
135 if (ret) {
136 /* s is writable so can be pos. */
137 char* pos = strchr(s, '\n');
138 if (pos) *pos = 0;
139 else ret = 0;
140 }
141 return ret;
142 }
The first two lines of the function guarantee that s is always null terminated: either by the
call to fgets, if successful, or by enforcing it to be an empty string. Then, if something was
read, the first '\n' character that can be found in s is replaced with 0. If none is found, a
partial line has been read. In that case, the caller can detect this situation and call fgetline
again to attempt to read the rest of the line or to detect an end-of-file condition.[Exs 1]
In addition to fgets, this uses strchr from the C library. The lack of const-safeness
of this function is not an issue here, since s is supposed to be modifiable anyway. Unfortunately, with the interfaces as they exist now, we always have to do this assessment ourselves.
[Exs 1] Improve the main of the example such that it is able to cope with arbitrarily long input lines.
236 More involved processing and IO
Since it involves a lot of detailed error handling, we will go into detail about the function
fprintnumbers in section 14.5. For our purpose here, we restrict ourselves to the discussion of function sprintnumbers, which is a bit simpler because it only writes to a string,
instead of a stream, and because it just assumes that the buffer buf that it receives provides
enough space:
numberline.c
sprintnumbers: print a series of numbers nums in buf, using printf format form,
separated by sep characters and terminated with a newline character.
Returns: the number of characters printed to buf.
This supposes that tot and buf are big enough and that form is a format suitable to print
size_t.
int sprintnumbers(size_t tot, char buf[restrict tot],
char const form[restrict static 1],
char const sep[restrict static 1],
size_t len, size_t nums[restrict len]);
The function sprintnumbers uses a function of the C library that we haven’t met yet:
sprintf. Its formatting capacities are the same as those of printf and fprintf, only it
doesn’t print to a stream but rather to a char array:
numberline.c
149 int sprintnumbers(size_t tot, char buf[restrict tot],
150 char const form[restrict static 1],
151 char const sep[restrict static 1],
152 size_t len, size_t nums[restrict len]) {
153 char* p = buf; /* next position in buf */
154 size_t const seplen = strlen(sep);
155 if (len) {
156 size_t i = 0;
157 for (;;) {
158 p += sprintf(p, form, nums[i]);
159 ++i;
160 if (i >= len) break;
161 memcpy(p, sep, seplen);
162 p += seplen;
163 }
164 }
165 memcpy(p, ”\n”, 2);
166 return (p-buf)+1;
167 }
The function sprintf always ensures that a 0 character is placed at the end of the string.
It also returnsthe length of thatstring, which isthe number of characters before the 0 character
that have been written. This is used in the example to update the pointer to the current position
in the buffer. sprintf still has an important vulnerability:
Text processing 237
TAKEAWAY 14.4 sprintf makes no provision against buffer overflow.
That is, if we pass an insufficient buffer as a first argument, bad things will happen. Here,
inside sprintnumbers, much like sprintf itself, we suppose the buffer is large enough
to hold the result. If we aren’t sure the buffer can hold the result, we can use the C library
function snprintf, instead:
1 int snprintf(char*restrict s, size_t n, char const*restrict form, ...);
This function ensures in addition that no more than n bytes are ever written to s. If the
return value is greater than or equal to n, the string is been truncated to fit. In particular, if n
is 0, nothing is written into s.
TAKEAWAY 14.5 Use snprintf when formatting output of unknown length.
In summary, snprintf has a lot of nice properties:
The buffer s will not overflow.
After a successful call, s is a string.
When called with n and s set to 0, snprintf just returns the length of the string
that would have been written.
By using that, a simple for loop to compute the length of all the numbers printed on one
line looks like this:
numberline.c
182 /* Count the chars for the numbers. */
183 for (size_t i = 0; i < len; ++i)
184 tot += snprintf(0, 0, form, nums[i]);
We will see later how this is used in the context of fprintnumbers.
CHALLENGE 15 Text processing in strings
We’ve covered quite a bit about text processing, so let’s see if we can actually use it.
Can you search for a given word in a string?
Can you replace a word in a string and return a copy with the new contents?
Can you implement some regular-expression-matching functions for strings? For example, find a character class such as [A-Q] or [^0-9], match with * (meaning “anything”), or match with ? (meaning “any character”).
Or can you implement a regular-expression-matching function for POSIX character
classes such as [[:alpha:]], [[:digit:]], and so on?
Can you stitch all these functionalities together to search for a regexp in a string?
Do query-replace with regexp against a specific word?
Extend a regexp with grouping?
238 More involved processing and IO
Extend query-replace with grouping?
14.2 Formatted input
Similar to the printf family of functions for formatted output, the C library has a series
of functions for formatted input: fscanf for input from an arbitrary stream, scanf for
stdin, and sscanf from a string. For example, the following would read a line of three
double values from stdin:
1 double a[3];
2 /* Read and process an entire line with three double values. */
3 if (scanf(” %lg %lg %lg ”, &a[0], &a[1], &a[2]) < 3) {
4 printf(”not enough input values!\n”);
5 }
Tables 14.1 to 14.3 give an overview of the format for specifiers. Unfortunately, these
functions are more difficult to use than printf and also have conventions that diverge from
printf in subtle ways.
Table 14.1 Format specifications for scanf and similar functions, with the general
syntax [XX][WW][LL]SS
XX * Assignment suppression
WW Field width Maximum number of input characters
LL Modifier Select width of target type
SS Specifier Select conversion
To be able to return values for all formats, the arguments are pointers to the type that
is scanned.
Whitespace handling is subtle and sometimes unexpected. A space character, ' ',
in the format matches any sequence of whitespace: spaces, tabs, and newline characters. Such a sequence may in particular be empty or contain several newline characters.
String handling is different. Because the arguments to the scanf functions are
pointers anyway, the formats ”%c” and ”%s” both refer to an argument of type
char*. Where ”%c” reads a character array of fixed size (of default 1), ”%s”
matches any sequence of non-whitespace characters and adds a terminating 0 character.
The specifications of typesin the format have subtle differences compared to printf,
in particular for floating-point types. To be consistent between the two, it is best
to use ”%lg” or similar for double and ”%Lg” for long double, for both
printf and scanf.
There is a rudimentary utility to recognize character classes. For example, a format
of ”%[aeiouAEIOU]” can be used to scan for the vowels in the Latin alphabet.
Extended character sets 239
Table 14.2 Format specifiers for scanf and similar functions With an 'l' modifier,
specifiers for characters or sets of characters ('c', 's', '[') transform multibyte character
sequences on input to wide-character wchar_t arguments; see subection 14.3.
SS Conversion Pointer to Skip space Analogous to function
'd' Decimal Signed type Yes strtol, base 10
'i' Decimal, octal, or hex Signed type Yes strtol, base 0
'u' Decimal Unsigned type Yes strtoul, base 10
'o' Octal Unsigned type Yes strtoul, base 8
'x' Hexadecimal Unsigned type Yes strtoul, base 16
'aefg' Floating point Floating point Yes strtod
'%' '%' character No assignment No
'c' Characters char No memcpy
's' Non-whitespace char Yes strcspn with
” \f\n\r\t\v”
'[' Scan set String No strspn or strcspn
'p' Address void Yes
'n' Character count Signed type No
In such a character class specification, the caret ^ negates the class if it is found at
the beginning. Thus ”%[^\n]%*[\n]” scans a whole line (which must be nonempty) and then discards the newline character at the end of the line.
These particularities make the scanf family of functions difficult to use. For example,
our seemingly simple example has the flaw (or feature) that it is not restricted to read a single
input line, but it would happily accept three double values spread over several lines.[Exs 2] In
most cases where you have a regular input pattern such as a series of numbers, they are best
avoided.
14.3 Extended character sets
Up to now, we have used only a limited set of characters to specify our programs or the
contents of string literals that we printed on the console: a set consisting of the Latin alphabet,
Arabic numerals, and some punctuation characters. This limitation is a historical accident that
originated in the early market domination by the American computer industry, on one hand,
and the initial need to encode characters with a very limited number of bits on the other.1 As
we saw with the use of the type name char for the basic data cell, the concepts of a text
character and an indivisible data component were not very well separated at the start.
1 The character encoding that is dominantly used for the basic character set is referred to as ASCII: American
standard code for information interchange.
[Exs 2] Modify the format string in the example such that it only accepts three numbers on a single line, separated by
blanks, and such that the terminating newline character (eventually preceded by blanks) is skipped.
240 More involved processing and IO
Table 14.3 Format modifiers for scanf and similar functions Note that the significance
of float* and double* arguments is different than for printf formats.
Character Type
”hh” char types
”h” short types
”” signed, unsigned, float, char arrays and strings
”l” long integer types, double, wchar_t characters and strings
”ll” long long integer types
”j” intmax_t, uintmax_t
”z” size_t
”t” ptrdiff_t
”L” long double
Latin, from which we inherited our character set, is long dead as a spoken language. Its
character set is not sufficient to encode the particularities of the phonetics of other languages.
Among the European languages, English has the peculiarity that it encodes missing sounds
with combinations of letters such as ai, ou, and gh (fair enough), not with diacritical marks,
special characters, or ligatures (fär ínó), as do most of its cousins. So for other languages that
use the Latin alphabet, the possibilities were already quite restricted; but for languages and
cultures that use completely different scripts (Greek, Russian) or even completely different
concepts (Japanese, Chinese), this restricted American character set was clearly not sufficient.
During the first years of market expansion around the world, different computer manufacturers, countries, and organizations provided native language support for their respective
communities more or less randomly, and added specialized support for graphical characters,
mathematical typesetting, musical scores, and so on without coordination. It was an utter
chaos. As a result, interchanging textual information between different systems, countries,
and cultures was difficult if not impossible in many cases; writing portable code that could be
used in the context of different languages and different computing platforms resembled the
black arts.
Luckily, these years-long difficulties are now mainly mastered, and on modern systems we
can write portable code that uses “extended” characters in a unified way. The following code
snippet shows how this is supposed to work:
mbstrings-main.c
87 setlocale(LC_ALL, ””);
88 /* Multibyte character printing only works after the locale
89 has been switched. */
90 draw_sep(TOPLEFT ” © 2014 jɛnz ˈgʊzˌtɛt ”, TOPRIGHT);
Extended character sets 241
That is, near the beginning of our program, we switch to the “native” locale, and then we
can use and output text containing extended characters: here, phonetics (so-called IPA). The
output of this looks similar to
© 2014 jɛnz ˈgʊz,tɛt
The means to achieve this are quite simple. We have some macros with magic string literals
for vertical and horizontal bars, and top-left and top-right corners:
mbstrings-main.c
43 #define VBAR ”\u2502” /**< a vertical bar character */
44 #define HBAR ”\u2500” /**< a horizontal bar character */
45 #define TOPLEFT ”\u250c” /**< topleft corner character */
46 #define TOPRIGHT ”\u2510” /**< topright corner character */
And an ad hoc function that nicely formats an output line:
mbstrings-main.c
draw_sep: Draw multibyte strings start and end separated by a horizontal line.
void draw_sep(char const start[static 1],
char const end[static 1]) {
fputs(start, stdout);
size_t slen = mbsrlen(start, 0);
size_t elen = 90 - mbsrlen(end, 0);
for (size_t i = slen; i < elen; ++i) fputs(HBAR, stdout);
fputs(end, stdout);
fputc('\n', stdout);
}
This uses a function to count the number of print charactersin a multibyte string (mbsrlen)
and our old friends fputs and fputc for textual output.
The start of all of this with the call to setlocale is important. Chances are, otherwise
you’d see garbage if you output characters from the extended set to your terminal. But once
you have issued that call to setlocale and your system is well installed, such characters
placed inside multibyte strings ”fär ínóff” should not work out too badly.
A multibyte character is a sequence of bytes that is interpreted as representing a single
character of the extended character set, and a multibyte string is a string that contains such
multibyte characters. Luckily, these beasts are compatible with ordinary strings as we have
handled them so far.
TAKEAWAY 14.6 Multibyte characters don’t contain null bytes.
TAKEAWAY 14.7 Multibyte strings are null terminated.
Thus, many of the standard string functions such as strcpy work out of the box for multibyte
strings. They introduce one major difficulty, though: the fact that the number of printed
characters can no longer be directly deduced from the number of elements of a char array or
242 More involved processing and IO
by the function strlen. This is why, in the previous code, we use the (nonstandard) function
mbsrlen:
mbstrings.h
mbsrlen: Interpret a mb string in mbs and return its length when interpreted as a wide
character string.
Returns: the length of the mb string or -1 if an encoding error occured.
This function can be integrated into a sequence of searches through a string, as long as a
state argument is passed to this function that is consistent with the mb character starting
in mbs. The state itself is not modified by this function.
Remarks: state of 0 indicates that mbs can be scanned without considering any context.
size_t mbsrlen(char const*restrict mbs,
mbstate_t const*restrict state);
As you can see from that description, parsing multibyte strings for the individual multibyte
characters can be a bit more complicated. In particular, generally we need to keep a parsing
state by means of the type mbstate_t that is provided by the C standard in the header files
wchar.h.
2 <wchar.h> This header provides utilities for multibyte strings and characters, and also for a
wide character type wchar_t. We will see that later.
But first, we have to introduce another international standard: ISO 10646, or Unicode
[2017]. As the naming indicates, Unicode (http://www.joelonsoftware.com/articles/Unicode.
html) attempts to provide a unified framework for character codes. It provides a huge table3 of
basically all character concepts that have been conceived by mankind so far. Concept here is
really important: we have to understand from the print form or glyph of a particular character
in a certain type that, for example, “Latin capital letter A” can appear as A, A, A, or A in the
present text. Other such conceptual characters like the character “Greek capital letter Alpha”
may even be printed with the same or similar glyph Α.
Unicode places each character concept, or code point in its own jargon, into a linguistic
or technical context. In addition to the definition of the character, Unicode classifies it, for
example, as being a capital letter, and relates it to other code points, such as by stating that A
is the capitalization of a.
If you need special characters for your particular language, there is a good chance that
you have them on your keyboard and that you can enter them into multibyte strings for
coding in C as is. That is, your system may be configured to insert the whole byte sequence for ä, say, directly into the text and do all the required magic for you. If you don’t
have or want that, you can use the technique that we used for the macros HBAR earlier.
There we used an escape sequence that was new in C11 (http://dotslashzero.net/2014/05/21/
the-interesting-state-of-unicode-in-c/): a backslash and a u followed by four hexadecimal
2 The header uchar.h also provides this type. 3 Today, Unicode has about 110,000 code points.
Extended character sets 243
digits encode a Unicode code point. For example, the code point for “latin small letter a with
diaeresis” is 228 or 0xE4. Inside a multibyte string, this then reads as ”\u00E4”. Since four
hexadecimal digits can address only 65,536 code points, there is also the option to specify 8
hexadecimal digits, introduced with a backslash and a capital U, but you will encounter this
only in very specialized contexts.
In the previous example, we encoded four graphical characters with such Unicode specifications, characters that most likely are not on any keyboard. There are several online sites
that allow you to find the code point for any character you need.
If we want to do more than simple input/output with multibyte characters and strings,
things become a bit more complicated. Simple counting of the characters already is not trivial:
strlen does not give the right answer, and other string functions such as strchr, strspn,
and strstr don’t work as expected. Fortunately, the C standard gives us a set of replacement
functions, usually prefixed with wcs instead of str, that will work on wide character strings,
instead. The mbsrlen function that we introduced earlier can be coded as
mbstrings.c
30 size_t mbsrlen(char const*s, mbstate_t const*restrict state) {
31 if (!state) state = MBSTATE;
32 mbstate_t st = *state;
33 size_t mblen = mbsrtowcs(0, &s, 0, &st);
34 if (mblen == -1) errno = 0;
35 return mblen;
36 }
The core of this function is the use of the library function mbsrtowcs (“multibyte string
(mbs), restartable, to wide character string (wcs)”), which constitutes one of the primitives
that the C standard provides to handle multibyte strings:
1 size_t mbsrtowcs(wchar_t*restrict dst, char const**restrict src,
2 size_t len, mbstate_t*restrict ps);
So once we decrypt the abbreviation of the name, we know that this function is supposed to
convert an mbs, src, to a wcs, dst. Here, wide characters (wc) of type wchar_t are use
to encode exactly one character of the extended character set, and these wide characters are
used to form wcs pretty much in the same way as char s compose ordinary strings: they are
null-terminated arrays of such wide characters.
The C standard doesn’t restrict the encoding used for wchar_t much, but any sane environment nowadays should use Unicode for its internal representations. You can check this
with two macros as follows:
mbstrings.h
24 #ifndef __STDC_ISO_10646__
25 # error ”wchar_t wide characters have to be Unicode code points”
26 #endif
27 #ifdef __STDC_MB_MIGHT_NEQ_WC__
28 # error ”basic character codes must agree on char and wchar_t”
29 #endif
244 More involved processing and IO
Modern platforms typically implement wchar_t with either 16-bit or 32-bit integer types.
Which one usually should not be of much concern to you, if you only use the code points that
are representable with four hexadecimal digits in the \uXXXX notation. Those platforms
that use 16-bit effectively can’t use the other code points in \UXXXXXXXX notation, but this
shouldn’t bother you much.
Wide characters and wide character string literals follow analogous rules to those we have
seen for char and strings. For both, a prefix of L indicates a wide character or string:
for example, L'ä' and L'\u00E4' are the same character, both of type wchar_t, and
L”b\u00E4” is an array of three elements of type wchar_t that contains the wide characters L'b', L'ä', and 0.
Classification of wide characters is also done in a similar way as for simple char. The
<wctype.h> header wctype.h provides the necessary functions and macros.
To come back to mbsrtowcs, this function parses the multibyte string src into snippets
that correspond to multibyte characters (mbc), and assigns the corresponding code point to the
wide characters in dst. The parameter len describes the maximal length that the resulting
wcs may have. The parameter state points to a variable that stores an eventual parsing state
of the mbs; we will discuss this concept briefly a bit later.
As you can see, the function mbsrtowcs has two peculiarities. First, when called with
a null pointer for dst, it simply doesn’t store the wcs but only returns the size that such a
wcs would have. Second, it can produce a coding error if the mbs is not encoded correctly.
In that case, the function returns (size_t)-1 and sets errno to the value EILSEQ (see
<errno.h> errno.h). Part of the code for mbsrlen is actually a repair of that error strategy by setting
errno to 0 again.
Let’s now look at a second function that will help us handle mbs:
mbstrings.h
mbsrdup: Interpret a sequence of bytes in s as mb string and convert it to a wide character string.
Returns: a newly malloc’ed wide character string of the appropriate length, 0 if an
encoding error occurred.
Remarks: This function can be integrated into a sequence of such searches through a
string, as long as a state argument is passed to this function that is consistent with the mb
character starting in c. The state itself is not modified by this function.
state of 0 indicates that s can be scanned without considering any context.
wchar_t* mbsrdup(char const*s, mbstate_t const*restrict state);
This function returns a freshly allocated wcs with the same contents as the mbs s that it
receives on input. Other than for the state parameter, its implementation is straightforward:
Extended character sets 245
mbstrings.c
38 wchar_t* mbsrdup(char const*s, mbstate_t const*restrict state) {
39 size_t mblen = mbsrlen(s, state);
40 if (mblen == -1) return 0;
41 mbstate_t st = state ? *state : *MBSTATE;
42 wchar_t* S = malloc(sizeof(wchar_t[mblen+1]));
43 /* We know that s converts well, so no error check */
44 if (S) mbsrtowcs(S, &s, mblen+1, &st);
45 return S;
46 }
After determining the length of the target string, we use malloc to allocate space and
mbsrtowcs to copy over the data.
To have more fine-grained control over the parsing of an mbs, the standard provides the
function mbrtowc:
1 size_t mbrtowc(wchar_t*restrict pwc,
2 const char*restrict s, size_t len,
3 mbstate_t* restrict ps);
In this interface, parameter len denotes the maximal position in s that is scanned for a
single multibyte character. Since in general we don’t know how such a multibyte encoding
works on the target machine, we have to do some guesswork that helps us determine len. To
encapsulate such a heuristic, we cook up the following interface. It has semantics similar to
mbrtowc but avoids the specification of len:
mbstrings.h
mbrtow: Interpret a sequence of bytes in c as mb character and return that as wide
character through C.
Returns: the length of the mb character or -1 if an encoding error occured.
This function can be integrated into a sequence of such searches through a string, as long
as the same state argument passed to all calls to this or similar functions.
Remarks: state of 0 indicates that c can be scanned without considering any context.
size_t mbrtow(wchar_t*restrict C, char const c[restrict static 1],
mbstate_t*restrict state);
This function returns the number of bytes that were identified for the first multibyte character in the string, or -1 on error. mbrtowc has another possible return value, -2, for the
case that len wasn’t big enough. The implementation uses that return value to detect such a
situation and to adjust len until it fits:
246 More involved processing and IO
mbstrings.c
14 size_t mbrtow(wchar_t*restrict C, char const c[restrict static 1],
15 mbstate_t*restrict state) {
16 if (!state) state = MBSTATE;
17 size_t len = -2;
18 for (size_t maxlen = MB_LEN_MAX; len == -2; maxlen *= 2)
19 len = mbrtowc(C, c, maxlen, state);
20 if (len == -1) errno = 0;
21 return len;
22 }
Here, MB_LEN_MAX is a standard value that is a good upper bound for len in most situations.
Let us now go to a function that uses the capacity of mbrtow to identify mbc and to use
that to search inside a mbs:
mbstrings.h
mbsrwc: Interpret a sequence of bytes in s as mb string and search for wide character C.
Returns: the occurrence’th position in s that starts a mb sequence corresponding to C
or 0 if an encoding error occurred.
If the number of occurrences is less than occurrence the last such position is returned. So
in particular using SIZE_MAX (or -1) will always return the last occurrence.
Remarks: This function can be integrated into a sequence of such searches through a
string, as long as the same state argument passed to all calls to this or similar functions
and as long as the continuation of the search starts at the position that is returned by this
function.
state of 0 indicates that s can be scanned without considering any context.
char const* mbsrwc(char const s[restrict static 1],
mbstate_t*restrict state,
wchar_t C, size_t occurrence);
mbstrings.c
68 char const* mbsrwc(char const s[restrict static 1], mbstate_t*restrict state
,
69 wchar_t C, size_t occurrence) {
70 if (!C || C == WEOF) return 0;
71 if (!state) state = MBSTATE;
72 char const* ret = 0;
73
74 mbstate_t st = *state;
75 for (size_t len = 0; s[0]; s += len) {
76 mbstate_t backup = st;
77 wchar_t S = 0;
78 len = mbrtow(&S, s, &st);
Binary streams 247
79 if (!S) break;
80 if (C == S) {
81 *state = backup;
82 ret = s;
83 if (!occurrence) break;
84 --occurrence;
85 }
86 }
87 return ret;
88 }
As we said, all of this encoding with multibyte strings and simple IO works fine if we have
an environment that is consistent: that is, if it uses the same multibyte encoding within your
source code as for other text files and on your terminal. Unfortunately, not all environments
use the same encoding yet, so you may encounter difficulties when transferring text files (including sources) or executables from one environment to another. In addition to the definition
of the big character table, Unicode also defines three encodings that are now widely used and
that hopefully will replace all others eventually: UTF-8, UTF-16, and UTF-32, for Unicode
Transformation Format with 8-bit, 16-bit, and 32-bit words, respectively. Since C11, the C
language includes rudimentary direct support for these encodings without having to rely on
the locale. String literals with these encodings can be coded as u8”text”, u”text”,
and U”text”, which have types char[], char16_t[], and char32_t[], respectively.
Chances are that the multibyte encoding on a modern platform is UTF-8, and then you
won’t need these special literals and types. They are mostly useful in a context where you have
to ensure one of these encodings, such as in network communication. Life on legacy platforms
might be more difficult; see http://www.nubaria.com/en/blog/?p=289 for an overview for the
Windows platform.
14.4 Binary streams
In section 8.3, we briefly mentioned that input and output to streams can also be performed in
binary mode in contrast to the usual text mode we have used up to now. To see the difference,
remember that text mode IO doesn’t write the bytes that we pass to printf or fputs oneto-one to the target file or device:
Depending on the target platform, a '\n' character can be encoded as one or several
characters.
Spaces that precede a newline can be suppressed.
Multibyte characters can be transcribed from the execution character set (the program’s internal representation) to the character set of the file system underlying the
file.
Similar observations hold for reading data from text files.
If the data that we manipulate is effectively human-readable text, all of this is fine; we
can consider ourselves happy that the IO functions together with setlocale make this
mechanism as transparent as possible. But if we are interested in reading or writing binary
data just as it is present in some C objects, this can be quite a burden and lead to serious
248 More involved processing and IO
difficulties. In particular, binary data could implicitly map to the end-of-line convention of
the file, and thus a write of such data could change the file’s internal structure.
As indicated previously, streams can be opened in binary mode. For such a stream, all
the translation between the external representation in the file and the internal representation
is skipped, and each byte of such a stream is written or read as such. From the interfaces we
have seen up to now, only fgetc and fputc can handle binary files portably. All others
may rely on some form of end-of-line transformation.
To read and write binary streams more easily, the C library has some interfaces that are
better suited:
1 size_t fread(void* restrict ptr, size_t size, size_t nmemb,
2 FILE* restrict stream);
3 size_t fwrite(void const*restrict ptr, size_t size, size_t nmemb,
4 FILE* restrict stream);
5 int fseek(FILE* stream, long int offset, int whence);
6 long int ftell(FILE* stream);
The use of fread and fwrite is relatively straightforward. Each stream has a current file position for reading and writing. If successful, these two functions read or write
size*nmemb bytes from that position onward and then update the file position to the new
value. The return value of both functions is the number of bytes that have been read or written,
usually size*nmemb, and thus an error occurred if the return value is less than that.
The functions ftell and fseek can be used to operate on that file position: ftell
returns the position in terms of bytes from the start of the file, and fseek positions the file
according to the arguments offset and whence. Here, whence can have one of these
values: SEEK_SET refers to the start of the file, and SEEK_CUR to the current file position
before the call.4
By means of these four functions, we may effectively move forward and backward in a
stream that represents a file and read or write any byte of it. This can, for example, be used to
write out a large object in its internal representation to a file and read it in later with a different
program, without performing any modifications.
This interface has some restrictions, though. To work portably, streams have to be opened
in binary mode. On some platforms, IO is always binary, because there is no effective transformation to perform. So, unfortunately, a program that does not use binary mode may work
reliably on these platforms, but then fail when ported to others.
TAKEAWAY 14.8 Open streams on which you use fread or fwrite in binary mode.
Since this works with internal representations of objects, it is only portable between platforms
and program executions that use that same representation: the same endian-ness. Different
platforms, operating systems, and even program executions can have different representations.
TAKEAWAY 14.9 Files that are written in binary mode are not portable between platforms.
4 There is also SEEK_END for the end-of-file position, but it may have platform-defined glitches.
Error checking and cleanup 249
The use of the type long for file positions limits the size of files that can easily be handled
with ftell and fseek to LONG_MAX bytes. On most modern platforms, this corresponds
to 2GiB.[Exs 3]
TAKEAWAY 14.10 fseek and ftell are not suitable for very large file offsets.
14.5 Error checking and cleanup
C programs can encounter a lot of error conditions. Errors can be programming errors, bugs
in the compiler or OS software, hardware errors, in some cases resource exhaustion (such as
out of memory), or any malicious combination of these. For a program to be reliable, we have
to detect such error conditions and deal with them gracefully.
As a first example, take the following description of a function fprintnumbers, which
continues the series of functions that we discussed in section 14.1:
numberline.c
fprintnumbers: print a series of numbers nums on stream, using printf format
form, separated by sep characters and terminated with a newline character.
Returns: the number of characters printed to stream, or a negative error value on error.
If len is 0, an empty line is printed and 1 is returned.
Possible error returns are:
EOF (which is negative) if stream was not ready to be written to
-EOVERFLOW if more than INT_MAX characters would have to be written,
including the case that len is greater than INT_MAX.
-EFAULT if stream or numb are 0
-ENOMEM if a memory error occurred
This function leaves errno to the same value as occurred on entry.
int fprintnumbers(FILE*restrict stream,
char const form[restrict static 1],
char const sep[restrict static 1],
size_t len, size_t numb[restrict len]);
As you can see, this function distinguishes four different error conditions, which are indicated by the return of negative constant values. The macros for these values are generally
<errno.h> provided by the platform in errno.h, and all start with the capital letter E. Unfortunately, the
C standard imposes only EOF (which is negative) and EDOM, EILSEQ, and ERANGE, which
are positive. Other values may or may not be provided. Therefore, in the initial part of our
code, we have a sequence of preprocessor statements that give default values for those that
are missing:
[Exs 3] Write a function fseekmax that uses intmax_t instead of long and achieves large seek values by combining calls to fseek.
250 More involved processing and IO
numberline.c
36 #include <limits.h>
37 #include <errno.h>
38 #ifndef EFAULT
39 # define EFAULT EDOM
40 #endif
41 #ifndef EOVERFLOW
42 # define EOVERFLOW (EFAULT-EOF)
43 # if EOVERFLOW > INT_MAX
44 # error EOVERFLOW constant is too large
45 # endif
46 #endif
47 #ifndef ENOMEM
48 # define ENOMEM (EOVERFLOW+EFAULT-EOF)
49 # if ENOMEM > INT_MAX
50 # error ENOMEM constant is too large
51 # endif
52 #endif
The idea is that we want to be sure to have distinct values for all of these macros. Now the
implementation of the function itself looks as follows:
numberline.c
169 int fprintnumbers(FILE*restrict stream,
170 char const form[restrict static 1],
171 char const sep[restrict static 1],
172 size_t len, size_t nums[restrict len]) {
173 if (!stream) return -EFAULT;
174 if (len && !nums) return -EFAULT;
175 if (len > INT_MAX) return -EOVERFLOW;
176
177 size_t tot = (len ? len : 1)*strlen(sep);
178 int err = errno;
179 char* buf = 0;
180
181 if (len) {
182 /* Count the chars for the numbers. */
183 for (size_t i = 0; i < len; ++i)
184 tot += snprintf(0, 0, form, nums[i]);
185 /* We return int so we have to constrain the max size. */
186 if (tot > INT_MAX) return error_cleanup(EOVERFLOW, err);
187 }
188
189 buf = malloc(tot+1);
190 if (!buf) return error_cleanup(ENOMEM, err);
191
192 sprintnumbers(tot, buf, form, sep, len, nums);
193 /* print whole line in one go */
194 if (fputs(buf, stream) == EOF) tot = EOF;
195 free(buf);
196 return tot;
197 }
Error checking and cleanup 251
Error handling pretty much dominates the coding effort for the whole function. The first
three lines handle errors that occur on entry to the function and reflect missed preconditions
or, in the language of Annex K (see section 8.1.4), runtime constraint violationsC .
Dynamic runtime errors are a bit more difficult to handle. In particular, some functions in
the C library may use the pseudo-variable errno to communicate an error condition. If we
want to capture and repair all errors, we have to avoid any change to the global state of the execution, including to errno. This is done by saving the current value on entry to the function
and restoring it in case of an error with a call to the small function error_cleanup:
numberline.c
144 static inline int error_cleanup(int err, int prev) {
145 errno = prev;
146 return -err;
147 }
The core of the function computes the total number of bytes that should be printed in a for
loop over the input array. In the body of the loop, snprintf with two 0 arguments is used
to compute the size for each number. Then our function sprintnumbers from section 14.1
is used to produce a big string that is printed using fputs.
Observe that there is no error exit after a successful call to malloc. If an error is detected
on return from the call to fputs, the information is stored in the variable tot, but the call
to free is not skipped. So even if such an output error occurs, no allocated memory is left
leaking. Here, taking care of a possible IO error was relatively simple because the call to
fputs occurred close to the call to free.
The function fprintnumbers_opt requires more care:
numberline.c
199 int fprintnumbers_opt(FILE*restrict stream,
200 char const form[restrict static 1],
201 char const sep[restrict static 1],
202 size_t len, size_t nums[restrict len]) {
203 if (!stream) return -EFAULT;
204 if (len && !nums) return -EFAULT;
205 if (len > INT_MAX) return -EOVERFLOW;
206
207 int err = errno;
208 size_t const seplen = strlen(sep);
209
210 size_t tot = 0;
211 size_t mtot = len*(seplen+10);
212 char* buf = malloc(mtot);
213
214 if (!buf) return error_cleanup(ENOMEM, err);
215
216 for (size_t i = 0; i < len; ++i) {
217 tot += sprintf(&buf[tot], form, nums[i]);
218 ++i;
219 if (i >= len) break;
220 if (tot > mtot-20) {
252 More involved processing and IO
221 mtot *= 2;
222 char* nbuf = realloc(buf, mtot);
223 if (buf) {
224 buf = nbuf;
225 } else {
226 tot = error_cleanup(ENOMEM, err);
227 goto CLEANUP;
228 }
229 }
230 memcpy(&buf[tot], sep, seplen);
231 tot += seplen;
232 if (tot > INT_MAX) {
233 tot = error_cleanup(EOVERFLOW, err);
234 goto CLEANUP;
235 }
236 }
237 buf[tot] = 0;
238
239 /* print whole line in one go */
240 if (fputs(buf, stream) == EOF) tot = EOF;
241 CLEANUP:
242 free(buf);
243 return tot;
244 }
It tries to optimize the procedure even further by printing the numbers immediately instead
of counting the required bytes first. This may encounter more error conditions as we go, and
we have to take care of them by guaranteeing to issue a call to free at the end. The first
such condition is that the buffer we allocated initially is too small. If the call to realloc
to enlarge it fails, we have to retreat carefully. The same is true if we encounter the unlikely
condition that the total length of the string exceeds INT_MAX.
In both cases, the function uses goto, to jump to the cleanup code that then calls free.
With C, this is a well-established technique that ensures that the cleanup takes place and
that also avoids hard-to-read nested if-else conditions. The rules for goto are relatively
simple:
TAKEAWAY 14.11 Labels for goto are visible in the entire function that contains them.
TAKEAWAY 14.12 goto can only jump to a label inside the same function.
TAKEAWAY 14.13 goto should not jump over variable initializations.
The use of goto and similar jumps in programming languages has been subject to intensive
debate, starting from an article by Dijkstra [1968]. You will still find people who seriously
object to code as it is given here, but let us try to be pragmatic about that: code with or without goto can be ugly and hard to follow. The main idea is to have the “normal” control
flow of the function be mainly undisturbed and to clearly mark changes to the control flow
that only occur under exceptional circumstances with a goto or return. Later, in section 17.5, we will see another tool in C that allows even more drastic changes to the control
flow: setjmp/longjmp, which enables us to jump to other positions on the stack of calling
functions.
Error checking and cleanup 253
CHALLENGE 16 Text processing in streams
For text processing in streams, can you read on stdin, dump modified text on stdout,
and report diagnostics on stderr? Count the occurrences of a list of words? Count
the occurrences of a regexp? Replace all occurrences of a word with another?
CHALLENGE 17 Text processor sophistication
Can you extend your text processor (challenge 12) to use multibyte characters?
Can you also extend it to do regular expression processing, such as searching for
a word, running a simple query-replace of one word against another, performing a
query-replace with a regex against a specific word, and applying regexp grouping?
Summary
The C library has several interfaces for text processing, but we must be careful about
const-qualification and buffer overflow.
Formatted input with scanf (and similar) has subtle issues with pointer types, null
termination of strings, white space, and new-line separation. If possible, you should
use the combination of fgets with strtod or similar, more specialized, functions.
Extended character sets are best handled by using multibyte strings. With some
caution, these can be used much like ordinary strings for input and output.
Binary data should be written to binary files by using fwrite and fread. Such
files are platform dependent.
Calls to C library functions should be checked for error returns.
Handling error conditions can lead to complicated case analysis. It can be organized
by a function-specific code block to which we jump with goto statements.



15.

Out-of-bounds access of arrays
Accessing uninitialized objects
Accessing objects after their lifetime has ended
Integer overflow

Functions that receive pointers should use array syntax and distinguish different cases:

– A pointerto a single object of the type – These functionsshould use the static 1
notation and thus indicate that they expect a pointer that is non-null:
void func(double a[static 1]);
– A pointer to a collection of objects of known number – These functions should use
the static N notation and thus indicate that they expect a pointer that points to
at least that number of elements:
void func(double a[static 7]);
– A pointer to a collection of objects of unknown number – These functions should
use the VLA notation:
void func(size_t n, double a[n]);
– A pointer to a single object of the type or a null pointer – Such a function must
guarantee that even when it receives a null pointer, the execution remains in a
defined state:
void func(double* a);
Compiler builders only start to implement checks for these cases, so your compiler
probably will not yet detect such errors. Nevertheless, writing these down and make
them clear for yourself will help you to avoid out-of-bounds errors.
Taking addresses of block-scope (local) variables should be avoided, if possible.
Therefore, it is good practice to mark all variables in complex code with register.
Error checking and cleanup 259
Use unsigned integer types for loop indices, and handle wrap-around explicitly. The
latter can, for example, be achieved by comparing the loop variable to the maximum
value of the type before the increment operation.


The keywords that introduce these optimization opportunities are register (C90), inline,
restrict (both from C99), and alignas (respectively _Alignas, C11). As indicated,
all four have the property that they could be omitted from a valid program without changing
its semantics.

Just remember that register can help to avoid aliasing between objects that are defined locally in a function.

C11’s alignas and the related alignof can
help to position objects on cache boundaries and thus improve memory access. We will not
go into more detail about this specialized feature.

The remaining two features, C99’s inline (section 15.1) and restrict 

The first is relatively easy to use and presents no danger. It is a
tool that is quite widely used and may ensure that the code for short functions can be directly
integrated and optimized at the caller side of the function.

The latter, restrict, relaxes the type-based aliasing considerations to allow for better
optimization. Thus it is subtle to use and can do considerable harm if used badly. It is often
found in library interfaces, but much less in user code.



<!-- INLINE FUNCTIONS -->

// compilers do something equivalent to replacing the call to small functions with the code of the function itself to eliminate the overhead of a function call

// 

A traditional C compiler can only inline functions for which it also knows the definition: only knowing the declaration is not enough. 

Unlike what the
naming might suggest, this does not force a function to be inlined, but only provides a way
that it may be.

A function definition that is declared with inline can be used in several TUs without causing a multiple-symbol-definition error.
All pointers to the same inline function will compare as equal, even if obtained
in different TUs.

An inline function that is not used in a specific TU will be completely absent from
the binary of that TU and, in particular, will not contribute to its size.


TAKEAWAY 15.6 Adding a compatible declaration without the inline keyword ensures the emission of the function symbol in the current TU.

As an example, suppose we have an inline function like this in a header file: say toto.h:
1 // Inline definition in a header file.
2 // Function argument names and local variables are visible
3 // to the preprocessor and must be handled with care.
4 inline
5 toto* toto_init(toto* toto_x){
6 if (toto_x) {
7 *toto_x = (toto){ 0 };
8 }
9 return toto_x;
10 }
Such a function is a perfect candidate for inlining. It is really small, and the initialization of
any variable of type toto is probably best made in place. The call overhead is of the same
order as the inner part of the function, and in many cases the caller of the function may even
omit the test for the if.
TAKEAWAY 15.7 An inline function definition is visible in all TUs.
This function may be inlined by the compiler in all TUs that see this code, but none of them
would effectively emit the symbol toto_init. But we can (and should) enforce the emission in one TU, toto.c, say, by adding a line like the following:
1 #include ”toto.h”
2
3 // Instantiate in exactly one TU.
4 // The parameter name is omitted to avoid macro replacement.
5 toto* toto_init(toto*);
TAKEAWAY 15.8 An inline definition goes in a header file.
TAKEAWAY 15.9 An additional declaration without inline goes in exactly one TU.
As we said, that mechanism of inline functions is there to help the compiler make the
decision whether to effectively inline a function. In most cases, the heuristics that compiler
builders have implemented to make that decision are completely appropriate, and you can’t
Using restrict qualifiers 263
do better. They know the particular platform for which the compilation is done much better
than you: maybe this platform didn’t even exist when you wrote your code. So they are in a
much better position to compare the trade-offs between the different possibilities.
An important family of functions that may benefit from inline definitions is pure functions, which we met in section 10.2.2. If we look at the example of the rat structure (listing 10.1), we see that all the functions implicitly copy the function arguments and the return
value. If we rewrite all these functions as inline in the header file, all these copies can be
avoided using an optimizing compiler.[Exs 1] [Exs 2]
So inline functions can be a precious tool to build portable code that shows good performance; we just help the compiler(s) to make the appropriate decision. Unfortunately, using
inline functions also has drawbacks that should be taken into account for our design.
First, 15.7 implies that any change you make to an inline function will trigger a complete rebuild of your project and all of its users.
TAKEAWAY 15.10 Only expose functions as inline if you consider them to be stable.
Second, the global visibility of the function definition also has the effect that local identifiers
of the function (parameters or local variables) may be subject to macro expansion for macros
that we don’t even know about. In the example, we used the toto_ prefix to protect the
function parameters from expansion by macros from other include files.
TAKEAWAY 15.11 All identifiers that are local to an inline function should be protected by
a convenient naming convention.
Third, other than conventional function definitions, inline functions have no particular
TU with which they are associated. Whereas a conventional function can access state and
functions that are local to the TU (static variables and functions), for an inline function,
it would not be clear which copy of which TU these refer to.
TAKEAWAY 15.12 inline functions can’t access identifiers of static functions.
TAKEAWAY 15.13 inline functions can’t define or access identifiers of modifiable static
objects.
Here, the emphasis is on the fact that access is restricted to the identifiers and not the objects
or functions themselves. There is no problem with passing a pointer to a static object or a
function to an inline function.
15.2 Using restrict qualifiers
We have seen many examples of C library functions that use the keyword restrict to
qualify pointers, and we also have used this qualification for our own functions. The basic
idea of restrict is relatively simple: it tells the compiler that the pointer in question is
[Exs 1] Rewrite the examples from section 10.2.2 with inline. [Exs 2] Revisit the function examples in section 7, and argue for each of them whether they should be defined
inline.
264 Performance
the only access to the object it points to. Thus the compiler can make the assumption that
changes to the object can only occur through that same pointer, and the object cannot change
inadvertently. In other words, with restrict, we are telling the compiler that the object
does not alias any other object the compiler handles in this part of the code.
TAKEAWAY 15.14 A restrict-qualified pointer has to provide exclusive access.
As is often the case in C, such a declaration places the burden of verifying this property on
the caller.
TAKEAWAY 15.15 A restrict-qualification constrains the caller of a function.
Consider, for example, the differences between memcpy and memmove:
1 void* memcpy(void*restrict s1, void const*restrict s2, size_t n);
2 void* memmove(void* s1, const void* s2, size_t n);
For memcpy, both pointers are restrict-qualified. So for the execution of this function,
the access through both pointers has to be exclusive. Not only that, s1 and s2 must have
different values, and neither of them can provide access to parts of the object of the other. In
other words, the two objects that memcpy “sees” through the two pointers must not overlap.
Assuming this can help to optimize the function.
In contrast, memmove does not make such an assumption. So s1 and s2 may be equal, or
the objects may overlap. The function must be able to cope with that situation. Therefore it
might be less efficient, but it is more general.
We saw in section 12.3 that it might be important for the compiler to decide whether two
pointers may in fact point to the same object (aliasing). Pointers to different base types are
not supposed to alias, unless one of them is a character type. So both parameters of fputs
are declared with restrict
1 int fputs(const char *restrict s, FILE *restrict stream);
although it might seem very unlikely that anyone might call fputs with the same pointer
value for both parameters.
This specification is more important for functions like printf and friends:
1 int printf(const char *restrict format, ...);
2 int fprintf(FILE *restrict stream, const char *restrict format, ...);
The format parameter shouldn’t alias any of the arguments that might be passed to the ...
part. For example, the following code has undefined behavior:
1 char const* format = ”format printing itself: %s\n”;
2 printf(format, format); // Restrict violation
This example will probably still do what you think it does. If you abuse the stream parameter, your program might explode:
Measurement and inspection 265
1 char const* format = ”First two bytes in stdin object: %.2s\n”;
2 char const* bytes = (char*)stdin; // Legal cast to char
3 fprintf(stdin, format, bytes); // Restrict violation
Sure, code like this is not very likely to occur in real life. But keep in mind that character
types have special rules concerning aliasing, and therefore all string-processing functions may
be subject to missed optimization. You could add restrict-qualifications in many places
where string parameters are involved, and which you know are accessed exclusively through
the pointer in question.
15.3 Measurement and inspection
We have several times spoken about the performance of programs without yet talking about
methods to assess it. And indeed, we humans are notoriously bad at predicting the performance of code. So, our prime directive for questions concerning performance should be:
TAKEAWAY E Don’t speculate about the performance of code; verify it rigorously.
The first step when we dive into a code project that may be performance-critical will always
be to choose the best algorithms that solve the problem(s) at hand. This should be done even
before coding starts, so we have to make a first complexity assessment by arguing (but not
speculating!) about the behavior of such an algorithm.
TAKEAWAY 15.16 Complexity assessment of algorithms requires proofs.
Unfortunately, a discussion of complexity proofs is far beyond the scope of this book, so we
will not be able to go into it. But, fortunately, many other books have been written about it.
The interested reader may refer to the textbook of Cormen et al. [2001] or to Knuth’s treasure
trove.
TAKEAWAY 15.17 Performance assessment of code requires measurement.
Measurement in experimental sciences is a difficult subject, and obviously we can’t tackle it
here in full detail. But we should first be aware that the act of measuring modifies the observed. This holds in physics, where measuring the mass of an object necessarily displaces it;
in biology, where collecting samples of species actually kills animals or plants; and in sociology, where asking for gender or immigration background before a test changes the behavior of
the test subjects. Not surprisingly it also holds in computer science and, in particular, for time
measurement, since all such time measurements need time themselves to be accomplished.
TAKEAWAY 15.18 All measurements introduce bias.
At the worst, the impact of time measurements can go beyond the additional time spent making the measurement. In the first place, a call to timespec_get, for example, is a call to
a function that wouldn’t be there if we didn’t measure. The compiler has to take some precautions before any such call, in particular saving hardware registers, and has to drop some
assumptions about the state of the execution. So time measurement can suppress optimization
opportunities. Also, such a function call usually translates into a system call (a call into the
266 Performance
operating system), and this can have effects on many properties of the program execution,
such as on the process or task scheduling, or can invalidate data caches.
TAKEAWAY 15.19 Instrumentation changes compile-time and runtime properties.
The art of experimental sciences is to address these issues and to ensure that the bias introduced by the measurement is small and so the result of an experiment can be assessed
qualitatively. Concretely, before we can do any time measurements on code that interests us,
we have to assess the bias that time measurements themselves introduce. A general strategy to
reduce the bias of measurement is to repeat an experiment several times and collect statistics
about the outcomes. Most commonly used statistics in this context are simple. They concern the number of experiments and their mean value µ (or average), and also their standard
deviation and sometimes their skew.
Let us look at the following sample S that consists of 20 timings, in seconds s:
0.7, 1.0, 1.2, 0.6, 1.3, 0.1, 0.8, 0.3, 0.4, 0.9, 0.5, 0.2, 0.6, 0.4, 0.4, 0.5, 0.5, 0.4,
0.6, 0.6
See figure 15.1 for a frequency histogram of this sample. The values show quite a variation
 0
 1
 2
 3
 4
 5
 6
 7
 8
 0 0.2 0.4 0.6 0.8 1 1.2 1.4 Mean
Standard deviation
Samples
Seconds
Figure 15.1 Frequency histogram for our sample, showing the frequency with which each of the measured
values was obtained
around 0.6 (µ(S), mean value), from 0.1 (minimum) to 1.3 (maximum). In fact, this
variation is so important that I personally would not dare to claim much about the relevance
of such a sample. These fictive measurements are bad, but how bad are they?
The standard deviation σ(S) measures (again, in seconds) how an observed sample deviates from an ideal world where all timings have exactly the same result. A small standard
deviation indicates that there is a good chance the phenomenon that we are observing follows
that ideal. Conversely, if the standard deviation is too high, the phenomenon may not have
that ideal property (there is something that perturbs our computation), or our measurements
might by unreliable (there is something that perturbs our measurement), or both.
Measurement and inspection 267
For our example, the standard deviation is 0.31, which is substantial compared to the
mean value of 0.6: the relative standard deviation σ(S)/µ(S) here is 0.52 (or 52%). Only
a value in a low percentage range can be considered good.
TAKEAWAY 15.20 The relative standard deviation of run times must be in a low percentage
range.
The last statistical quantity that we might be interested in is the skew (0.79 for our sample
S). It measures the lopsidedness (or asymmetry) of the sample. A sample that is distributed
symmetrically around the mean would have a skew of 0, and a positive value indicates that
there is a “tail” to the right. Time measurements usually are not symmetric. We can easily
see that in our sample: the maximum value 1.3 is at distance 0.7 from the mean. So for the
sample to be symmetric around the mean of 0.6, we would need one value of -0.1, which
is not possible.
If you are not familiar with these very basic statistical concepts, you should probably revisit
them a bit, now. In this chapter, we will see that all these statistical quantities that interest us
can be computed with the raw moments:
mk(S) = ∑
for all s ∈ S
sk
So, the zeroth raw moment counts the number of samples, the first adds up the total number
of values, the second is the sum of the squares of the values, and so on.
For computer science, the repetition of an experiment can easily be automated by putting
the code that is to be sampled inside a for loop and placing the measurements before and
after this loop. Thereby, we can execute the sample code thousands or millions of times
and compute the average time spent for a loop iteration. The hope then is that the time
measurement can be neglected because the overall time spent in the experiment is maybe
several seconds, whereas the time measurement itself may take just several milliseconds.
In this chapter’s example code, we will try to assessthe performance of callsto timespec_
get and also of a small utility that collects statistics of measurements. Listing 15.1 contains several for loops around different versions of code that we want to investigate. The
time measurements are collected in a statistic and use a tv_nsec value obtained from
timespec_get. In this approach, the experimental bias that we introduce is obvious: we
use a call to timespec_get to measure its own performance. But this bias is easily mastered: augmenting the number of iterations reduces the bias. The experiments that we report
here were performed with a value of iterations of 224 − 1.
Listing 15.1 Measuring several code snippets repeatedly
53 timespec_get(&t[0], TIME_UTC);
54 /* Volatile for i ensures that the loop is effected */
55 for (uint64_t volatile i = 0; i < iterations; ++i) {
56 /* do nothing */
57 }
58 timespec_get(&t[1], TIME_UTC);
59 /* s must be volatile to ensure that the loop is effected */
268 Performance
60 for (uint64_t i = 0; i < iterations; ++i) {
61 s = i;
62 }
63 timespec_get(&t[2], TIME_UTC);
64 /* Opaque computation ensures that the loop is effected */
65 for (uint64_t i = 1; accu0 < upper; i += 2) {
66 accu0 += i;
67 }
68 timespec_get(&t[3], TIME_UTC);
69 /* A function call can usually not be optimized out. */
70 for (uint64_t i = 0; i < iterations; ++i) {
71 timespec_get(&tdummy, TIME_UTC);
72 accu1 += tdummy.tv_nsec;
73 }
74 timespec_get(&t[4], TIME_UTC);
75 /* A function call can usually not be optimized out, but
76 an inline function can. */
77 for (uint64_t i = 0; i < iterations; ++i) {
78 timespec_get(&tdummy, TIME_UTC);
79 stats_collect1(&sdummy[1], tdummy.tv_nsec);
80 }
81 timespec_get(&t[5], TIME_UTC);
82 for (uint64_t i = 0; i < iterations; ++i) {
83 timespec_get(&tdummy, TIME_UTC);
84 stats_collect2(&sdummy[2], tdummy.tv_nsec);
85 }
86 timespec_get(&t[6], TIME_UTC);
87 for (uint64_t i = 0; i < iterations; ++i) {
88 timespec_get(&tdummy, TIME_UTC);
89 stats_collect3(&sdummy[3], tdummy.tv_nsec);
90 }
91 timespec_get(&t[7], TIME_UTC);
But this mostly trivial observation is not the goal; it only serves as an example of some code
that we want to measure. The for loops in listing 15.1 contain code that does the statistics
collection with more sophistication. The goal is to be able to assert, step by step, how this
increasing sophistication influences the timing.
timespec.c
struct timespec tdummy;
stats sdummy[4] = { 0 };
The loop starting on line 70 just accumulates the values, so we may determine their average. The next loop (line 77) uses a function stats_collect1 that maintains a running
mean: that is, it implements a formula that computes a new average µn by modifying the
previous one by δ(xn, µn−1), where xn is the new measurement and µn−1 is the previous
average. The other two loops (lines 82 and 87) then use the functions stats_collect2
and stats_collect3, respectively, which use similar formulas for the second and third
moment, respectively, to compute variance and skew. We will discuss these functions shortly.
But first, let us have a look at the tools we use for the instrumentation of the code.
Measurement and inspection 269
Listing 15.2 Collecting time statistics with timespec_diff and stats_collect2
102 for (unsigned i = 0; i < loops; i++) {
103 double diff = timespec_diff(&t[i+1], &t[i]);
104 stats_collect2(&statistic[i], diff);
105 }
We use timespec_diff from section 11.2 to compute the time difference between two
measurements and stats_collect2 to sum up the statistics. The whole is then wrapped
in another loop (not shown) that repeats that experiment 10 times. After finishing that loop,
we use functions for the stats type to print out the result.
Listing 15.3 Printing time statistics with stats_mean and stats_rsdev_unbiased
109 for (unsigned i = 0; i < loops; i++) {
110 double mean = stats_mean(&statistic[i]);
111 double rsdev = stats_rsdev_unbiased(&statistic[i]);
112 printf(”loop %u: E(t) (sec):\t%5.2e ± %4.02f%%,\tloop body %5.2e\n”,
113 i, mean, 100.0*rsdev, mean/iterations);
114 }
Here, obviously, stats_mean gives access to the mean value of the measurements. The
function stats_rsdev_unbiased returns the unbiased relative standard deviation: that
is, a standard deviation that is unbiased1 and that is normalized with the mean value.
A typical output of that on my laptop looks like the following:
Terminal
0 loop 0: E(t) (sec): 3.31e-02 ± 7.30%, loop body 1.97e-09
1 loop 1: E(t) (sec): 6.15e-03 ± 12.42%, loop body 3.66e-10
2 loop 2: E(t) (sec): 5.78e-03 ± 10.71%, loop body 3.45e-10
3 loop 3: E(t) (sec): 2.98e-01 ± 0.85%, loop body 1.77e-08
4 loop 4: E(t) (sec): 4.40e-01 ± 0.15%, loop body 2.62e-08
5 loop 5: E(t) (sec): 4.86e-01 ± 0.17%, loop body 2.90e-08
6 loop 6: E(t) (sec): 5.32e-01 ± 0.13%, loop body 3.17e-08
Here, lines 0, 1, and 2 correspond to loops that we have not discussed yet, and lines 3 to 6
correspond to the loops we have discussed. Their relative standard deviations are less than
1%, so we can assert that we have a good statistic and that the times on the right are good
estimates of the cost per iteration. For example, on my 2.1 GHz laptop, this means the execution of one loop iteration of loops 3, 4, 5, or 6 takes about 36, 55, 61, and 67 clock cycles,
respectively. So the extra cost when replacing the simple sum by stats_collect1 is 19
cycles, from there to stats_collect2 is 6, and yet another 6 cycles are needed if we use
stats_collect3 instead.
To see that this is plausible, let us look at the stats type:
1 Such that it is a true estimation of the standard deviation of the expected time, not only of our arbitrary sample.
270 Performance
1 typedef struct stats stats;
2 struct stats {
3 double moment[4];
4 };
Here we reserve one double for all statistical moments. Function stats_collect in
the following listing then shows how these are updated when we collect a new value that we
insert.
Listing 15.4 Collecting statistics up to the third moment
120 /** 121 ** @brief Add value @a val to the statistic @a c.
122 **/
123 inline
124 void stats_collect(stats* c, double val, unsigned moments) {
125 double n = stats_samples(c);
126 double n0 = n-1;
127 double n1 = n+1;
128 double delta0 = 1;
129 double delta = val - stats_mean(c);
130 double delta1 = delta/n1;
131 double delta2 = delta1*delta*n;
132 switch (moments) {
133 default:
134 c->moment[3] += (delta2*n0 - 3*c->moment[2])*delta1;
135 case 2:
136 c->moment[2] += delta2;
137 case 1:
138 c->moment[1] += delta1;
139 case 0:
140 c->moment[0] += delta0;
141 }
142 }
As previously mentioned, we see that this is a relatively simple algorithm to update the
moments incrementally. Important features compared to a naive approach are that we avoid
numerical imprecision by using the difference from the current estimation of the mean value,
and that this can be done without storing all the samples. This approach was first described for
mean and variance (first and second moments) by Welford [1962] and was then generalized
to higher moments; see Pébay [2008]. In fact, our functions stats_collect1 and so on
are just instantiations of that for the chosen number of moments.
stats.h
154 inline
155 void stats_collect2(stats* c, double val) {
156 stats_collect(c, val, 2);
157 }
Measurement and inspection 271
The assembler listing in stats_collect2 shows that our finding of using 25 cycles for
this functions seems plausible. It corresponds to a handful of arithmetic instructions, loads,
and stores.2
Listing 15.5 GCC’s assembler for stats_collect2(c)
vmovsd 8(%rdi), %xmm1
vmovsd (%rdi), %xmm2
vaddsd .LC2(%rip), %xmm2, %xmm3
vsubsd %xmm1, %xmm0, %xmm0
vmovsd %xmm3, (%rdi)
vdivsd %xmm3, %xmm0, %xmm4
vmulsd %xmm4, %xmm0, %xmm0
vaddsd %xmm4, %xmm1, %xmm1
vfmadd213sd 16(%rdi), %xmm2, %xmm0
vmovsd %xmm1, 8(%rdi)
vmovsd %xmm0, 16(%rdi)
Now, by using the example measurements, we still made one systematic error. We took
the points of measure outside the for loops. By doing so, our measurements also form the
instructions that correspond to the loops themselves. Listing 15.6 shows the three loops that
we skipped in the earlier discussion. These are basically empty, in an attempt to measure the
contribution of such a loop.
Listing 15.6 Instrumenting three for loops with struct timespec
53 timespec_get(&t[0], TIME_UTC);
54 /* Volatile for i ensures that the loop is effected */
55 for (uint64_t volatile i = 0; i < iterations; ++i) {
56 /* do nothing */
57 }
58 timespec_get(&t[1], TIME_UTC);
59 /* s must be volatile to ensure that the loop is effected */
60 for (uint64_t i = 0; i < iterations; ++i) {
61 s = i;
62 }
63 timespec_get(&t[2], TIME_UTC);
64 /* Opaque computation ensures that the loop is effected */
65 for (uint64_t i = 1; accu0 < upper; i += 2) {
66 accu0 += i;
67 }
68 timespec_get(&t[3], TIME_UTC);
In fact, when trying to measure for loops with no inner statement, we face a severe problem: an empty loop with no effect can and will be eliminated at compile time by the optimizer.
Under normal production conditions, this is a good thing; but here, when we want to measure,
this is annoying. Therefore, we show three variants of loops that should not be optimized
2 This assembler shows x86_64 assembler features that we have not yet seen: floating-point hardware registers
and instructions, and SSE registers and instructions. Here, memory locations (%rdi), 8(%rdi), and 16(%rdi)
correspond to c->moment[i], for i = 0, 1, 2, the name of the instruction minus the v-prefix; sd-postfix shows
the operation that is performed; and vfmadd213sd is a floating-point multiply add instruction.
272 Performance
out. The first declares the loop variable as volatile such that all operations on the variable
must be emitted by the compiler. Listings 15.7 and 15.8 show GCC’s and Clang’s versions of
this loop. We see that to comply with the volatile qualification of the loop variable, both
have to issue several load and store instructions.
Listing 15.7 GCC’s version of the first loop from Listing 15.6
.L510:
movq 24(%rsp), %rax
addq $1, %rax
movq %rax, 24(%rsp)
movq 24(%rsp), %rax
cmpq %rax, %r12
ja .L510
Listing 15.8 Clang’s version of the first loop from listing 15.6
.LBB9_17:
incq 24(%rsp)
movq 24(%rsp), %rax
cmpq %r14, %rax
jb .LBB9_17
For the next loop, we try to be a bit more economical by only forcing one volatile
store to an auxiliary variable s. As we can see in listings 15.9, the result is assembler code
that looks quite efficient: it consists of four instructions, an addition, a comparison, a jump,
and a store.
Listing 15.9 GCC’s version of the second loop from listing 15.6
.L509:
movq %rax, s(%rip)
addq $1, %rax
cmpq %rax, %r12
jne .L509
To come even closer to the loop of the real measurements, in the next loop we use a trick:
we perform index computations and comparisons for which the result is meant to be opaque to
the compiler. Listing 15.10 shows that this results in assembler code similar to the previous,
only now we have a second addition instead of the store operation.
Listing 15.10 GCC’s version of the third loop from listing 15.6
.L500:
addq %rax, %rbx
addq $2, %rax
cmpq %rbx, %r13
ja .L500
Table 15.1 summarizes the results we collected here and relates the differences between
the various measurements. As we might expect, we see that loop 1 with the volatile store
Measurement and inspection 273
is 80% faster than the loop with a volatile loop counter. So, using a volatile loop
counter is not a good idea, because it can deteriorate the measurement.
On the other hand, moving from loop 1 to loop 2 has a not-very-pronounced impact. The
6% gain that we see is smaller than the standard deviation of the test, so we can’t even be sure
there is a gain at all. If we would really like to know whether there is a difference, we would
have to do more tests and hope that the standard deviation was narrowed down.
But for our goal to assess the time implications of our observation, the measurements are
quite conclusive. Versions 1 and 2 of the for loop have an impact that is about one to two
orders of magnitude below the impact of calls to timespec_get or stats_collect. So
we can assume that the values we see for loops 3 to 6 are good estimators for the expected
time of the measured functions.
There is a strong platform-dependent component in these measurements: time measurement with timespec_get. In fact, we learned from this experience that on my machine,3
time measurement and statistics collection have a cost that is of the same order of magnitude.
For me, personally, this was a surprising discovery: when I wrote this chapter, I thought time
measurement would be much more expensive.
We also learned that simple statistics such as the standard deviation are easy to obtain and
can help to assert claims about performance differences.
TAKEAWAY 15.21 Collecting higher-order moments of measurements to compute variance
and skew is simple and cheap.
So, whenever you make performance claims in the future or see such claims made by others,
be sure the variability of the results has at least been addressed.
TAKEAWAY 15.22 Runtime measurements must be hardened with statistics.
Summary
Performance should not be traded for correctness.
inline is an adequate tool to optimize small, pure, functions in place.
Table 15.1 Comparison of measurements
Loop Sec per iteration Difference Gain/loss Conclusive
0 volatile loop 1.97·10−09
1 volatile store 3.66·10−10 -1.60·10−09 -81% Yes
2 Opaque addition 3.45·10−10 -2.10·10−11 -6% No
3 Plus timespec_get 1.77·10−08 1.74·10−08 +5043% Yes
4 Plus mean 2.62·10−08 8.5·10−09 +48% Yes
5 Plus variance 2.90·10−08 2.8·10−09 +11% Yes
6 Plus skew 3.17·10−08 2.7·10−09 +9% Yes
3 A commodity Linux laptop with a recent system and modern compilers as of 2016.
274 Performance
restrict helps to deal with aliasing properties of function parameters. It has to
be used with care, because it imposes restrictions on the calling side of the function
that may not be enforceable at compile time.
Claims of performance improvements must be accompanied by thorough measurements and statistics.