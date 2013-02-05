#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum tokens { NUMBER=1, STRING, ASSIGN, DIV, ERROR, ID, READ, WRITE };

int lineno = 0;
int number;
char string[100];

char get_nonblank() {
  int c;
  while ((c=getchar()) == ' ' || c == '\t' || c == '\n');
  return c;
}

int next_is(char c) {
  return getchar() == c;
}

int yylex() {
  int c = get_nonblank();
  if (c == EOF) return 0;

  // Function tokens
  if (c == '+' || c == '-' || c == '*' || c == '(' || c == ')') {
    return c;
  }

  // Assignment
  else if (c == ':') {
    if (next_is('=')) {
      return ASSIGN;
    } else {
      return ERROR;
    }
  }

  // Division and comments
  else if (c == '/') {
    c = getchar();
    if (c == '/') {
      while (!next_is('\n'));
      return yylex();
    } else if (c == '*') {
      while (!((c = getchar()) == '*' && (c = getchar()) == '/') && (c != EOF));
      return yylex();
    } else {
      return DIV;
    }
  }

  // Leading point
  else if (c == '.') {
    if (isdigit(getchar())) {
      while (isdigit(getchar()));
      return NUMBER;
    } else {
      return ERROR;
    }
  }

  // Numbers
  else if (isdigit(c)) {
    int seen_point = 0;
    int cont = 1;
    while (cont) {
      c = getchar();
      if (isdigit(c)) 
        continue;
      else if (c == '.' && !seen_point)
        seen_point = 1;
      else
        cont = 0;
    }
    return NUMBER;
  }

  // Letters
  // TODO: Accumulate a string. Consider input 'readfoo',
  //       which would parse to READ ID, instead of just ID.
  else if (isalpha(c)) {
    if (c == 'r' &&
        next_is('e') &&
        next_is('a') &&
        next_is('d'))
      return READ;
    if (c == 'w' &&
        next_is('r') &&
        next_is('i') &&
        next_is('t') &&
        next_is('e'))
      return WRITE;
    while (isalnum(getchar()));
    return ID;
  }

  else return ERROR;

}

int main(int argc, char **argv) {
  int rc;
  while ((rc=yylex())) {
    switch (rc) {
      case NUMBER:
        printf("NUMBER\n");
        break;
      case STRING:
        printf("STRING\n");
        break;
      case ASSIGN:
        printf("ASSIGN\n");
        break;
      case DIV:
        printf("DIV\n");
        break;
      case ERROR:
        printf("ERROR\n");
        break;
      case ID:
        printf("ID\n");
        break;
      case READ:
        printf("READ\n");
        break;
      case WRITE:
        printf("WRITE\n");
        break;
      default:
        printf("%c\n", rc);
    }
  }
  return 0;
}
