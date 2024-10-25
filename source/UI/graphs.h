//------------------------------------------------------------------------------
//  TITLE :          graphs.h
//  DESCRIPTION :    LCD Bar graph encapsulation
//------------------------------------------------------------------------------

#ifndef GRAPHS_H
#define GRAPHS_H

#include <string>
#include "project.h"

class DoubleSidedVerticalBarGraph {
private:
    int32_t x_location;  // X coordinate
    int32_t y_location;  // Y coordinate
    bool flipped;        // orientation
    uint32_t m_eidBar;
    std::string name;       //Device (bucket, nozzle, interceptor)
    std::string side;       //port or stbd, station 1 or 2

public:
    // Constructor that initializes the graph
    DoubleSidedVerticalBarGraph(std::string side, std::string name, int32_t x_location, int32_t y_location, bool flipped);

    // Default constructor
    DoubleSidedVerticalBarGraph();

    // Method to draw the bar graph on the screen
    void draw(int position) const;
};

class DoubleSidedHorizontalBarGraph {
private:
    int32_t x_location;  // X coordinate
    int32_t y_location;  // Y coordinate
    int orientation;        // orientation: 0 = centered, 1 = left, 2 = right
    uint32_t m_eidBar;
    std::string name;       //Device (bucket, nozzle, interceptor)
    std::string side;       //port or stbd, station 1 or 2

public:
    // Constructor that initializes the graph's position and size
    DoubleSidedHorizontalBarGraph(std::string side, std::string name, int32_t x_location, int32_t y_location, int orientation);

    // Default constructor
    DoubleSidedHorizontalBarGraph();

    // Method to draw the bar graph on the screen
    void draw(int position) const;
};

class SingleSidedVerticalBarGraph {
private:
    std::string name;
    std::string side;
    int32_t x_location;  // X coordinate
    int32_t y_location;  // Y coordinate
    bool flipped;        // orientation
    uint32_t m_eidBar;

public:
    // Constructor that initializes the graph's position and size
    SingleSidedVerticalBarGraph(std::string side, std::string name, int32_t x_location, int32_t y_location, bool flipped);

    // Default constructor
    SingleSidedVerticalBarGraph();

    // Method to draw the bar graph on the screen
    void draw(int position) const;
};

class SingleSidedHorizontalBarGraph {
private:
    int32_t x_location;  // X coordinate
    int32_t y_location;  // Y coordinate
    bool flipped;        // orientation, 0 = centered, 1 = left, 2 = right
    uint32_t m_eidBar;

public:
    // Constructor that initializes the graph's position and size
    SingleSidedHorizontalBarGraph(int32_t x_location, int32_t y_location, bool flipped);

    // Default constructor
    SingleSidedHorizontalBarGraph();

    // Method to draw the bar graph on the screen
    void draw(int position) const;
};

#endif