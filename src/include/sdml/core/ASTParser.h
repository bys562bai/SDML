#pragma once
#include <sdml/container/Buffer.h>

#include "ASTNode.h"
#include "lexer.h"

namespace sdml{
    namespace ast{

        template<typename T = const char*>
        struct AST{
            LexResult<T> lexResult;
            
        };

        struct ASTParser{

        };
    }
}