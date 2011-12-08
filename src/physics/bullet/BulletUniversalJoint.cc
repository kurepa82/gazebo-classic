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
/* Desc: A universal joint
 * Author: Nate Keonig
 * Date: 24 May 2009
 * SVN: $Id:$
 */

#include "common/Exception.hh"
#include "common/Console.hh"
#include "BulletLink.hh"
#include "BulletUniversalJoint.hh"

using namespace gazebo;
using namespace physics;

using namespace physics;

using namespace physics;


//////////////////////////////////////////////////////////////////////////////
// Constructor
BulletUniversalJoint::BulletUniversalJoint(btDynamicsWorld *world )
    : UniversalJoint<BulletJoint>()
{
  this->world = world;
}

//////////////////////////////////////////////////////////////////////////////
// Destructor
BulletUniversalJoint::~BulletUniversalJoint()
{
}

//////////////////////////////////////////////////////////////////////////////
/// Attach the two bodies with this joint
void BulletUniversalJoint::Attach( Link *one, Link *two )
{
  UniversalJoint<BulletJoint>::Attach(one,two);

  BulletLink *bulletLink1 = dynamic_cast<BulletLink*>(this->body1);
  BulletLink *bulletLink2 = dynamic_cast<BulletLink*>(this->body2);

  if (!bulletLink1 || !bulletLink2)
    gzthrow("Requires bullet bodies");

  btRigidLink *rigidLink1 = bulletLink1->GetBulletLink();
  btRigidLink *rigidLink2 = bulletLink2->GetBulletLink();

  btmath::Vector3 anchor, axis1, axis2;

  anchor = btmath::Vector3(this->anchorPos.x, this->anchorPos.y, this->anchorPos.z);
  axis1 = btmath::Vector3((**this->axis1P).x,(**this->axis1P).y,(**this->axis1P).z);
  axis2 = btmath::Vector3((**this->axis2P).x,(**this->axis2P).y,(**this->axis2P).z);

  this->constraint = new btUniversalConstraint( *rigidLink1, *rigidLink2,
      anchor, axis1, axis2);

  // Add the joint to the world
  this->world->addConstraint(this->constraint);

  // Allows access to impulse
  this->constraint->enableFeedback(true);
}

//////////////////////////////////////////////////////////////////////////////
// Get the anchor point
math::Vector3 BulletUniversalJoint::GetAnchor(int /*index*/) const
{
  return this->anchorPos;
}

//////////////////////////////////////////////////////////////////////////////
// Set the anchor point
void BulletUniversalJoint::SetAnchor( int index, const math::Vector3 &anchor )
{
  gzerr << "Not implemented\n";
}

//////////////////////////////////////////////////////////////////////////////
// Get the first axis of rotation
math::Vector3 BulletUniversalJoint::GetAxis(int index) const
{
  btmath::Vector3 axis = ((btUniversalConstraint*)this->constraint)->getAxis(index);
  return math::Vector3(axis.getX(), axis.getY(), axis.getZ());
}

//////////////////////////////////////////////////////////////////////////////
// Set the joint damping
void BulletUniversalJoint::SetDamping( int /*index*/, const double damping )
{
  gzerr << "Not implemented\n";
}

//////////////////////////////////////////////////////////////////////////////
// Set the first axis of rotation
void BulletUniversalJoint::SetAxis( int index, const math::Vector3 &axis )
{
  gzerr << "Not implemented\n";
}

//////////////////////////////////////////////////////////////////////////////
// Get the angle of an axis 
math::Angle BulletUniversalJoint::GetAngle(int index) const
{
  if (index == 0)
    return ((btUniversalConstraint*)this->constraint)->getmath::Angle1();
  else
    return ((btUniversalConstraint*)this->constraint)->getmath::Angle2();
}

//////////////////////////////////////////////////////////////////////////////
// Get the angular rate of an axis
double BulletUniversalJoint::GetVelocity(int index) const
{
  gzerr << "Not implemented\n";
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
/// Set the velocity of an axis(index).
void BulletUniversalJoint::SetVelocity(int index, double angle)
{
  gzerr << "Not implemented\n";
}

//////////////////////////////////////////////////////////////////////////////
// Set the torque of this joint
void BulletUniversalJoint::SetForce(int index, double torque)
{
  gzerr << "Not implemented\n";
}

//////////////////////////////////////////////////////////////////////////////
/// Set the max allowed force of an axis(index).
void BulletUniversalJoint::SetMaxForce(int index, double t)
{
  gzerr << "Not implemented\n";
}

//////////////////////////////////////////////////////////////////////////////
/// Get the max allowed force of an axis(index).
double BulletUniversalJoint::GetMaxForce(int index)
{
  gzerr << "Not implemented\n";
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
/// Set the high stop of an axis(index).
void BulletUniversalJoint::SetHighStop(int index, math::Angle angle)
{
  if (this->constraint)
    if (index == 0)
      ((btUniversalConstraint*)this->constraint)->setUpperLimit(
        angle.GetAsRadian(), this->GetHighStop(1).GetAsRadian() );
    else
      ((btUniversalConstraint*)this->constraint)->setUpperLimit(
        this->GetHighStop(0).GetAsRadian(), angle.GetAsRadian() );
  else
    gzthrow("Joint must be created first");

}

//////////////////////////////////////////////////////////////////////////////
/// Set the low stop of an axis(index).
void BulletUniversalJoint::SetLowStop(int index, math::Angle angle)
{
  if (this->constraint)
    if (index == 0)
      ((btUniversalConstraint*)this->constraint)->setLowerLimit(
        angle.GetAsRadian(), this->GetLowStop(1).GetAsRadian() );
    else
      ((btUniversalConstraint*)this->constraint)->setUpperLimit(
        this->GetLowStop(0).GetAsRadian(), angle.GetAsRadian() );
  else
    gzthrow("Joint must be created first");
}

//////////////////////////////////////////////////////////////////////////////
/// \brief Get the high stop of an axis(index).
math::Angle BulletUniversalJoint::GetHighStop(int index)
{
  math::Angle result;

  if (this->constraint)
  {
    btRotationalLimitMotor *motor;
    motor = ((btUniversalConstraint*)this->constraint)->getRotationalLimitMotor(index);

    return motor->m_hiLimit;
  }
  else
    gzthrow("Joint must be created first");

  return result;
}

//////////////////////////////////////////////////////////////////////////////
/// \brief Get the low stop of an axis(index).
math::Angle BulletUniversalJoint::GetLowStop(int index)
{
  math::Angle result;

  if (this->constraint)
  {
    btRotationalLimitMotor *motor;
    motor = ((btUniversalConstraint*)this->constraint)->getRotationalLimitMotor(index);

    return motor->m_loLimit;
  }
  else
    gzthrow("Joint must be created first");

  return result;
}