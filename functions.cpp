//
// Created by Святослав on 02.10.2023.
//
#include "functions.h"

char SpecialSymbolConversion(char symbol) {
  switch (symbol) {
    case 't':return '\t';
    case 'n':return '\n';
    case '\\':return '\\';
    case '\"':return '\"';
    case '\'':return '\'';
    default:WarningMessage(ErrorCodes::kIncorrectDelimiter);
      return '\n';
  }
}

void HelpMessage() {
  std::cout << "Options: \n"
            << std::setiosflags(std::ios::left) << std::setw(20) << "-h, --help " << "Show help message\n"
            << std::setw(20) << "-l, --lines=n" << "Print n first / last lines\n"
            << std::setw(20) << "-t, --tail" << "Print the last lines of the file\n"
            << std::setw(20) << "-d, --delimiter=c" << "String limiter (default \\n)";
}

Params Parsing( int argc, char** argv) {
  Params params;
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h") {
      HelpMessage();
      exit(EXIT_SUCCESS);
    } else if (arg == "-l") {
      i++;

      if (0 <= std::stoi(argv[i])) {
        params.lines = std::stoi(argv[i]);
      } else {
        WarningMessage(ErrorCodes::kIncorrectLinesParameter);
      }
    } else if (arg.substr(0, (static_cast<std::string>("--lines=")).size()) == "--lines=") {
      params.lines = std::stoi(arg.substr((static_cast<std::string>("--lines=")).size(),
                                          arg.length() - (static_cast<std::string>("--lines=")).size()));
    } else if (arg == "-d") {
      arg = argv[++i];

      if (arg[1] != '\\' && arg.length() == (static_cast<std::string>("\'*\'")).size()) {
        params.delimiter = arg[1];
      } else if (arg[1] == '\\' && arg.length() == (static_cast<std::string>("\'**\'")).size()) {
        params.delimiter = SpecialSymbolConversion(arg[2]);
      } else {
        WarningMessage(ErrorCodes::kIncorrectDelimiter);
      }
    } else if (arg.substr(0, (static_cast<std::string>("--delimiter=")).size()) == "--delimiter=") {
      if (arg[(static_cast<std::string>("--delimiter=\'")).size()] != '\\') {
        if (arg.length() == (static_cast<std::string>("--delimiter=\'*\'")).size()) {
          params.delimiter = arg[(static_cast<std::string>("--delimiter=\'")).size()];
        } else {
          WarningMessage(ErrorCodes::kIncorrectDelimiter);
        }
      } else if (arg.length() == (static_cast<std::string>("--delimiter=\'**\'")).size()) {
        params.delimiter = SpecialSymbolConversion(arg[(static_cast<std::string>("--delimiter=\'\\*" - 1)).size()]);
      } else {
        WarningMessage(ErrorCodes::kIncorrectDelimiter);
      }
    } else if (arg == "--tail" || arg == "-t") {
      params.tail = true;
    } else if (params.filename.empty()) {
      params.filename = arg;
    } else {
      WarningMessage(ErrorCodes::kIncorrectAmountOfArgs);
    }
  }
  return params;
}

void WarningMessage(ErrorCodes error_type) {

  switch (error_type) {
    case (ErrorCodes::kIsFileOpen):std::cerr << "file is not open\n";
      break;
    case (ErrorCodes::kIncorrectLinesParameter):
      std::cerr << "incorrect number of lines\n";
      break;
    case (ErrorCodes::kIncorrectDelimiter):
      std::cerr << "incorrect delimeter char given\n";
      break;
    case (ErrorCodes::kIncorrectAmountOfArgs):
      std::cerr << "Too much arguments given\n";
      break;
    default:std::cerr << "unexpected error\n";
      break;
  }
}

void PrintFile(const Params& params) {
  std::ifstream file(params.filename, std::ios::binary);
  int number_of_lines, last_byte;
  number_of_lines = params.lines;

  if (!file.is_open()) {
    WarningMessage(ErrorCodes::kIsFileOpen);
    exit(EXIT_FAILURE);
  }
  if (params.tail) {
    file.seekg(0, std::ios::end);
    last_byte = file.tellg();
    file.seekg(last_byte - 1, std::ios::beg);

    for (int j = 0; j < last_byte; j++) {
      if ((file.get()) == params.delimiter) {
        number_of_lines--;
      }
      if (number_of_lines == 0){
        break;
      }
      file.seekg(-j - 1, std::ios::end);
    }
    char ch = file.get();
    std::cout << ch;
    while (file) {
      ch = file.get();
      std::cout << ch;
    }
  } else {
    char ch;
    while (file && number_of_lines > 0) {
      ch = (file.get());
      if (ch == params.delimiter)
        number_of_lines--;
      std::cout << ch ;
    }
  }
}