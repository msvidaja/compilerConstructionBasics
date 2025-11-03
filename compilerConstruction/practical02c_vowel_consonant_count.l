%{ /* ---------- C prologue ---------- */
#include <stdio.h> /* We use printf to display counts and tokens. */

int vowel_count = 0;      /* Tally for characters that match the vowel rule. */
int consonant_count = 0;  /* Tally for characters that match the consonant rule. */

int yywrap(void) {        /* Flex calls this when it hits the end of input. */
    return 1;             /* Returning 1 tells Flex to stop scanning cleanly. */
}
%}

%% /* ---------- Scanner rules ---------- */
[aeiouAEIOU]                  { vowel_count++; printf("VOWEL: '%s'\n", yytext); }         /* Report each vowel. */
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z] { consonant_count++; printf("CONSONANT: '%s'\n", yytext); } /* Report each consonant. */
.                              { /* Ignore any other character (digits, punctuation, etc.). */ }
%%

int main(void) {
    printf("Enter text and press Ctrl+D (Ctrl+Z on Windows) when finished.\n"); /* Usage hint. */
    yylex();                                                                    /* Start the Flex-generated scanner. */
    printf("Number of Vowels : %d\n", vowel_count);                             /* Show final vowel tally. */
    printf("Number of Consonants : %d\n", consonant_count);                     /* Show final consonant tally. */
    return 0;                                                                   /* Indicate successful termination. */
}
