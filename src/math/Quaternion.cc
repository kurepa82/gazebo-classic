/*
 * Copyright 2011 Nate Koenig & Andrew Howard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
/* Desc: External interfaces for Gazebo
 * Author: Nate Koenig
 * Date: 03 Apr 2007
 */
#include <math.h>
#include "math/Helpers.hh"
#include "math/Quaternion.hh"

using namespace gazebo;
using namespace math;


////////////////////////////////////////////////////////////////////////////////
// Default Constructor
Quaternion::Quaternion()
    : w(1), x(0), y(0), z(0)
{
}

////////////////////////////////////////////////////////////////////////////////
// Constructor
Quaternion::Quaternion( const double &w, const double &x, const double &y, const double &z)
    : w(w), x(x), y(y), z(z)
{
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor
Quaternion::Quaternion( const double &_roll, const double &_pitch, const double &_yaw )
{
  this->SetFromEuler(Vector3(_roll, _pitch, _yaw));
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor
Quaternion::Quaternion( const Vector3 &_rpy)
{
  this->SetFromEuler(_rpy);
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor from axis angle
Quaternion::Quaternion( const Vector3 &_axis, const double &_angle )
{
  this->SetFromAxis(_axis, _angle);
}

////////////////////////////////////////////////////////////////////////////////
// Copy Constructor
Quaternion::Quaternion( const Quaternion &qt )
{
  this->w = qt.w;
  this->x = qt.x;
  this->y = qt.y;
  this->z = qt.z;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor
Quaternion::~Quaternion()
{
}

////////////////////////////////////////////////////////////////////////////////
// Equal operator
const Quaternion &Quaternion::operator=(const Quaternion &qt)
{
  this->w = qt.w;
  this->x = qt.x;
  this->y = qt.y;
  this->z = qt.z;

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Set quatern to identity
void Quaternion::SetToIdentity()
{
  this->w = 1.0;
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}

////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::GetLog() const
{
  // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
  // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
  // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

  Quaternion result;
  result.w = 0.0;

  if (std::fabs(this->w) < 1.0)
  {
    double fAngle = acos(this->w);
    double fSin = sin(fAngle);
    if (std::fabs(fSin) >= 1e-3 )
    {
      double fCoeff = fAngle/fSin;
      result.x = fCoeff*x;
      result.y = fCoeff*y;
      result.z = fCoeff*z;
      return result;
    }
  }

  result.x = x;
  result.y = y;
  result.z = z;

  return result;
}

////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::GetExp() const
{
  // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
  // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
  // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

  double fAngle = sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
  double fSin = sin(fAngle);

  Quaternion result;
  result.w = cos(fAngle);

  if ( std::fabs(fSin) >= 1e-3 )
  {
    double fCoeff = fSin/fAngle;
    result.x = fCoeff*this->x; 
    result.y = fCoeff*this->y;
    result.z = fCoeff*this->z;
  }
  else
  {
    result.x = this->x;
    result.y = this->y;
    result.z = this->z;
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Invert the quaternion
void Quaternion::Invert()
{
  double norm = this->w*this->w+this->x*this->x+this->y*this->y+this->z*this->z;
  this->w = this->w/norm;
  this->x = -this->x/norm;
  this->y = -this->y/norm;
  this->z = -this->z/norm;
}

////////////////////////////////////////////////////////////////////////////////
// Normalize the quaternion
void Quaternion::Normalize()
{
  double s = 0;

  s = sqrt(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);

  if (s == 0)
  {
    this->w = 1.0;
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
  }
  else
  {
    this->w /= s;
    this->x /= s;
    this->y /= s;
    this->z /= s;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Set the quaternion from an axis and angle
void Quaternion::SetFromAxis(double ax, double ay, double az, double aa)
{
  double l;

  l = ax * ax + ay * ay + az * az;

  if (l > 0.0)
  {
    aa *= 0.5;
    l = sin(aa) / sqrt(l);
    this->w = cos(aa);
    this->x = ax * l;
    this->y = ay * l;
    this->z = az * l;
  }
  else
  {
    this->w = 1;
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }

  this->Normalize();
}

////////////////////////////////////////////////////////////////////////////////
/// Set the quaternion from an axis and angle
void Quaternion::SetFromAxis(const Vector3 &_axis, double _a)
{
  this->SetFromAxis(_axis.x, _axis.y, _axis.z, _a);
}
 
////////////////////////////////////////////////////////////////////////////////
/// Set this quaternion from another
void Quaternion::Set(double u, double x, double y, double z)
{
  this->w = u;
  this->x = x;
  this->y = y;
  this->z = z;
}

////////////////////////////////////////////////////////////////////////////////
// Set the quaternion from Euler angles
void Quaternion::SetFromEuler(const Vector3 &_vec)
{
  double phi, the, psi;

  phi = _vec.x / 2.0;
  the = _vec.y / 2.0;
  psi = _vec.z / 2.0;

  this->w = cos(phi) * cos(the) * cos(psi) + sin(phi) * sin(the) * sin(psi);
  this->x = sin(phi) * cos(the) * cos(psi) - cos(phi) * sin(the) * sin(psi);
  this->y = cos(phi) * sin(the) * cos(psi) + sin(phi) * cos(the) * sin(psi);
  this->z = cos(phi) * cos(the) * sin(psi) - sin(phi) * sin(the) * cos(psi);

  this->Normalize();
}

////////////////////////////////////////////////////////////////////////////////
// Return the rotation in Euler angles
Vector3 Quaternion::GetAsEuler() const
{
  Vector3 vec;

  Quaternion copy = *this;
  double squ;
  double sqx;
  double sqy;
  double sqz;

  copy.Normalize();

  squ = copy.w * copy.w;
  sqx = copy.x * copy.x;
  sqy = copy.y * copy.y;
  sqz = copy.z * copy.z;

  copy.Normalize();

  // Roll
  vec.x = atan2(2 * (copy.y*copy.z + copy.w*copy.x), squ - sqx - sqy + sqz);

  // Pitch
  double sarg = -2 * (copy.x*copy.z - copy.w * copy.y);
  vec.y = sarg <= -1.0 ? -0.5*M_PI : (sarg >= 1.0 ? 0.5*M_PI : asin(sarg));

  // Yaw
  vec.z = atan2(2 * (copy.x*copy.y + copy.w*copy.z), squ + sqx - sqy - sqz);

  return vec;
}

////////////////////////////////////////////////////////////////////////////////
/// Convert euler angles to quatern.
Quaternion Quaternion::EulerToQuaternion( const Vector3 &vec )
{
  Quaternion result;
  result.SetFromEuler(vec);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// Convert euler angles to quatern.
Quaternion Quaternion::EulerToQuaternion( double x, double y, double z)
{
  return EulerToQuaternion( Vector3(x,y,z) );
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Get the Euler roll angle in radians
double Quaternion::GetRoll()
{
  return this->GetAsEuler().x;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Get the Euler pitch angle in radians
double Quaternion::GetPitch()
{
  return this->GetAsEuler().y;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Get the Euler yaw angle in radians
double Quaternion::GetYaw()
{
  return this->GetAsEuler().z;
}

////////////////////////////////////////////////////////////////////////////////
// Return rotation as axis and angle
void Quaternion::GetAsAxis(Vector3 &axis, double &angle) const
{
  double len = this->x*this->x + this->y*this->y + this->z*this->z;
  if (len > 0.0)
  {
    angle = 2.0 * acos(this->w);
    double invLen =  1.0 / sqrt(len);
    axis.Set( this->x*invLen, this->y*invLen, this->z*invLen);
  }
  else
  {
    angle = 0.0;
    axis.Set(1,0,0);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Scale a Quaternionion
void Quaternion::Scale(double scale)
{
  Quaternion b;
  Vector3 axis;
  double angle;

  // Convert to axis-and-angle
  this->GetAsAxis(axis, angle);
  angle *= scale;

  this->SetFromAxis(axis.x, axis.y, axis.z, angle);
}

////////////////////////////////////////////////////////////////////////////////
/// Addition operator
Quaternion Quaternion::operator+( const Quaternion &qt ) const
{
  Quaternion result(this->w + qt.w, this->x + qt.x, 
                 this->y + qt.y, this->z + qt.z);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// Addition operator
Quaternion Quaternion::operator+=( const Quaternion &qt ) 
{
  *this = *this + qt;

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Subtraction operator
Quaternion Quaternion::operator-=( const Quaternion &qt )
{
  *this = *this - qt;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Substraction operator
Quaternion Quaternion::operator-( const Quaternion &qt ) const
{
  Quaternion result(this->w - qt.w, this->x - qt.x, 
                 this->y - qt.y, this->z - qt.z);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Multiplication operator
Quaternion Quaternion::operator*=( const Quaternion &qt )
{
  *this = *this * qt;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Rotate a vector 
Vector3 Quaternion::operator*( const Vector3 &v ) const
{
/*  Quaternion tmp(0.0, v.x, v.y, v.z);
  
  tmp = (*this) * (tmp * this->GetInverse());

  return Vector3(tmp.x, tmp.y, tmp.z);
  */

  Vector3 uv, uuv;
  Vector3 qvec(this->x, this->y, this->z);
  uv = qvec.GetCrossProd(v);
  uuv = qvec.GetCrossProd(uv);
  uv *= (2.0f * this->w);
  uuv *= 2.0f;

  return v + uv + uuv;
}

////////////////////////////////////////////////////////////////////////////////
/// Multipcation operator
Quaternion Quaternion::operator*( const double &_f ) const
{
  return Quaternion(this->w*_f, this->x*_f, this->y*_f, this->z*_f);
}

////////////////////////////////////////////////////////////////////////////////
// Do the reverse rotation of a vector by this quaternion
Vector3 Quaternion::RotateVectorReverse(Vector3 vec) const
{
  Quaternion tmp;
  Vector3 result;

  tmp.w = 0.0;
  tmp.x = vec.x;
  tmp.y = vec.y;
  tmp.z = vec.z;

  tmp =  this->GetInverse() * (tmp * (*this));

  result.x = tmp.x;
  result.y = tmp.y;
  result.z = tmp.z;

  return result;
}


////////////////////////////////////////////////////////////////////////////////
// See if a quatern is finite (e.g., not nan)
bool Quaternion::IsFinite() const
{
  return finite(this->w) && finite(this->x) && finite(this->y) && finite(this->z);
}

Vector3 Quaternion::GetXAxis() const
{
  double fTy  = 2.0f*this->y;
  double fTz  = 2.0f*this->z;

  double fTwy = fTy*this->w;
  double fTwz = fTz*this->w;
  double fTxy = fTy*this->x;
  double fTxz = fTz*this->x;
  double fTyy = fTy*this->y;
  double fTzz = fTz*this->z;

  return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

}

Vector3 Quaternion::GetYAxis() const
{
  double fTx  = 2.0f*this->x;
  double fTy  = 2.0f*this->y;
  double fTz  = 2.0f*this->z;
  double fTwx = fTx*this->w;
  double fTwz = fTz*this->w;
  double fTxx = fTx*this->x;
  double fTxy = fTy*this->x;
  double fTyz = fTz*this->y;
  double fTzz = fTz*this->z;

  return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
}

Vector3 Quaternion::GetZAxis() const
{
  double fTx  = 2.0f*this->x;
  double fTy  = 2.0f*this->y;
  double fTz  = 2.0f*this->z;
  double fTwx = fTx*this->w;
  double fTwy = fTy*this->w;
  double fTxx = fTx*this->x;
  double fTxz = fTz*this->x;
  double fTyy = fTy*this->y;
  double fTyz = fTz*this->y;

  return Vector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
}

////////////////////////////////////////////////////////////////////////////////
/// Equality operator
bool Quaternion::operator==(const Quaternion &_qt) const
{
  return equal(this->x, _qt.x, 0.001) && 
         equal(this->y, _qt.y, 0.001) && 
         equal(this->z, _qt.z, 0.001) && 
         equal(this->w, _qt.w, 0.001);
}

////////////////////////////////////////////////////////////////////////////////
/// Inequality operator
bool Quaternion::operator!=(const Quaternion &_qt) const
{
  return !equal(this->x, _qt.x, 0.001) || 
         !equal(this->y, _qt.y, 0.001) || 
         !equal(this->z, _qt.z, 0.001) || 
         !equal(this->w, _qt.w, 0.001);
}

////////////////////////////////////////////////////////////////////////////////
/// Negate operator
Quaternion Quaternion::operator-() const
{
  return Quaternion(-this->w, -this->x, -this->y, -this->z);
}

////////////////////////////////////////////////////////////////////////////////
/// Get the quaternion as a 3x3 matrix
Matrix3 Quaternion::GetAsMatrix3() const
{
  Quaternion q = *this;
  q.Normalize();
  return Matrix3(1 - 2*q.y*q.y - 2 *q.z*q.z,
                 2 * q.x*q.y - 2*q.z*q.w,
                 2 * q.x * q.z + 2 * q.y * q.w,
                 2 * q.x * q.y + 2 * q.z * q.w,
                 1 - 2*q.x*q.x - 2 * q.z*q.z,
                 2 * q.y * q.z - 2 * q.x * q.w,
                 2 * q.x * q.z - 2 * q.y * q.w,
                 2 * q.y * q.z + 2 * q.x * q.w,
                 1 - 2 * q.x*q.x - 2 * q.y*q.y );
}

////////////////////////////////////////////////////////////////////////////////
/// Get the quaternion as a 4x4 matrix
Matrix4 Quaternion::GetAsMatrix4() const
{
  Matrix4 result( Matrix4::IDENTITY );
  result = this->GetAsMatrix3();
  return result;
}

//////////////////////////////////////////////////////////////////////////////
/// Round all values to _decimalPlaces
void Quaternion::Round(int _precision)
{
  this->x = precision(this->x, _precision);
  this->y = precision(this->y, _precision);
  this->z = precision(this->z, _precision);
  this->w = precision(this->w, _precision);
}

/// Dot product
double Quaternion::Dot(const Quaternion &_q) const
{
  return this->w*_q.w + this->x * _q.x + this->y*_q.y + this->z*_q.z;
}


//////////////////////////////////////////////////////////////////////////////
/// Spherical quadratic interpolation
Quaternion Quaternion::Squad(double _fT, const Quaternion &_rkP,
    const Quaternion &_rkA, const Quaternion &_rkB,
    Quaternion &_rkQ, bool _shortestPath )
{
  double fSlerpT = 2.0f*_fT*(1.0f-_fT);
  Quaternion kSlerpP = Slerp(_fT, _rkP, _rkQ, _shortestPath);
  Quaternion kSlerpQ = Slerp(_fT, _rkA, _rkB);
  return Slerp(fSlerpT, kSlerpP, kSlerpQ);
}

Quaternion Quaternion::Slerp (double _fT, const Quaternion &_rkP,
    const Quaternion &_rkQ, bool _shortestPath)
{ 
  double fCos = _rkP.Dot(_rkQ);
  Quaternion rkT;

  // Do we need to invert rotation?
  if (fCos < 0.0f && _shortestPath)
  {
    fCos = -fCos;
    rkT = -_rkQ;
  }
  else
  {
    rkT = _rkQ;
  }

  if (std::fabs(fCos) < 1 - 1e-03)
  {
    // Standard case (slerp)
    double fSin = sqrt(1 - (fCos*fCos));
    double fAngle = atan2(fSin, fCos);
    double fInvSin = 1.0f / fSin;
    double fCoeff0 = sin((1.0f - _fT) * fAngle) * fInvSin;
    double fCoeff1 = sin(_fT * fAngle) * fInvSin;
    return _rkP * fCoeff0 + rkT * fCoeff1;
  }
  else
  {
    // There are two situations: 
    // 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
    //    interpolation safely.
    // 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there  
    //    are an infinite number of possibilities interpolation. but we haven't 
    //    have method to fix this case, so just use linear interpolation here.  
    Quaternion t = _rkP * (1.0f - _fT) + rkT * _fT;
    // taking the complement requires renormalisation
    t.Normalize();
    return t;
  }
}