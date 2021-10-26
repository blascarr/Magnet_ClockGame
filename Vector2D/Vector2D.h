#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

class Vector2D
{
  public:
  Vector2D();
  Vector2D(int PosX, int PosY);
  ~Vector2D();

  float Magnitude();
  float Angle();
  float MagnitudeSquared();
  void Normalize();

  Vector2D operator*(const Vector2D &rhs);
  Vector2D operator*(const float rhs);
  Vector2D operator*=(const Vector2D &rhs);
  Vector2D operator+(const Vector2D &rhs);
  Vector2D operator+=(const Vector2D &rhs);
  Vector2D operator-(const Vector2D &rhs);
  Vector2D operator-=(const Vector2D &rhs);
  float Dot( Vector2D b );
  Vector2D Rotate( float angle );
  
  float x;
  float y;
};

static float Deg2Rad( float Deg );
static float Rad2Deg( float rad );

#endif