/*------------------------------------------------------------------------------------------------
|         _       _     ___                                            _ _   _      _            |
|     _ _| |_ _ _(_)___/ __| __ __ _ _ _  _ _  ___ _ _       _ __ _  _| | |_(_)_ __| |_____ __   |
|   / _` |  _| '_| / -_)__ \/ _/ _` | ' \| ' \/ -_) '_|     | '  \ || | |  _| | '_ \ / -_) \ /   |
|   \__, |\__|_| |_\___|___/\__\__,_|_||_|_||_\___|_|       |_|_|_\_,_|_|\__|_| .__/_\___/_\_\   |
|   |___/                                                                     |_|                |
|                                                                                                |
|------------------------------------------------------------------------------------------------|
|   Pedro Ribeiro ( pribeiro@dcc.fc.up.pt )    &    Andre Couto Meira ( up2014048772fc.up.pt )   |
-------------------------------------------------------------------------------------------------*/
/*
 *
 * Last Update: 04/2020
 *
 */


#ifndef _GRAPH_
#define _GRAPH_

#include "Common.h"
#include "Error.h"


class MultiplexGraph {
public:

    virtual ~MultiplexGraph() {};

    virtual void createGraph(int n, int l) = 0;  // create graph with 'n' nodes and 'l' def_layers

    virtual void zero() = 0;  // Remove all connections

    virtual void addEdge(int a, int b, int l) = 0;

    virtual bool isConnected(int a, int b, int l) = 0;  // Edge between (a and b) or (b and a) in layer l
    virtual bool isConnected(int a, int b) = 0;         // Edge between (a and b) or (b and a) in any layer

    virtual int numNodes()  = 0;  // Number of nodes in graph
    virtual int numEdges()  = 0;  // Number of edges in graph
    virtual int numLayers() = 0;  // Number of def_layers in graph

    virtual int numNeighboursTotal(int a) = 0; // Number of neighbours of node a in any layer

    virtual bool ***adjacencyMatrices() = 0;
    virtual int *arrayNeighboursTotal(int a) = 0;
    virtual void makeArrayNeighbours() = 0;
    virtual void sortNeighbours() = 0;

    virtual void createDefLayers(int n) = 0;
    virtual void addDefLayer(int l, int pos) = 0;
    virtual void makeOverlapAndNeighbours() = 0;
    virtual int numDefLayers() = 0;
    virtual int defLayer(int n) = 0;
};

#endif
