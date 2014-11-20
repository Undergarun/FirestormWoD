#include "AreaTrigger.h"
#include "ScriptMgr.h"

namespace MS
{
    class AreaTriggerEntityScript : public ScriptObject
    {
    protected:

        AreaTriggerEntityScript(const char* name);

    public:
        bool IsDatabaseBound() const { return true; }

        virtual AreaTriggerEntityScript* GetAI() = 0;

        virtual ~AreaTriggerEntityScript() {}

        // Proc when AreaTrigger is created.
        virtual void OnCreate(AreaTrigger* p_AreaTrigger) {}

        // Proc when AreaTrigger is updated.
        virtual void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) {}

        // Proc when AreaTrigger is removed.
        virtual void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) {};
    };
}