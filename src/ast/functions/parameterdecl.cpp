#include "parameterdecl.h"

ParameterDecl::ParameterDecl(Type *type, IdentifierList *identifiers) : type{type}, identifiers{identifiers} {}