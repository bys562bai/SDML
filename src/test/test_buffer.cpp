#include <iostream>
#include <fmt/format.h>
#include <string>
#include <sdml/container/Buffer.h>

namespace {
	using namespace std;
	using sdml::Buffer;
	using sdml::BufRef;

	struct Tester{
		Buffer<char> buffer;

		bool test(){
			test_init();
			return true;
		}

		bool test_init(){
			static char s[] = "Hello, world!";
			buffer = Buffer<char>(s, s + sizeof(s));
			cout<<buffer<<endl;
			return true;
		}

	};
}

int main(char** args, int argc){
	Tester test;
	test.test();
	return 0;
}