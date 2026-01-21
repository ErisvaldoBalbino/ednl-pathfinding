#ifndef GRID_H
#define GRID_H

#include "raylib.h"
#include <utility>
#include <vector>

enum class CellType { EMPTY, WALL, START, END, VISITED, FRONTIER, PATH };

struct Cell {
  CellType type = CellType::EMPTY;
  int gCost = 0;
  int hCost = 0;
  int fCost = 0;
  int parentX = -1;
  int parentY = -1;
  bool inOpenSet = false;
  bool inClosedSet = false;
};

class Grid {
public:
  Grid(int width, int height, int cellSize);
  void draw() const;
  void drawCell(int x, int y) const;
  void reset();
  void clearPath();
  void setCell(int x, int y, CellType type);
  CellType getCell(int x, int y) const;
  void clearWalls();
  std::pair<int, int> getGridPosition(int screenX, int screenY) const;
  std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;
  bool isValid(int x, int y) const;
  bool isWalkable(int x, int y) const;
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  int getCellSize() const { return cellSize; }
  int getOffsetX() const { return offsetX; }
  int getOffsetY() const { return offsetY; }
  std::pair<int, int> getStart() const { return start; }
  std::pair<int, int> getEnd() const { return end; }
  void setStart(int x, int y);
  void setEnd(int x, int y);
  Cell &getCellRef(int x, int y) { return cells[y][x]; }
  const Cell &getCellRef(int x, int y) const { return cells[y][x]; }

private:
  int width;
  int height;
  int cellSize;
  int offsetX;
  int offsetY;
  std::vector<std::vector<Cell>> cells;
  std::pair<int, int> start;
  std::pair<int, int> end;

  Color getCellColor(int x, int y) const;
  void recursiveDivision(int x, int y, int w, int h, bool horizontal);
};

#endif // GRID_H
