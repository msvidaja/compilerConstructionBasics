#include <stdio.h>   /* Standard I/O for scanf, printf, fprintf. */
#include <ctype.h>   /* Character classification helpers like isalnum. */

/*
 * Verifies whether a supplied token is a valid C identifier:
 *   1. The first character cannot be a digit.
 *   2. All characters must be alphanumeric or an underscore.
 */
static int is_valid_identifier(const char *str) {
    if (str == NULL || str[0] == '\0') {                        /* Reject NULL pointers or empty strings immediately. */
        return 0;
    }

    if (isdigit((unsigned char)str[0])) {                       /* Identifiers cannot start with a digit. */
        return 0;
    }

    for (size_t i = 0; str[i] != '\0'; ++i) {                   /* Scan each character until we reach the terminator. */
        if (!(isalnum((unsigned char)str[i]) || str[i] == '_')) { /* Character must be letter, digit, or underscore. */
            return 0;                                           /* Reject as soon as we find an illegal character. */
        }
    }
    return 1;                                                   /* All tests passed, identifier is valid. */
}

int main(void) {
    char candidate[128];                                        /* Buffer to store the identifier typed by the user. */

    printf("Enter an identifier: ");                            /* Prompt for input. */
    if (scanf("%127s", candidate) != 1) {                       /* Read at most 127 characters plus terminator. */
        fprintf(stderr, "No identifier supplied.\n");           /* Notify about missing input. */
        return 1;                                               /* Exit with failure status. */
    }

    if (is_valid_identifier(candidate)) {                       /* Run our validator and branch on the result. */
        printf("'%s' is a valid identifier.\n", candidate);     /* Confirm validity to the learner. */
    } else {
        printf("'%s' is NOT a valid identifier.\n", candidate); /* Explain that the supplied token was rejected. */
    }

    return 0;                                                   /* Signal success to the operating system. */
}
