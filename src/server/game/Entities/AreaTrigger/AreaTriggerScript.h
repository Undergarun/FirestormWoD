#include "AreaTrigger.h"

namespace MS
{
    class AreaTriggerScript : public ScriptObject
    {
        uint32 _someId;

    private:

        void RegisterSelf();

    protected:

        AreaTriggerScript(const char* name, uint32 someId)
            : ScriptObject(name), _someId(someId)
        {
            ScriptRegistry<AreaTriggerScript>::AddScript(this);
        }

    public:

        // If a virtual function in your script type class is not necessarily
        // required to be overridden, just declare it virtual with an empty
        // body. If, on the other hand, it's logical only to override it (i.e.
        // if it's the only method in the class), make it pure virtual, by adding
        // = 0 to it.
        virtual void OnUpdate(uint32 someArg1, std::string& someArg2) { }

        // This is a pure virtual function:
        virtual void OnRemove(uint32 someArg) = 0;
    };
}