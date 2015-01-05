#ifndef REPORTS_HXX
# define REPORTS_HXX

namespace MS
{
    namespace Reporting
    {
        enum ReportOpcodes
        {
            CharacterCreation = 0,
        };

        /// Craft a 'CharacterCreation' report.
        /// @p_Arg0 : AccountId (expect int64)
        /// @p_Arg1 : TimeStamp (expect int64)
        template<>
        template<typename... Args>
        static std::string MakeReport<ReportOpcodes::CharacterCreation>::Craft(Args... p_Args)
        {
            static_assert(sizeof... (p_Args) == 2, "The number of arguments given is not the one expected.");
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            EasyJSon::Node<std::string> l_Node;
            l_Node["Opcode"] = "0";
            l_Node["AccountId"] = std::to_string(std::get<0>(l_DeveloppedArgs));
            l_Node["TimeStamp"] = std::to_string(std::get<1>(l_DeveloppedArgs));

            return l_Node.Serialize<std::ostringstream>();
        }
    }
}
#endif /// !REPORTS_HXX
