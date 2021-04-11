#pragma once
#include "kind_def.h"

namespace sdml
{
    namespace ast{
        struct ASTNode{

            virtual SDML_NODE_KIND get_kind() = 0;
            virtual uint32_t token_start() const = 0;
            virtual uint32_t token_end() const = 0;
            virtual ~ASTNode(){}

            bool is_leaf() const {
                return _size == 0;
            }

            //返回子节点数量
            uint32_t size() const {
                return _size;
            }
        protected:
            uint32_t _size = 0;
        };


        struct Token : ASTNode {
            uint32_t get_idx() const {
                return _lex_node_idx;
            }

        protected:
            void set_idx(uint32_t idx) {
                _lex_node_idx = _lex_node_idx;
            }

        private:
            uint32_t _lex_node_idx;
        };

        struct Var : Token {
            Var(uint32_t idx) {
                set_idx(idx);
            }

            virtual SDML_NODE_KIND get_kind() {
                return KIND_VAR;
            }  
            virtual uint32_t token_start() const {
                return get_idx();
            }
            virtual uint32_t token_end() const {
                return get_idx() + 1;
            }
        };
        
    }

} // namespace sdml
