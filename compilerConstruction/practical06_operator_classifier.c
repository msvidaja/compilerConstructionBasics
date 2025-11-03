#include <stdio.h>    /* Standard I/O functions such as printf and scanf. */
#include <string.h>   /* strcmp lets us compare operator tokens. */
#include <stdbool.h>  /* Provides the bool type for clarity. */

/*
 * Categorises an operator token into one of the common operator families found
 * in the C language. This mirrors what a lexical analyser would do before
 * emitting operator tokens.
 */

static bool is_arithmetic_operator(const char *op) {
    return (!strcmp(op, "+")  ||  /* Addition */
            !strcmp(op, "-")  ||  /* Subtraction */
            !strcmp(op, "*")  ||  /* Multiplication */
            !strcmp(op, "/")  ||  /* Division */
            !strcmp(op, "%"));    /* Modulus */
}

static bool is_relational_operator(const char *op) {
    return (!strcmp(op, "==") ||  /* Equality comparison */
            !strcmp(op, "!=") ||  /* Inequality comparison */
            !strcmp(op, ">")  ||  /* Greater than */
            !strcmp(op, "<")  ||  /* Less than */
            !strcmp(op, ">=") ||  /* Greater than or equal */
            !strcmp(op, "<="));   /* Less than or equal */
}

static bool is_logical_operator(const char *op) {
    return (!strcmp(op, "&&") || /* Logical AND */
            !strcmp(op, "||") || /* Logical OR */
            !strcmp(op, "!"));   /* Logical NOT */
}

static bool is_assignment_operator(const char *op) {
    return (!strcmp(op, "=")  ||  /* Simple assignment */
            !strcmp(op, "+=") ||  /* Addition assignment */
            !strcmp(op, "-=") ||  /* Subtraction assignment */
            !strcmp(op, "*=") ||  /* Multiplication assignment */
            !strcmp(op, "/=") ||  /* Division assignment */
            !strcmp(op, "%="));   /* Modulus assignment */
}

static bool is_bitwise_operator(const char *op) {
    return (!strcmp(op, "&")  ||  /* Bitwise AND */
            !strcmp(op, "|")  ||  /* Bitwise OR */
            !strcmp(op, "^")  ||  /* Bitwise XOR */
            !strcmp(op, "~")  ||  /* Bitwise NOT */
            !strcmp(op, "<<") ||  /* Left shift */
            !strcmp(op, ">>"));   /* Right shift */
}

int main(void) {
    char op[8];                                                     /* Buffer to hold the operator token from input. */

    printf("Enter an operator: ");                                  /* Prompt the learner for an operator. */
    if (scanf("%7s", op) != 1) {                                    /* Read up to 7 chars, leaving room for terminator. */
        fprintf(stderr, "No operator supplied.\n");                 /* Handle unexpected lack of input. */
        return 1;                                                   /* Exit with failure to indicate problem. */
    }

    if (is_arithmetic_operator(op)) {                               /* Test predicates in descending specificity. */
        printf("%s is an Arithmetic Operator\n", op);                /* Report arithmetic classification. */
    } else if (is_relational_operator(op)) {
        printf("%s is a Relational Operator\n", op);                 /* Report relational classification. */
    } else if (is_logical_operator(op)) {
        printf("%s is a Logical Operator\n", op);                    /* Report logical classification. */
    } else if (is_assignment_operator(op)) {
        printf("%s is an Assignment Operator\n", op);                /* Report assignment classification. */
    } else if (is_bitwise_operator(op)) {
        printf("%s is a Bitwise Operator\n", op);                    /* Report bitwise classification. */
    } else {
        printf("%s is not a recognised operator\n", op);            /* Fallback for tokens outside our known sets. */
    }

    return 0;                                                       /* Successful completion of the classifier. */
}
