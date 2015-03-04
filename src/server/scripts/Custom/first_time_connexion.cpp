#include <ScriptPCH.h>
#include <ScriptMgr.h>
#include <Reporting/Reporter.hpp>

class ReportingOnFirstTimeConnexion : public PlayerScript
{
public:
    ReportingOnFirstTimeConnexion() : PlayerScript("ReportingOnFirstTimeConnexion") {}

    void OnLogin(Player* p_Player)
    {
        if (p_Player && p_Player->GetTotalPlayedTime() == 0)
        {
            sReporter->Report(MS::Reporting::MakeReport<MS::Reporting::ReportOpcodes::FirstTimeConnexion>::Craft(p_Player->GetSession()->GetAccountId(), p_Player->getRace(), p_Player->getClass()));
        }
    }
};

void AddSC_first_time_connexion()
{
    new ReportingOnFirstTimeConnexion();
};