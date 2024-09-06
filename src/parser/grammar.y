%code requires{
 #include <stdio.h>
 #include "../ast/include.h"

 int yylex(void);
 int yyerror(char *s);

 extern SourceFile* ast;

}
%union{
        enum UnaryOperator unaryOp;
        enum BinaryOp binaryOp;
        enum AssignOp assignOp;
        
        SourceFile* sourceFile;

        Decl *decl;
        DeclList* declList;

        TypeList* typeList;
        Type* type; 

        char* id;
        bool* boollit;
        bool boolval;
        int* intlit;
        
        StmList* stmList;
        Stm* stm;
        
        ExpList* expList;
        Exp* exp; 
        
        IdentifierList* identifier_list;
        Identifier* identifier;
        
        Operand* operand;

        FunctionSpecs* func_specs;
        ReturnType* return_types;
        Block* block;

        Literal* literal;

        ParameterList* parameterList;
        ParameterDecl* parameterDecl;

}
%token
    SEMICOLON COLON COMMA ARROW DOT INTEGERTYPE BOOLTYPE
    BREAK RETURN LET IF ELSE FOR WHILE LOOP
    LPAREN RPAREN LBRACE RBRACE MUT
    PLUS MIN MUL DIV IN
    PLUSASSIGN MINASSIGN MULASSIGN DIVASSIGN
    AND OR NOT INC DEC GT GE LT LE EQ NE
    ASSIGN FUNC NEWLINE
    PRINT PRINTLN

%token <id> IDENTIFIER
%token <boollit> BOOLLITERAL
%token <intlit> INTLITERAL
%type <boolval> mutable

%type <literal> literal
%type <literal> basiclit

%type <sourceFile> sourcefile 
%type <declList> global_declarations
%type <decl> global_declaration
%type <decl> variable_declaration
%type <decl> function_declaration

%type <identifier_list> identifier_list
%type <identifier> identifier
%type <block> block
%type <block> block_content

%type <exp> expr
%type <exp> unary_expr
%type <exp> primary_expr
%type <expList> expression_list
%type <unaryOp> unary_op
%type <operand> operand
%type <identifier> operandname

%type <stmList> statement_list
%type <stm> statement
%type <stm> expression_statement
%type <stm> forstatement
%type <stm> whilestatement
%type <stm> loopstatement
%type <stm> assignmentstatement
%type <stm> breakstatement
%type <stm> if_statement
%type <stm> returnstatement
%type <stm> printstatement
%type <stm> printlnstatement
%type <assignOp> assign_op

%type <block> func_body
%type <identifier> func_name
%type <func_specs> func_specs
%type <return_types> return_types
%type <expList> func_args

%type <type> type
%type <type> typename
%type <typeList> typelist

%type <parameterList> parameters
%type <parameterList> parameterlist
%type <parameterDecl> parameter_declaration

%right UMINUS RCOMMA

%left RPAREN
%left AND EQ NE GT GE LT LE OR
%left PLUS MIN MUL DIV
%%
sourcefile: 
        global_declarations {$$ = new SourceFile($1);ast = $$;}
        ;

global_declarations: 
        global_declaration SEMICOLON global_declarations    {$$ = new PairDeclList($1, $3);}
    |   global_declaration SEMICOLON                        {$$ = new LastDeclList($1);}
    ;

global_declaration: 
        variable_declaration         {$$ = $1;}
    |   function_declaration    {$$ = $1;}
    ;

variable_declaration: 
        LET mutable identifier_list COLON type ASSIGN expression_list  {$$ = new VariableDecl($3, $5, $7, nullptr,$2);}
    |   LET mutable identifier_list COLON type                        {$$ = new VariableDecl($3, $5, nullptr, nullptr,$2); }
    |   LET mutable identifier_list ASSIGN expression_list             {$$ = new VariableDecl($3, nullptr, $5, nullptr,$2); }
    ;

mutable: MUT { $$ = true; }
          |     { $$ = false;}
          ;

