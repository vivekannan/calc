#calc

  calc is a simple command-line based calculator that supports most of the functions that one can find in a scientific calculator.

##Usage

  Compile with,
  
    gcc -std=c99 -o calc calc.c helpers.c -lm

  Run with,
  
    ./calc [OPTIONS] <EXPRESSION>...

  Obviously, the expressions are expected to be quoted within single quotes to prevent bash/shell from parsing them.

##Options

  The following flags can be used,

  1. -d, consider all angles to be degrees. Results are also in degrees.
  2. -n, print results of each epressions in a new line instead of separating them by ';'.
  3. -h (or) --help, print help and exit.

##Expressions
  calc supports multiple expressions in a single call. For example,
  
    ./calc '3.4' 'atan(1/0) - 3.14159265358979323846 * sqrt 3'
  
  The result of each expression is printed either in the same line separated by semi-colons or in separate lines if the -n flag is used.
  
  For example, the output of the above is
  
    3.4;-3.870601766
  
  With -n flag,
  
    3.4
    -3.870601766

##Features

###Operators
The following operators are supported,

    + --> addition
    - --> subtraction
    * --> multiplication
    / --> division
    % --> modulo
    ! --> factorial
    ^ --> exponent

Unary '+' and '-' are also supported. '~' and '_' operators act as aliases for unary '-' and '+' respectively.

The '$' (dollar) operator can be used to refer to the result of a particular sub-expression. Sub-expression are created using the ',' (comma) operator. For example,

    ./calc '7 - 5, sin$1, log($1 - -$2)'

The expression has three sub-expression.

    $1 = 7 - 5
    $2 = sin$1 = sin(7 - 5)
    $3 = log($1 - -$2) = log(7 - 5 - -sin(7 - 5))

The results of all three expressions is printed in comma separated format.

    2,0.9092974268,0.4637881228

###Functions

  The following function are supported,
  
    sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, asinh, acosh, atanh, log (base 10), ln (base e), exp (e raised), floor, ceil, round, sqrt, abs, sgn.
  
  sgn returns 1 if positive, -1 if negative else 0.
  
  Trignometric function may have infinite precision related issues. For example,
  
    ./calc 'sin pi' 'cos pi'
  
  Results in,
  
    7.338823075E-05;-0.9999999973
  
  The actual values being 0 and -1. pi here is a symbolic constant.

###Symbolic Constants

  The following symbolic constants are supported,
  
    pi    -->  3.14159265358979323846
    e     -->  2.7182818284590452354
    g     -->  1.6180339887498948482
    inf   -->  infinity
    rand  -->  random double in the range (0, 1). (ironic, i know...)
  
  For example,
  
    ./calc 'atan inf' 'rand, sin $1, cos($2 - $1)' '-+_+_++_--~---~+_rand'
    
    1.570796327;0.4473471527,0.4325752577,0.9998908975;0.6601152609

##TODO

  1. Add support for complex numbers.
  2. Implicit multiplication. (sin 3 cos 4 --> sin 3 * cos 4, (9 - 4) (sin 3) --> (9 - 4) * (sin 3))
  3. Symbolic mathematics.