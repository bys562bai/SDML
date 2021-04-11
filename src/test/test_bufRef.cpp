#include <iostream>
#include <string>
#include <sdml/container/BufRef.h>
#include <fmt/format.h>

namespace{
    using namespace std;
    using sdml::BufRef;

struct Tester{

    string s;
    BufRef<char> ref;

    bool test(){
        test_init();
        test_iterator();
        test_slice();
        return true;
    }


    bool test_init(){
        s = "Hello, world!";
        ref = BufRef(s.data(), s.size());
        ref = ref;
        cout<<ref<<endl;
        return true;
    }

    bool test_iterator(){
        for(auto& x: ref){
            x = '2';
        }
        cout<<ref<<endl;
        for(auto p = ref.rbegin(), end = ref.rend(); p!=end; p++){
            *p = '3';
        }
        cout<<ref<<endl;
        return true;
    }

    bool test_slice(){
        for(auto& x: ref.slice(3,6)){
            x = '4';
        }
        cout<<ref<<endl;
        for (auto& x : ref.slice(6, -2)) {
            x = '5';
        }
        cout << ref << endl;
        return true;
    }

};

}

int main(char** args, int argc){
    Tester test;
    test.test();
    return 0;
}