%{
#include "../parser/parser.hpp"
#include <stdlib.h>
#include <string.h>

  /* Keep track of current position of lex for error messages, i.e. 
     the position just *after* the last token read */
  int line_nr = 1;
  int col_nr = 1; 
  int lex_pos = 1;
  bool insert_semicolon = false;
  void ADJUST(bool insert_semicol = false) { col_nr = lex_pos; 
                                             lex_pos += yyleng; 
                                             insert_semicolon = insert_semicol;}

%}
whitespace (" "|"\t"|"\n")
comment "//".*"\n"
semicolon ";"
colon ":"
comma ","
dot "."
in "in"
arrow "->"
break "break"
identifier ((_[a-zA-Z]+)|([a-zA-Z]))[a-zA-Z0-9_]*
integertype "i32"|"u32"
integer 0|-?([1-9]+)(_[0-9]|[0-9])*
booltype "bool"
bool ("true"|"false")
function "fn"
return "return"
print "print!"
println "println!"
let "let"
mut "mut"
if "if"
else "else"
for "for"
while "while"
loop "loop"
lparen "("
rparen ")"
lbrace "{"
rbrace "}"
plus "+"
minus "-"
multiply "*"
divide "/"
plusassign "+="
minassign "-="
mulassign "*="
divassign "/="
and "&&"
or "||"
not "!"
greaterthan ">"
greaterthanequal ">="
lessthan "<"
lessthanequal "<="
equal "=="
notequal "!=" 
assign "="

%%
\n { line_nr++; col_nr = 1; lex_pos = 1; if(insert_semicolon){ insert_semicolon = false; return SEMICOLON; } }
{semicolon} { ADJUST(); return SEMICOLON; }
{comma} { ADJUST(); return COMMA; }
{arrow} { ADJUST(); return ARROW; }
{dot} { ADJUST(); return DOT; }
{colon} { ADJUST(); return COLON; }
{break} { ADJUST(); return BREAK; }
{in} { ADJUST(); return IN; }
{integertype} { ADJUST(true); return INTEGERTYPE; }
{booltype} { ADJUST(true); return BOOLTYPE; }
{function} { ADJUST(); return FUNC; }
{return} { ADJUST(true); return RETURN; }
{print} {ADJUST(); return PRINT; }
{println} {ADJUST(); return PRINTLN; }
{let} { ADJUST(); return LET; }
{if} { ADJUST(); return IF; }
{else} {ADJUST(); return ELSE; }
{for} { ADJUST(); return FOR; }
{while} { ADJUST(); return WHILE; }
{loop} { ADJUST(); return LOOP; }
{mut} { ADJUST(); return MUT; }
{integer} { ADJUST(); 
            char* s = strdup(yytext);
            long int value = strtol(s, NULL, 10);
            *yylval.intlit = value;
            return INTLITERAL;
            }
{bool} { ADJUST(); 
               char* s = strdup(yytext);
               if(strcmp(s, "true") == 0){
                *yylval.boollit = true;
               }
               else{
                 *yylval.boollit = false;
               }
              return BOOLLITERAL; 
          }
{identifier} { ADJUST(true); 
               char* s = strdup(yytext);
               yylval.id = s; 
               return IDENTIFIER; 
              }
{lparen} { ADJUST(); return LPAREN; }
{rparen} { ADJUST(); return RPAREN; }
{lbrace} { ADJUST(); return LBRACE; }
{rbrace} { ADJUST(true); return RBRACE; }
{plus} { ADJUST(); return PLUS; }
{minus} { ADJUST(); return MIN; }
{divide} { ADJUST(); return DIV; }
{multiply} { ADJUST(); return MUL; }
{minassign} { ADJUST(); return MINASSIGN; }
{plusassign} { ADJUST(); return PLUSASSIGN; }
{mulassign} { ADJUST(); return MULASSIGN; }
{divassign} { ADJUST(); return DIVASSIGN; }
{and} { ADJUST(); return AND; }
{or} { ADJUST(); return OR; }
{not} { ADJUST(); return NOT; }
{greaterthan} { ADJUST(); return GT; }
{greaterthanequal} { ADJUST(); return GE; }
{lessthan} { ADJUST(); return LT; }
{lessthanequal} { ADJUST(); return LE; }
{equal} { ADJUST(); return EQ; }
{notequal} { ADJUST(); return NE; }
{assign} { ADJUST(); return ASSIGN; }

{comment} {line_nr++;}
{whitespace} {/* Nothing */}
.      {
  if (yytext[0] < ' '){
    /*yyerror*/ fprintf(stderr,"illegal character: ^%c",yytext[0] + '@'); 
  }
  fprintf(stderr," at line %d column %d\n", line_nr, (col_nr-1));
			       }

<<EOF>>  {
  col_nr = 1; lex_pos = 1;line_nr++; 
  if(insert_semicolon){ insert_semicolon = false; return SEMICOLON; }
  yyterminate();
}
%%

int yywrap(){
  return (1);
}
int yyerror(char* s){
  fprintf(stderr, "Syntax error: line %d column %d .\n", line_nr, (col_nr-1));
  return (1);
}
