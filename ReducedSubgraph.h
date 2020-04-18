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


#ifndef _REDUCED_
#define _REDUCED_

#include "MultiplexGraph.h"


class ReducedSubgraph {
private:

    bool **_supra;    //supra adjacency matrix
    int _num_layers;
    int _num_nodes;
    int _matrix_size;

    void _zeroSupra();//reset the matrix to reuse

    void _superNodes(int permute);//add super nodes

public:

    ReducedSubgraph();
    ~ReducedSubgraph();

    void createSupra(int nodes, int layers, int matrix_size);
    void makeSupraEsu(MultiplexGraph *mg, int *v, int permute);
    void makeSupraFase(char *s, int nodes, int layers, int permute);
    bool isConnected(int a, int b) { return _supra[a][b]; }
};

#endif


