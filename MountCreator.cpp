#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum FEEDERS {
  F11 = 11, // LED-green, SMD1206
  F12 = 12, // LED-red, SMD1206
  F13 = 13, // 100nF/50V
  F14 = 14, // 220nF/250V
  F15 = 15, // 1nF/600V
  F16 = 16, // 2.2kOhm/1%
  F17 = 17, // 1kOhm/1%
  F18 = 18, // 2.2nF/500V
  F19 = 19, // 47nF/50V
  F20 = 20, // 10nF/630V
  F21 = 21, // 10nF/50V
  F22 = 22, // 100uF/25V
  F23 = 23, // 390pF/6300V
  F24 = 24, // 47nF/500V
  F25 = 25  // 22pF/50V
};

std::string ORIGIN_START = "O2";
std::string BADMARK_START = "i0";
std::string bitMask = "   100001";

#define tempFilename "temp.txt"
#define _test std::cout << "test\n";
#define writeLine(output, line)                                                \
  {                                                                            \
    std::cout << "  ";                                                         \
    std::cout << line << "\n";                                                 \
    output << line << '\n';                                                    \
  }

std::string generateBitmask(uint8_t feeder, uint8_t header, bool skip) {
  std::string result = bitMask;
  // header 1/2

  std::string buffer;

  // setting feeder type
  buffer = std::to_string(feeder);
  result.replace(result.size() - 6, buffer.size(), buffer);

  // setting header type
  buffer = std::to_string(header);
  result.replace(result.size() - 1, buffer.size(), buffer);

  // setting skip type
  buffer = std::to_string(skip);
  result.replace(result.size() - 4, buffer.size(), buffer);

  result = result + "\n";

  return result;
}

std::string convert(const std::string &origin) {
  std::string mount = origin;
  std::string mountNumber = origin.substr(1, 2);

  int n = std::stoi(mountNumber) - 2;
  mountNumber = "M" + std::to_string(n);

  // pastes the number
  mount[2] = ' ';
  mount.replace(0, (n < 10) ? 2 : 3, mountNumber);

  std::string _bitmask = generateBitmask(3, 1, false);
  mount.replace(mount.size() - _bitmask.size(), _bitmask.size(), _bitmask);

  return mount;
}

void validate(bool statement, std::string errorMessage) {
  if (!statement)
    return;

  std::cerr << errorMessage;
  std::exit(-1);
}

int main(int argc, char *argv[]) {
  validate(argc != 2, "Wrong amount of arguments.\n");

  char *readedFilename = argv[1];
  std::vector<std::string> mountLines;
  std::string line;
  std::ifstream readedFile(readedFilename);
  std::ofstream tempFile(tempFilename, std::ios::out | std::ios::app);

  validate(!readedFile, "Passed file not found.\n");
  validate(!tempFile, "Temporary file was not created.\n");

  // find first line of origin
  while (std::getline(readedFile, line) &&
         (ORIGIN_START.compare(line.substr(0, 2))))
    writeLine(tempFile, line);

  mountLines.push_back(convert(line));
  while (std::getline(readedFile, line) && line[0] == 'O') {
    mountLines.push_back(convert(line));
    writeLine(tempFile, line);
  }

  // go to first line of mount
  while (std::getline(readedFile, line) &&
         (BADMARK_START.compare(line.substr(0, 2))))
    writeLine(tempFile, line);

  for (auto _mountLine : mountLines)
    tempFile << _mountLine;

  tempFile.close();
  readedFile.close();

  // Remove the original file and replace it with the temporary file
  validate(std::remove(readedFilename) != 0,
           "Error while deleting the original file.\n");
  validate(std::rename(tempFilename, readedFilename) != 0,
           "Error while replacing the files.\n");
  return 1;
}
