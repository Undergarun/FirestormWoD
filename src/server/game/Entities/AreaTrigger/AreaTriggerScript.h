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

        // Proc when AreaTrigger is updated.
        virtual void OnUpdate(AreaTrigger* p_AreaTrigger) { }

        // Proc when AreaTrigger is removed.
        virtual void OnRemove(AreaTrigger* p_AreaTrigger) {};
    };
}