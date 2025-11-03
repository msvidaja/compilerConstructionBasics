%{ /* ---------- C prologue ---------- */
#include <stdio.h>       /* printf prototype used in actions and main. */

/*
 * Global flag that becomes 0 once the parser encounters an invalid identifier
 * structure. The parser accepts if the input starts with a letter and is
 * followed by letters or digits only.
 */
static int valid = 1;    /* Assume success unless yyerror marks it invalid. */
%}

%token digit letter      /* Tokens supplied by the scanner (Flex). */

%% /* ---------- Grammar rules ---------- */
start : letter sequence  /* Valid identifier must start with a letter token. */
      ;

sequence : letter sequence  /* Zero or more additional letters keep it valid. */
         | digit sequence   /* Digits allowed after first character. */
         | /* empty */      /* Empty production allows identifier of length 1. */
         ;
%%

int yyerror(const char *unused) {
    (void)unused;                         /* Avoid unused parameter warning. */
    printf("\nIt's not an identifier!\n");/* Feedback for invalid input. */
    valid = 0;                            /* Mark the input as invalid. */
    return 0;                             /* Returning 0 keeps parser running gracefully. */
}

int main(void) {
    printf("Enter a name to test for identifier: "); /* Prompt the learner. */
    yyparse();                                       /* Start parsing tokens from the scanner. */
    if (valid) {                                     /* Only print success message if no errors occurred. */
        printf("\nIt is a valid identifier!\n");
    }
    return 0;                                       /* Exit cleanly. */
}
