#include "grid.h"
#include "pathfinder.h"
#include "raylib.h"
#include "ui.h"
#include <memory>

enum class AppState {
  EDITING,
  RUNNING,
  FINISHED
};

int main() {
  const int screenWidth = 1200;
  const int screenHeight = 720;

  const int gridWidth = 45;
  const int gridHeight = 32;
  const int cellSize = 20;

  InitWindow(screenWidth, screenHeight, "Pathfinding");
  SetTargetFPS(60);

  Grid grid(gridWidth, gridHeight, cellSize);

  std::unique_ptr<Pathfinder> pathfinder =
      std::make_unique<AStarPathfinder>(grid);
  AlgorithmType currentAlgorithm = AlgorithmType::ASTAR;
  AppState state = AppState::EDITING;

  const int PANEL_X = 10;
  const int PANEL_Y = 10;
  const int PANEL_WIDTH = 220;
  const int PANEL_HEIGHT = 700;
  const int UI_PADDING = 15;

  const int STATS_END_Y =
      60 + 140; 
  const int BUTTON_START_Y = STATS_END_Y + 40; 
  const int BUTTON_SPACING = 10;
  const int BUTTON_HEIGHT = 35;

  int btnY = BUTTON_START_Y;
  int btn1Y = btnY;
  int btn2Y = btnY + BUTTON_HEIGHT + BUTTON_SPACING;
  int btn3Y = btnY + (BUTTON_HEIGHT + BUTTON_SPACING) * 2;

  int sliderY = btn3Y + BUTTON_HEIGHT + 30;

  UIPanel panel(PANEL_X, PANEL_Y, PANEL_WIDTH, PANEL_HEIGHT);

  Button btnStart(PANEL_X + UI_PADDING, btn1Y, 190, BUTTON_HEIGHT, "Iniciar");
  Button btnClear(PANEL_X + UI_PADDING, btn2Y, 190, BUTTON_HEIGHT,
                  "Limpar Grid");
  Button btnAlgorithm(PANEL_X + UI_PADDING, btn3Y, 190, BUTTON_HEIGHT,
                      "Trocar Algoritmo");

  Slider speedSlider(PANEL_X + UI_PADDING, sliderY, 190, 1, 50, 10);

  int controlsY = sliderY + 50;
  int legendY = controlsY + 80;

  int stepsPerFrame = 10;

  while (!WindowShouldClose()) {

    speedSlider.update();
    stepsPerFrame = speedSlider.getValue();

    Vector2 mouse = GetMousePosition();
    auto [gridX, gridY] = grid.getGridPosition((int)mouse.x, (int)mouse.y);
    bool mouseInGrid =
        grid.isValid(gridX, gridY) && mouse.x > grid.getOffsetX();

    if (state == AppState::EDITING) {
      if (mouseInGrid) {
        if (IsKeyDown(KEY_LEFT_SHIFT) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          grid.setStart(gridX, gridY);
        } else if (IsKeyDown(KEY_LEFT_CONTROL) &&
                   IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          grid.setEnd(gridX, gridY);
        } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
          grid.setCell(gridX, gridY, CellType::WALL);
        } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
          grid.setCell(gridX, gridY, CellType::EMPTY);
        }
      }
    }

    if (btnStart.isClicked()) {
      if (state == AppState::EDITING) {
        pathfinder->init();
        state = AppState::RUNNING;
      } else if (state == AppState::FINISHED) {
        grid.clearPath();
        state = AppState::EDITING;
      }
    }

    if (btnClear.isClicked() && state == AppState::EDITING) {
      grid.clearWalls();
    }

    if (btnAlgorithm.isClicked() &&
        (state == AppState::EDITING || state == AppState::FINISHED)) {
      if (currentAlgorithm == AlgorithmType::ASTAR) {
        currentAlgorithm = AlgorithmType::GREEDY;
        pathfinder = std::make_unique<GreedyPathfinder>(grid);
      } else {
        currentAlgorithm = AlgorithmType::ASTAR;
        pathfinder = std::make_unique<AStarPathfinder>(grid);
      }
      grid.clearPath();
      state = AppState::EDITING;
    }

    if (IsKeyPressed(KEY_R)) {
      grid.clearPath();
      state = AppState::EDITING;
    }

    if (state == AppState::RUNNING) {
      for (int i = 0; i < stepsPerFrame; i++) {
        if (!pathfinder->step()) {
          state = AppState::FINISHED;
          break;
        }
      }
    }

    btnClear.setEnabled(state == AppState::EDITING);
    btnAlgorithm.setEnabled(state == AppState::EDITING ||
                            state == AppState::FINISHED);

    BeginDrawing();

    ClearBackground({30, 41, 59, 255});

    grid.draw();

    panel.draw();
    panel.drawTitle("Pathfinding");
    panel.drawStats(pathfinder->getName(), pathfinder->getVisitedCount(),
                    pathfinder->getPathLength(), pathfinder->isPathFound());
    panel.drawControls(controlsY);
    panel.drawLegend(legendY);

    DrawText("Velocidade:", PANEL_X + UI_PADDING, sliderY - 20, 14,
             {148, 163, 184, 255});
    speedSlider.draw();

    btnStart.draw();
    btnClear.draw();
    btnAlgorithm.draw();

    const char *startText = "Iniciar";
    if (state == AppState::RUNNING)
      startText = "Executando...";
    else if (state == AppState::FINISHED)
      startText = "Reiniciar";

    Button btnStartDynamic(PANEL_X + UI_PADDING, btn1Y, 190, BUTTON_HEIGHT,
                           startText);
    btnStartDynamic.draw();

    const char *statusMsg = "";
    Color statusColor = WHITE;

    switch (state) {
    case AppState::EDITING:
      statusMsg = "Desenhe as paredes";
      statusColor = {148, 163, 184, 255};
      break;
    case AppState::RUNNING:
      statusMsg = "Buscando...";
      statusColor = {250, 204, 21, 255};
      break;
    case AppState::FINISHED:
      if (pathfinder->isPathFound()) {
        statusMsg = "Caminho encontrado!";
        statusColor = {34, 197, 94, 255};
      } else {
        statusMsg = "Sem caminho!";
        statusColor = {239, 68, 68, 255};
      }
      break;
    }

    DrawText(statusMsg, 25, 210, 14, statusColor);

    if (mouseInGrid && state == AppState::EDITING) {
      int screenX = grid.getOffsetX() + gridX * cellSize;
      int screenY = grid.getOffsetY() + gridY * cellSize;
      DrawRectangleLines(screenX, screenY, cellSize - 1, cellSize - 1, WHITE);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
