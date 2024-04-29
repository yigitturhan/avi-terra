#include "helper.h"
#include <cmath>
#include <iostream>
double helper::round(double val) {
    return ((double)((int)(val*1000)))/ 1000; //used round in order to escape floating point miscalculations
}
void helper::new_velocity(Velocity &velocity, double deceleration, double timestep) {
    double theta = get_theta(velocity);
    double nx = 0, ny = 0;
    if (abs(velocity.x) > deceleration* timestep){
        nx = velocity.x > 0 ? velocity.x - deceleration*timestep*abs(cos(theta)) : velocity.x + deceleration*timestep* abs(cos(theta));
    }
    if (abs(velocity.y) > deceleration*timestep){
        ny = velocity.y > 0 ? velocity.y - deceleration*timestep* abs(sin(theta)) : velocity.y + deceleration*timestep*abs(sin(theta));
    }
    velocity.x = nx, velocity.y = ny;
}
Position* helper::new_position(Position &position, Velocity &average_velocity, double timestep) {
    auto* new_pos = new Position();
    new_pos->x = round(average_velocity.x*timestep + position.x);
    new_pos->y = round(average_velocity.y*timestep + position.y);
    return new_pos;
}
Velocity* helper::average_velocity(Velocity &velocity, double deceleration, double timestep) {
    auto* average_velocity = new Velocity();
    average_velocity->x = velocity.x > 0 ? round(velocity.x-(timestep*deceleration/2)) : round(velocity.x+(timestep*deceleration/2));
    average_velocity->y = velocity.y > 0 ? round(velocity.y-(timestep*deceleration/2)) : round(velocity.y+(timestep*deceleration/2));
    return average_velocity;
}
Collision_side helper::collision_with_table(Position &position, Table &table) {
    bool l = position.x <= table.data.radius, r = position.x + table.data.radius >= table.width;
    bool t = position.y + table.data.radius>= table.height , b = position.y <= table.data.radius;
    if (l && b) return Bottom_Left;
    if (l && t) return Top_Left;
    if (r && b) return Bottom_Right;
    if (r && t) return Top_Right;
    if (r) return Right;
    if (l) return Left;
    if (t) return  Top;
    if (b) return Bottom;
    return None;
}
void helper::position_after_collision_with_table(Position &position, Collision_side &collisionSide, double delta_x,
                                                 double delta_y, Table &table) {
    switch (collisionSide) {
        case Left:
            position.x = table.data.radius*2 + abs(delta_x) - position.x;
            position.y += delta_y;
            break;
        case Right:
            position.x = 2*table.width - table.data.radius*2 - abs(delta_x) - position.x;
            position.y += delta_y;
            break;
        case Top:
            position.y = 2*table.height - table.data.radius*2 - abs(delta_y) - position.y;
            position.x += delta_x;
            break;
        case Bottom:
            position.y = table.data.radius*2 + abs(delta_y) - position.y;
            position.x += delta_x;
            break;
        case Top_Left:
            position.x = table.data.radius*2 + abs(delta_x) - position.x;
            position.y = 2*table.height - table.data.radius*2 - abs(delta_y) - position.y;
            break;
        case Top_Right:
            position.y = 2*table.height - table.data.radius*2 - abs(delta_y) - position.y;
            position.x = 2*table.width - table.data.radius *2- abs(delta_x) - position.x;
            break;
        case Bottom_Right:
            position.y = 2*table.data.radius + abs(delta_y) - position.y;
            position.x = 2*table.width - 2*table.data.radius - abs(delta_x) - position.x;
            break;
        case Bottom_Left:
            position.y = table.data.radius*2 + abs(delta_y) - position.y;
            position.x =2*table.data.radius + abs(delta_x) - position.x;
            break;
        case None:
            break;
    }
    position.y = round(position.y), position.x = round(position.x);
}
void helper::velocity_after_collision_with_table(Velocity &velocity, Collision_side &collisionSide) {
    if (collisionSide == Right || collisionSide == Left) velocity.x *= -1;
    else if (collisionSide == Top || collisionSide == Bottom) velocity.y *= -1;
    else{
        velocity.x *= -1;
        velocity.y *= -1;
    }
}
void helper::step(Ball &ball, Table &table, double timestep = 1) {
    if (ball.velocity.x == 0 && ball.velocity.y == 0) return;
    Velocity* average_vel = average_velocity(ball.velocity, table.data.deceleration, timestep);
    Position* new_pos = new_position(ball.position, *average_vel, timestep);
    Collision_side side = collision_with_table(*new_pos, table);
    if (side == None){
        new_velocity(ball.velocity, table.data.deceleration, timestep);
        ball.position.x = new_pos->x;
        ball.position.y = new_pos->y;
    }
    else{
        velocity_after_collision_with_table(ball.velocity, side);
        new_velocity(ball.velocity, table.data.deceleration, timestep);
        position_after_collision_with_table(ball.position, side, average_vel->x*timestep, average_vel->y*timestep, table);
    }
    delete new_pos;
    delete average_vel;
}
double helper::distance(Position &position1, Position &position2) {
    return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y,2));
}
double helper::get_contact_angle(Position &position1, Position &position2) {
    return atan2((position2.y - position1.y) , (position2.x - position1.x));
}
double helper::get_theta(Velocity &velocity) {
    return atan2(velocity.y , velocity.x);
}
void helper::adjust_velocity_after_collision(Ball &ball1, Ball &ball2, Table &table) {
    if (distance(ball1.position, ball2.position) > table.data.radius * 2) return;
    double phi = get_contact_angle(ball1.position, ball2.position);
    double theta1 = atan2(ball1.velocity.y, ball1.velocity.x);
    double theta2 = atan2(ball2.velocity.y, ball2.velocity.x);
    double v1 = sqrt(ball1.velocity.x * ball1.velocity.x + ball1.velocity.y * ball1.velocity.y);
    double v2 = sqrt(ball2.velocity.x * ball2.velocity.x + ball2.velocity.y * ball2.velocity.y);
    ball1.velocity.x = v2 * cos(theta2 - phi) * cos(phi) + v1 * sin(theta1 - phi) * cos(phi + (M_PI / 2));
    ball1.velocity.y = v2 * cos(theta2 - phi) * sin(phi) + v1 * sin(theta1 - phi) * sin(phi + (M_PI / 2));
    ball2.velocity.x = v1 * cos(theta1 - phi) * cos(phi) + v2 * sin(theta2 - phi) * cos(phi + (M_PI / 2));
    ball2.velocity.y = v1 * cos(theta1 - phi) * sin(phi) + v2 * sin(theta2 - phi) * sin(phi + (M_PI / 2));
}

