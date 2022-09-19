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

Vector3 aaaaa;

VehicleAI::VehicleAI(Actor* b) :
                                 mStopped(false)
{
    beam = b;
    mDetourCrowd = App::GetSimTerrain()->mDetourCrowd;
    mAgentID = mDetourCrowd->addAgent(beam->getPosition());
    mAgent = mDetourCrowd->m_crowd->getEditableAgent(mAgentID);
    mAgent->params.radius = 3;

    mNode = App::GetGfxScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
    auto mEnt = App::GetGfxScene()->GetSceneManager()->createEntity("Cylinder.mesh");
    mEnt->setMaterialName("Cylinder/Blue");
    mNode->attachObject(mEnt);
    mNode->setPosition(beam->getPosition());
}

VehicleAI::~VehicleAI()
{
}

void VehicleAI::SetActive(bool value)
{
    mStopped = value;
}

bool VehicleAI::IsActive()
{
    return mStopped;
}

void VehicleAI::updateDestination(Ogre::Vector3 destination, bool updatePreviousPath)
{
    // Find position on navmesh
    if(!mDetourCrowd->m_recast->findNearestPointOnNavmesh(destination, destination))
      return;

    mDetourCrowd->setMoveTarget(mAgentID, destination, updatePreviousPath);
    mDestination = destination;
    mStopped = false;

    int ret = mDetourCrowd->m_recast->FindPath(beam->getPosition(), destination, 0, mAgentID) ;
    if( ret >= 0 )
      mDetourCrowd->m_recast->CreateRecastPathLine(0) ; // Draw a line showing path at specified slot
}

void VehicleAI::update(float dt, int doUpdate)
{
if (!mStopped) {
  /* Vector3 mAgentPosition = beam->getPosition();

Vector3 TargetPosition;
OgreRecast::FloatAToOgreVect3(mAgent->npos, TargetPosition);
mNode->setPosition(TargetPosition);
float distance = TargetPosition.distance(mAgentPosition);

TargetPosition.y = 0; //Vector3 > Vector2
Quaternion TargetOrientation = Quaternion::ZERO;

mAgentPosition.y = 0; //Vector3 > Vector2
Quaternion mAgentOrientation = Quaternion(Radian(beam->getRotation()),
Vector3::NEGATIVE_UNIT_Y); mAgentOrientation.normalise();

Vector3 mVectorToTarget = TargetPosition - mAgentPosition; // A-B = B->A
mAgentPosition.normalise();

Vector3 mAgentHeading = mAgentOrientation * mAgentPosition;
Vector3 mTargetHeading = TargetOrientation * TargetPosition;
mAgentHeading.normalise();
mTargetHeading.normalise();

// Compute new torque scalar (-1.0 to 1.0) based on heading vector to target.
Vector3 mSteeringForce = mAgentOrientation.Inverse() * mVectorToTarget;
mSteeringForce.normalise();



float mYaw = mSteeringForce.x;
float mPitch = mSteeringForce.z;
//float mRoll   = mTargetVO.getRotationTo( mAgentVO ).getRoll().valueRadians();

if (mPitch > 0)
{
if (mYaw > 0)
 mYaw = 1;
else
 mYaw = -1;
}

// actually steer
beam->ar_hydro_dir_command = mYaw;//mYaw

if (beam->ar_engine)
{
// start engine if not running
if (!beam->ar_engine->IsRunning())
 beam->ar_engine->StartEngine();

//if (distance > 5)
{
 beam->ar_brake = 0;
 beam->ar_engine->autoSetAcc(0.3f);
}
else
{
 beam->ar_brake = 1;
 beam->ar_engine->autoSetAcc(0);
}
}*/

      Vector3 TargetPosition;
      OgreRecast::FloatAToOgreVect3(mAgent->npos, TargetPosition);
      mNode->setPosition(TargetPosition);

      Vector3 velocity;
      OgreRecast::FloatAToOgreVect3(mAgent->nvel, velocity);

      Quaternion mAgentOrientation =
          Quaternion(Radian(beam->getRotation()), Vector3::NEGATIVE_UNIT_Y);
      mAgentOrientation.normalise();

      // Compute new torque scalar (-1.0 to 1.0) based on heading vector to target.
      Vector3 mSteeringForce = mAgentOrientation.Inverse() * velocity;
      mSteeringForce.normalise();



      beam->ar_hydro_dir_command = mSteeringForce.x;
      OgreRecast::OgreVect3ToFloatA(beam->getPosition(), mAgent->npos);
    }

    auto st = beam->ar_design_name + " AI";
    ImGui::Begin(st.c_str(), nullptr);
    ImGui::InputFloat("x", &aaaaa.x);
    ImGui::InputFloat("y", &aaaaa.y);
    ImGui::InputFloat("z", &aaaaa.z);
    if (ImGui::Button("Setdest"))
      updateDestination(aaaaa, true);
    if (ImGui::Button(mStopped ? "Stop" : "Start"))
      mStopped = !mStopped;
    ImGui::End();
}

#endif // USE_ANGELSCRIPT
