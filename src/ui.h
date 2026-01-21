#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <string>

class Button {
public:
  Button(int x, int y, int width, int height, const char *text);

  void draw() const;
  bool isClicked() const;
  bool isHovered() const;

  void setEnabled(bool enabled) { this->enabled = enabled; }
  bool isEnabled() const { return enabled; }

private:
  Rectangle bounds;
  const char *text;
  bool enabled;
};

class UIPanel {
public:
  UIPanel(int x, int y, int width, int height);

  void draw() const;
  void drawTitle(const char *title) const;
  void drawText(const char *text, int offsetY) const;
  void drawStats(const char *algorithm, int visited, int pathLength,
                 bool found) const;
  void drawControls(int startY = -1) const;
  void drawLegend(int startY = -1) const;

private:
  int x, y, width, height;
};

class Slider {
public:
  Slider(int x, int y, int width, int minVal, int maxVal, int defaultVal);

  void draw() const;
  void update();
  int getValue() const { return value; }

private:
  Rectangle bounds;
  int minVal, maxVal, value;
  bool dragging;
};

#endif // UI_H
