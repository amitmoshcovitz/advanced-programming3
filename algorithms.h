#ifndef _ALGORITHMS
#define _ALGORITHMS

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
string classify(Point unclassified, map<Point, string>& points, Point::DistanceMetric distanceType, int k);

/**
 * Partitions the vector
 * @param vector the vector to partition
 * @param left the left index of the vector
 * @param right the right index of the vector, also the pivot index
 * @param compare the comparison function
 * @return the index of the pivot
 */
template <typename T>
int partition(vector<T>& vector, int left, int right, bool(*compare)(T, T)) {
    T pivot = vector[right];
    int i = (left - 1);
    for (int j = left; j <= right - 1; j++) {
        if (!compare(pivot, vector[j])) {
            i++;
            swap(vector[i], vector[j]);
        }
    }
    swap(vector[i + 1], vector[right]);
    return (i + 1);
}

/**
 * Sorts the vector so that the first k elements are the k smallest elements
 * @param vector the vector to sort
 * @param k the number of elements at the beginning of the vector
 * @param compare the comparison function
 * @return the kth smallest element
 */
template <typename T>
T kthSmallest(vector<T>& vector, int k, bool(*compare)(T, T)) {
    int left = 0, right = vector.size() - 1;
    while (left <= right) {
        int pivotIndex = partition(vector, left, right, compare);
        if (pivotIndex == k - 1)
            return vector[pivotIndex];
        else if (pivotIndex > k - 1)
            right = pivotIndex - 1;
        else
            left = pivotIndex + 1;
    }
    return vector[k - 1];
}

/**
 * Splits a string into a vector of strings.
 * @param line string to split
 * @param delimiter delimiter to split the string with
 * @return vector of strings
 */
vector<string> split(string& line, char delimiter);

/**
 * Checks if a string is a number (double).
 * @param str string to check
 * @return true if the string is a number, false otherwise
 */
bool isNumber(string& str);

/**
 * Checks if a string is a point.
 * @param str string to check
 * @return true if the string is a point, false otherwise
 */
bool isPoint(string& str);

/**
 * Trims a string.
 * @param str string to trim
 */
void trim(string& str);
#endif