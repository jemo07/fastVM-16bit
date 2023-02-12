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
  VM_NEXT
  VM_EXCECUTE
  VM_DROP,
  VM_EXIT,
  VM_BRANCH,
  VM_DUP,
  VM_FETCH,
  VM_LIT,
  VM_R_FROM,
  VM_R_FETCH,
  VM_TO_R,
  VM_STORE,
  VM_ENTER,
  VM_OVER,
  VM_AND,
  VM_ZERO_LESS,
  VM_SWAP,
  VM_COND_BRANCH,
  VM_UM_PLUS
};

void VM_NEXT(void) {
  ++code_pointer;
}

void VM_EXECUTE(void) {
  switch (code[code_pointer]) {
    case VM_DROP: {
      --stack_pointer;
      break;
    }
    case EXIT: {
      exit(0);
      break;
    }
    case VM_BRANCH: {
      code_pointer = stack[--stack_pointer];
      break;
    }
    case VM_DUP: {
      stack[stack_pointer] = stack[stack_pointer - 1];
      ++stack_pointer;
      break;
    }
    case VM_FETCH: {
      stack[stack_pointer] = code[stack[stack_pointer - 1]];
      ++stack_pointer;
      break;
    }
    case VM_LIT: {
      ++code_pointer;
      stack[stack_pointer] = code[code_pointer];
      ++stack_pointer;
      break;
    }
    case VM_R_FROM: {
      --stack_pointer;
      break;
    }
    case VM_R_FETCH: {
      stack[stack_pointer] = stack[stack_pointer - 1];
      ++stack_pointer;
      break;
    }
    case VM_TO_R: {
      --stack_pointer;
      break;
    }
    case VM_STORE: {
      code[stack[--stack_pointer]] = stack[stack_pointer - 1];
      stack_pointer -= 2;
      break;
    }
    case VM_ENTER: {
      stack[stack_pointer] = code_pointer;
      ++stack_pointer;
      code_pointer = stack[stack_pointer - 2];
      break;
    }
    case VM_OVER: {
      stack[stack_pointer] = stack[stack_pointer - 2];
      ++stack_pointer;
      break;
    }
    case VM_AND: {
      stack[stack_pointer - 2] = stack[stack_pointer - 2] & stack[stack_pointer - 1];
      --stack_pointer;
      break;
    }
    case VM_ZERO_LESS: {
      stack[stack_pointer - 1] = (stack[stack_pointer - 1] == 0) ? 1 : 0;
      break;
    }
    case VM_SWAP: {
      Word temp = stack[stack_pointer - 1];
      stack[stack_pointer - 1] = stack[stack_pointer - 2];
      stack[stack_pointer - 2] = temp;
      break;
    }
    case VM_COND_BRANCH: {
      if (stack[stack_pointer - 1] == 0) {
        code_pointer = stack[stack_pointer - 2];
      }
      stack_pointer -= 2;
      break;
    }
    case VM_UM_PLUS: {
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
