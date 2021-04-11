#include <string>

#include <fmt/format.h>

#include <sdml/core/lexer.h>

#include "test_util.h"


namespace {
	using namespace sdml;
	using std::string;
	struct Tester {
		Lexer lexer;
		inline static string number_s = R"xxx(
 123 456 s789 +324 -342 +0b333 
777 888 33abc_e333
	+0abfdsf.fds,dfs,5.55,e-342e+32**)xxx";

		inline static string comment_s = R"xxx(
	32fdas var1 var2 #var4224 varxxx
	#民#主 和￥谐
#%fsds   # ttt % xxx	
#富强 文明
	xxxx#友善)xxx";

		inline static string identifier_s = R"xxx(
	32fdas var1 var2 var4224 varxxx
变量
	变量 变量333xdfs xx变量33xx3)xxx";

		inline static string string_s = R"xxxxxx(
	"Hello" "World"
"富强" "民主" "和x谐" "#123"
	prefix"xxjfosf"
前缀"dfsfs"
	\"fdfsa"
	" \" fds \" fds\\\"fs哈哈哈aijcvx\\\\"  
	前prefix缀"cxcxxc"[]
""
	R"""[ \ \ """"""\ \ ]"""; R"tag[ df\ \\ \\ \jsos fsd]tag";
	R"中文[ 富强 民主 和谐]中文"后缀
	r"\ \ \ \\  \\ \"

	前prefix缀"cxcxxc"后suffix缀)xxxxxx";
		


		Tester() {
		}

		bool test() {
			test_init();
			test_number();
			test_comment();
			test_vars();
			test_string();

			return true;
		}

		bool test_init() {
			return true;
		}

		bool test_number() {
			return test_by_source("TEST NUMBER", number_s);
		}

		bool test_comment() {
			return test_by_source("TEST COMMENT", comment_s);
		}

		bool test_vars() {
			return test_by_source("TEST IDENTIFIER", identifier_s);
		}

		bool test_string() {
			return test_by_source("TEST STRING", string_s);
		}

	private:
		bool test_by_source(const std::string& name, const std::string& src) {
			using std::cout, std::endl;
			cout << name << endl;
			lexer.set_source(src);
			auto res = lexer.parse();
			for (int i = 0; i < res.nodes.size(); i++) {
				fmt::print("{}\n", res.get_node_desc_string(i));
			}
			cout << endl;
			return true;
		}
	};
}


int main(char** args, int argc) {
	Tester test;
	test.test();
	return 0;
}