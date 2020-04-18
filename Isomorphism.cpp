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
 * Isomorphism Utilities
 *
 * Last Update: 04/2020
 *
 */


#include "Isomorphism.h"


/***************************************************/


setword Isomorphism::workspace[WORKSPACE_SIZE];

int Isomorphism::n;
int Isomorphism::m;
int Isomorphism::_num_layers;
int Isomorphism::_num_nodes;
int Isomorphism::_size;
int Isomorphism::_nauty_calls;
int Isomorphism::_permute;

int Isomorphism::lab[MAXN * (MAXL + 1) + MAXL];
int Isomorphism::ptn[MAXN * (MAXL + 1) + MAXL];
int Isomorphism::orbits[MAXN * (MAXL + 1) + MAXL];

bool Isomorphism::printed[BAR_SIZE];

set *Isomorphism::gv;
graph Isomorphism::g[MAXN * (MAXL + 1) * MAXM];

bool Isomorphism::dir;

DEFAULTOPTIONS(options);
statsblk(stats);
graph mm[MAXN * (MAXL + 1) * MAXM];


/***************************************************/


void Isomorphism::initNauty(int size, int nodes, int layers, bool dir, int permute) {
    n = size;
    _size = size;
    _num_nodes = nodes;
    _num_layers = layers;
    _permute = permute;

    _nauty_calls = 0;

    m = (n + WORDSIZE - 1) / WORDSIZE;
    nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);

    dir = dir;
    options.getcanon = TRUE;
    options.writeautoms = FALSE;

    options.defaultptn = FALSE;

    if (dir) options.digraph = TRUE;
    else options.digraph = FALSE;
}


void Isomorphism::finishNauty() {
    nauty_freedyn();
    naugraph_freedyn();
    nautil_freedyn();
}


void Isomorphism::canonicalStrNauty(MultiplexGraph *mg, ReducedSubgraph *rs, int *v, char *cl, int *no, int *co) {
    int i, j, aux;

    rs->makeSupraEsu(mg, v, _permute);

    for (i = 0; i < _size; i++) {
        gv = GRAPHROW(g, i, m);
        EMPTYSET(gv, m);

        for (j = 0; j < _size; j++)
            if (rs->isConnected(i, j)) {
                ADDELEMENT (gv, j);
            }
    }

    for (i = 0; i < _size; i++) {
        lab[i] = i;
        ptn[i] = 1;
        if (_permute == 1 && i > 0 && i % _num_nodes == 0)
            ptn[i - 1] = 0;
    }
    if (_permute == 2) {
        ptn[_num_layers * _num_nodes - 1] = 0;
        ptn[(_num_layers + 1) * _num_nodes - 1] = 0;
    }
    ptn[_size - 1] = 0;


    _nauty_calls++;
    nauty(g, lab, ptn, NULL, orbits, &options, &stats, workspace, WORKSPACE_SIZE, m, n, mm);

    aux = 0;
    for (i = 0; i < n; i++) {
        gv = GRAPHROW(mm, i, m);
        for (j = 0; j < n; j++)
            cl[aux++] = ISELEMENT(gv, j) ? '1' : '0';
    }
    cl[aux] = 0;

    for (i = 0; i < _size; i++) {
        no[i] = lab[i];
        co[i] = (ptn[i]) ? 1 : 0;
    }
}


void Isomorphism::canonicalFromTree(GraphTree *gt_fase, GraphTree *gt, ReducedSubgraph *rs) {
    int i;

    char s[MAX_SG_SIZE * (MAX_LAYERS + 1) + 1];
    for (i = 0; i < BAR_SIZE; i++) printed[i] = false;
    printf("CLASSES PROGRESS      [");
    _iterateTree(0, s, gt_fase->root, gt, rs, _permute);

    //finish
    for (i = 0; i < BAR_SIZE; i++)
        if (!printed[i]) printf("#");
    printf("]\n");
}


void
Isomorphism::_iterateTree(int pos, char *s, GraphTreeNode *gt_fase_root, GraphTree *gt, ReducedSubgraph *rs, int permute) {
    int i, j, aux;

    if (gt_fase_root->zero == NULL && gt_fase_root->one == NULL && gt_fase_root->frequency > 0) {
        s[pos] = 0;
        rs->makeSupraFase(s, _num_nodes, _num_layers, permute);

        char cl[_size * _size + 1];

        for (i = 0; i < _size; i++) {
            gv = GRAPHROW(g, i, m);
            EMPTYSET(gv, m);

            for (j = 0; j < _size; j++)
                if (rs->isConnected(i, j)) {
                            ADDELEMENT (gv, j);
                }
        }

        for (i = 0; i < _size; i++) {
            lab[i] = i;
            ptn[i] = 1;
            if (permute == 1 && i > 0 && i % _num_nodes == 0)
                ptn[i - 1] = 0;
        }
        if (permute == 2) {
            ptn[_num_layers * _num_nodes - 1] = 0;
            ptn[(_num_layers + 1) * _num_nodes - 1] = 0;
        }
        ptn[_size - 1] = 0;

        //updating consola
        int total = gt->getLeafs();
        int actual = _nauty_calls * BAR_SIZE / total;
        if (!printed[actual]) {
            printed[actual] = true;
            printf("#");
            fflush(stdout);
        }

        _nauty_calls++;

        nauty(g, lab, ptn, NULL, orbits, &options, &stats, workspace, WORKSPACE_SIZE, m, n, mm);

        aux = 0;
        for (i = 0; i < n; i++) {
            gv = GRAPHROW(mm, i, m);
            for (j = 0; j < n; j++)
                cl[aux++] = ISELEMENT(gv, j) ? '1' : '0';
        }
        cl[aux] = 0;

        gt->incrementString(cl, gt_fase_root->frequency);

    } else {
        if (gt_fase_root->zero != NULL) {
            s[pos] = '0';
            _iterateTree(pos + 1, s, gt_fase_root->zero, gt, rs, permute);
        }
        if (gt_fase_root->one != NULL) {
            s[pos] = '1';
            _iterateTree(pos + 1, s, gt_fase_root->one, gt, rs, permute);
        }
    }
}