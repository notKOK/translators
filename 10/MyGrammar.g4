grammar MyGrammar;

// start -> VAR = stmt
// stmt -> VAR stmt'
// stmt -> INT stmt'
// stmt -> ++ VAR
// stmt' -> ''
// stmt' -> = stmt
//
// a=b=++c

start: VAR equal='=' stmt;
stmt: VAR stmt_ | INT stmt_ | plus='+' '+' VAR;
stmt_ : equal='=' stmt | ;

INT: [0-9]+;
VAR:  [a-zA-Z][a-zA-Z0-9]*;
