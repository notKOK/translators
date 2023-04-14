%{
  #include "main.tab.hh"
  extern "C" int yylex(void);

// start -> VAR = stmt
// stmt -> VAR stmt'
// stmt -> INT stmt'
// stmt -> ++ VAR
// stmt' -> ''
// stmt' -> = stmt
//
// a=b=++c
%}

%%

[ \t\n]     {}
\+ { return '+'; }
\= { return '='; }

[0-9]+	{ 
  yylval.i = atoi(yytext);  
  return INT; 
}
[a-zA-Z][a-zA-Z0-9]* {
  yylval.var = strdup(yytext);
  return VAR;
}


%%
