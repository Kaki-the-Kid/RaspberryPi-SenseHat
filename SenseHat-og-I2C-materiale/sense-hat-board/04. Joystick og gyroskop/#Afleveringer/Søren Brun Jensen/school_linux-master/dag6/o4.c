#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
   printf("The maximum value of int = %d\n", INT_MAX);
   printf("The maximum value of short int = %d\n", SHRT_MAX); 
   printf("The maximum value of long int = %ld\n", LONG_MAX);
   printf("The maximum value of float = %.10e\n", FLT_MAX);
   printf("The maximum value of double = %.10e\n", DBL_MAX);
   printf("The maximum value of char = %d\n", CHAR_MAX);
}
