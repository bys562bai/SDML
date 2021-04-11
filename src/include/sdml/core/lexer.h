#pragma once
#include <string>
#include <string_view>
#include <concepts>
#include <type_traits>

#include <fmt/format.h>
#include <utf8cpp/utf8.h>

#include <sdml/container/container.h>
#include <sdml/util/re2.h>
#include "kind_def.h"

namespace sdml
{
    struct LexNode {
        SDML_NODE_KIND kind;
        uint32_t pos;
    };


    template<typename T = const char>
    struct LexResult {
        BufRef<T> source;
        Buffer<LexNode> nodes;

        BufRef<T> get_node_bufref(uint32_t i) {
            if (i == nodes.size() - 1) {
                return source.slice(nodes[i].pos);
            }
            else {
                return source.slice(nodes[i].pos, nodes[i + 1].pos);
            }
        }

        std::basic_string_view<std::decay_t<T> > get_node_string_view(uint32_t i) {
            auto ref = get_node_bufref(i);
            return std::basic_string_view<std::decay_t<T> >(ref.data(), ref.size());
        }

        std::string get_node_desc_string(uint32_t i) {
            auto& n = nodes[i];
            return fmt::format("({}, {}, {})", n.pos, to_string(n.kind), get_node_string_view(i));
        }
    };

    struct LexException{
        SDML_NODE_KIND kind;
    };

    struct LexEOFException : LexException {
        LexEOFException(SDML_NODE_KIND k) {
            k = kind;
        }
    };

    struct Lexer{
        Lexer(){}


        Lexer(const BufRef<const char>& src) {
            source = src;
        }

        BufRef<const char>& get_source() {
            return source;
        }

        void set_source(const std::string_view& src) {
            source = src;
        }

        template<typename T> requires std::same_as<std::decay_t<T>, char>
        void set_source(const BufRef<T>& src) {
            source = src;
        }

        LexResult<const char> parse(const std::string_view& src) {
            set_source(src);
            return parse();
        }

        //utf8
        LexResult<const char> parse();
        bool parse_space();
        bool parse_comment();
        bool parse_number();
        bool parse_string();
        bool parse_identifier();
        bool parse_keyword();
        bool parse_operator();

    private:
        static bool is_space(char c) {
            switch (c)
            {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
            case '\v':
            case '\f':
                return true;
            default:
                return false;
            }
        }

        bool parse_by_regex(const re2::RE2& pat);

        bool parse_string1();
        bool parse_string_raw();
        bool parse_string_r(); //r
        bool parse_string_R(); //R
        bool parse_string_escape();

        void inc_pc() {
            signed char ch = *_pc;
            if (ch < 0) {
                utf8::next(_pc, _pe);
            }
            else {
                _pc++;
            }
        }
        bool is_end() {
            return _pc >= _pe;
        }
        uint32_t pc_pos() { return _pc - _pb; }

        void put_node(SDML_NODE_KIND kind, uint32_t pos) {
            nodes.push_back(LexNode{ kind, pos });
        }
    private:
        BufRef<const char> source;
        Buffer<LexNode> nodes;
        const char* _pb = nullptr; //begin
        const char* _pe = nullptr; //end
        const char* _pc = nullptr; //cur
        inline const static std::string pat_utf_general = R"xxx((\w|[^\x00-\x7F]))xxx"; //包括下划线，字母，数字，和任何非ASCII字符以及非控制字符
        inline const static std::string pat_utf_general_no_num = R"xxx(([a-zA-Z_]|[^\x00-\x7F]))xxx";
    };

} // namespace sdml

