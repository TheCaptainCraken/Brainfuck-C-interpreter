#include <stdio.h>
#include <stdlib.h>

#define OUTPUTFILE "brainfuck.c"

char* open_code(const char*);  // opens the file and (hopefully) returns the brainfuck code in it as a single line with only meaningfull chatacters
int is_meaningfull_character(char);  // returns 1 if the char is a meaningfull char in bf, 0 otherwise
FILE* initialize_output(const char*); // creates and initializes the output file
void add_end_of_program(FILE*); // ends the text of the program
void add_tabulations(FILE* ,int); // adds tabs to the program (not needed but cool to see the result)

//functions for all the instructions in brainfuck
void instruction_right_arrow(FILE*, int); // >
void instruction_left_arrow(FILE*, int);  // <
void instruction_plus(FILE*, int);  // +
void instruction_minus(FILE*, int); // -
void instruction_open_bracket(FILE*, int); // [
void instruction_closed_bracked(FILE*, int); // ]
void instruction_point(FILE*, int); // .
void instruction_comma(FILE*, int); // ,

int main(int argc, char** argv) {
  if(argc > 1){
    char* code = open_code(argv[1]);
    if(code != NULL){
      const char* output_file_path = OUTPUTFILE;
      FILE* output_file = initialize_output(output_file_path); //start creating the file
      if(output_file != NULL){
        int number_of_tabs = 1;
        for(int i = 0; code[i] != '\0'; i++){
          switch (code[i]){
          case '>':
            instruction_right_arrow(output_file, number_of_tabs);
            break;
          case '<':
            instruction_left_arrow(output_file, number_of_tabs);
            break;
          case '+':
            instruction_plus(output_file, number_of_tabs);
            break;
          case '-':
            instruction_minus(output_file, number_of_tabs);
            break;
          case '.':
            instruction_point(output_file, number_of_tabs);
            break;
          case ',':
            instruction_comma(output_file, number_of_tabs);
            break;
          case '[':
            instruction_open_bracket(output_file, number_of_tabs);
            number_of_tabs++;
            break;
          case ']':
            number_of_tabs--;
            instruction_closed_bracked(output_file, number_of_tabs);
            break;
          default:
          return 1;
            break;
          }
        }
        add_end_of_program(output_file);
        return 0;
      }else{
        free(code);
        return 1;
      }
      free(code);
    }else{
      return 1;
    }
  }else{
    fprintf(stderr, "Usage '%s' <path to brainfuck code>.\n", argv[0]);
    return 1;
  }
  return 0;
}

// opens the file and (hopefully) returns the brainfuck code in it as a single
// line with only meaningfull chatacters
char* open_code(const char* path) {
  FILE* file_pointer = fopen(path, "r");
  char* code = NULL;
  int meaningfull_characters = 0;  // the number of meaningfull characters in the file.

  if (file_pointer != NULL) {  // if we can open the file the we read into it

    // count the number of meaningfull characters
    char current_character = getc(file_pointer);
    while (current_character != EOF) {
      if (is_meaningfull_character(current_character)) {
        meaningfull_characters++;
      }
      current_character = getc(file_pointer);
    }
    rewind(file_pointer);

    code = (char*)malloc(sizeof(char) * (meaningfull_characters + 1)); //create a string
    if (code != NULL) {
      int i = 0;
      current_character = getc(file_pointer);
      while (current_character != EOF) {
        if (is_meaningfull_character(current_character)) {
          code[i] = current_character;
          i++;
        }
        current_character = getc(file_pointer);
      }
      fclose(file_pointer);

      code[i] = '\0';
      return code;
    } else {
      // we do not have succifient memory to allocate the string.
      fclose(file_pointer);  // no leaking boiz
      fprintf(stderr, "Insufficient memory to allocate the code in.\n");
      return NULL;
    }
  } else {
    fprintf(stderr, "Couldn't open the file '%s'\n", path); // if we can't open a file there is no text to return
    return NULL;
  }
  fprintf(stderr, "Something's gone very wrong if you see this.\n"); // WTF JUST HAPPENED
  return NULL;
}

// returns 1 if the char is a meaningfull char in bf, 0 otherwise
int is_meaningfull_character(char character) {
  const char* accepted_characters = "><+-.,[]";
  const int number_of_accepted_characters = 8;

  for (int i = 0; i < number_of_accepted_characters; i++) {
    if (character == accepted_characters[i]) {
      return 1;
    }
  }
  return 0;
}

// initializes the c file with the main funcion etc etc
FILE* initialize_output(const char* output_file_path){
  FILE* output_file = fopen(output_file_path, "w");
  if(output_file != NULL){
    fprintf(output_file, "#include <stdio.h>\n\n");
    fprintf(output_file, "int main(int argc, char** argv) {\n");
    fprintf(output_file, "\tchar nastro[3000] = {0};\n");
    fprintf(output_file, "\tchar* testina = nastro;\n");

    return output_file;
  }else{
    fprintf(stderr, "Couldn't create the file: '%s'", output_file_path);
    return NULL;
  }
}

// ends the C output program.
void add_end_of_program(FILE* output_file){
  fprintf(output_file, "\treturn 0;");
  fprintf(output_file, "\n}");
  fclose(output_file);
}

// adds tabs to the program (not needed but cool to see the result)
void add_tabulations(FILE* output_file, int number_of_tabs){
  for(int i = 0; i < number_of_tabs; i++){
    fprintf(output_file, "\t");
  }
}

void instruction_right_arrow(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "testina++;\n");
}
void instruction_left_arrow(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "testina--;\n");
}
void instruction_plus(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "(*testina)++;\n");
}
void instruction_minus(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "(*testina)--;\n");
}
void instruction_open_bracket(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "while((*testina) != 0) {\n");
}
void instruction_closed_bracked(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "}\n");
}
void instruction_point(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "putchar(*testina);\n");
}
void instruction_comma(FILE* output_file, int number_of_tabs){
  add_tabulations(output_file, number_of_tabs);
  fprintf(output_file, "*testina = getchar()\n");
}