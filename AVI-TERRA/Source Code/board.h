#ifndef UNTITLED_BOARD_H
#define UNTITLED_BOARD_H
#include <vector>

class Position{
public:
    double x;
    double y;
};

class Velocity{
public:
    double x;
    double y;
};

class BallData{
public:
    double radius;
    double deceleration;
};

class Ball{
public:
    Position position;
    Velocity velocity;
};

class Table{
public:
    std::vector<Ball> balls;
    double height;
    double width;
    BallData data;
};

#endif //UNTITLED_BOARD_H
