#include "pathfinder.h"
#include <cmath>

Pathfinder::Pathfinder(Grid& grid) 
    : grid(grid), pathFound(false), finished(false), 
      visitedCount(0), pathLength(0) {
}

void Pathfinder::init() {
    start = grid.getStart();
    end = grid.getEnd();
    pathFound = false;
    finished = false;
    visitedCount = 0;
    pathLength = 0;
    
    while (!openSet.empty()) {
        openSet.pop();
    }
    
    grid.clearPath();
}

int Pathfinder::heuristic(int x1, int y1, int x2, int y2) const {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

void Pathfinder::reconstructPath() {
    int x = end.first;
    int y = end.second;
    
    pathLength = 0;
    
    while (x != start.first || y != start.second) {
        Cell& cell = grid.getCellRef(x, y);
        
        if (cell.type != CellType::START && cell.type != CellType::END) {
            cell.type = CellType::PATH;
        }
        
        int px = cell.parentX;
        int py = cell.parentY;
        
        if (px == -1 && py == -1) break;
        
        x = px;
        y = py;
        pathLength++;
    }
}

AStarPathfinder::AStarPathfinder(Grid& grid) : Pathfinder(grid) {}

void AStarPathfinder::init() {
    Pathfinder::init();
    
    Cell& startCell = grid.getCellRef(start.first, start.second);
    startCell.gCost = 0;
    startCell.hCost = heuristic(start.first, start.second, end.first, end.second);
    startCell.fCost = startCell.gCost + startCell.hCost;
    startCell.inOpenSet = true;
    
    openSet.push({startCell.fCost, start});
}

bool AStarPathfinder::step() {
    if (finished) return false;
    
    if (openSet.empty()) {
        finished = true;
        return false;
    }
    
    auto current = openSet.top();
    openSet.pop();
    
    int x = current.second.first;
    int y = current.second.second;
    
    Cell& currentCell = grid.getCellRef(x, y);
    
    if (currentCell.inClosedSet) {
        return true; 
    }
    
    currentCell.inClosedSet = true;
    currentCell.inOpenSet = false;
    visitedCount++;
    
    if (currentCell.type != CellType::START && currentCell.type != CellType::END) {
        currentCell.type = CellType::VISITED;
    }
    
    if (x == end.first && y == end.second) {
        pathFound = true;
        finished = true;
        reconstructPath();
        return false;
    }
    
    auto neighbors = grid.getNeighbors(x, y);
    for (const auto& neighbor : neighbors) {
        int nx = neighbor.first;
        int ny = neighbor.second;
        
        Cell& neighborCell = grid.getCellRef(nx, ny);
        
        if (neighborCell.inClosedSet) continue;
        
        int tentativeG = currentCell.gCost + 1;
        
        if (!neighborCell.inOpenSet || tentativeG < neighborCell.gCost) {
            neighborCell.parentX = x;
            neighborCell.parentY = y;
            neighborCell.gCost = tentativeG;
            neighborCell.hCost = heuristic(nx, ny, end.first, end.second);
            neighborCell.fCost = neighborCell.gCost + neighborCell.hCost;
            
            if (!neighborCell.inOpenSet) {
                neighborCell.inOpenSet = true;
                openSet.push({neighborCell.fCost, neighbor});
                
                if (neighborCell.type != CellType::END) {
                    neighborCell.type = CellType::FRONTIER;
                }
            }
        }
    }
    
    return true;
}

GreedyPathfinder::GreedyPathfinder(Grid& grid) : Pathfinder(grid) {}

void GreedyPathfinder::init() {
    Pathfinder::init();
    
    Cell& startCell = grid.getCellRef(start.first, start.second);
    startCell.hCost = heuristic(start.first, start.second, end.first, end.second);
    startCell.inOpenSet = true;
    
    openSet.push({startCell.hCost, start});
}

bool GreedyPathfinder::step() {
    if (finished) return false;
    
    if (openSet.empty()) {
        finished = true;
        return false;
    }
    
    auto current = openSet.top();
    openSet.pop();
    
    int x = current.second.first;
    int y = current.second.second;
    
    Cell& currentCell = grid.getCellRef(x, y);
    
    if (currentCell.inClosedSet) {
        return true;
    }
    
    currentCell.inClosedSet = true;
    currentCell.inOpenSet = false;
    visitedCount++;
    
    if (currentCell.type != CellType::START && currentCell.type != CellType::END) {
        currentCell.type = CellType::VISITED;
    }
    
    if (x == end.first && y == end.second) {
        pathFound = true;
        finished = true;
        reconstructPath();
        return false;
    }
    
    auto neighbors = grid.getNeighbors(x, y);
    for (const auto& neighbor : neighbors) {
        int nx = neighbor.first;
        int ny = neighbor.second;
        
        Cell& neighborCell = grid.getCellRef(nx, ny);
        
        if (neighborCell.inClosedSet || neighborCell.inOpenSet) continue;
        
        neighborCell.parentX = x;
        neighborCell.parentY = y;
        neighborCell.hCost = heuristic(nx, ny, end.first, end.second);
        neighborCell.inOpenSet = true;
        
        openSet.push({neighborCell.hCost, neighbor});
        
        if (neighborCell.type != CellType::END) {
            neighborCell.type = CellType::FRONTIER;
        }
    }
    
    return true;
}
