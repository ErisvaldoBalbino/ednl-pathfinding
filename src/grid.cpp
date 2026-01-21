#include "grid.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>


Grid::Grid(int width, int height, int cellSize)
    : width(width), height(height), cellSize(cellSize) {

  offsetX = 250;
  offsetY = 50;

  cells.resize(height, std::vector<Cell>(width));

  start = {1, 1};
  end = {width - 2, height - 2};

  cells[start.second][start.first].type = CellType::START;
  cells[end.second][end.first].type = CellType::END;

  srand(static_cast<unsigned>(time(nullptr)));
}

void Grid::draw() const {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      drawCell(x, y);
    }
  }

  DrawRectangleLines(offsetX, offsetY, width * cellSize, height * cellSize,
                     DARKGRAY);
}

void Grid::drawCell(int x, int y) const {
  int screenX = offsetX + x * cellSize;
  int screenY = offsetY + y * cellSize;

  Color color = getCellColor(x, y);
  DrawRectangle(screenX, screenY, cellSize - 1, cellSize - 1, color);
}

Color Grid::getCellColor(int x, int y) const {
  const Cell &cell = cells[y][x];

  if (cell.type == CellType::START)
    return {34, 197, 94, 255};
  if (cell.type == CellType::END)
    return {239, 68, 68, 255};
  if (cell.type == CellType::PATH)
    return {168, 85, 247, 255};
  if (cell.type == CellType::FRONTIER)
    return {250, 204, 21, 255};
  if (cell.type == CellType::VISITED)
    return {96, 165, 250, 255};
  if (cell.type == CellType::WALL)
    return {51, 65, 85, 255};

  return {241, 245, 249, 255};
}

void Grid::reset() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cells[y][x] = Cell();
    }
  }

  start = {1, 1};
  end = {width - 2, height - 2};
  cells[start.second][start.first].type = CellType::START;
  cells[end.second][end.first].type = CellType::END;
}

void Grid::clearPath() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Cell &cell = cells[y][x];
      if (cell.type == CellType::VISITED || cell.type == CellType::FRONTIER ||
          cell.type == CellType::PATH) {
        cell.type = CellType::EMPTY;
      }
      cell.gCost = 0;
      cell.hCost = 0;
      cell.fCost = 0;
      cell.parentX = -1;
      cell.parentY = -1;
      cell.inOpenSet = false;
      cell.inClosedSet = false;
    }
  }

  cells[start.second][start.first].type = CellType::START;
  cells[end.second][end.first].type = CellType::END;
}

void Grid::setCell(int x, int y, CellType type) {
  if (isValid(x, y)) {
    if (cells[y][x].type != CellType::START &&
        cells[y][x].type != CellType::END) {
      cells[y][x].type = type;
    }
  }
}

CellType Grid::getCell(int x, int y) const {
  if (isValid(x, y)) {
    return cells[y][x].type;
  }
  return CellType::WALL;
}

std::pair<int, int> Grid::getGridPosition(int screenX, int screenY) const {
  int gridX = (screenX - offsetX) / cellSize;
  int gridY = (screenY - offsetY) / cellSize;
  return {gridX, gridY};
}

std::vector<std::pair<int, int>> Grid::getNeighbors(int x, int y) const {
  std::vector<std::pair<int, int>> neighbors;

  const int dx[] = {0, 0, -1, 1};
  const int dy[] = {-1, 1, 0, 0};

  for (int i = 0; i < 4; i++) {
    int nx = x + dx[i];
    int ny = y + dy[i];

    if (isValid(nx, ny) && isWalkable(nx, ny)) {
      neighbors.push_back({nx, ny});
    }
  }

  return neighbors;
}

bool Grid::isValid(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

bool Grid::isWalkable(int x, int y) const {
  if (!isValid(x, y))
    return false;
  CellType type = cells[y][x].type;
  return type != CellType::WALL;
}

void Grid::setStart(int x, int y) {
  if (isValid(x, y) && cells[y][x].type != CellType::END &&
      cells[y][x].type != CellType::WALL) {
    cells[start.second][start.first].type = CellType::EMPTY;
    start = {x, y};
    cells[y][x].type = CellType::START;
  }
}

void Grid::setEnd(int x, int y) {
  if (isValid(x, y) && cells[y][x].type != CellType::START &&
      cells[y][x].type != CellType::WALL) {
    cells[end.second][end.first].type = CellType::EMPTY;
    end = {x, y};
    cells[y][x].type = CellType::END;
  }
}

void Grid::clearWalls() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Cell &cell = cells[y][x];
      if (cell.type == CellType::WALL) {
        cell.type = CellType::EMPTY;
      }
    }
  }

  cells[start.second][start.first].type = CellType::START;
  cells[end.second][end.first].type = CellType::END;
}
