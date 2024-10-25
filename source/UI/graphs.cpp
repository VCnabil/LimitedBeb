#include "project.h"
#include <iostream>


DoubleSidedVerticalBarGraph::DoubleSidedVerticalBarGraph(std::string side, std::string name, int32_t x_location, int32_t y_location, bool flipped)
    : side(side), name(name), x_location(x_location), y_location(y_location), flipped(flipped) {
    m_eidBar = ElementGetNewId();

}

DoubleSidedVerticalBarGraph::DoubleSidedVerticalBarGraph()
    : side(""), name(""), x_location(0), y_location(), flipped(false), m_eidBar(0) {};

void DoubleSidedVerticalBarGraph::draw(int position) const {

    if (flipped) {
        // Drawing the graph frame
        vlineEx(x_location + 12, y_location - 73, y_location + 77, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location + 12, y_location - 73, x_location - 32, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location + 12, y_location + 77, x_location - 32, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);

        SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 0);
        SimpleTextDraw(x_location - 25, y_location - 7, side.c_str(), BLACK, 100, LAYER_FRONT);
        SimpleTextDraw(x_location - 25, y_location + 8, name.c_str(), BLACK, 100, LAYER_FRONT);

        // Draw the initial bar at zero position
        outimagearea(x_location, y_location, &bar, 0, 0, 10, 4, 100, ALPHA_INVARIANT, LAYER_FRONT);

        // Handle positive positions (move downwards)
        if (position > 0) {
            for (int i = 0; i < position; i++) {
                outimagearea(x_location - (i * i / 3 * 1.001), y_location + 7 + (i * 7), &bar, 0, 0, 10 + (i * i / 3 * 1.001), 4, 100, ALPHA_INVARIANT, LAYER_FRONT);
            }
        }
        // Handle negative positions (move upwards)
        else if (position < 0) {
            for (int i = 0; i > position; i--) {
                outimagearea(x_location - (i * i / 3 * 1.001), y_location - 7 + (i * 7), &bar, 0, 0, 10 + (i * i / 3 * 1.001), 4, 100, ALPHA_INVARIANT, LAYER_FRONT);
            }
        }
    }
    else {
        // Similar logic for left-sided graph
        vlineEx(x_location - 2, y_location - 73, y_location + 77, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location - 2, y_location - 73, x_location + 42, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location - 2, y_location + 77, x_location + 42, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);

        SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
        SimpleTextDraw(x_location + 35, y_location - 7, side.c_str(), BLACK, 100, LAYER_FRONT);
        SimpleTextDraw(x_location + 35, y_location + 8, name.c_str(), BLACK, 100, LAYER_FRONT);

        // Draw the initial bar at zero position
        outimagearea(x_location, y_location, &bar, 0, 0, 10, 4, 100, ALPHA_INVARIANT, LAYER_FRONT);

        // Handle positive positions (move downwards)
        if (position > 0) {
            for (int i = 0; i < position; i++) {
                outimagearea(x_location, y_location + 7 + (i * 7), &bar, 0, 0, 10 + (i * i / 3 * 1.001), 4, 100, ALPHA_INVARIANT, LAYER_FRONT);
            }
        }
        // Handle negative positions (move upwards)
        else if (position < 0) {
            for (int i = 0; i > position; i--) {
                outimagearea(x_location, y_location - 7 + (i * 7), &bar, 0, 0, 10 + (i * i / 3 * 1.001), 4, 100, ALPHA_INVARIANT, LAYER_FRONT);
            }
        }
    }

    
}

DoubleSidedHorizontalBarGraph::DoubleSidedHorizontalBarGraph(std::string side, std::string name, int32_t x_location, int32_t y_location, int orientation)
    : side(side), name(name), x_location(x_location), y_location(y_location), orientation(orientation) {
    m_eidBar = ElementGetNewId();
}

DoubleSidedHorizontalBarGraph::DoubleSidedHorizontalBarGraph()
    : side(""), name(""), x_location(0), y_location(), orientation(0), m_eidBar(0) {};

