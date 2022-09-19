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

#ifdef USE_ANGELSCRIPT

#include "Application.h"
#include "scriptdictionary/scriptdictionary.h"
#include <DetourCrowd.h>
#include <DetourNavMesh.h>
#include <OgreDetourTileCache.h>

namespace RoR {

class VehicleAI : public ZeroedMemoryAllocator
{
public:
    VehicleAI(Actor* b);
    ~VehicleAI();
    /**
     *  Activates/Deactivates the AI.
     *  @param [in] value Activate or deactivation the AI
     */
    void SetActive(bool value);
    /**
     *  Returns the status of the AI.
     *  @return True if the AI is driving
     */
    bool IsActive();

#ifdef USE_ANGELSCRIPT
    // we have to add this to be able to use the class as reference inside scripts
    void addRef()
    {
    };

    void release()
    {
    };
#endif
    /**
     *  Updates the AI.
     */
    void update(float dt, int doUpdate);
    /**
     *  Adds one waypoint.
     *
     *  @param [in] id The waypoint ID.
     *  @param [in] point The coordinates of the waypoint.
     */
    void updateDestination(Ogre::Vector3 destination, bool updatePreviousPath= false);


private:
  /**
      * Crowd in which the agent of this character is.
      **/
  OgreDetourCrowd *mDetourCrowd;

  /**
      * The agent controlling this character.
      **/
  dtCrowdAgent *mAgent{};

  /**
      * ID of mAgent within the crowd.
      **/
  int mAgentID{};

  /**
      * The current destination set for this agent.
      * Take care in properly setting this variable, as it is only updated properly when
      * using Character::updateDestination() to set an individual destination for this character.
      * After updating the destination of all agents this variable should be set externally using
      * setDestination().
      **/
  Ogre::Vector3 mDestination;

  /**
      * True if this character is stopped.
      **/
  bool mStopped;


  Actor* beam;//!< The verhicle the AI is driving.

  /**
      * Node in which this character is.
      **/
  Ogre::SceneNode *mNode;

};

} // namespace RoR

#endif // USE_ANGELSCRIPT
