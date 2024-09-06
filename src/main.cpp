#include <stdio.h>
#include "./lexer/lexer.hpp"
#include "./parser/parser.hpp"
#include "./environment/symboltable.h"
#include "./environment/env.h"

extern int yyparse();
SourceFile *ast;

int main(int argc, char *argv[])
{
    FILE *input;
    input = fopen(argv[1], "r");
    if (input == NULL)
        perror("Can not given file.");
    yyrestart(input);
    yyparse();
    fclose(input);

    std::vector<std::string> typeErrors{};
    ScopedEnv typeCheckingScopeEnv{};
    FunctionEnv typeCheckingFuncEnv{};
    ast->typeCheck(typeCheckingScopeEnv, typeCheckingFuncEnv, typeErrors);
    if (!typeErrors.empty())
    {
        for (std::string typeError : typeErrors)
        {
            std::cout << typeError << std::endl;
        }
    }
    else
    {
        ScopedEnv scopeEnvironment{};
        FunctionEnv functionEnvironment{};
        ast->interpret(scopeEnvironment, functionEnvironment);
    }

    return 1;
}
