%{ /* ---------- C prologue ---------- */
#include <stdio.h>   /* Provides printf, fprintf, fopen, etc. */

extern FILE *yyin;   /* Flex's global pointer for the active input stream. */

int yywrap(void) {   /* Automatically called when Flex sees EOF. */
    return 1;        /* Returning non-zero tells Flex to terminate scanning. */
}
%}

%% /* ---------- Pattern rules ---------- */
<[^>]+>   { printf("TAG: %s\n", yytext); } /* Match everything between '<' and '>' and print it. */
.|\n      { /* Consume any other characters (text, whitespace) without output. */ }
%%

int main(int argc, char **argv) {
    if (argc != 2) {                                               /* Require a single HTML filename. */
        fprintf(stderr, "Usage: %s <html-file>\n", argv[0]);       /* Display usage instructions on the error stream. */
        return 1;                                                  /* Signal incorrect usage. */
    }

    yyin = fopen(argv[1], "r");                                    /* Open the HTML file in read mode. */
    if (!yyin) {                                                   /* fopen returns NULL if it fails. */
        perror("Unable to open input file");                       /* Print the OS-provided error message. */
        return 1;                                                  /* Abort because we cannot continue without input. */
    }

    printf("HTML tags found in %s:\n", argv[1]);                   /* Tell the user what we are scanning. */
    yylex();                                                       /* Start the Flex-generated scanning loop. */

    fclose(yyin);                                                  /* Clean up resources by closing the file. */
    return 0;                                                      /* Exit successfully. */
}
