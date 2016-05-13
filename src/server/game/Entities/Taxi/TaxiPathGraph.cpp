////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "TaxiPathGraph.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "DBCStores.h"
#include "DB2Stores.h"
#include "Config.h"
#include "Util.h"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/transform_value_property_map.hpp>

/// Constructor
TaxiPathGraph::TaxiPathGraph()
{

}

/// Destructor
TaxiPathGraph::~TaxiPathGraph()
{

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Evaluate distance from player
/// @p_Player : Distance target
uint32 TaxiPathGraph::EdgeCost::EvaluateDistance(Player const* player) const
{
    uint32 l_RequireFlag = (player->GetTeam() == ALLIANCE) ? TAXI_NODE_FLAG_ALLIANCE : TAXI_NODE_FLAG_HORDE;

    if (!(To->Flags & l_RequireFlag))
        return std::numeric_limits<uint16>::max();

    //if (To->ConditionID && !player->MeetsCondition(To->ConditionID))
    //    return std::numeric_limits<uint16>::max();

    return Distance;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Initialize the path graph
void TaxiPathGraph::Initialize()
{
    if (GetVertexCount() > 0)
        return;

    std::vector<std::pair<edge, EdgeCost>> l_Edges;

    /// Initialize here
    for (uint32 l_I = 0; l_I < sTaxiPathStore.GetNumRows(); ++l_I)
    {
        TaxiPathEntry const* l_Path = sTaxiPathStore.LookupEntry(l_I);

        if (!l_Path)
            continue;

        TaxiNodesEntry const* l_From    = sTaxiNodesStore.LookupEntry(l_Path->From);
        TaxiNodesEntry const* l_To      = sTaxiNodesStore.LookupEntry(l_Path->To);

        if (l_From && l_To && l_From->Flags & (TAXI_NODE_FLAG_ALLIANCE | TAXI_NODE_FLAG_HORDE) && l_To->Flags & (TAXI_NODE_FLAG_ALLIANCE | TAXI_NODE_FLAG_HORDE))
            AddVerticeAndEdgeFromNodeInfo(l_From, l_To, l_Path->ID, l_Edges);
    }

    /// Create graph
    m_Graph = Graph(GetVertexCount());
    WeightMap l_WeightMap = boost::get(boost::edge_weight, m_Graph);

    for (std::size_t l_J = 0; l_J < l_Edges.size(); ++l_J)
    {
        edge_descriptor l_EdgeDescriptor = boost::add_edge(l_Edges[l_J].first.first, l_Edges[l_J].first.second, m_Graph).first;
        l_WeightMap[l_EdgeDescriptor] = l_Edges[l_J].second;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Get complete node route
/// @p_From : Origin
/// @p_To : Destination
/// @p_Player : Target player
/// @p_ShortestPath : Optimized path output
std::size_t TaxiPathGraph::GetCompleteNodeRoute(TaxiNodesEntry const* p_From, TaxiNodesEntry const* p_To, Player const* p_Player, std::vector<uint32>& p_ShortestPath)
{
    /// Information about node algorithm from client
    /// Since client does not give information about *ALL* nodes you have to pass by when going from sourceNodeID to destinationNodeID, we need to use Dijkstra algorithm.
    /// Examining several paths I discovered the following algorithm:
    /// * If destinationNodeID has is the next destination, connected directly to sourceNodeID, then, client just pick up this route regardless of distance
    /// * else we use dijkstra to find the shortest path.
    /// * When early landing is requested, according to behavior on retail, you can never end in a node you did not discovered before

    /// Find if we have a direct path
    uint32 l_PathID, goldCost;
    sObjectMgr->GetTaxiPath(p_From->ID, p_To->ID, l_PathID, goldCost);

    if (l_PathID)
        p_ShortestPath = { p_From->ID, p_To->ID };
    else
    {
        p_ShortestPath.clear();
        std::vector<vertex_descriptor> l_Properties(boost::num_vertices(m_Graph));

        boost::dijkstra_shortest_paths
        (
           m_Graph, GetVertexIDFromNodeID(p_From),
           boost::predecessor_map(boost::make_iterator_property_map(l_Properties.begin(), boost::get(boost::vertex_index, m_Graph))).weight_map
           (
               boost::make_transform_value_property_map
               (
                   [p_Player](EdgeCost const& p_EdgeCost)
                   {
                       return p_EdgeCost.EvaluateDistance(p_Player);
                   },
                   boost::get(boost::edge_weight, m_Graph)
               )
           )
        );

        /// Found a path to the goal
        for (vertex_descriptor l_VertexDescriptor = GetVertexIDFromNodeID(p_To);; l_VertexDescriptor = l_Properties[l_VertexDescriptor])
        {
            p_ShortestPath.push_back(GetNodeIDFromVertexID(l_VertexDescriptor));

            if (l_VertexDescriptor == l_Properties[l_VertexDescriptor])
                break;
        }

        std::reverse(p_ShortestPath.begin(), p_ShortestPath.end());
    }

    return p_ShortestPath.size();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Fill vertices and edge info from taxi node info
/// @p_From   : Origin
/// @p_To     : Destination
/// @p_PathID : Taxi path ID
/// @p_Edges  : Output edges
void TaxiPathGraph::AddVerticeAndEdgeFromNodeInfo(TaxiNodesEntry const* p_From, TaxiNodesEntry const* p_To, uint32 p_PathId, std::vector<std::pair<edge, EdgeCost>>& p_Edges)
{
    if (p_From != p_To)
    {
        vertex_descriptor p_FromVertexID = CreateVertexFromFromNodeInfoIfNeeded(p_From);
        vertex_descriptor p_ToVertexID = CreateVertexFromFromNodeInfoIfNeeded(p_To);

        float l_TotalDist = 0.0f;

        TaxiPathNodeList const& l_Nodes = sTaxiPathNodesByPath[p_PathId];
        if (l_Nodes.size() < 2)
        {
            p_Edges.push_back(std::make_pair(edge(p_FromVertexID, p_ToVertexID), EdgeCost{ p_To, 0xFFFF }));
            return;
        }

        std::size_t l_Last = l_Nodes.size();
        std::size_t l_First = 0;
        if (l_Nodes.size() > 2)
        {
            --l_Last;
            ++l_First;
        }

        for (std::size_t l_I = l_First + 1; l_I < l_Last; ++l_I)
        {
            if (l_Nodes[l_I - 1]->Flags & TAXI_PATH_NODE_FLAG_TELEPORT)
                continue;

            uint32 l_Map1;
            float l_Pos1X;
            float l_Pos1Y;

            uint32 l_Map2;
            float l_Pos2X;
            float l_Pos2Y;

            DeterminaAlternateMapPosition(l_Nodes[l_I - 1]->MapID, l_Nodes[l_I - 1]->x, l_Nodes[l_I - 1]->y, l_Nodes[l_I - 1]->z, &l_Map1, &l_Pos1X, &l_Pos1Y);
            DeterminaAlternateMapPosition(l_Nodes[l_I]->MapID, l_Nodes[l_I]->x, l_Nodes[l_I]->y, l_Nodes[l_I]->z, &l_Map2, &l_Pos2X, &l_Pos2Y);

            if (l_Map1 != l_Map2)
                continue;

            l_TotalDist += std::sqrt(std::pow(l_Pos2X - l_Pos1X, 2) +
                                     std::pow(l_Pos2Y - l_Pos1Y, 2) +
                                     std::pow(l_Nodes[l_I]->z - l_Nodes[l_I - 1]->z, 2));
        }

        uint32 l_Dist = uint32(l_TotalDist);

        if (l_Dist > 0xFFFF)
            l_Dist = 0xFFFF;

        p_Edges.push_back(std::make_pair(edge(p_FromVertexID, p_ToVertexID), EdgeCost{ p_To, l_Dist }));
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Create a vertex from node info
/// @p_Node : Node info
TaxiPathGraph::vertex_descriptor TaxiPathGraph::CreateVertexFromFromNodeInfoIfNeeded(TaxiNodesEntry const* p_Node)
{
    //Check if we need a new one or if it may be already created
    if (m_Vertices.size() <= p_Node->LearnableIndex)
        m_Vertices.resize(p_Node->LearnableIndex + 1);

    m_Vertices[p_Node->LearnableIndex] = p_Node;

    return p_Node->LearnableIndex;
}

/// Get vertex from node ID
/// @p_Node : Target node
TaxiPathGraph::vertex_descriptor TaxiPathGraph::GetVertexIDFromNodeID(TaxiNodesEntry const* p_Node)
{
    return p_Node->LearnableIndex;
}

/// Get node ID from vertex ID
/// @p_VertexID : vertex ID
uint32 TaxiPathGraph::GetNodeIDFromVertexID(vertex_descriptor p_VertexID)
{
    if (p_VertexID < m_Vertices.size())
        return m_Vertices[p_VertexID]->ID;

    return std::numeric_limits<uint32>::max();
}

/// Get vertex count
std::size_t TaxiPathGraph::GetVertexCount()
{
    /// So we can use this function for readability, we define either max defined vertices or already loaded in graph count
    return std::max(boost::num_vertices(m_Graph), m_Vertices.size());
}