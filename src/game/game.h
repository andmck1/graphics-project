#ifndef GAME_H
#define GAME_H

#include "../vertices/vertices.h"
#include "../camera/camera.h"

class GameState
{
  private:
    double deltaTime;

  public:
    Shape shape;
    Camera camera;

    static GameState &Get();
    GameState();

    double getDeltaTime() const;
    void setDeltaTime(double deltaTime);

    void moveAndUpdate(std::array<float, 3> moveCoordinates);
    void rotateAndUpdate(std::array<float, 3> rotateAmount);

    void moveCameraAndUpdate(XMVECTOR moveCoordinates);
    void rotateCameraAndUpdate(const float mouseDx, const float mouseDy);
};
#endif
