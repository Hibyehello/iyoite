#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Common.h"

class Tokenizer {
public:
  Tokenizer() = default;
  std::vector<Token> Tokenize(std::string path);

private:
  std::string m_source;
  std::vector<Token> m_tokens;
  size_t m_current_idx;

  [[nodiscard]] std::optional<char> peek(int offset = 0) const;
  char get_current();
  std::string ReadFile(std::string path);
};