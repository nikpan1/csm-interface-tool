#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

std::string convert(const std::string &origin) {
  std::string mount = origin;
  std::string mountNumber = origin.substr(1, 2);

  int n = std::stoi(mountNumber) - 2;
  mountNumber = "M" + std::to_string(n);

  mount.replace(0, (n < 10) ? 2 : 3, mountNumber);

  mount.replace(mount.size() - bitMask.size() - 1, bitMask.size() + 1,
                bitMask + "\n");

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
