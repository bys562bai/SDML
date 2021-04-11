/*
Copyright 2020 happyxianyu.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdexcept>
#include <exception>

#include "lexer.h"


namespace sdml {
	namespace {

		static const std::string add_para(std::string& s) {
			return std::string("(") + s + ")";
		}
	}


	LexResult<const char> Lexer::parse()
	{
		if (source.empty())
			return {};

		_pb = _pc = source.begin();
		_pe = source.end();

		nodes.~Buffer();
		new (&nodes) Buffer<LexNode>();
		nodes.reserve(source.size() / 20);

		decltype(&Lexer::parse_space) parse_funcs[] = {
			&Lexer::parse_space,
			&Lexer::parse_comment,
			&Lexer::parse_number,
			&Lexer::parse_string,
			&Lexer::parse_identifier,
			&Lexer::parse_keyword,
			&Lexer::parse_operator,
		};

		bool unk_flag = true;
		while (_pc != _pe) {
			auto* pc_tmp = _pc; //used for restoring if fail to parse
			bool success_flag = false;
			for (auto f : parse_funcs) {
				if ((this->*f)()) {
					success_flag = true;
					break;
				}
				_pc = pc_tmp;
			}

			if (!success_flag) {
				if (is_end())
					break;
				if (unk_flag) {
					put_node(KIND_UNKNOWN, pc_pos());
					unk_flag = false;
				}
				inc_pc();
			}
			else{
				unk_flag = true;
			}
		}
		return LexResult<const char>{ source, std::move(nodes) };
	}

	bool Lexer::parse_space()
	{
		if (!is_space(*_pc))
			return false;

		auto pos = pc_pos();
		while (is_space(*_pc)) {
			inc_pc();
		}
		put_node(KIND_SPACE, pos);
		return true;
	}

	bool Lexer::parse_comment()
	{
		const static re2::RE2 pat(R"xxx(#[^\r\n]*\n|$)xxx");
		auto pos = pc_pos();
		if (parse_by_regex(pat)) {
			put_node(KIND_COMMENT, pos);
			return true;
		}
		return false;
	}

	bool Lexer::parse_identifier()
	{
		const static re2::RE2 pat( pat_utf_general_no_num + pat_utf_general + "*");
		auto pos = pc_pos();
		if (parse_by_regex(pat)) {
			put_node(KIND_IDENTIFIER, pos);
			return true;
		}
		return false;
	}

	bool Lexer::parse_keyword()
	{
		const static re2::RE2 pat("\\\\" + pat_utf_general + "*");
		auto pos = pc_pos();
		if (parse_by_regex(pat)) {
			put_node(KIND_KEYWORD, pos);
			return true;
		}
		return false;
	}

	bool Lexer::parse_operator()
	{
		char ch;
		ch = *_pc;
		auto pos = pc_pos();
		switch (ch)
		{
		case '(': case ')':
		case '[': case ']':
		case '{': case '}':

		case '.': case ',': case ';': case ':':
		case '=': 
			_pc++;
			put_node(KIND_OPERATOR, pos);
			return true;
		default:
			break;
		}

		//TODO: ƥ���������ַ��ķ���
		return false;
	}

	bool Lexer::parse_number()
	{
		const static re2::RE2 pat(R"xxx((\+|-)?\.?[0-9][0-9\w,\.\+-]*)xxx");
		auto pos = pc_pos();
		if (parse_by_regex(pat)) {
			put_node(KIND_IDENTIFIER, pos);
			return true;
		}
		return false;
	}

	bool Lexer::parse_string()
	{
		const static re2::RE2 prefix(pat_utf_general_no_num + pat_utf_general + "*");
		const static re2::RE2 suffix(pat_utf_general + "*");
		
		auto pos = pc_pos();
		if (parse_string_raw()) {
			parse_by_regex(suffix);
			put_node(KIND_STRING, pos);
			return true;
		}

		parse_by_regex(prefix);
		if (parse_string1()) {
			parse_by_regex(suffix);
			put_node(KIND_STRING, pos);
			return true;
		}
		return false;
	}

	bool Lexer::parse_string1()
	{
		const static re2::RE2 suffix(pat_utf_general + "*"); //pat of suffix
		auto quote = *_pc;
		if (quote == '"' || quote == '\'') {
			_pc++;
		}
		else {
			return false;
		}

		for (;;) {
			if (is_end()) {
				throw LexEOFException(KIND_STRING);
			}
			auto ch = *_pc;
			inc_pc();
			if (ch == quote) {
				return true;
			}
			else if (ch == '\\') {
				parse_string_escape();
			}
		}

		return false;
	}

	bool Lexer::parse_string_raw()
	{
		//TODO
		auto ch = *_pc;
		if (ch == 'r') {
			_pc++;
			if (is_end())
				return false;
			return parse_string_r();
		}
		else if (ch == 'R') {
			_pc++;
			if (is_end())
				return false;
			return parse_string_R();
		}
		
		return false;
	}

	bool Lexer::parse_string_r()
	{
		auto quote = *_pc;
		char ch;
		if (quote == '"' || quote == '\'') {
			_pc++;
		}
		else {
			return false;
		}

		for (;;) {
			if (is_end())
				throw LexEOFException(KIND_STRING);

			ch = *_pc;
			if (ch == quote) {
				_pc++;
				return true;
			}
			inc_pc();
		}
		return false;
	}

	bool Lexer::parse_string_R()
	{
		auto quote = *_pc;
		char ch;
		if (quote == '"' || quote == '\'') {
			_pc++;
		}
		else {
			return false;
		}

		auto tag_pb = _pc;
		for(;;){
			if (is_end())
				throw LexEOFException(KIND_STRING);
			ch = *_pc;
			if (ch == '[')
				break;
			inc_pc();
		}

		std::string_view tag(tag_pb, _pc - tag_pb); //tag
		_pc++; 

		for (;;) {
			if(is_end())
				throw LexEOFException(KIND_STRING);

			ch = *_pc;
			if (ch == ']') {
				_pc++;
				if (is_end())
					throw LexEOFException(KIND_STRING);
				std::string_view dst_s(_pc, _pe - _pc);
				if (dst_s.starts_with(tag)) {
					_pc += tag.size();
					if (is_end())
						throw LexEOFException(KIND_STRING);
					if (*_pc == quote) {
						_pc++;

						return true;
					}
				}
			}
			inc_pc();
		}
		return false;
	}

	bool Lexer::parse_string_escape()
	{
		inc_pc();
		//TODO
		return true;
	}

	bool Lexer::parse_by_regex(const re2::RE2& pat)
	{
		re2::StringPiece piece(_pc, _pe - _pc);
		if (!RE2::Consume(&piece, pat)) {
			return false;
		}
		
		_pc = piece.data();
		return true;
	}

}