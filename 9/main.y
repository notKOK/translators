%{
#include <string>
#include <unordered_map>
#include <iostream>
#include <stdio.h>


extern "C" int yylex(void);
extern int yyparse();
extern FILE *yyin;
extern "C" void yyerror(const char *s);

std::unordered_map<std::string, int> variables;
bool isEmpty = false;

%}

%token INT VAR 

%union{
  int i;
  char* var;
  int value;
}

%type <i> INT 
%type <var> VAR 
%type <value> stmt;
%type <value> stmt_;

// start -> VAR = stmt
// stmt -> VAR stmt'
// stmt -> INT stmt'
// stmt -> ++ VAR
// stmt' -> ''
// stmt' -> = stmt
//
// a=b=++c

%%

start: VAR '=' stmt { std::cout<< "start\n"; variables[$1] = $3; };
stmt: VAR stmt_ { 
    if( isEmpty ){
      if ( variables.count($1) == 0  ){
        variables[$1] = 0; 
      }
    } else {
      variables[$1] = $2; 
    }
    $$ = variables[$1];
} | INT stmt_ { 
    if ( !isEmpty )
      throw std::runtime_error("Ошибка!\n");
    $$ = $1; 
} | '+' '+' VAR { 
    if ( variables.count($3)  ){
      variables[$3]++; 
    } else {
      variables[$3] = 1; 
    }
    $$ = variables[$3];
};
stmt_: '=' stmt { isEmpty = false; $$ = $2; } | %empty { isEmpty = true; $$ = 0; };

%%

int main(int, char**) {
  yyin = stdin;

  yyparse();

  for (const auto &p : variables) {
    std::cout << p.first << " = " << p.second << std::endl;
  }
}


void yyerror(const char *s) {
  std::cout << "EEK, parse error!  Message: " << s << std::endl;
  exit(-1);
}
