#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 48
#define MAX_CODE_SIZE 1000

typedef unsigned short Word;

Word stack[MAX_STACK_SIZE];
int stack_pointer = 0;
Word code[MAX_CODE_SIZE];
int code_pointer = 0;

enum Operations {
  DROP,
  EXIT,
  BRANCH,
  DUP,
  FETCH,
  LIT,
  R_FROM,
  R_FETCH,
  TO_R,
  STORE,
  ENTER,
  OVER,
  AND,
  ZERO_LESS,
  SWAP,
  COND_BRANCH,
  UM_PLUS
};

void NEXT(void) {
  ++code_pointer;
}

void EXECUTE(void) {
  switch (code[code_pointer]) {
    case DROP: {
      --stack_pointer;
      break;
    }
    case EXIT: {
      exit(0);
      break;
    }
    case BRANCH: {
      code_pointer = stack[--stack_pointer];
      break;
    }
    case DUP: {
      stack[stack_pointer] = stack[stack_pointer - 1];
      ++stack_pointer;
      break;
    }
    case FETCH: {
      stack[stack_pointer] = code[stack[stack_pointer - 1]];
      ++stack_pointer;
      break;
    }
    case LIT: {
      ++code_pointer;
      stack[stack_pointer] = code[code_pointer];
      ++stack_pointer;
      break;
    }
    case R_FROM: {
      --stack_pointer;
      break;
    }
    case R_FETCH: {
      stack[stack_pointer] = stack[stack_pointer - 1];
      ++stack_pointer;
      break;
    }
    case TO_R: {
      --stack_pointer;
      break;
    }
    case STORE: {
      code[stack[--stack_pointer]] = stack[stack_pointer - 1];
      stack_pointer -= 2;
      break;
    }
    case ENTER: {
      stack[stack_pointer] = code_pointer;
      ++stack_pointer;
      code_pointer = stack[stack_pointer - 2];
      break;
    }
    case OVER: {
      stack[stack_pointer] = stack[stack_pointer - 2];
      ++stack_pointer;
      break;
    }
    case AND: {
      stack[stack_pointer - 2] = stack[stack_pointer - 2] & stack[stack_pointer - 1];
      --stack_pointer;
      break;
    }
    case ZERO_LESS: {
      stack[stack_pointer - 1] = (stack[stack_pointer - 1] == 0) ? 1 : 0;
      break;
    }
    case SWAP: {
      Word temp = stack[stack_pointer - 1];
      stack[stack_pointer - 1] = stack[stack_pointer - 2];
      stack[stack_pointer - 2] = temp;
      break;
    }
    case COND_BRANCH: {
      if (stack[stack_pointer - 1] == 0) {
        code_pointer = stack[stack_pointer - 2];
      }
      stack_pointer -= 2;
      break;
    }
    case UM_PLUS: {
      stack[stack_pointer - 2] = stack[stack_pointer - 2] + stack[stack_pointer - 1];
      --stack_pointer;
      break;
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Error: no input file specified\n");
    return 1;
  }

  char* filename = argv[1];
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: unable to open file '%s'\n", filename);
    return 1;
  }

  int code_index = 0;
  while (fscanf(fp, "%hu", &code[code_index]) == 1) {
    ++code_index;
  }

  code_pointer = 0;
  while (code_pointer < code_index) {
    EXECUTE();
    NEXT();
  }

  return 0;
}
