#ifndef _FILE_HANDLER
#define _FILE_HANDLER

#include <map>
#include "point.h"
#include <sstream>

using namespace std;


/**
 * Reads a file and decrypts it.
 * @param fileName name of the file
 * @return map of decrypted objects
 */
map<Point, string> decryptClassifiedFile(string fileName);

/**
 * Reads a file and decrypts it.
 * @param fileName name of the file
 * @return vector of unclassified points
 */
vector<Point> decryptUnclassifiedFile(string fileName);

/**
 * Writes a file and encrypts the data to it.
 * @param fileName name of the file
 * @param func function to encrypt to the file
 * @param data map of encrypted objects
 */
void encryptFile(string& fileName, map<Point, string>& data);
#endif