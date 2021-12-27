// arithmetic operators
1 + 2
1 - 2
1 * 2
1 / 2
1 % 2

// boolean operators
true && false
true || false
!true

// comparison operators
1 == 2
1 != 2
1 > 2
1 >= 2
1 &lt; 2
1 &lt;= 2

// logical operators
var x uint8 = 1&lt;&lt;1 | 1&lt;&lt;5  // 00100010
var y uint8 = 1&lt;&lt;1 | 1&lt;&lt;2  // 00000110

x&y   // 00000010
x|y   // 00100110
x^y   // 00100100
x&^y  // 00100000
x&lt;&lt;1  // 01000100
x>>1  // 00010001



// arrays can be compared with the == and != operators
fmt.Println([3]int{1, 2, 3} == [3]int{1, 2, 3})


Integer operators
Go integers support the usual arithmetic operators: +, -, *, /, with % for modulus. The
result of an integer division is an integer; if you want to get a floating point result, you
need to use a type conversion to make your integers into floating point numbers.
Also, be careful not to divide an integer by 0; this causes a panic (we talk more about
panics in “panic and recover” on page 174).
Integer division in Go follows truncation toward zero; see the Go
spec’s section on arithmetic operators for the full details.
You can combine any of the arithmetic operators with = to modify a variable: +=, -=,
*=, /=, and %=. For example, the following code results in x having the value 20:
var x int = 10
x *= 2
You compare integers with ==, !=, >, >=, <, and <=.
Go also has bit-manipulation operators for integers. You can bit shift left and right
with << and >>, or do bit masks with & (logical AND), | (logical OR), ^ (logical XOR),
and &^ (logical AND NOT). Just like the arithmetic operators, you can also combine
all of the logical operators with = to modify a variable: &=, |=, ^=, &^=, <<=, and >>=.



You can use all the standard mathematical and comparison operators with floats,
except %. Floating point division has a couple of interesting properties. Dividing a
nonzero floating point variable by 0 returns +Inf or -Inf (positive or negative infin‐
ity), depending on the sign of the number. Dividing a floating point variable set to 0
by 0 returns NaN (Not a Number).



While Go lets you use == and != to compare floats, don’t do it. Due to the inexact
nature of floats, two floating point values might not be equal when you think they
should be. Instead, define a maximum allowed variance and see if the difference
between two floats is less than that. This value (sometimes called epsilon) depends on
what your accuracy needs are


All of the standard arithmetic operators work on complex numbers. Just like floats,
you can use == or != to compare them, but they have the same precision limitations,
so it’s best to use the epsilon technique. You can extract the real and imaginary por‐
tions of a complex number with the real and imag built-in functions, respectively.
There are also some additional functions in the math/cmplx package for manipulating
complex128 values.

Like integers and floats, strings are compared for equality using ==, difference with !=, or ordering with >, >=, <, or <=. They are concatenated by using the +
operator.


 x := complex(2.5, 3.1)
 y := complex(10.2, 2)
 fmt.Println(x + y)
 fmt.Println(x - y)
 fmt.Println(x * y)
 fmt.Println(x / y)
 fmt.Println(real(x))
 fmt.Println(imag(x))
 