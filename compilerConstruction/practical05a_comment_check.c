#include <stdio.h>   /* Provides fgets, printf, fprintf. */
#include <string.h>  /* Provides strlen and strcspn helpers. */

/*
 * Classifies a single line of text as a C style comment or not.
 * This is a direct translation of the manual exercise from the practical.
 */
int main(void) {
    char line[256];                                               /* Buffer to hold one line of user input. */

    printf("Enter a line of code: ");                             /* Prompt the user for input. */
    if (!fgets(line, sizeof(line), stdin)) {                      /* fgets returns NULL if no characters were read. */
        fprintf(stderr, "No input provided.\n");                  /* Explain why we are exiting. */
        return 1;                                                 /* Non-zero exit code indicates failure. */
    }

    line[strcspn(line, "\n")] = '\0';                             /* Replace newline at end (if present) with terminator. */

    if (line[0] == '/' && line[1] == '/') {                       /* Check if the line starts with // comment marker. */
        printf("Single-line comment.\n");                         /* Output the classification result. */
    } else if (line[0] == '/' && line[1] == '*') {                /* Check if the line starts a block comment. */
        size_t len = strlen(line);                                /* Determine total length so we can inspect the end. */
        if (len >= 4 && line[len - 2] == '*' && line[len - 1] == '/') { /* Detect star-slash pair to see if comment closes here. */
            printf("Multi-line comment on a single line.\n");      /* Detected complete block comment on one line. */
        } else {
            printf("Start of a multi-line comment (unterminated on this line).\n"); /* Comment continues on later lines. */
        }
    } else {                                                      /* Anything else is treated as regular source text. */
        printf("Not a comment.\n");                              /* Inform the user that it is regular code. */
    }

    return 0;                                                     /* Return success after classification. */
}
