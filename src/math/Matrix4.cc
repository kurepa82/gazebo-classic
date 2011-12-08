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
#include <string.h>

#include "common/Exception.hh"
#include "math/Matrix4.hh"

using namespace gazebo;
using namespace math;

const Matrix4 Matrix4::IDENTITY(
       1.0, 0.0, 0.0, 0.0,
       0.0, 1.0, 0.0, 0.0,
       0.0, 0.0, 1.0, 0.0,
       0.0, 0.0, 0.0, 1.0 );


const Matrix4 Matrix4::ZERO(
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0 );


////////////////////////////////////////////////////////////////////////////////
// Default constructor
Matrix4::Matrix4()
{
  memset(this->m, 0, sizeof(double)*16);
}

////////////////////////////////////////////////////////////////////////////////
// Copy constructor
Matrix4::Matrix4(const Matrix4 &m)
{
  memcpy(this->m, m.m, sizeof(double)*16);
}

////////////////////////////////////////////////////////////////////////////////
// Constructor
Matrix4::Matrix4(double v00, double v01, double v02, double v03,
                 double v10, double v11, double v12, double v13,
                 double v20, double v21, double v22, double v23,
                 double v30, double v31, double v32, double v33)
{
  this->Set( v00, v01, v02, v03,
             v10, v11, v12, v13,
             v20, v21, v22, v23,
             v30, v31, v32, v33);
}

////////////////////////////////////////////////////////////////////////////////
// Destructor
Matrix4::~Matrix4()
{
}

////////////////////////////////////////////////////////////////////////////////
// Constructor
void Matrix4::Set(double v00, double v01, double v02, double v03,
                  double v10, double v11, double v12, double v13,
                  double v20, double v21, double v22, double v23,
                  double v30, double v31, double v32, double v33)
{
  this->m[0][0] = v00;
  this->m[0][1] = v01;
  this->m[0][2] = v02;
  this->m[0][3] = v03;

  this->m[1][0] = v10;
  this->m[1][1] = v11;
  this->m[1][2] = v12;
  this->m[1][3] = v13;

  this->m[2][0] = v20;
  this->m[2][1] = v21;
  this->m[2][2] = v22;
  this->m[2][3] = v23;

  this->m[3][0] = v30;
  this->m[3][1] = v31;
  this->m[3][2] = v32;
  this->m[3][3] = v33;
}

////////////////////////////////////////////////////////////////////////////////
// Set translation
void Matrix4::SetTranslate(const Vector3 &t)
{
  this->m[0][3] = t.x;
  this->m[1][3] = t.y;
  this->m[2][3] = t.z;
}

////////////////////////////////////////////////////////////////////////////////
/// Set the scale
void Matrix4::SetScale(const Vector3 &_s)
{
  this->m[0][0] = _s.x;
  this->m[1][1] = _s.y;
  this->m[2][2] = _s.z;
  this->m[3][3] = 1.0;
}
 