identifier_list: LPAREN identifier_list RPAREN COMMA identifier       { $$ = new PairIdentifierList($5, $2); }
              | identifier COMMA identifier_list                     { $$ = new PairIdentifierList($1, $3); }
              | LPAREN identifier_list RPAREN                        { $$ = $2; }
              | LPAREN identifier RPAREN                            { $$ = new LastIdentifierList($2); }
              | identifier                                          { $$ = new LastIdentifierList($1); }
              ;

identifier: LPAREN identifier RPAREN { $$ = $2; }
          | IDENTIFIER               { $$ = new Identifier($1); };

function_declaration: 
    FUNC func_name func_specs func_body {$$ = new FunctionDecl($2, $3, $4); }
    ;

func_name: 
    IDENTIFIER  {$$ = new Identifier($1); }
    ;

func_body:
    block  {$$ = $1; }
    ; 

func_specs: 
        parameters ARROW return_types    { $$ = new FunctionSpecs($1, $3);}
    |   parameters                  { $$ = new FunctionSpecs($1, nullptr); } 
    ; 

typelist: 
        type   %prec RPAREN                                {$$ = new LastTypeList($1); }
    |   typelist COMMA type                {$$ = new PairTypeList($3, $1); }
    ;

type: 
        typename                  { $$ = $1; }
    |   LPAREN type RPAREN        { $$ = $2; }
    ;

typename: 
        INTEGERTYPE               { $$ = new IntegerType(); } 
    |   BOOLTYPE               { $$ = new BooleanType(); }
    ;


return_types: 
        LPAREN return_types RPAREN       {$$ = $2;}
    |   typelist                    {$$ = new ReturnType($1); }
    ;

parameters: LPAREN RPAREN                         { $$ = nullptr; }
           | LPAREN parameterlist COMMA RPAREN    { $$ = $2; }
           | LPAREN parameterlist RPAREN          { $$ = $2; }
           ;

parameterlist: parameterlist COMMA parameter_declaration     { $$ = new PairParamList($3, $1); } 
             | parameter_declaration                         { $$ = new LastParamList($1); }
             ;

parameter_declaration: identifier_list COLON type               { $$ = new ParameterDecl($3, $1); }
             | type                                             { $$ = new ParameterDecl($1, nullptr); }
             ;

expr: unary_expr                 { $$ = $1; }
    | expr EQ expr              { $$ = new BinaryExp($1, $3, EQ_BINARY); }
    | expr NE expr              { $$ = new BinaryExp($1, $3, NE_BINARY); }
    | expr LT expr              { $$ = new BinaryExp($1, $3, LT_BINARY); }
    | expr LE expr              { $$ = new BinaryExp($1, $3, LE_BINARY); } 
    | expr GT expr              { $$ = new BinaryExp($1, $3, GT_BINARY); } 
    | expr GE expr              { $$ = new BinaryExp($1, $3, GE_BINARY); } 
    | expr MUL expr             { $$ = new BinaryExp($1, $3, MUL_BINARY); }
    | expr DIV expr             { $$ = new BinaryExp($1, $3, DIV_BINARY); }
    | expr PLUS expr            { $$ = new BinaryExp($1, $3, PLUS_BINARY); }
    | expr MIN expr             { $$ = new BinaryExp($1, $3, MIN_BINARY); }
    | expr OR expr              { $$ = new BinaryExp($1, $3, OR_BINARY); }
    | expr AND expr             { $$ = new BinaryExp($1, $3, AND_BINARY); }
    ;

expression_list: expression_list COMMA expr       { $$ = new PairExpList($3, $1); }
               | LPAREN expression_list RPAREN   { $$ = $2; }
               | expr                           { $$ = new LastExpList($1); }
               ;

unary_expr: primary_expr         { $$ = $1; }
          | unary_op unary_expr { $$ = new UnaryExp($2, $1); }
          ;

unary_op: PLUS                  { $$ = PLUS_UNARY; }
        | MIN   %prec UMINUS    { $$ = MIN_UNARY; }
        | NOT                   { $$ = NOT_UNARY; }
        ;

