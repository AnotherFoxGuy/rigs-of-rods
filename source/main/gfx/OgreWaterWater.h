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

#pragma once

#include "OWWater.h"
#include "IWater.h"

#include <Ogre.h>

namespace RoR {

/// @addtogroup Gfx
/// @{

class OgreWaterWater : public IWater
{
public:
    OgreWaterWater(float waterHeight);
    ~OgreWaterWater();

    // Interface IWater
    float          GetStaticWaterHeight() override;
    void           SetStaticWaterHeight(float value) override;
    float          CalcWavesHeight(Ogre::Vector3 pos) override { return 0; }
    Ogre::Vector3  CalcWavesVelocity(Ogre::Vector3 pos) override { return Ogre::Vector3::ZERO; }
    void           SetWaterVisible(bool value) override {}
    void           WaterSetSunPosition(Ogre::Vector3) override {}
    bool           IsUnderWater(Ogre::Vector3 pos) override { return false; }
    void           FrameStepWater(float dt) override;
    void           UpdateWater() override {}

    OgreWater::Water* GetOWater() { return mWater.get(); }

protected:

    std::unique_ptr<OgreWater::Water> mWater;
};

/// @} // addtogroup Gfx

} // namespace RoR
