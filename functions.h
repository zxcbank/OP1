#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>

const char kDefaultDelimiter = '\n';
const int kFullLine = -1; // с этим параметром будет печататься весь файл
const bool kDefaultTail = false;



struct Params {
  int64_t lines = kFullLine;
  std::string filename;
  char delimiter = kDefaultDelimiter;
  bool tail = kDefaultTail;
};

enum class ErrorCodes{
  kIsFileOpen,
  kIncorrectLinesParameter,
  kIncorrectDelimiter,
  kIncorrectAmountOfArgs,
};

void WarningMessage(ErrorCodes error_type);
void HelpMessage();
void PrintFile(const Params& params);
char SpecialSymbolConversion(char symbol);
Params Parsing (int argc, char** argv);