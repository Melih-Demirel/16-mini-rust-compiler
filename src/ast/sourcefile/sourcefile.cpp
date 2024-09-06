#include "./sourcefile.h"
#include "../../environment/env.h"

SourceFile::SourceFile(DeclList *globalDecls) : globalDecls{globalDecls} {}

void SourceFile::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (globalDecls != NULL)
    {
        globalDecls->setupFuncEnv(funcEnv);
        globalDecls->interpret(env, funcEnv);
    }
    env.pushScope();
    funcEnv.pushFunc(std::string("main"));
    funcEnv.lookupVariable(std::string("main"))->funcDecl->funcBody->interpret(env, funcEnv);
    funcEnv.popFunc();
    env.popScope();
    return;
}

void SourceFile::typeCheck(ScopedEnv &typeCheckingEnv, FunctionEnv &typeCheckingFuncEnv, std::vector<std::string> &typeErrors)
{
    if (globalDecls != NULL)
    {
        globalDecls->setupFuncEnv(typeCheckingFuncEnv);
        globalDecls->setupGlobalEnv(typeCheckingEnv, typeCheckingFuncEnv);

        globalDecls->typeCheck(typeCheckingEnv, typeCheckingFuncEnv, typeErrors);
    }

    if (typeCheckingFuncEnv.lookupVariable(std::string("main")) == nullptr)
    {
        typeErrors.push_back("Type error: No main function was found.");
    }
}