////////////////////////////////////////////////////////////////////////////////
// Equality operator
const Matrix4 &Matrix4::operator=( const Matrix4 &mat )
{
  memcpy(this->m, mat.m, sizeof(double)*16);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Equality operator
const Matrix4 &Matrix4::operator=( const Matrix3 &mat )
{
  this->m[0][0] = mat.m[0][0];
  this->m[0][1] = mat.m[0][1];
  this->m[0][2] = mat.m[0][2];

  this->m[1][0] = mat.m[1][0];
  this->m[1][1] = mat.m[1][1];
  this->m[1][2] = mat.m[1][2];

  this->m[2][0] = mat.m[2][0];
  this->m[2][1] = mat.m[2][1];
  this->m[2][2] = mat.m[2][2];

  return *this;
}


////////////////////////////////////////////////////////////////////////////////
// Mult operator
Matrix4 Matrix4::operator*(const Matrix3 &m2) const
{
  Matrix4 r;
  r = *this;

  r.m[0][0] = m[0][0]*m2.m[0][0] + m[0][1]*m2.m[1][0] + m[0][2] * m2.m[2][0];
  r.m[0][1] = m[0][0]*m2.m[0][1] + m[0][1]*m2.m[1][1] + m[0][2] * m2.m[2][1];
  r.m[0][2] = m[0][0]*m2.m[0][2] + m[0][1]*m2.m[1][2] + m[0][2] * m2.m[2][2];

  r.m[1][0] = m[1][0]*m2.m[0][0] + m[1][1]*m2.m[1][0] + m[1][2] * m2.m[2][0];
  r.m[1][1] = m[1][0]*m2.m[0][1] + m[1][1]*m2.m[1][1] + m[1][2] * m2.m[2][1];
  r.m[1][2] = m[1][0]*m2.m[0][2] + m[1][1]*m2.m[1][2] + m[1][2] * m2.m[2][2];

  r.m[2][0] = m[2][0]*m2.m[0][0] + m[2][1]*m2.m[1][0] + m[2][2] * m2.m[2][0];
  r.m[2][1] = m[2][0]*m2.m[0][1] + m[2][1]*m2.m[1][1] + m[2][2] * m2.m[2][1];
  r.m[2][2] = m[2][0]*m2.m[0][2] + m[2][1]*m2.m[1][2] + m[2][2] * m2.m[2][2];

  return r;
}

////////////////////////////////////////////////////////////////////////////////
// Mult operator
Matrix4 Matrix4::operator*(const Matrix4 &m2) const
{
  Matrix4 r;

  r.m[0][0] = this->m[0][0] * m2.m[0][0] + this->m[0][1] * m2.m[1][0] + this->m[0][2] * m2.m[2][0] + this->m[0][3] * m2.m[3][0];
  r.m[0][1] = this->m[0][0] * m2.m[0][1] + this->m[0][1] * m2.m[1][1] + this->m[0][2] * m2.m[2][1] + this->m[0][3] * m2.m[3][1];
  r.m[0][2] = this->m[0][0] * m2.m[0][2] + this->m[0][1] * m2.m[1][2] + this->m[0][2] * m2.m[2][2] + this->m[0][3] * m2.m[3][2];
  r.m[0][3] = this->m[0][0] * m2.m[0][3] + this->m[0][1] * m2.m[1][3] + this->m[0][2] * m2.m[2][3] + this->m[0][3] * m2.m[3][3];

  r.m[1][0] = this->m[1][0] * m2.m[0][0] + this->m[1][1] * m2.m[1][0] + this->m[1][2] * m2.m[2][0] + this->m[1][3] * m2.m[3][0];
  r.m[1][1] = this->m[1][0] * m2.m[0][1] + this->m[1][1] * m2.m[1][1] + this->m[1][2] * m2.m[2][1] + this->m[1][3] * m2.m[3][1];
  r.m[1][2] = this->m[1][0] * m2.m[0][2] + this->m[1][1] * m2.m[1][2] + this->m[1][2] * m2.m[2][2] + this->m[1][3] * m2.m[3][2];
  r.m[1][3] = this->m[1][0] * m2.m[0][3] + this->m[1][1] * m2.m[1][3] + this->m[1][2] * m2.m[2][3] + this->m[1][3] * m2.m[3][3];

  r.m[2][0] = this->m[2][0] * m2.m[0][0] + this->m[2][1] * m2.m[1][0] + this->m[2][2] * m2.m[2][0] + this->m[2][3] * m2.m[3][0];
  r.m[2][1] = this->m[2][0] * m2.m[0][1] + this->m[2][1] * m2.m[1][1] + this->m[2][2] * m2.m[2][1] + this->m[2][3] * m2.m[3][1];
  r.m[2][2] = this->m[2][0] * m2.m[0][2] + this->m[2][1] * m2.m[1][2] + this->m[2][2] * m2.m[2][2] + this->m[2][3] * m2.m[3][2];
  r.m[2][3] = this->m[2][0] * m2.m[0][3] + this->m[2][1] * m2.m[1][3] + this->m[2][2] * m2.m[2][3] + this->m[2][3] * m2.m[3][3];

  r.m[3][0] = this->m[3][0] * m2.m[0][0] + this->m[3][1] * m2.m[1][0] + this->m[3][2] * m2.m[2][0] + this->m[3][3] * m2.m[3][0];
  r.m[3][1] = this->m[3][0] * m2.m[0][1] + this->m[3][1] * m2.m[1][1] + this->m[3][2] * m2.m[2][1] + this->m[3][3] * m2.m[3][1];
  r.m[3][2] = this->m[3][0] * m2.m[0][2] + this->m[3][1] * m2.m[1][2] + this->m[3][2] * m2.m[2][2] + this->m[3][3] * m2.m[3][2];
  r.m[3][3] = this->m[3][0] * m2.m[0][3] + this->m[3][1] * m2.m[1][3] + this->m[3][2] * m2.m[2][3] + this->m[3][3] * m2.m[3][3];

  return r;
}
////////////////////////////////////////////////////////////////////////////////
/// Multiplication operator
Vector3 Matrix4::operator*(const Vector3 &_vec) const
{
  Vector3 result;
  result.x = this->m[0][0]*_vec.x + this->m[0][1]*_vec.y + this->m[0][2]*_vec.z + this->m[0][3];
  result.y = this->m[1][0]*_vec.x + this->m[1][1]*_vec.y + this->m[1][2]*_vec.z + this->m[1][3];
  result.z = this->m[2][0]*_vec.x + this->m[2][1]*_vec.y + this->m[2][2]*_vec.z + this->m[2][3];
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Return true if affine
bool Matrix4::IsAffine() const
{
  return this->m[3][0] == 0 && this->m[3][1] == 0 && this->m[3][2] == 0 && this->m[3][3] == 1;
}

////////////////////////////////////////////////////////////////////////////////
// Affine transform
Vector3 Matrix4::TransformAffine( const Vector3 &v ) const
{
  if (!this->IsAffine())
  {
    gzthrow("Not and affine matrix");
  }

  return Vector3(
      this->m[0][0]*v.x + this->m[0][1]*v.y + this->m[0][2]*v.z + this->m[0][3],
      this->m[1][0]*v.x + this->m[1][1]*v.y + this->m[1][2]*v.z + this->m[1][3],
      this->m[2][0]*v.x + this->m[2][1]*v.y + this->m[2][2]*v.z + this->m[2][3]
      );
}