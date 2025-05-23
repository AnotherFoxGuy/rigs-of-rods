/*
    This source file is part of Rigs of Rods
    Copyright 2005-2012 Pierre-Michel Ricordel
    Copyright 2007-2012 Thomas Fischer
    Copyright 2013-2020 Petr Ohlidal

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
/// @author Petr Ohlidal
/// @date   12/2013

#include "RigDef_Validator.h"

#include "Actor.h"
#include "SimConstants.h"
#include "Application.h"
#include "Console.h"

#define CHECK_SECTION_IN_ALL_MODULES(_CLASS_, _FIELD_, _FUNCTION_) \
{ \
    std::list<std::shared_ptr<RigDef::Document::Module>>::iterator module_itor = m_selected_modules.begin(); \
    for (; module_itor != m_selected_modules.end(); module_itor++) \
    { \
        std::vector<_CLASS_>::iterator section_itor = module_itor->get()->_FIELD_.begin(); \
        for (; section_itor != module_itor->get()->_FIELD_.end(); section_itor++) \
        { \
            if (! _FUNCTION_(*section_itor))\
            { \
                section_itor = module_itor->get()->_FIELD_.erase(section_itor); \
                if (section_itor == module_itor->get()->_FIELD_.end()) \
                { \
                    break; \
                } \
            } \
        } \
    } \
}

using namespace RigDef;
using namespace RoR;

bool Validator::Validate()
{
    bool valid = true;

    /* CHECK CONFIGURATION (SELECTED MODULES TOGETHER) */

    valid &= CheckSectionSubmeshGroundmodel(); /* Unique */

    valid &= CheckGearbox(); /* Min. 1 forward gear */

    /* CHECK INDIVIDUAL LINES (remove invalid entries) */

    CHECK_SECTION_IN_ALL_MODULES(Animator, animators, CheckAnimator);

    CHECK_SECTION_IN_ALL_MODULES(Shock2, shocks2, CheckShock2);

    CHECK_SECTION_IN_ALL_MODULES(Shock3, shocks3, CheckShock3);

    CHECK_SECTION_IN_ALL_MODULES(Command2, commands2, CheckCommand);

    CHECK_SECTION_IN_ALL_MODULES(Trigger, triggers, CheckTrigger);

    CHECK_SECTION_IN_ALL_MODULES(Trigger, triggers, CheckTrigger);

    CHECK_SECTION_IN_ALL_MODULES(Flare2, flares2, CheckFlare2);

    return valid;
}

void Validator::Setup(RigDef::DocumentPtr file)
{
    m_file = file;
    m_selected_modules.push_back(file->root_module);
    m_check_beams = true;
}

void Validator::AddMessage(Validator::Message::Type type, Ogre::String const & text)
{
    RoR::Console::MessageType cm_type;
    switch (type)
    {
    case Message::TYPE_FATAL_ERROR:
        cm_type = RoR::Console::MessageType::CONSOLE_SYSTEM_ERROR;
        break;

    case Message::TYPE_ERROR:
    case Message::TYPE_WARNING:
        cm_type = RoR::Console::MessageType::CONSOLE_SYSTEM_WARNING;
        break;

    default:
        cm_type = RoR::Console::MessageType::CONSOLE_SYSTEM_NOTICE;
        break;
    }

    RoR::App::GetConsole()->putMessage(RoR::Console::CONSOLE_MSGTYPE_ACTOR, cm_type, text);
}

bool Validator::CheckSectionSubmeshGroundmodel()
{
    Ogre::String *containing_module_name = nullptr;

    std::list<std::shared_ptr<RigDef::Document::Module>>::iterator module_itor = m_selected_modules.begin();
    for (; module_itor != m_selected_modules.end(); module_itor++)
    {
        if (! module_itor->get()->submesh_groundmodel.empty())
        {
            if (containing_module_name == nullptr)
            {
                containing_module_name = & module_itor->get()->name;
            }
            else
            {
                std::stringstream text;
                text << "Duplicate inline-section 'submesh_groundmodel'; found in modules: '" 
                    << *containing_module_name << "' & '" << module_itor->get()->name << "'";
                AddMessage(Message::TYPE_FATAL_ERROR, text.str());
                return false;
            }
        }
    }

    return true;
}

