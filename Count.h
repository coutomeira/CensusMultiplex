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


#ifndef _ESU_
#define _ESU_

#include "MultiplexGraph.h"
#include "MultiplexGraphUtils.h"
#include "GraphTree.h"
#include "ReducedSubgraph.h"
#include "Isomorphism.h"


class Count {
private:

    static int *_current;
    static int *_layers;
    static int _graph_nodes;
    static int _graph_def_layers; //if defined a subset of def_layers
    static int _graph_layers;
    static int _num_layers;      //_graph_layers or _graph_def_layers
    static int _sg_size;
    static int _permute;

    static bool _fase;//not fase means esu

    static MultiplexGraph *_mg;
    static GraphTree *_gt;
    static ReducedSubgraph *_rs;

    static void _go(int n, int size, int next, int *ext, GraphTreeNode *gtn);
    static void _go_fase(int size, GraphTreeNode *gtn, GraphTreeNode **gtn_final);
    static void _go_esu();

public:

    static void countSubgraphs(MultiplexGraph *mg, GraphTree *gt, ReducedSubgraph *rs, int sg, int permute, bool fase);
};

#endif
