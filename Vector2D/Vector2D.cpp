#include "Vector2D.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
//#define Deg2Rad( Deg ) { Deg * PI / 180 };
#include <math.h> 
Vector2D::Vector2D()
{
  x = 0;
  y = 0;
}

Vector2D::Vector2D( int PosX, int PosY )
{
  x = PosX;
  y = PosY;
}

Vector2D::~Vector2D()
{
}

float Vector2D::Magnitude()
{
  return sqrt( MagnitudeSquared() );
}

float Vector2D::Angle()
{
  return Rad2Deg( atan2( y, x ) );
}

float Vector2D::MagnitudeSquared()
{
  return ( x * x + y * y );
}

void Vector2D::Normalize()
{
  float mag = this->Magnitude();
  x = x / mag;
  y = y / mag;
}

Vector2D Vector2D::operator*( const Vector2D &rhs )
{
  Vector2D result;
  result.x = x * rhs.x;
  result.y = y * rhs.y;
  return result;
}

Vector2D Vector2D::operator*( const float rhs )
{
  Vector2D result;
  result.x = x * rhs;
  result.y = y * rhs;
  return result;
}

Vector2D Vector2D::operator*=( const Vector2D &rhs )
{
  x = x * rhs.x;
  y = y * rhs.y;
}

Vector2D Vector2D::operator+( const Vector2D &rhs )
{
  Vector2D result;
  result.x = x + rhs.x;
  result.y = y + rhs.y;
  return result;
}

Vector2D Vector2D::operator+=( const Vector2D &rhs )
{
  x = x + rhs.x;
  y = y + rhs.y;
}

Vector2D Vector2D::operator-( const Vector2D &rhs )
{
  Vector2D result;
  result.x = x - rhs.x;
  result.y = y - rhs.y;
  return result;
}

Vector2D Vector2D::operator-=( const Vector2D &rhs )
{
  x = x - rhs.x;
  y = y - rhs.y;
}

float Vector2D::Dot( Vector2D b )
{
  return ( x * b.x + y * b.y );
}

Vector2D Vector2D::Rotate( float angle )
{
  float radian = Deg2Rad(angle);
  float cs = cos( radian );
  float sn = sin( radian );

  float px = x * cs - y * sn;
  float py = x * sn + y * cs;

  x = px;
  y = py;
}

float Deg2Rad( float Deg )
{
  return ( Deg * PI / 180 );
}

float Rad2Deg ( float rad )
{
  return ( rad * 180 / PI );
}