bool Validator::AddModule(Ogre::String const & module_name)
{
    std::map< Ogre::String, std::shared_ptr<RigDef::Document::Module> >::iterator result 
        = m_file->user_modules.find(module_name);

    if (result != m_file->user_modules.end())
    {
        m_selected_modules.push_back(result->second);
        return true;
    }
    return false;
}

bool Validator::CheckGearbox()
{
    /* Find it */
    std::shared_ptr<RigDef::Engine> engine;
    std::list<std::shared_ptr<RigDef::Document::Module>>::iterator module_itor = m_selected_modules.begin();
    for (; module_itor != m_selected_modules.end(); module_itor++)
    {
        if (module_itor->get()->engine.size() > 0)
        {
            if (module_itor->get()->engine[module_itor->get()->engine.size() - 1].gear_ratios.size() > 0)
            {
                return true;
            }
            else
            {
                AddMessage(Message::TYPE_FATAL_ERROR, "Engine must have at least 1 forward gear.");
                return false;
            }
        }
    }
    return true;
}

bool Validator::CheckShock2(RigDef::Shock2 & shock2)
{
    std::list<Ogre::String> bad_fields;

    /* Keep these in sync with wiki doc: http://www.rigsofrods.org/wiki/pages/Truck_Description_File#Shocks2 */
    /* We safely check for value -1.f */
    if (shock2.spring_in < -0.8f)
    {
        bad_fields.push_back("spring_in_rate");
    }
    if (shock2.damp_in < -0.8f)
    {
        bad_fields.push_back("damping_in_rate");
    }
    if (shock2.spring_out < -0.8f)
    {
        bad_fields.push_back("spring_out_rate");
    }
    if (shock2.damp_out < -0.8f)
    {
        bad_fields.push_back("damping_out_rate");
    }
    if (shock2.progress_factor_spring_in < -0.8f)
    {
        bad_fields.push_back("spring_in_progression_factor");
    }
    if (shock2.progress_factor_damp_in < -0.8f)
    {
        bad_fields.push_back("damping_in_progression_factor");
    }
    if (shock2.progress_factor_spring_out < -0.8f)
    {
        bad_fields.push_back("spring_out_progression_factor");
    }
    if (shock2.progress_factor_damp_out < -0.8f)
    {
        bad_fields.push_back("damping_out_progression_factor");
    }
    if (shock2.short_bound < -0.8f)
    {
        bad_fields.push_back("max_contraction");
    }
    if (shock2.long_bound < -0.8f)
    {
        bad_fields.push_back("max_extension");
    }
    if (shock2.precompression < -0.8f)
    {
        bad_fields.push_back("precompression");
    }

    if (bad_fields.size() > 0)
    {
        std::stringstream msg;
        msg << "Invalid values in section 'shocks2', fields: ";
        std::list<Ogre::String>::iterator itor = bad_fields.begin();
        bool first = true;
        for( ; itor != bad_fields.end(); itor++)
        {
            msg << (first ? "" : ", ") << *itor;
            first = false;
        }

        AddMessage(Message::TYPE_ERROR, msg.str());
        return false;
    }
    return true;
}

