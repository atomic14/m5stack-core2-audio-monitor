#pragma once

class M5Display;
class Component
{
public:
  int x, y, width, height;
  bool visible;
  Component(int x, int y, int width, int height) : x(x), y(y), width(width), height(height), visible(true)
  {
  }
  virtual void _draw(M5Display &display) = 0;
  void draw(M5Display &display)
  {
    if (visible)
    {
      _draw(display);
    }
  }
};