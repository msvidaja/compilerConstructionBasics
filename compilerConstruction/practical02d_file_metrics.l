%{ /* ---------- C prologue ---------- */
#include <stdio.h>  /* Required for printf, fprintf, fopen, etc. */

extern FILE *yyin;  /* Flex declares this pointer; we point it at our input file. */

int char_count = 0;  /* Total number of characters seen (including whitespace). */
int word_count = 0;  /* Number of word tokens identified by the scanner. */
int line_count = 0;  /* Number of newline characters encountered. */

int yywrap(void) {   /* Invoked when Flex reaches EOF on yyin. */
    return 1;        /* Non-zero return tells Flex to finish scanning. */
}
%}

%% /* ---------- Scanner rules ---------- */
[A-Za-z0-9_]+   { word_count++; char_count += yyleng; } /* Word token: increase word count and add length to char tally. */
\n              { line_count++; char_count++; }         /* Newline: advance both line and character totals. */
[ \t]+          { char_count += yyleng; }               /* Whitespace: add however many spaces/tabs we matched. */
.               { char_count++; }                       /* Any other single character still contributes to char count. */
%%

int main(int argc, char **argv) {
    if (argc < 2) {                                                    /* Expect exactly one filename argument. */
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);            /* stderr makes the usage hint visible on errors. */
        return 1;                                                      /* Exit with failure if missing file. */
    }

    FILE *fp = fopen(argv[1], "r");                                    /* Open the requested file for reading. */
    if (!fp) {                                                         /* fopen returns NULL if it fails. */
        perror("Error opening file");                                  /* Explain why the open failed. */
        return 1;                                                      /* Abort because we cannot scan without the file. */
    }

    yyin = fp;                                                         /* Direct Flex to read from our file instead of stdin. */
    yylex();                                                           /* Run the scanning loop to gather metrics. */
    fclose(fp);                                                        /* Always close files you open. */

    printf("\nLines : %d\nWords : %d\nCharacters : %d\n",               /* Present the final tallies to the user. */
           line_count, word_count, char_count);
    return 0;                                                          /* Return success status. */
}
