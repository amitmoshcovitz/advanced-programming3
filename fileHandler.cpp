#include <vector>
#include <fstream>
#include <map>
#include "point.h"
#include "fileHandler.h"

using namespace std;

map<Point, string> decryptClassifiedFile(string fileName) {
    map<Point, string> decryptedFile;
    ifstream file(fileName);
    if (!file.is_open()) {
        return decryptedFile;
    }
    string line;
    while (getline(file, line)) {
        int seperatorIndex = line.find_last_of(",");
        decryptedFile[Point(line.substr(0, seperatorIndex))] = line.substr(seperatorIndex + 1);
    }
    file.close();
    return decryptedFile;
}

vector<Point> decryptUnclassifiedFile(string fileName) {
    vector<Point> decryptedFile;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        decryptedFile.push_back(Point(line));
    }
    file.close();
    return decryptedFile;
}

void encryptFile(string fileName, map<Point, string>& data) {
    ofstream file(fileName);
    for (auto it = data.begin(); it != data.end(); it++) {
        file << it->first.toString() << "," << it->second << endl;
    }
    file.close();
}