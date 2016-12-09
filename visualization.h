#pragma once
#include <FastLED.h>
#include "input.h"

namespace pixelmap {

/* A Visualization is an interface to an n-dimensional set of colors to be used
   in Animations.  Colors are stored as FastLED's CHSV class to make it easier
   for animations to map the colors to a set of LEDs */
class Visualization {
  public:
    Visualization(Input* input, int size = 1)
      : input(input),
        size_(size)
    {
      viz_ = new CRGB[size];
    };
    ~Visualization() { delete[] viz_; }

    virtual void update()=0;
    int getSize() { return size_; };
    CRGB getColorByRatio(float ratio);
    CRGB getColorByIndex(float index);
    CRGB getColorByIndex(int index);
    Input* input;
  protected:
    CRGB* viz_;
    int size_;
};

class HueVisualization : public Visualization {
  public:
    HueVisualization(Input* input, int size = 1);
    void update();
};

class NullVisualization : public Visualization {
  public:
    NullVisualization(Input* input, int size = 1) :
      Visualization(input, size) {}
    void update() {};
};

} // end namespace pixelmap
