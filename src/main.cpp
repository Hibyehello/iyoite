#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "ArenaAlloc.h"
#include "ArgParser.h"
#include "ErrorHandler.h"
#include "Generation.h"
#include "Parser.h"
#include "Tokenizer.h"

void WriteASM(std::string name, std::string assembly) {

  // t = t.substr(t.find_last_of("/") + 1, t.find_last_of(".") - 2);
  std::stringstream outfile;
#ifdef IYO_DEBUG
  outfile << name << ".asm";
#endif
  std::cout << outfile.str() << std::endl;
  std::fstream file_stream(outfile.str(), std::ios::out);
  file_stream << assembly;
  file_stream.close();

  std::stringstream asmcmd;
  asmcmd << "nasm -fmacho64 " << name << ".asm -o " << name << ".o";
  std::stringstream ldcmd;
  ldcmd << "ld " << name << ".o "
        << "-o " << name
        << " -macosx_version_min 11.0 "
           "-L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/ "
           "-lSystem";

  system(asmcmd.str().c_str());
  system(ldcmd.str().c_str());
}

int main(int argc, char **argv) {
  ArgParser argParser = ArgParser();
  Tokenizer tokenizer = Tokenizer();
  Generation generator = Generation();
  Parser parser = Parser();

  argParser.parse(argc, argv);

  auto inputfiles = argParser.get_input_files();

  std::vector<std::vector<Token>> tokens;
  try {
    for (const auto &inputfile : inputfiles) {
      tokens.push_back(tokenizer.Tokenize(inputfile));
    }
    Node::Exec exec = parser.parse_exec(tokens.at(0));
    std::string asm_source = generator.gen_exec(exec);
    WriteASM("test", asm_source);
  } catch (CompilerException &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}
