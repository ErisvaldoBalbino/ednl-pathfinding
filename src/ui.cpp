#include "ui.h"
#include <cstring>

Button::Button(int x, int y, int width, int height, const char *text)
    : text(text), enabled(true) {
  bounds = {(float)x, (float)y, (float)width, (float)height};
}

void Button::draw() const {
  Color bgColor;
  Color textColor;

  if (!enabled) {
    bgColor = {100, 116, 139, 255};
    textColor = {148, 163, 184, 255};
  } else if (isHovered()) {
    bgColor = {99, 102, 241, 255};
    textColor = WHITE;
  } else {
    bgColor = {79, 70, 229, 255};
    textColor = WHITE;
  }

  DrawRectangleRounded(bounds, 0.3f, 8, bgColor);

  int textWidth = MeasureText(text, 16);
  int textX = bounds.x + (bounds.width - textWidth) / 2;
  int textY = bounds.y + (bounds.height - 16) / 2;

  DrawText(text, textX, textY, 16, textColor);
}

bool Button::isClicked() const {
  if (!enabled)
    return false;
  return CheckCollisionPointRec(GetMousePosition(), bounds) &&
         IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool Button::isHovered() const {
  return CheckCollisionPointRec(GetMousePosition(), bounds);
}

UIPanel::UIPanel(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

void UIPanel::draw() const {
  DrawRectangle(x, y, width, height, {15, 23, 42, 255});

  DrawRectangleLinesEx({(float)x, (float)y, (float)width, (float)height}, 2,
                       {51, 65, 85, 255});
}

void UIPanel::drawTitle(const char *title) const {
  DrawText(title, x + 15, y + 15, 24, WHITE);
  DrawLine(x + 15, y + 45, x + width - 15, y + 45, {51, 65, 85, 255});
}

void UIPanel::drawText(const char *text, int offsetY) const {
  DrawText(text, x + 15, y + offsetY, 16, {203, 213, 225, 255});
}

void UIPanel::drawStats(const char *algorithm, int visited, int pathLength,
                        bool found) const {
  int startY = y + 60;

  DrawText("Algoritmo:", x + 15, startY, 14, {148, 163, 184, 255});

  DrawText(algorithm, x + 15, startY + 18, 18, {34, 211, 238, 255});

  DrawText("Nos Visitados:", x + 15, startY + 50, 14, {148, 163, 184, 255});

  char visitedStr[32];
  snprintf(visitedStr, sizeof(visitedStr), "%d", visited);
  DrawText(visitedStr, x + 15, startY + 68, 18, {250, 204, 21, 255});

  DrawText("Tamanho do Caminho:", x + 15, startY + 100, 14,
           {148, 163, 184, 255});
  char pathStr[32];
  if (found) {
    snprintf(pathStr, sizeof(pathStr), "%d", pathLength);
  } else {
    snprintf(pathStr, sizeof(pathStr), visited > 0 ? "Buscando..." : "-");
  }
  DrawText(pathStr, x + 15, startY + 118, 18, {168, 85, 247, 255});
}

void UIPanel::drawControls(int startY) const {
  if (startY == -1) {
    startY = y + 485;
  }

  DrawText("Controles:", x + 15, startY, 14, WHITE);
  DrawLine(x + 15, startY + 18, x + width - 15, startY + 18, {51, 65, 85, 255});

  int lineHeight = 18;
  int cy = startY + 28;

  DrawText("Esq: Parede | Dir: Apagar", x + 15, cy, 11, {148, 163, 184, 255});
  cy += lineHeight;
  DrawText("R: Reiniciar grid", x + 15, cy, 11, {148, 163, 184, 255});
}

void UIPanel::drawLegend(int startY) const {
  if (startY == -1) {
    startY = y + 580;
  }

  int boxSize = 14;
  int lineHeight = 20;

  DrawText("Legenda:", x + 15, startY, 16, WHITE);
  DrawLine(x + 15, startY + 22, x + width - 15, startY + 22, {51, 65, 85, 255});

  int cy = startY + 35;

  DrawRectangle(x + 15, cy, boxSize, boxSize, {34, 197, 94, 255});
  DrawText("Inicio", x + 40, cy + 2, 14, {203, 213, 225, 255});
  cy += lineHeight;

  DrawRectangle(x + 15, cy, boxSize, boxSize, {239, 68, 68, 255});
  DrawText("Fim", x + 40, cy + 2, 14, {203, 213, 225, 255});
  cy += lineHeight;

  DrawRectangle(x + 15, cy, boxSize, boxSize, {51, 65, 85, 255});
  DrawText("Parede", x + 40, cy + 2, 14, {203, 213, 225, 255});
  cy += lineHeight;

  DrawRectangle(x + 15, cy, boxSize, boxSize, {96, 165, 250, 255});
  DrawText("Visitado", x + 40, cy + 2, 14, {203, 213, 225, 255});
  cy += lineHeight;

  DrawRectangle(x + 15, cy, boxSize, boxSize, {250, 204, 21, 255});
  DrawText("Fronteira", x + 40, cy + 2, 14, {203, 213, 225, 255});
  cy += lineHeight;

  DrawRectangle(x + 15, cy, boxSize, boxSize, {168, 85, 247, 255});
  DrawText("Caminho", x + 40, cy + 2, 14, {203, 213, 225, 255});
}

Slider::Slider(int x, int y, int width, int minVal, int maxVal, int defaultVal)
    : minVal(minVal), maxVal(maxVal), value(defaultVal), dragging(false) {
  bounds = {(float)x, (float)y, (float)width, 20.0f};
}

void Slider::draw() const {
  DrawRectangleRounded({bounds.x, bounds.y + 8, bounds.width, 4}, 0.5f, 4,
                       {51, 65, 85, 255});

  float ratio = (float)(value - minVal) / (float)(maxVal - minVal);
  float thumbX = bounds.x + ratio * (bounds.width - 16);

  DrawCircle(thumbX + 8, bounds.y + 10, 8, {99, 102, 241, 255});
}

void Slider::update() {
  Vector2 mouse = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      CheckCollisionPointRec(mouse, {bounds.x, bounds.y, bounds.width, 20})) {
    dragging = true;
  }

  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    dragging = false;
  }

  if (dragging) {
    float ratio = (mouse.x - bounds.x) / bounds.width;
    if (ratio < 0)
      ratio = 0;
    if (ratio > 1)
      ratio = 1;
    value = minVal + (int)(ratio * (maxVal - minVal));
  }
}
