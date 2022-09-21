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

#ifdef USE_ANGELSCRIPT

#include "VehicleAI.h"

#include "Actor.h"
#include "Console.h"
#include "EngineSim.h"
#include "Terrain.h"
#include "GfxScene.h"
#include "GUIManager.h"

#include "scriptdictionary/scriptdictionary.h"

#include "OgreDetourCrowd.h"

using namespace Ogre;
using namespace RoR;

VehicleAI::VehicleAI(Actor* b) : is_enabled(false)
{
    beam = b;
    mDetourCrowd = App::GetSimTerrain()->mDetourCrowd;
    mAgentID = mDetourCrowd->addAgent(beam->getPosition());
    if(mAgentID < 0)
      return;
    mAgent = mDetourCrowd->m_crowd->getEditableAgent(mAgentID);
    mAgent->params.radius = 3;
    mAgent->params.maxSpeed = 5;

    mNode = App::GetGfxScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
    auto mEnt = App::GetGfxScene()->GetSceneManager()->createEntity("Cylinder.mesh");
    mEnt->setMaterialName("Cylinder/Blue");
    mNode->attachObject(mEnt);
    mNode->setPosition(beam->getPosition());
}

VehicleAI::~VehicleAI()
{
    mDetourCrowd->removeAgent(mAgentID);
    mNode->removeAndDestroyAllChildren();
}

void VehicleAI::SetActive(bool value)
{
    is_enabled = value;
}

bool VehicleAI::IsActive()
{
    return is_enabled;
}

void VehicleAI::updateDestination(Ogre::Vector3 destination, bool updatePreviousPath)
{
    // Find position on navmesh
    if(!mDetourCrowd->m_recast->findNearestPointOnNavmesh(destination, destination))
      return;

    mDetourCrowd->setMoveTarget(destination, updatePreviousPath);
    //mDetourCrowd->setMoveTarget(mAgentID, destination, updatePreviousPath);
    mDestination = destination;
    is_enabled = true;

    int ret = mDetourCrowd->m_recast->FindPath(beam->getPosition(), destination, 0, mAgentID) ;
    if( ret >= 0 )
      mDetourCrowd->m_recast->CreateRecastPathLine(0) ; // Draw a line showing path at specified slot
}

void VehicleAI::setPosition(Ogre::Vector3 position)
{
    // Find position on navmesh
    if (!mDetourCrowd->m_recast->findNearestPointOnNavmesh(position, position))
      return;

    // Remove agent from crowd and re-add at position
    mDetourCrowd->removeAgent(mAgentID);
    mAgentID = mDetourCrowd->addAgent(position);
    mAgent = mDetourCrowd->m_crowd->getEditableAgent(mAgentID);

    mNode->setPosition(position);
}

void VehicleAI::update(float dt, int doUpdate)
{
    Vector3 velocity;
    Vector3 TargetPosition;
    Vector3 mSteeringForce;

    float speed = 0;
    if (is_enabled) {
        OgreRecast::FloatAToOgreVect3(mAgent->npos, TargetPosition);
        mNode->setPosition(TargetPosition);

        OgreRecast::FloatAToOgreVect3(mAgent->nvel, velocity);

        Quaternion mAgentOrientation =
            Quaternion(Radian(beam->getRotation()), Vector3::UNIT_Y);
        mAgentOrientation.normalise();

        // Compute new torque scalar (-1.0 to 1.0) based on heading vector to target.
        mSteeringForce = mAgentOrientation * velocity;
        mSteeringForce.normalise();

        Vector3 _currVel;
        OgreRecast::FloatAToOgreVect3(mAgent->vel, _currVel);

        Vector3 speed_offset = velocity - _currVel;
        speed_offset = mAgentOrientation * speed_offset;
        speed = speed_offset.z;

        beam->ar_hydro_dir_command = mSteeringForce.x;

        if (speed < 0.5)
        {
          beam->ar_brake = 0;
          beam->ar_engine->autoSetAcc((-speed) / 10);
        }
        else if (speed > 0.5)
        {
          beam->ar_brake = 1.0f / 3.0f;
          beam->ar_engine->autoSetAcc(0);
        }
        else
        {
          beam->ar_brake = 0;
          beam->ar_engine->autoSetAcc(0);
        }

        OgreRecast::OgreVect3ToFloatA(beam->getPosition(), mAgent->npos);
        OgreRecast::OgreVect3ToFloatA(beam->getVelocity(), mAgent->vel);
    }

    auto st = beam->ar_design_name + " AI";
    ImGui::Begin(st.c_str(), nullptr);
    ImGui::Text("Speed %f", (-speed) / 10);
    ImGui::Text("velocity %f %f %f", velocity.x, velocity.y, velocity.z);
    ImGui::Text("TargetPosition %f %f %f", TargetPosition.x, TargetPosition.y, TargetPosition.z);
    ImGui::Text("mSteeringForce %f %f %f", mSteeringForce.x, mSteeringForce.y, mSteeringForce.z);
    ImGui::InputFloat("x", &aaaaa.x);
    ImGui::InputFloat("y", &aaaaa.y);
    ImGui::InputFloat("z", &aaaaa.z);
    ImGui::InputFloat("maxSpeed", &mAgent->params.maxSpeed);
    if (ImGui::Button("updateDestination"))
    {
        this->updateDestination(aaaaa, false);
        aaaaa = mDestination;
    }

    if (ImGui::Button("setCurrDest"))
    {
        aaaaa = beam->getPosition();
    }

    if (ImGui::Button("setPosition"))
    {
        this->setPosition(beam->getPosition());
    }

    if (ImGui::Button("Random"))
    {
        aaaaa = App::GetSimTerrain()->mRecast->getRandomNavMeshPoint();
        this->updateDestination(aaaaa, false);
    }
    if (ImGui::Button(is_enabled ? "Stop" : "Start"))
      is_enabled = !is_enabled;
    ImGui::End();
}

#endif // USE_ANGELSCRIPT
