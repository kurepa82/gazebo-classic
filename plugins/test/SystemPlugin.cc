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
#include <boost/bind.hpp>
#include "gazebo.h"

namespace gazebo
{
  class ServerTest : public SystemPlugin
  {
    public: void Load()
    {
      printf("Loaded a server plugin\n");
    }
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SYSTEM_PLUGIN(ServerTest)
}
 