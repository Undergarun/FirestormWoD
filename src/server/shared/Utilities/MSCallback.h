#ifndef _MILLENIUMSTUDIO_CALLBACK_H
#define _MILLENIUMSTUDIO_CALLBACK_H

#include "Common.h"

namespace MS
{
    namespace Util
    {
        typedef std::function<void()> FuncCallBack;

        enum class CallBackState : uint8
        {
            Waiting,
            Ready
        };

        struct Callback
        {
            Callback(FuncCallBack p_Callback)
            {
                m_CallBack = p_Callback;
                m_State    = CallBackState::Waiting;
            }

            FuncCallBack   m_CallBack;
            CallBackState  m_State;
        };
    }
}

#endif