In Kotlin there are two types of equality:

// there are two types of equality: structural equality (a check of equals()) and referential equality (two references point to the same object)


Structural equality is checked by the == operation and its negated counterpart !=. By convention, an expression like a == b is translated to:

a?.equals(b) ?: (b === null)
Copied!
If a is not null, it calls the equals(Any?) function, otherwise (a is null) it checks that b is referentially equal to null.

Note that there's no point in optimizing your code when comparing to null explicitly: a == null will be automatically translated to a === null.

To provide a custom equals check implementation, override the equals(other: Any?): Boolean function. Functions with the same name and other signatures, like equals(other: Foo), don't affect equality checks with the operators == and !=.

Structural equality has nothing to do with comparison defined by the Comparable<...> interface, so only a custom equals(Any?) implementation may affect the behavior of the operator.

Referential equality﻿
Referential equality is checked by the === operation and its negated counterpart !==. a === b evaluates to true if and only if a and b point to the same object. For values represented by primitive types at runtime (for example, Int), the === equality check is equivalent to the == check.

Floating-point numbers equality﻿
When an equality check operands are statically known to be Float or Double (nullable or not), the check follows the IEEE 754 Standard for Floating-Point Arithmetic.

Otherwise, the structural equality is used, which disagrees with the standard so that NaN is equal to itself, and -0.0 is not equal to 0.0.