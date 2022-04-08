#include "../../include/objects/SoftBall.hpp"

#include <iostream>


SoftBall::SoftBall(const Vector2d& StartPos, const Vector2d& StartSpeed, float BodyRad,   \
                   int BallCount, int BallRad, float ballDensity,const sf::Color& clr)
{
    Rad = BodyRad;

    float tempx, tempy;

    for(float angle = 0; angle < 360; angle += 360 / BallCount){
        tempx = Rad * cos(angle * M_PI / 180);
        tempy = Rad * sin(angle * M_PI / 180);

        Balls.push_back(new Ball(StartPos + Vector2d(tempx, tempy),StartSpeed, BallRad, ballDensity, clr));
    }

    for(unsigned i =0; i < Balls.size(); i++){
        if(i == Balls.size() - 1)
            Springs.push_back(new Spring(Balls[i], Balls[0]));
        else
            Springs.push_back(new Spring(Balls[i], Balls[i + 1]));
    }

    NumOfBalls = Balls.size();
}


void SoftBall::ApplyEqautionOfIdealGas()
{
    unsigned s = Balls.size() / 2;
    Vector2d center;
    for(unsigned i = 0; i < s; i++)
        center += 0.5 * (Balls[i]->getPos() + Balls[i + s]->getPos());
    center = {center.x / s, center.y / s};

    Vector2d normal, F, pos;
    float Area = CalculateArea() / 20000;

    for(unsigned i = 0; i < s * 2; i++){
        pos = Balls[i]->getPos();
        normal = (pos - center).normalize();
        F = nRT / Area * normal;
        Balls[i]->AddForce(F);
    }
}


void SoftBall::InnerForces()
{
    ApplyEqautionOfIdealGas();

    for(auto& spring: Springs){
        spring->ApplyHookesForce();
        spring->ApplyDampingFactor();
        spring->ApplySelfCollision();
    }
}


float SoftBall::CalculateArea() const
{
    float sum = 0;
    Vector2d pos1;
    Vector2d pos2;

    int j = Balls.size() - 1;
    for(unsigned i = 0; i < Balls.size(); i++){
        pos1 = Balls[j]->getPos();
        pos2 = Balls[i]->getPos();

        sum += (pos1.x + pos2.x) * (pos1.y - pos2.y);
        j = i;
    }
    return 0.5 * fabs(sum);
}


Vector2d SoftBall::getMaxPoint()
{
    Vector2d maxPoint;
    maxPoint = Balls[0]->getPos();
    for(unsigned i = 1; i < Balls.size(); i++){
        if(maxPoint.x < Balls[i]->getPos().x)
            maxPoint.x = Balls[i]->getPos().x;
        if(maxPoint.y < Balls[i]->getPos().y)
            maxPoint.y = Balls[i]->getPos().y;
    }
    return maxPoint;
}


Vector2d SoftBall::getMinPoint()
{
    Vector2d minPoint;
    minPoint = Balls[0]->getPos();
    for(unsigned i = 1; i < Balls.size(); i++){
        if(minPoint.x > Balls[i]->getPos().x)
            minPoint.x = Balls[i]->getPos().x;
        if(minPoint.y > Balls[i]->getPos().y)
            minPoint.y = Balls[i]->getPos().y;
    }
    return minPoint;
}
