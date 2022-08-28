#include <vector>
#include <sstream>
#include "point.h"
#include <map>

using namespace std;

/**
 * Classifies a point.
 * @param unclassified point to classify
 * @param points the map of points to their classifications
 * @param distanceType the distance metric to use
 * @param k the number of neighbors to use
 * @return the classification of the point
 */
string classify(Point unclassified, map<Point, string>& points, Point::DistanceMetric distanceType, int k) {
    int size = points.size();
    vector<Point> pointsVector = vector<Point>(size);
    int i = 0;
    for (auto &point : points) {
        pointsVector[i] = point.first;
        i++;
    }
    vector<Point> kNearest = unclassified.getKClosest(pointsVector, distanceType, k);
    map<string, int> amountOfType = map<string, int>();
    for (auto &point : kNearest) {
        amountOfType[points[point]]++;
    }
    pair<string, int> correctType = pair<string, int>("", -1);
    for (auto &type : amountOfType) {
        if (type.second > correctType.second) {
            correctType = type;
        }
    }
    return correctType.first;
}

vector<string> split(string& line, char delimiter) {
    vector<string> resultVector;
    stringstream stringStream(line);
    string item;
    while (getline(stringStream, item, delimiter)) {
        resultVector.push_back(item);
    }
    return resultVector;
}

bool isNumber(string& str) {
    if (str.empty()) {
        return false;
    }
    bool wasThereADot = false;
    for (char c : str) {
        if (!isdigit(c)) {
            if (wasThereADot) {
                return false;
            } else if (c == '.'){
                wasThereADot = true;
            } else return false;
        }
    }
    return true;
}

void trim(string& str) {
    const string WHITESPACES = " \n\r\t\f\v";
    size_t last = str.find_last_not_of(WHITESPACES);
    str = str.substr(0, last + 1);
}

bool isPoint(string& str) {
    trim(str);
    vector<string> splitStr = split(str, ',');
    for (string& item : splitStr) {
        if (!isNumber(item)) {
            return false;
        }
    }
    return true;
}
