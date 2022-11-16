/*
    This source file is part of Rigs of Rods

    Copyright 2005-2012 Pierre-Michel Ricordel
    Copyright 2007-2012 Thomas Fischer
    Copyright 2013-2016 Petr Ohlidal

    For more information, see http://www.rigsofrods.org/

    Rigs of Rods is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3, as
    published by the Free Software Foundation.

    Rigs of Rods is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rigs of Rods. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief  Simple waypoint AI
/// @author AnotherFoxGuy
/// @date   03/2016

#pragma once

#include "Application.h"

namespace RoR {

/// Road def
/// ─────────────────           ↑
///                             │
/// ══ ══ ══ ══ ══ ══           │
///                    ←┐       │
/// ═════════════════   │ lanes │ road_width
///                    ←┘       │
/// ══ ══ ══ ══ ══ ══           │
///                             │
/// ─────────────────           ↓
struct Road
{
  Ogre::String type;
  Ogre::String name;
  float road_width = 6;
  int lanes = 2;
  std::vector<Ogre::Vector3> points;
};

class Navigable_Road : public ZeroedMemoryAllocator
{
public:
    Navigable_Road();
    ~Navigable_Road();

  private:
    Ogre::String type;
    Ogre::String name;
    float road_width;
    int lanes;
    std::vector<Ogre::Vector3> points;
};

} // namespace RoR