bool Validator::CheckShock3(RigDef::Shock3 & shock3)
{
    std::list<Ogre::String> bad_fields;

    /* Keep these in sync with wiki doc: http://www.rigsofrods.org/wiki/pages/Truck_Description_File#Shocks3 */
    /* We safely check for value -1.f */
    if (shock3.spring_in < -0.8f)
    {
        bad_fields.push_back("spring_in_rate");
    }
    if (shock3.damp_in < -0.8f)
    {
        bad_fields.push_back("damping_in_rate");
    }
    if (shock3.spring_out < -0.8f)
    {
        bad_fields.push_back("spring_out_rate");
    }
    if (shock3.damp_out < -0.8f)
    {
        bad_fields.push_back("damping_out_rate");
    }
    if (shock3.damp_in_slow < -0.8f)
    {
        bad_fields.push_back("damp_in_slow");
    }
    if (shock3.split_vel_in < -0.8f)
    {
        bad_fields.push_back("split_in");
    }
    if (shock3.damp_in_fast < -0.8f)
    {
        bad_fields.push_back("damp_in_fast");
    }
    if (shock3.damp_out_slow < -0.8f)
    {
        bad_fields.push_back("damp_out_slow");
    }
    if (shock3.split_vel_out < -0.8f)
    {
        bad_fields.push_back("split_out");
    }
    if (shock3.damp_out_fast < -0.8f)
    {
        bad_fields.push_back("damp_out_fast");
    }
    if (shock3.short_bound < -0.8f)
    {
        bad_fields.push_back("max_contraction");
    }
    if (shock3.long_bound < -0.8f)
    {
        bad_fields.push_back("max_extension");
    }
    if (shock3.precompression < -0.8f)
    {
        bad_fields.push_back("precompression");
    }

    if (bad_fields.size() > 0)
    {
        std::stringstream msg;
        msg << "Invalid values in section 'shocks3', fields: ";
        std::list<Ogre::String>::iterator itor = bad_fields.begin();
        bool first = true;
        for( ; itor != bad_fields.end(); itor++)
        {
            msg << (first ? "" : ", ") << *itor;
            first = false;
        }

        AddMessage(Message::TYPE_ERROR, msg.str());
        return false;
    }
    return true;
}

bool Validator::CheckAnimator(RigDef::Animator & def)
{
    // Ignore non-source flags
    unsigned int source_check = def.flags;
    BITMASK_SET_0(source_check, RigDef::Animator::OPTION_SHORT_LIMIT);
    BITMASK_SET_0(source_check, RigDef::Animator::OPTION_LONG_LIMIT);
    BITMASK_SET_0(source_check, RigDef::Animator::OPTION_VISIBLE);
    BITMASK_SET_0(source_check, RigDef::Animator::OPTION_INVISIBLE);
    if (source_check != 0 || def.aero_animator.flags != 0)
    {
        return true;
    }
    else
    {
        AddMessage(Message::TYPE_ERROR, "Animator: No animator source defined");
        return false;
    }
}

bool Validator::CheckCommand(RigDef::Command2 & def)
{
    bool ok = true;

    // Caution: commandkeys are numbered from 1 to MAX_COMMANDS including.

    if (def.extend_key == COMMANDKEYID_INVALID || def.extend_key > MAX_COMMANDS)
    {
        std::stringstream msg;
        msg << "Section 'commands' or 'commands2': Invalid 'extend_key': ";
        msg << def.extend_key;
        msg << "; valid range is <1 - " << MAX_COMMANDS << "> (plus any negative number is acceptable)";
        AddMessage(Message::TYPE_ERROR, msg.str());
        ok = false;
    }

    if (def.contract_key == COMMANDKEYID_INVALID || def.contract_key > MAX_COMMANDS)
    {
        std::stringstream msg;
        msg << "Section 'commands' or 'commands2': Invalid 'contract_key': ";
        msg << def.contract_key;
        msg << "; valid range is <1 - " << MAX_COMMANDS << "> (plus any negative number is acceptable)";
        AddMessage(Message::TYPE_ERROR, msg.str());
        ok = false;
    }

    return ok;
}

bool Validator::CheckFlare2(RigDef::Flare2 & def)
{
    bool ok = true;
    
    if (def.control_number < -1 || def.control_number > 500)
    {
        std::stringstream msg;
        msg << "Wrong parameter 'control_number' (" << def.control_number << "), must be in range <-1, 500>";
        AddMessage(Message::TYPE_ERROR, msg.str());
        ok = false;
    }

    if (def.blink_delay_milis < -2 || def.blink_delay_milis > 60000)
    {
        std::stringstream msg;
        msg << "Wrong parameter 'blink_delay_milis' (" << def.blink_delay_milis << "), must be in range <-2, 60000>";
        AddMessage(Message::TYPE_ERROR, msg.str());
        ok = false;
    }

    return ok;
}

