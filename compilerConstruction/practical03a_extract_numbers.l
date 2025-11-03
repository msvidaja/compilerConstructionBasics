%{ /* ---------- C prologue ---------- */
#include <stdio.h>   /* Provide declarations for printf, fprintf, fopen, etc. */

extern FILE *yyin;   /* Flex uses yyin to decide where to read characters from. */

int yywrap(void) {   /* Called automatically once Flex hits end-of-file. */
    return 1;        /* Return non-zero so scanning stops cleanly. */
}
%}

%% /* ---------- Pattern rules ---------- */
[0-9]+    { printf("NUMBER: %s\n", yytext); } /* Match one-or-more digits and print them. */
.|\n      { /* Match any other character (including newline) and ignore it silently. */ }
%%

int main(int argc, char **argv) {
    if (argc != 2) {                                                /* Expect exactly one filename from the user. */
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);       /* Print correct usage to the error stream. */
        return 1;                                                   /* Exit with failure if the argument is missing. */
    }

    yyin = fopen(argv[1], "r");                                     /* Try to open the supplied file for reading. */
    if (!yyin) {                                                    /* fopen returns NULL when it fails. */
        perror("Unable to open input file");                        /* Report why we could not open it. */
        return 1;                                                   /* Abort because scanning cannot continue. */
    }

    printf("Numbers found in %s:\n", argv[1]);                      /* Friendly banner before scanning. */
    yylex();                                                        /* Run the Flex-generated scanner loop. */

    fclose(yyin);                                                   /* Close the file once we are done with it. */
    return 0;                                                       /* Successful completion. */
}
