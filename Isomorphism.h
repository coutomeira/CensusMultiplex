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


#ifndef _ISOMORPHISM_
#define _ISOMORPHISM_

#include "Common.h"
#include "MultiplexGraph.h"
#include "ReducedSubgraph.h"
#include "GraphTree.h"

#define MAXN MAX_SG_SIZE
#define MAXL MAX_LAYERS
#define WORKSPACE_SIZE MAXN*160

#include "nauty27rc2/nauty.h"


class Isomorphism {
private:

    static bool dir;

    static setword workspace[WORKSPACE_SIZE];
    static int n, m, lab[MAXN * (MAXL + 1) + MAXL], ptn[MAXN * (MAXL + 1) + MAXL], orbits[MAXN * (MAXL + 1) + MAXL];

    static int _num_layers;
    static int _num_nodes;
    static int _size;
    static int _permute;

    static int _nauty_calls;

    static set *gv;
    static graph g[MAXN * (MAXL + 1) * MAXM];

    static bool printed[BAR_SIZE];

    static void
    _iterateTree(int pos, char *s, GraphTreeNode *gt_fase_root, GraphTree *gt, ReducedSubgraph *rs, int permute);

public:

    static void initNauty(int size, int nodes, int layers, bool dir, int permute);

    static void finishNauty();

    static void
    canonicalStrNauty(MultiplexGraph *mg, ReducedSubgraph *rs, int *v, char *cl, int *no, int *co);

    static int getNautyCalls() { return _nauty_calls; }

    static void canonicalFromTree(GraphTree *gt_fase, GraphTree *gt, ReducedSubgraph *rs);
};

#endif