bool Validator::CheckTrigger(RigDef::Trigger & def)
{
    bool ok = true;

    bool hook_toggle = 
        BITMASK_IS_1(def.options, RigDef::Trigger::OPTION_H_LOCKS_HOOK_GROUP)
        || BITMASK_IS_1(def.options, RigDef::Trigger::OPTION_h_UNLOCKS_HOOK_GROUP);

    bool trigger_blocker = BITMASK_IS_1(def.options, RigDef::Trigger::OPTION_B_TRIGGER_BLOCKER);
    bool inv_trigger_blocker = BITMASK_IS_1(def.options, RigDef::Trigger::OPTION_A_INV_TRIGGER_BLOCKER);

    if (BITMASK_IS_0(def.options, RigDef::Trigger::OPTION_E_ENGINE_TRIGGER))
    {
        if (! trigger_blocker && ! inv_trigger_blocker && ! hook_toggle )
        {
            /* Make the full check */
            if (def.shortbound_trigger_action < 1 || def.shortbound_trigger_action > MAX_COMMANDS)
            {
                std::stringstream msg;
                msg << "Wrong parameter 'shortbound_trigger_action': " << def.shortbound_trigger_action;
                msg << "; Alloved range is <0 - " << MAX_COMMANDS << ">. ";
                msg << "Trigger deactivated.";
                AddMessage(Message::TYPE_ERROR, msg.str());
                ok = false;
            }
        }
        else if (! hook_toggle)
        {
            /* This is a Trigger-Blocker, make special check */
            if (def.shortbound_trigger_action < 0)
            {
                std::stringstream msg;
                msg << "Wrong parameter 'shortbound_trigger_action': " << def.shortbound_trigger_action;
                msg << "; Alloved range is <0 - " << MAX_COMMANDS << ">. ";
                msg << "Trigger deactivated.";
                AddMessage(Message::TYPE_ERROR, msg.str());
                ok = false;
            }
            if (def.longbound_trigger_action < 0)
            {
                std::stringstream msg;
                msg << "Wrong parameter 'longbound_trigger_action': " << def.longbound_trigger_action;
                msg << "; Alloved range is <0 - " << MAX_COMMANDS << ">. ";
                msg << "Trigger deactivated.";
                AddMessage(Message::TYPE_ERROR, msg.str());
                ok = false;
            }
        }
    }
    else
    {
        /* Engine trigger */
        if (trigger_blocker || inv_trigger_blocker || hook_toggle || BITMASK_IS_1(def.options, RigDef::Trigger::OPTION_s_CMD_NUM_SWITCH))
        {
            AddMessage(Message::TYPE_ERROR, "Wrong command-eventnumber. Engine trigger deactivated.");
            ok = false;
        }
    }

    return ok;
}

bool Validator::CheckVideoCamera(RigDef::VideoCamera & def)
{
    bool ok = true;

    /* disabled isPowerOfTwo, as it can be a renderwindow now with custom resolution */
    if (def.texture_width <= 0 || def.texture_height <= 0)
    {
        AddMessage(Message::TYPE_ERROR, "Wrong texture size definition.");
        ok = false;
    }

    if (def.min_clip_distance < 0 || def.min_clip_distance > def.max_clip_distance || def.max_clip_distance < 0)
    {
        AddMessage(Message::TYPE_ERROR, "Wrong clipping sizes definition.");
        ok = false;
    }

    if (def.camera_mode < -2 )
    {
        AddMessage(Message::TYPE_ERROR, "Camera Mode setting incorrect.");
        ok = false;
    }

    if (def.camera_role < -1 || def.camera_role >1)
    {
        AddMessage(Message::TYPE_ERROR, "Camera Role (camera, trace, mirror) setting incorrect.");
        ok = false;
    }

    return ok;
}
