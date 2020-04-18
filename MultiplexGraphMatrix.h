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


#ifndef _GRAPHMATRIX_
#define _GRAPHMATRIX_

#include "MultiplexGraph.h"


class MultiplexGraphMatrix : public MultiplexGraph {
private:

    int _num_nodes;
    int _num_edges;
    int _num_layers;

    vector<int> **_adjOut;
    vector<int> **_adjIn;
    vector<int> **_nei;

    vector<int> *_nei_all_layers;

    int *_num_edges_layer;

    int **_in;
    int **_out;
    int **_num_nei;
    int *_num_nei_all_layers;
    int **_array_nei_all_layers;

    int *_def_layers;
    int _num_def_layers;

    bool ***_adjM;
    bool **_adjMOverlap;

    void _init();
    void _delete();
    void _addNeighbours(int a, int b, int l);

public:

    MultiplexGraphMatrix();

    ~MultiplexGraphMatrix();

    void createGraph(int n, int l);

    void zero();

    int numNodes() { return _num_nodes; }
    int numEdges() { return _num_edges; }
    int numLayers() { return _num_layers; }

    void addEdge(int a, int b, int l) { _adjM[a][b][l] = true; }

    bool isConnected(int a, int b, int l) { return _adjM[a][b][l]; }
    bool isConnected(int a, int b) { return _adjMOverlap[a][b] || _adjMOverlap[b][a]; }

    int numNeighboursTotal(int a) { return _num_nei_all_layers[a]; }


    bool ***adjacencyMatrices() { return _adjM; }
    int *arrayNeighboursTotal(int a) { return _array_nei_all_layers[a]; }
    void makeArrayNeighbours();
    void sortNeighbours();

    virtual void createDefLayers(int n);
    virtual void addDefLayer(int l, int pos);
    void makeOverlapAndNeighbours();
    virtual int numDefLayers() { return _num_def_layers; }
    virtual int defLayer(int n) { return _def_layers[n]; }
};

#endif


