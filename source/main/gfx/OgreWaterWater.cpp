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

#include "OgreWaterWater.h"

#include "AppContext.h"
#include "CameraManager.h"
#include "GameContext.h"
#include "GfxScene.h"
#include "SkyManager.h"
#include "Terrain.h"

#ifdef USE_CAELUM
#include <Caelum.h>

#include <memory>
#endif // USE_CAELUM

using namespace Ogre;
using namespace RoR;
using namespace OgreWater;

// OgreWaterWater
OgreWaterWater::OgreWaterWater(float water_height)
{
    mWater.reset(new Water(
      App::GetAppContext()->GetRenderWindow(),
      App::GetGfxScene()->GetSceneManager(),
      App::GetCameraManager()->GetCamera()
    ));
    mWater->createTextures();

    //mWater->setWaterDustEnabled(true);
    //mWater->setAirBubblesEnabled(true);
    mWater->init();
    mWater->setWaterHeight(water_height);
}

OgreWaterWater::~OgreWaterWater()
{
    mWater = nullptr;
}

void OgreWaterWater::FrameStepWater(float dt)
{
    mWater->update(dt);
}

float OgreWaterWater::GetStaticWaterHeight()
{
    return mWater->getWaterHeight();
}

void OgreWaterWater::SetStaticWaterHeight(float value)
{
    mWater->setWaterHeight(value);
}
