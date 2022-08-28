#ifndef _POINT
#define _POINT

#include <vector>
#include <sstream>

using namespace std;

class Point {

    protected:
    vector<double> fields;
    int size;

    public: 

    /**
     * Default constructor
     */
    Point();

    /**
     * Constructor.
     * @param data the string containing the data.
     */
    Point(string data); 

    /**
     * Get Euclidean distance between this point and other point.
     * @param other other point
     * @return Euclidean distance
     */
    double getEuclideanDistance(const Point& other) const;
    
    /**
     * Get Manhattan distance between this point and other point.
     * @param other other point
     * @return Manhattan distance
     */
    double getManhattanDistance(const Point& other) const;

    /**
     * Get Chebyshev distance between this point and other point.
     * @param other other point
     * @return Chebyshev distance
     */
    double getChebyshevDistance(const Point& other) const;

    enum DistanceMetric {
        EUCLIDEAN,
        MANHATTAN,
        CHEBYSHEV
    };

    /**
     * Get the k nearest neighbors of this point.
     * @param otherPoints vector of other points
     * @param distanceType distance metric
     * @param k number of neighbors
     * @return the k nearest neighbors
     */
    vector<Point> getKClosest(const vector<Point>& otherPoints, DistanceMetric distanceType, int k) const;

    /**
    * Get the string representation of this point.
    * @return string representation of this point
    */
    string toString() const;

    /**
     * Get the string representation of the DistanceMetric.
     * @return string representation of the DistanceMetric
     */
    static string toString(DistanceMetric distanceType);

    /**
     * Compare this point with other point.
     * @param other other point
     * @return true if this point is less than the other point, false otherwise
     */
    bool operator<(const Point& other) const;

    /**
     * Returns the dimension of this point.
     * @return the dimension of this point
     */
    int getDimension() const;
};
#endif