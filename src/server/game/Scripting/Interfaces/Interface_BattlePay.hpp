////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifnedef CROSS
#pragma once
#ifndef SCRIPTING_INTERFACES_BATTLEPAY_HPP_INCLUDED
#define SCRIPTING_INTERFACES_BATTLEPAY_HPP_INCLUDED

#include "InterfaceBase.hpp"
#include "BattlepayMgr.h"

/// BattlePay Product Script Interface
class BattlePayProductScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_ScriptNamle : BattlePay Product Scriptname
        BattlePayProductScript(std::string p_ScriptName);

    public:
        virtual void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& p_Product)
        {
            UNUSED(p_Session);
            UNUSED(p_Product);
        }

        virtual bool CanShow(WorldSession* p_Session, Battlepay::Product const& p_Product)
        {
            UNUSED(p_Session);
            UNUSED(p_Product);

            return true;
        }

        virtual bool CanBuy(WorldSession* p_Session, Battlepay::Product const& p_Product, std::string& p_Reason)
        {
            UNUSED(p_Session);
            UNUSED(p_Product);
            UNUSED(p_Reason);

            return true;
        }
};

#endif  ///< SCRIPTING_INTERFACES_BATTLEPAY_HPP_INCLUDED
#endif
