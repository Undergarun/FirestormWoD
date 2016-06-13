////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TAXIPATHGRAPH_H_INCLUDED
# define TAXIPATHGRAPH_H_INCLUDED

# include "Object.h"
# include "Define.h"
# include <boost/graph/adjacency_list.hpp>

class Player;
struct TaxiNodesEntry;

class TaxiPathGraph
{
    private:
        /// Constructor
        TaxiPathGraph();
        /// Destructor
        ~TaxiPathGraph();

        /// Disable copy constructor
        TaxiPathGraph(TaxiPathGraph const&) = delete;
        /// Disable copy operator
        TaxiPathGraph& operator=(TaxiPathGraph const&) = delete;

    private:
        /// Edge cost structure
        struct EdgeCost
        {
            /// Destination
            TaxiNodesEntry const* To;
            /// Distance to "To"
            uint32 Distance;

            /// Evaluate distance from player
            /// @p_Player : Distance target
            uint32 EvaluateDistance(Player const* p_Player) const;
        };

        typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::property<boost::vertex_index_t, uint32>, boost::property<boost::edge_weight_t, EdgeCost>> Graph;
        typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
        typedef Graph::vertex_descriptor vertex_descriptor;
        typedef Graph::edge_descriptor edge_descriptor;
        typedef std::pair<uint32, uint32> edge;

    public:
        /// Get singleton
        static TaxiPathGraph& Instance()
        {
            static TaxiPathGraph instance;
            return instance;
        }

    public:
        /// Initialize the path graph
        void Initialize();

        /// Get complete node route
        /// @p_From : Origin
        /// @p_To : Destination
        /// @p_Player : Target player
        /// @p_ShortestPath : Optimized path output
        std::size_t GetCompleteNodeRoute(TaxiNodesEntry const* p_From, TaxiNodesEntry const* p_To, Player const* p_Player, std::vector<uint32>& p_ShortestPath);

        /// Fill vertices and edge info from taxi node info
        /// @p_From   : Origin
        /// @p_To     : Destination
        /// @p_PathID : Taxi path ID
        /// @p_Edges  : Output edges
        void AddVerticeAndEdgeFromNodeInfo(TaxiNodesEntry const* p_From, TaxiNodesEntry const* p_To, uint32 p_PathId, std::vector<std::pair<edge, EdgeCost>>& p_Edges);

        /// Create a vertex from node info
        /// @p_Node : Node info
        vertex_descriptor CreateVertexFromFromNodeInfoIfNeeded(TaxiNodesEntry const* node);
        /// Get vertex from node ID
        /// @p_Node : Target node
        vertex_descriptor GetVertexIDFromNodeID(TaxiNodesEntry const* p_Node);
        /// Get node ID from vertex ID
        /// @p_VertexID : vertex ID
        uint32 GetNodeIDFromVertexID(vertex_descriptor p_VertexID);
        /// Get vertex count
        std::size_t GetVertexCount();

    private:
        /// Graph
        Graph m_Graph;
        /// Vertices
        std::vector<TaxiNodesEntry const*> m_Vertices;

};

/// Taxi graph singleton
#define sTaxiPathGraph TaxiPathGraph::Instance()

#endif  ///< TAXIPATHGRAPH_H_INCLUDED
