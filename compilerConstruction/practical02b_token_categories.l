%{ /* ---------- C prologue ---------- */
#include <stdio.h>                /* printf / fprintf come from stdio. */

int yylineno = 1;                 /* Start counting lines at 1 like text editors do. */

void yyerror(const char *s);      /* Forward declaration for our custom error handler. */

int yywrap(void) {                /* Called when Flex reaches EOF. */
    return 1;                     /* Return non-zero so the scanner stops gracefully. */
}
%}

%% /* ---------- Token rules ---------- */
[0-9]           { printf("DIGIT: '%s'\n", yytext); }                      /* Single digit token. */
[a-zA-Z]        { printf("LETTER: '%s'\n", yytext); }                     /* Alphabetic character token. */
[ \t]+          { printf("WHITESPACE: '%s'\n", yytext); }                 /* One or more spaces or tabs. */
\n              { yylineno++; printf("NEWLINE (Line: %d)\n", yylineno); } /* Track newlines to update yylineno. */
.               { printf("OTHER_CHARACTER: '%s'\n", yytext); }            /* Any other character falls here. */
%%

void yyerror(const char *s) { /* Print detailed error information when parsing fails. */
    fprintf(stderr, "Error at line %d: %s ('%s')\n", yylineno, s, yytext); /* stderr makes the error stand out. */
}

int main(void) {
    printf("Lexical Analyzer for character categories starting.\n"); /* Banner for the user. */
    yylex();                                                         /* Run the scanner loop produced by Flex. */
    printf("Lexical analysis finished.\n");                          /* All input consumed. */
    return 0;                                                        /* Signal success to the shell. */
}
