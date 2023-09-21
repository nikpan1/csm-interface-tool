#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string ORIGIN_START = "O2";
std::string bitMask = "   100001";
#define tempFilename "temp.txt"

std::string convert(const std::string &origin) {
  std::string mount = origin;
  std::string number = origin.substr(1, 2);
  std::cout << number << "-D\n";
  // int n = std::stoi(number) - 2;
  int n = 2;
  number = "M" + std::to_string(n);

  mount.insert(0, number);
  mount.insert(mount.size() - bitMask.size(), bitMask);

  return mount;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Zla ilosc argumentow\n wzor:\n ./MountCreator [sciezka]\n";
    return -1;
  }

  std::ifstream readedFile(argv[1]);
  if (!readedFile) {
    std::cerr << "File not found\n";
    return -1;
  }

  std::ofstream tempFile(tempFilename, std::ios::out | std::ios::app);
  if (!tempFile) {
    std::cerr << "Temporary file was not created\n";
    return -1;
  }

  std::vector<std::string> mountLines;
  std::string line;

  // find first line of origin
  while (std::getline(readedFile, line) ||
         !(ORIGIN_START.compare(line.substr(0, 1)))) {
    tempFile << line;
    std::cout << line;
  }

  mountLines.push_back(convert(line));
  while (std::getline(readedFile, line) && line[0] == 'O') {
    mountLines.push_back(convert(line));
  }

  // go to first line of mount
  while (std::getline(readedFile, line) && !(line[0] == 'i')) {
    tempFile << line;
  }
  for (auto _line : mountLines) {
    tempFile << line;
  }

  tempFile.close();
  readedFile.close();

  // Remove the original file and replace it with the temporary file
  if (std::remove(argv[1]) != 0) {
    std::cerr << "Error deleting the original file.\n";
    return -1;
  }
  if (std::rename(tempFilename, argv[1]) != 0) {
    std::cerr << "Error replacing the original file with the temporary file.\n";
    return -1;
  }

  return 1;
}