operand: literal                { $$ = new LiteralOperand($1); }
        | operandname           { $$ = new VariableOperand($1); }
        | LPAREN expr RPAREN    { $$ = new ExprOperand($2); }
        ;

literal: 
        INTLITERAL            { $$ = new IntLiteral(*$1); }
    |   BOOLLITERAL           { $$ = new BoolLiteral(*$1); }
    ;

operandname: IDENTIFIER         { $$ = new Identifier($1);}
            ;

primary_expr: operand                              { $$ = new OperandExp($1); }
           | primary_expr func_args                { $$ = new FuncCall($1, $2); }
           ;

func_args: LPAREN RPAREN                                          { $$ = nullptr; }
         | LPAREN expression_list RPAREN                           { $$ = $2; }
         | LPAREN expression_list RCOMMA RPAREN                    { $$ = $2; }
         ;
         
block: LBRACE block_content RBRACE  { $$ = $2; }
     | LBRACE RBRACE                { $$ = new Block(nullptr, nullptr); }
     ;

block_content:
            statement_list               {$$ = new Block($1, nullptr); }
        |   expression_list             {$$ = new Block(nullptr, $1); }     
        ;

statement_list: 
        statement                      { $$ = new LastStmList($1); }
    |   statement statement_list      { $$ = new PairStmList($1, $2);}
    ;

statement: variable_declaration  SEMICOLON                 { $$ = new DeclarationStm($1); }
        | block   SEMICOLON                       { $$ = new BlockStm($1); }
        | if_statement  SEMICOLON                  { $$ = $1; }
        | forstatement  SEMICOLON                 { $$ = $1; }
        | loopstatement SEMICOLON                  { $$ = $1; }
        | whilestatement  SEMICOLON                 { $$ = $1; }
        | breakstatement  SEMICOLON              { $$ = $1; } 
        | returnstatement SEMICOLON               { $$ = $1; }
        | expression_statement SEMICOLON               { $$ = $1; }
        | assignmentstatement SEMICOLON               { $$ = $1; }  
        | printstatement  SEMICOLON               { $$ = $1; }     
        | printlnstatement SEMICOLON              { $$ = $1; }            
        ;

expression_statement: expr { $$ = new ExpressionStm($1); } ;

assignmentstatement: expression_list assign_op expression_list { $$ = new AssignmentStm($1, $3, $2); };

assign_op: ASSIGN                                   { $$ = _ASSIGN;} 
         | PLUSASSIGN                               { $$ = PLUS_ASSIGN; } 
         | MINASSIGN                                { $$ = MIN_ASSIGN; } 
         | MULASSIGN                                { $$ = MUL_ASSIGN; } 
         | DIVASSIGN                                { $$ = DIV_ASSIGN;}            
         ;
if_statement: 
        IF expr block ELSE if_statement                            { $$ = new IfStm($3, $2, nullptr, $5); }
    |   IF expr block ELSE block                                  { $$ = new IfStm($3, $2, $5, nullptr); }
    |   IF expr block                                             { $$ = new IfStm($3, $2, nullptr, nullptr); }
    ;
forstatement: 
        FOR mutable identifier IN literal DOT DOT literal block       { $$ = new ForStm($3, $5, $8, $2, $9); }
    ;
whilestatement: 
        WHILE expr block    { $$ = new WhileStm($2, $3); }
    ;
loopstatement: 
        LOOP block  { $$ = new LoopStm($2); }
    ;
breakstatement: 
        BREAK   { $$ = new BreakStm(); }
    ;
returnstatement: 
        RETURN expression_list  { $$ = new ReturnStm($2); }
    |   RETURN                  { $$ = new ReturnStm(nullptr); }
    ;
printstatement: 
        PRINT LPAREN expression_list RPAREN { $$ = new PrintStm($3); }
    |   PRINT LPAREN RPAREN                 { $$ = new PrintStm(nullptr); }
    ;
printlnstatement: 
        PRINTLN LPAREN expression_list RPAREN   { $$ = new PrintLnStm($3); }
    |   PRINTLN LPAREN RPAREN                   { $$ = new PrintLnStm(nullptr); }
    ;
%%