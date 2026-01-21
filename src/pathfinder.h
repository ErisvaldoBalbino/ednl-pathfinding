#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "grid.h"
#include <vector>
#include <queue>
#include <utility>

enum class AlgorithmType {
    ASTAR,
    GREEDY
};

class Pathfinder {
public:
    Pathfinder(Grid& grid);
    virtual ~Pathfinder() = default;
    
    virtual void init();
    
    virtual bool step() = 0;
    
    bool isPathFound() const { return pathFound; }
    
    bool isFinished() const { return finished; }
    
    int getVisitedCount() const { return visitedCount; }
    int getPathLength() const { return pathLength; }
    
    virtual const char* getName() const = 0;
    
protected:
    Grid& grid;
    std::pair<int, int> start;
    std::pair<int, int> end;
    bool pathFound;
    bool finished;
    int visitedCount;
    int pathLength;
    
    int heuristic(int x1, int y1, int x2, int y2) const;
    
    void reconstructPath();
    
    struct NodeCompare {
        bool operator()(const std::pair<int, std::pair<int, int>>& a,
                       const std::pair<int, std::pair<int, int>>& b) const {
            return a.first > b.first;
        }
    };
    
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                        std::vector<std::pair<int, std::pair<int, int>>>,
                        NodeCompare> openSet;
};

class AStarPathfinder : public Pathfinder {
public:
    AStarPathfinder(Grid& grid);
    void init() override;
    bool step() override;
    const char* getName() const override { return "A*"; }
};

class GreedyPathfinder : public Pathfinder {
public:
    GreedyPathfinder(Grid& grid);
    void init() override;
    bool step() override;
    const char* getName() const override { return "Guloso (Greedy)"; }
};

#endif // PATHFINDER_H
