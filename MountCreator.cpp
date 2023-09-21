#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string ORIGIN_START = "O2";
std::string bitMask = "   100001";
#define tempFilename "temp.txt"
#define _test std::cout << "test\n";

std::string convert(const std::string &origin) {
  std::cout << origin << "|\n";
  std::string mount = origin;
  std::string number = origin.substr(1, 2);
  std::cout << number << "-D\n";
  // int n = std::stoi(number) - 2;
  int n = 2;
  number = "M" + std::to_string(n);
  std::cout << "Num = " << number << "\nmount= " << mount << "\n";
  mount.insert(0, number);
  mount.insert(mount.size() - bitMask.size(), bitMask);

  return mount;
}

void validate(bool result,
              std::string errorMessage) { // @TODO zmienic nazwe result
  if (!result)
    return;

  std::cerr << errorMessage;
  std::exit(-1);
}
#define writeLine(output, line) output << line << '\n';

int main(int argc, char *argv[]) {
  validate(argc != 2, "Wrong amount of arguments.\n");

  char *readedFilename = argv[1];
  std::ifstream readedFile(readedFilename);
  validate(!readedFile, "Passed file not found.\n");

  std::ofstream tempFile(tempFilename, std::ios::out | std::ios::app);
  validate(!tempFile, "Temporary file was not created.\n");

  std::vector<std::string> mountLines;
  std::string line;

  // find first line of origin
  while (std::getline(readedFile, line) &&
         !(ORIGIN_START.compare(line.substr(0, 1))))
    writeLine(tempFile, line);

  mountLines.push_back(convert(line));
  while (std::getline(readedFile, line) && line[0] == 'O')
    mountLines.push_back(convert(line));

  // go to first line of mount
  while (std::getline(readedFile, line) && !(line[0] == 'i'))
    writeLine(tempFile, line);

  for (auto _mountLine : mountLines)
    tempFile << _mountLine;

  tempFile.close();
  readedFile.close();

  // Remove the original file and replace it with the temporary file
  validate(std::remove(readedFilename) != 0,
           "Deleting the original file not sucessfull.\n");
  validate(std::rename(tempFilename, readedFilename) != 0,
           "Replacing the files not sucessfull.\n");
  return 1;
}
