#include "MyGrammarBaseListener.h"
#include "MyGrammarLexer.h"
#include "MyGrammarParser.h"
#include <ANTLRInputStream.h>
#include <antlr4-runtime.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Listener : public MyGrammarBaseListener {
public:
  std::unordered_map<std::string, int> variables;
  std::string last_var;

  void exitStart(MyGrammarParser::StartContext *ctx) override {
    // start -> VAR = stmt
    std::string var = ctx->VAR()->getText();
    variables[var] = variables[last_var];
  }

  void exitStmt(MyGrammarParser::StmtContext *ctx) override {
    if (ctx->plus != nullptr) {
      // stmt -> ++ VAR
      std::string var = ctx->VAR()->getText();
      variables[var]++;
      if (!ctx->parent->children.empty() &&
          ctx->parent->children[0]->getText() == "=") {
        auto var2 = ctx->parent->parent->children[0]->getText();
        variables[var2] = variables[var];
        std::cout << var2 << "=" << variables[var] << "\n";
        last_var = var2;
      }
    } else if (ctx->VAR() != nullptr) {
      // stmt -> VAR stmt'
      std::string var = ctx->VAR()->getText();
      if (variables.find(var) == variables.end()) {
        variables[var] = 0;
      }

      if (!ctx->parent->children.empty() &&
          ctx->parent->children[0]->getText() == "=") {
        auto var2 = ctx->parent->parent->children[0]->getText();
        variables[var2] = variables[var];
        std::cout << var2 << "=" << variables[var] << "\n";
        last_var = var2;
      }
    } else if (ctx->INT() != nullptr) {
      // stmt -> INT stmt'
      if (ctx->stmt_()->equal != nullptr)
        throw std::runtime_error("Ошибка!\n");
      int value = std::stoi(ctx->INT()->getText());
      if (!ctx->parent->children.empty() &&
          ctx->parent->children[0]->getText() == "=") {
        auto var = ctx->parent->parent->children[0]->getText();
        variables[var] = value;
        std::cout << var << "=" << value << "\n";
        last_var = var;
      }
    }
  }
};

int main(int argc, const char *argv[]) {
  std::ifstream stream;
  if (argc > 1) {
    stream.open(argv[1]);
    std::cout << argv[1] << std::endl;
  } else {
    stream.open("../test");
  }
  antlr4::ANTLRInputStream input(stream);

  MyGrammarLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  tokens.fill();
  for (auto token : tokens.getTokens()) {
    std::cout << token->toString() << std::endl;
  }

  MyGrammarParser parser(&tokens);
  Listener listener;
  parser.addParseListener(&listener);

  antlr4::tree::ParseTree *tree = parser.start();

  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

  // Выводим значения переменных
  for (const auto &[var, value] : listener.variables) {
    std::cout << var << "=" << value << std::endl;
  }

  return 0;
}
