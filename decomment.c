#include <stdio.h>
#include <stdlib.h>

typedef enum state {
   START,                                                             
   CHECK_START_COMMENT,
   IN_COMMENT,
   CHECK_END_COMMENT,
   IN_STRING,
   ESCAPE_STRING,
   IN_CHAR,
   ESCAPE_CHAR
} State;

/* global variable for keeping track of line number */
int line_number;

/* global variable for keeping track of line number where a comment starts*/
int comment_start;

/* Implement the START state of the DFA. c is the current DFA character. If c is not '\', write c to stdout, as specified by the DFA.
Return the next state. */

State handle_start (char c) {
   if (c == '/') {
       return CHECK_START_COMMENT;
   } else if (c == '"') {
       putchar(c);
       return IN_STRING;
   } else if (c == '\'') {   
       putchar(c);      
       return IN_CHAR;  
    }else if (c == '\n') {   
       putchar(c);    
       line_number++;  
       return START;  
   } else {
       putchar(c);
       return START;
   }                
}

/* Implement the CHECK_START_COMMENT state of the DFA. c is the current DFA character. If c is '*', write " " to stdout. Otherwise, 
write c to stdout, as specified by the DFA. Return the next state. */

State handle_check_start_comment (char c) {
    if (c == '*') {
        putchar(' ');
        comment_start = line_number;
        return IN_COMMENT;
    } else if (c == '/') {
        putchar(c);
        return CHECK_START_COMMENT;
    } else if (c == '"') {
        putchar('/');
        putchar(c);
        return IN_STRING;
    } else if (c == '\'') {
        putchar('/');
        putchar(c);
        return IN_CHAR;
    } else {
        putchar('/');
        putchar(c);
        return START;
    }
}

/* Implement the IN_COMMENT state of the DFA. c is the current DFA character. If c is '*', go to the next DFA state. If
c is a new line character, print that to stdout and return the next state. Otherwise, only return the next state. */

State handle_in_comment (char c) {
    if (c == '*') {
        return CHECK_END_COMMENT;
    } else if (c == '\n') {
        putchar(c);
        line_number++;
        return IN_COMMENT;
    } else {
        return IN_COMMENT;
    }
}

/* Implement the CHECK_END_COMMENT state of the DFA. c is the current DFA character. If c is a new line 
character, write c to stdout, as specified by the DFA. Otherwise, only return the next state. */

State handle_check_end_comment (char c) {
    if (c == '/') {
        return START;
    } else if (c == '*') {
        return CHECK_END_COMMENT;
    } else if (c == '\n') {
        putchar(c);
        line_number++;
        return IN_COMMENT;
    } else {
        return IN_COMMENT;
    }
}

/* Implement the IN_STRING state of the DFA. c is the current DFA character. Write c to stdout, as specified by the DFA. 
Return the next state. */

State handle_in_string (char c) {
    if (c == '"') {
        putchar(c);
        return START;
    } else if (c == '\\') {
        putchar(c);
        return ESCAPE_STRING;
    } else if (c == '\n') {
        putchar(c);
        line_number++;
        return IN_STRING;
    } else {
        putchar(c);
        return IN_STRING;
    }
}

/* Implement the ESCAPE_STRING state of the DFA. c is the current DFA character. Write c to stdout, as specified by the DFA. 
Return the next state. */

State handle_escape_string (char c) {
    putchar(c);
    return IN_STRING;
}

/* Implement the IN_CHAR state of the DFA. c is the current DFA character. Write c to stdout, as specified by the DFA. 
Return the next state. */

State handle_in_char (char c) {
    if (c == '\'') {
        putchar(c);
        return START;
    } else if (c == '\\') {
        putchar(c);
        return ESCAPE_CHAR;
    } else if (c == '\n') {
        putchar(c);
        line_number++;
        return IN_CHAR;
    } else {
        putchar(c);
        return IN_CHAR;
    }
}

/* Implement the ESCAPE_CHAR state of the DFA. c is the current DFA character. Write c to stdout, as specified by the DFA. 
Return the next state. */

State handle_escape_char (char c) {
    putchar(c);
    return IN_CHAR;
}

/* Read text from stdin. Write the text to stdout with each comment replaced by a space, as well as 
writing appropriate error and warning messages to the stderror stream. */

int main(void) {
    int c;
    State s = START;
    line_number = 1;
    comment_start = 1;

    while ((c = getchar()) != EOF) { 
        switch (s) {
            case START:
                s = handle_start(c);
                break;
            case CHECK_START_COMMENT:
                s = handle_check_start_comment(c);
                break;
            case IN_COMMENT:
                s = handle_in_comment(c);
                break;
            case CHECK_END_COMMENT:
                s = handle_check_end_comment(c);
                break;
            case IN_STRING:
                s = handle_in_string(c);
                break;
            case ESCAPE_STRING:
                s = handle_escape_string(c);
                break;
            case IN_CHAR:
                s = handle_in_char(c);
                break;
            case ESCAPE_CHAR:
                s = handle_escape_char(c);
                break;
        }
    }

    if (s == CHECK_START_COMMENT) {
        putchar('/');
    }

    if (s == IN_COMMENT || s == CHECK_END_COMMENT) {
        fprintf(stderr, "Error: line %i: unterminated comment\n",comment_start);
        return 1;
    }


    return 0;
}
