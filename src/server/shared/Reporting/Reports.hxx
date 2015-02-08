#ifndef REPORTS_HXX
# define REPORTS_HXX

namespace MS
{
    namespace Reporting
    {
        enum ReportOpcodes
        {
            AccountCreation = 0,
            BattlegroundInstanciation = 1,
            RealmPopulation = 2,
            BossKilled = 3,
        };

        /// Craft a 'AccountCreation' report.
        /// @p_Arg0 : AccountId (expect int64)
        /// @p_Arg1 : TimeStamp (expect int64)
        /// @p_Arg2 : Referer (expect text)
        /// @p_Arg3 : Ip2Country (expect text)
        /// @p_Arg4 : MailAddress (expect text)
        template<>
        template<typename... Args>
        static std::string MakeReport<ReportOpcodes::AccountCreation>::Craft(Args... p_Args)
        {
            static_assert(sizeof... (p_Args) == 5, "The number of arguments given is not the one expected.");
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            EasyJSon::Node<std::string> l_Node;
            l_Node["Opcode"] = "0";
            l_Node["AccountId"] = std::to_string(std::get<0>(l_DeveloppedArgs));
            l_Node["TimeStamp"] = std::to_string(std::get<1>(l_DeveloppedArgs));
            l_Node["Referer"] = std::to_string(std::get<2>(l_DeveloppedArgs));
            l_Node["Ip2Country"] = std::to_string(std::get<3>(l_DeveloppedArgs));
            l_Node["MailAddress"] = std::to_string(std::get<4>(l_DeveloppedArgs));

            return l_Node.Serialize<std::ostringstream>();
        }

        /// Craft a 'BattlegroundInstanciation' report.
        /// @p_Arg0 : BattlegroundType (expect int32)
        /// @p_Arg1 : TimeStamp (expect int64)
        template<>
        template<typename... Args>
        static std::string MakeReport<ReportOpcodes::BattlegroundInstanciation>::Craft(Args... p_Args)
        {
            static_assert(sizeof... (p_Args) == 2, "The number of arguments given is not the one expected.");
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            EasyJSon::Node<std::string> l_Node;
            l_Node["Opcode"] = "1";
            l_Node["BattlegroundType"] = std::to_string(std::get<0>(l_DeveloppedArgs));
            l_Node["TimeStamp"] = std::to_string(std::get<1>(l_DeveloppedArgs));

            return l_Node.Serialize<std::ostringstream>();
        }

        /// Craft a 'RealmPopulation' report.
        /// @p_Arg0 : RealmName (expect text)
        /// @p_Arg1 : TimeStamp (expect int64)
        /// @p_Arg2 : Population (expect int32)
        template<>
        template<typename... Args>
        static std::string MakeReport<ReportOpcodes::RealmPopulation>::Craft(Args... p_Args)
        {
            static_assert(sizeof... (p_Args) == 3, "The number of arguments given is not the one expected.");
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            EasyJSon::Node<std::string> l_Node;
            l_Node["Opcode"] = "2";
            l_Node["RealmName"] = std::to_string(std::get<0>(l_DeveloppedArgs));
            l_Node["TimeStamp"] = std::to_string(std::get<1>(l_DeveloppedArgs));
            l_Node["Population"] = std::to_string(std::get<2>(l_DeveloppedArgs));

            return l_Node.Serialize<std::ostringstream>();
        }

        /// Craft a 'BossKilled' report.
        /// @p_Arg0 : BossName (expect text)
        /// @p_Arg1 : TimeStamp (expect int64)
        /// @p_Arg2 : NbKillers (expect int32)
        template<>
        template<typename... Args>
        static std::string MakeReport<ReportOpcodes::BossKilled>::Craft(Args... p_Args)
        {
            static_assert(sizeof... (p_Args) == 3, "The number of arguments given is not the one expected.");
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            EasyJSon::Node<std::string> l_Node;
            l_Node["Opcode"] = "3";
            l_Node["BossName"] = std::to_string(std::get<0>(l_DeveloppedArgs));
            l_Node["TimeStamp"] = std::to_string(std::get<1>(l_DeveloppedArgs));
            l_Node["NbKillers"] = std::to_string(std::get<2>(l_DeveloppedArgs));

            return l_Node.Serialize<std::ostringstream>();
        }
    }
}
#endif /// !REPORTS_HXX