void DoubleSidedHorizontalBarGraph::draw(int position) const {
    outimagearea(x_location, y_location, &bar, 0, 0, 3, 10, 100, ALPHA_INVARIANT, LAYER_FRONT);

    // Handle positive positions (move right)
    if (position > 0) {
        for (int i = 0; i < position; i++) {
            outimagearea(x_location + 6 + (i * 6), y_location - (i * i / 3 * 1.0001), &bar, 0, 0, 3, 10 + (i * i / 3 * 1.0005), 100, ALPHA_INVARIANT, LAYER_FRONT);
        }
    }
    // Handle negative positions (move left)
    else if (position < 0) {
        for (int i = 0; i > position; i--) {
            outimagearea(x_location - 6 + (i * 6), y_location - (i * i / 3 * 1.0001), &bar, 0, 0, 3, 10 + (i * i / 3 * 1.0005), 100, ALPHA_INVARIANT, LAYER_FRONT);
        }
    }

    SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 0);
    if (orientation == 0) {
        SimpleTextDraw(x_location, y_location - 40, side.c_str(), BLACK, 100, LAYER_FRONT);
        SimpleTextDraw(x_location, y_location - 25, name.c_str(), BLACK, 100, LAYER_FRONT);
    }
    else if (orientation == 1) {

    }
    else {

    }

    hlineEx(x_location - 63, y_location + 12, x_location + 66, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
    vlineEx(x_location - 63, y_location + 12, y_location - 32, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
    vlineEx(x_location + 66, y_location + 12, y_location - 32, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
}

SingleSidedVerticalBarGraph::SingleSidedVerticalBarGraph(std::string side, std::string name, int32_t x_location, int32_t y_location, bool flipped)
    : side(side), name(name), x_location(x_location), y_location(y_location), flipped(flipped) {
    m_eidBar = ElementGetNewId();
}

SingleSidedVerticalBarGraph::SingleSidedVerticalBarGraph()
    : x_location(0), y_location(0), flipped(false), m_eidBar(0) {};

void SingleSidedVerticalBarGraph::draw(int position) const {
    outimagearea(x_location, y_location, &bar, 0, 0, 10, 4, 100, ALPHA_INVARIANT, LAYER_FRONT);

    if (flipped) {
        for (int i = 0; i < position; i++) {
            outimagearea(x_location, y_location + 7 + (i * 7), &bar, 0, 0, 10 + (i * i / 8 * 1.001), 4, 100, ALPHA_INVARIANT, LAYER_FRONT);
        }
        vlineEx(x_location - 2, y_location - 3, y_location + 147, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location - 2, y_location - 3, x_location + 58, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location - 2, y_location + 147, x_location + 58, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);

        SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
        SimpleTextDraw(x_location + 35, y_location - 3, side.c_str(), BLACK, 100, LAYER_FRONT);
        SimpleTextDraw(x_location + 35, y_location + 12, name.c_str(), BLACK, 100, LAYER_FRONT);
    }
    else {
        for (int i = 0; i < position; i++) {
            outimagearea(x_location - (i * i / 8 * 1.001), y_location + 7 + (i * 7), &bar, 0, 0, 10 + (i * i / 8 * 1.001), 4, 100, ALPHA_INVARIANT, LAYER_FRONT);
        }

        vlineEx(x_location + 12, y_location - 3, y_location + 147, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location + 12, y_location - 3, x_location - 48, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
        hlineEx(x_location + 12, y_location + 147, x_location - 48, NORMAL, SOLID, BLACK, 100, LAYER_FRONT);

        SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
        SimpleTextDraw(x_location - 47, y_location - 3, side.c_str(), BLACK, 100, LAYER_FRONT);
        SimpleTextDraw(x_location - 47, y_location + 12, name.c_str(), BLACK, 100, LAYER_FRONT);
    }
    
}

SingleSidedHorizontalBarGraph::SingleSidedHorizontalBarGraph(int32_t x_location, int32_t y_location, bool flipped)
    : x_location(x_location), y_location(y_location), flipped(flipped) {
    m_eidBar = ElementGetNewId();
}

SingleSidedHorizontalBarGraph::SingleSidedHorizontalBarGraph()
    : x_location(0), y_location(0), flipped(false), m_eidBar(0) {};

void SingleSidedHorizontalBarGraph::draw(int position) const {
    outimagearea(x_location, y_location, &bar, 0, 0, 5, 10, 100, ALPHA_INVARIANT, LAYER_FRONT);

    for (int i = 0; i < position; i++) {
        outimagearea(x_location + 8 + (i * 8), y_location - (i * i / 8 * 1.0001), &bar, 0, 0, 5, 10 + (i * i / 8 * 1.0005), 100, ALPHA_INVARIANT, LAYER_FRONT);
    }
}