#include <stdio.h>   /* Standard I/O for printf, perror, fopen. */
#include <string.h>  /* strcmp used to compare words against keywords. */
#include <ctype.h>   /* Character classification helpers like isalpha. */

/*
 * Simple lexical recognizer that classifies tokens as keywords, identifiers,
 * numbers, or unknown symbols. This mirrors what a minimal lexical analyser
 * would do before passing tokens to later compiler phases.
 */

static const char *keywords[] = {
    "int", "float", "char", "if", "else", "while", "for", "do", "return",
    "main", "void", "double", "break", "continue"
};                                                                  /* Table of reserved words understood by this demo. */
static const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]); /* Compute how many entries are in the table. */

static int is_keyword(const char *text) {
    for (size_t i = 0; i < keyword_count; ++i) {                    /* Iterate over the reserved word list. */
        if (strcmp(text, keywords[i]) == 0) {                       /* strcmp returns zero when strings are equal. */
            return 1;                                               /* Found a match, so classify as keyword. */
        }
    }
    return 0;                                                       /* No match found, thus not a keyword. */
}

int main(int argc, char **argv) {
    FILE *fp = NULL;                                                /* File pointer for input source; defaults to stdin. */
    char word[128];                                                 /* Buffer to hold each token extracted from input. */

    if (argc == 2) {                                                /* If a filename is provided, use it. */
        fp = fopen(argv[1], "r");                                   /* Try to open the file in read mode. */
        if (!fp) {                                                  /* fopen returns NULL on failure. */
            perror("Unable to open input file");                    /* Print OS-level error message. */
            return 1;                                               /* Exit with non-zero status to signal the error. */
        }
    } else {                                                        /* No filename provided, fall back to standard input. */
        fp = stdin;                                                 /* stdin points to the console input stream. */
        printf("Reading tokens from standard input (Ctrl+D to end)...\n"); /* Let the learner know how to finish input. */
    }

    while (fscanf(fp, "%127s", word) != EOF) {                      /* Read whitespace-delimited tokens until EOF. */
        if (is_keyword(word)) {                                     /* First check if the token is in the keyword list. */
            printf("%-16s Keyword\n", word);                        /* %-16s left-aligns tokens for neat columns. */
        } else if (isalpha((unsigned char)word[0]) || word[0] == '_') { /* Identifiers must start with letter or underscore. */
            printf("%-16s Identifier\n", word);                     /* Report identifier classification. */
        } else if (isdigit((unsigned char)word[0])) {               /* Tokens that start with a digit are numbers here. */
            printf("%-16s Number\n", word);                         /* Report numeric classification. */
        } else {                                                    /* Anything else is treated as an unknown symbol. */
            printf("%-16s Unknown\n", word);                        /* Flag the token as unknown. */
        }
    }

    if (fp != stdin) {                                              /* Only close the file if we actually opened one. */
        fclose(fp);                                                 /* Release the file handle to avoid leaks. */
    }
    return 0;                                                       /* Return success after processing the whole stream. */
}
