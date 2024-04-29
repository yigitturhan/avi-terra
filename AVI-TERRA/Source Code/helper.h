#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H
#include "board.h"

enum Collision_side{
    None,
    Left,
    Right,
    Top,
    Bottom,
    Top_Left,
    Top_Right,
    Bottom_Left,
    Bottom_Right
};
class helper {
public:
    static void step(Ball &ball, Table &table, double timestep);
    static void adjust_velocity_after_collision(Ball &ball1, Ball &ball2, Table &table);
    static double distance(Position &position1, Position &position2);
private:
    static Collision_side collision_with_table(Position &position, Table &table);
    static Velocity* average_velocity(Velocity &velocity, double deceleration, double timestep);
    static void velocity_after_collision_with_table(Velocity &velocity, Collision_side &collisionSide);
    static void position_after_collision_with_table(Position &position, Collision_side &collisionSide, double delta_x, double delta_y, Table &table);
    static void new_velocity(Velocity &velocity, double deceleration, double timestep);
    static Position* new_position(Position &old_position, Velocity &average_velocity, double timestep);
    static double round(double val);

    static double get_contact_angle(Position &position1, Position &position2);
    static double get_theta(Velocity &velocity);
};


#endif
