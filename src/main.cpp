#include "calc.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::string tmp = "%s: invalid number of arguments";
        error(tmp.c_str(), argv[0]);
    }

    char* raw = argv[1];
    calc::Tokenizer* tokenizer = new calc::Tokenizer();
    calc::Parser* parser = new calc::Parser();
    calc::Generator* generator = new calc::Generator();

    calc::Token* token = tokenizer->Tokenize(raw);

    calc::Node* node = parser->Parse(token);

    generator->GenCode(node);

    return 0;
}
