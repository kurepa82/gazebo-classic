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
/* Desc: Trimesh shape
 * Author: Nate Keonig
 * Date: 21 May 2009
 * SVN: $Id:$
 */

#include "BulletPhysics.hh"
#include "rendering/Visual.hh"
#include "Link.hh"
#include "common/Exception.hh"
#include "BulletTrimeshShape.hh"

using namespace gazebo;
using namespace physics;

using namespace physics;

using namespace physics;


//////////////////////////////////////////////////////////////////////////////
// Constructor
BulletTrimeshShape::BulletTrimeshShape(Collision *parent) 
  : TrimeshShape(parent)
{
}


//////////////////////////////////////////////////////////////////////////////
// Destructor
BulletTrimeshShape::~BulletTrimeshShape()
{
}

//////////////////////////////////////////////////////////////////////////////
/// Update function.
void BulletTrimeshShape::Update()
{
}

//////////////////////////////////////////////////////////////////////////////
/// Load the trimesh
void BulletTrimeshShape::Load(common::XMLConfigNode *node)
{
}