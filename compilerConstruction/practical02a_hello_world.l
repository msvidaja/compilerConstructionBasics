%{ /* ----- C prologue: compiled into the generated scanner ----- */
#include <stdio.h> /* Include standard I/O so we can call printf. */

/* yywrap tells Flex we are done whenever it reaches end-of-file. */
int yywrap(void) {
    return 1; /* Return non-zero to signal completion of input. */
}
%} /* ----- End C prologue ----- */

%% /* ----- Pattern-to-action rules ----- */
.   { printf("Hello, World\n"); } /* For every character read, print our message. */

%% /* ----- User code section ----- */

int main(void) {
    yylex();     /* Ask the generated scanner to start reading from stdin. */
    return 0;    /* Exit status 0 means the program completed successfully. */
}
