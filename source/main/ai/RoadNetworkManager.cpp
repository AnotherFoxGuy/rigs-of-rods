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

#include "RoadNetworkManager.h"

using namespace Ogre;
using namespace RoR;

RoadNetworkManager::RoadNetworkManager() = default;
RoadNetworkManager::~RoadNetworkManager() = default;

void RoadNetworkManager::AddRoad(const ProceduralObject& po)
{
    auto r = Road();
    float l = 0;

    for (auto pt : po.points)
    {
        r.points.push_back(pt.position.xy());
    }
    Vector2 a = r.points[0];
    r.firstpoint = a;
    r.lastpoint = r.points[r.points.size() - 1];

    for (int i = 1; i < r.points.size(); i++)
    {
        l += r.points[i-1].distance(r.points[1]);
    }

    r.length = l;

    m_roads.push_back(r);
}
