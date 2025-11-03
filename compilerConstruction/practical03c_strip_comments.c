#include <stdio.h>    /* FILE, printf, fgetc, fputc, perror live here. */
#include <stdlib.h>   /* EXIT_SUCCESS / EXIT_FAILURE macros. */
#include <stdbool.h>  /* bool type used for escape tracking. */

/*
 * This program counts C/C++ style comments in an input file and writes the
 * source code without comments to a second file.
 *
 * Usage:
 *   ./a.out input.c output.c
 *
 * The comment counter treats every line touched by a comment as a comment line.
 */

typedef enum {
    STATE_NORMAL,            /* Copy characters directly to output. */
    STATE_AFTER_SLASH,       /* Saw '/', need to know if comment begins. */
    STATE_LINE_COMMENT,      /* Inside // comment until newline. */
    STATE_BLOCK_COMMENT,     /* Inside block comment between slash-star and star-slash markers. */
    STATE_BLOCK_COMMENT_STAR,/* Saw '*' inside block comment; '/' may end it. */
    STATE_STRING_LITERAL,    /* Inside "string", care about escapes. */
    STATE_CHAR_LITERAL       /* Inside 'c', care about escapes. */
} ScannerState;

int main(int argc, char **argv) {
    if (argc != 3) {                                                  /* Expect executable plus input and output path. */
        fprintf(stderr, "Usage: %s <input.c> <output.c>\n", argv[0]); /* Remind user about correct syntax. */
        return EXIT_FAILURE;                                          /* Non-zero exit signals incorrect usage. */
    }

    FILE *in = fopen(argv[1], "r");                                   /* Open original source file for reading. */
    if (!in) {                                                        /* fopen returns NULL on failure. */
        perror("Unable to open input file");                          /* perror prints OS-specific reason. */
        return EXIT_FAILURE;                                          /* Bail out because we cannot proceed. */
    }

    FILE *out = fopen(argv[2], "w");                                  /* Create destination file to store stripped code. */
    if (!out) {                                                       /* If creation fails we must clean up. */
        perror("Unable to open output file");                         /* Report the issue to the user. */
        fclose(in);                                                   /* Close the already-open input file. */
        return EXIT_FAILURE;                                          /* Abort execution with failure status. */
    }

    int comment_lines = 0;                                            /* Tracks how many lines contained comments. */
    ScannerState state = STATE_NORMAL;                                /* Current finite-state machine position. */
    bool string_escape = false;                                       /* True when last char in string was backslash. */
    bool char_escape = false;                                         /* True when last char in char literal was backslash. */
    int c;                                                            /* Holds each character read from input. */

    while ((c = fgetc(in)) != EOF) {                                  /* Read characters until the stream ends. */
        switch (state) {                                              /* Branch based on current scanner state. */
        case STATE_NORMAL:                                            /* Default: outside comments or quotes. */
            if (c == '/') {                                           /* Slash may start a comment, so inspect next char. */
                state = STATE_AFTER_SLASH;                            /* Transition so we can look ahead. */
            } else {                                                  /* Any other character is emitted immediately. */
                fputc(c, out);                                        /* Copy non-comment characters to output. */
                if (c == '"') {                                       /* Detect start of a string literal. */
                    state = STATE_STRING_LITERAL;                     /* Track string literal to preserve content. */
                    string_escape = false;                            /* Reset escape tracker for string context. */
                } else if (c == '\'') {                               /* Detect start of a character literal. */
                    state = STATE_CHAR_LITERAL;                       /* Switch state for char literal handling. */
                    char_escape = false;                              /* Reset escape tracker for char context. */
                }
            }
            break;                                                    /* Leave switch once we handled the character. */

        case STATE_AFTER_SLASH:                                       /* We previously saw a '/'. */
            if (c == '/') {                                           /* Second slash means a // comment. */
                state = STATE_LINE_COMMENT;                           /* Move to line comment state. */
                comment_lines++;                                      /* Count the line that contains the comment. */
            } else if (c == '*') {                                    /* Slash-star begins a block comment. */
                state = STATE_BLOCK_COMMENT;                          /* Enter block comment state. */
                comment_lines++;                                      /* Count the first line of the block comment. */
            } else {                                                  /* Not a comment, keep both characters. */
                fputc('/', out);                                      /* Output the slash since it is regular text. */
                fputc(c, out);                                        /* Output the current character as well. */
                if (c == '"') {                                       /* Slash followed by quote starts string literal. */
                    state = STATE_STRING_LITERAL;                     /* Switch to string state to preserve escapes. */
                    string_escape = false;
                } else if (c == '\'') {                               /* Slash followed by single quote. */
                    state = STATE_CHAR_LITERAL;                       /* Switch to char literal state. */
                    char_escape = false;
                } else {                                              /* No special construct, return to normal. */
                    state = STATE_NORMAL;
                }
            }
            break;

        case STATE_LINE_COMMENT:                                      /* Inside // comment, drop characters until newline. */
            if (c == '\n') {                                          /* Newline ends a line comment. */
                fputc('\n', out);                                     /* Preserve line structure in output. */
                state = STATE_NORMAL;                                 /* Resume normal processing afterwards. */
            }
            break;

        case STATE_BLOCK_COMMENT:                                     /* Inside /* ... comment body. */
            if (c == '\n') {                                          /* Each newline extends the comment to new line. */
                comment_lines++;                                      /* Count the additional comment line. */
                fputc('\n', out);                                     /* Preserve blank lines so formatting stays. */
            } else if (c == '*') {                                    /* '*' might signal the end if followed by '/'. */
                state = STATE_BLOCK_COMMENT_STAR;                     /* Transition to look for the trailing slash. */
            }
            break;

        case STATE_BLOCK_COMMENT_STAR:                                /* Saw '*' while inside block comment. */
            if (c == '/') {                                           /* '*' followed by '/' ends the comment. */
                state = STATE_NORMAL;                                 /* Return to normal processing state. */
            } else {                                                  /* Comment continues. */
                if (c == '\n') {                                      /* Handle newline inside the comment. */
                    comment_lines++;                                  /* Count the line touched by comment text. */
                    fputc('\n', out);                                 /* Maintain layout in the output file. */
                }
                state = (c == '*') ? STATE_BLOCK_COMMENT_STAR         /* Another '*' keeps us waiting for '/'. */
                                    : STATE_BLOCK_COMMENT;            /* Otherwise we are back inside comment body. */
            }
            break;

        case STATE_STRING_LITERAL:                                    /* Inside a double-quoted string. */
            fputc(c, out);                                            /* Strings must be copied verbatim. */
            if (string_escape) {                                      /* Previous char was backslash, so ignore special meaning. */
                string_escape = false;                                /* Reset escape flag after using it. */
            } else if (c == '\\') {                                   /* Backslash starts an escape sequence. */
                string_escape = true;                                 /* Remember so the next char is treated literally. */
            } else if (c == '"') {                                    /* Closing quote ends the string literal. */
                state = STATE_NORMAL;                                 /* Return to normal scanning. */
            }
            break;

        case STATE_CHAR_LITERAL:                                      /* Inside a single-quoted character constant. */
            fputc(c, out);                                            /* Copy characters to keep literal intact. */
            if (char_escape) {                                        /* Previous char was backslash. */
                char_escape = false;                                  /* Reset escape once consumed. */
            } else if (c == '\\') {                                   /* Start of an escape sequence within char literal. */
                char_escape = true;                                   /* Flag so next character is taken literally. */
            } else if (c == '\'') {                                   /* Closing single quote ends the literal. */
                state = STATE_NORMAL;                                 /* Resume normal scanning. */
            }
            break;
        }
    }

    if (state == STATE_AFTER_SLASH) {                                 /* File ended right after a single '/'. */
        fputc('/', out);                                              /* Emit the slash because it is real code. */
    }

    printf("Total comment lines in %s: %d\n", argv[1], comment_lines);/* Report how many lines contained comments. */

    fclose(in);                                                       /* Close the input file handle. */
    fclose(out);                                                      /* Close the output file handle. */
    return EXIT_SUCCESS;                                              /* Return zero to indicate success. */
}
