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
 * Methods to count subgraphs
 *
 * Last Update: 04/2020
 *
 */


#include "Count.h"


/***************************************************/


int Count::_sg_size = 0;
int Count::_graph_nodes = 0;
int Count::_graph_layers = 0;
int Count::_graph_def_layers = 0;
int Count::_num_layers = 0;
int Count::_permute;
int *Count::_current = NULL;
int *Count::_layers = NULL;

bool Count::_fase;

MultiplexGraph *Count::_mg = NULL;
GraphTree *Count::_gt = NULL;
ReducedSubgraph *Count::_rs = NULL;


/***************************************************/


// This function is called when a subgraph is found
// It creates a canonical string to store in the gtrie
void Count::_go_esu() {
    int _length;


    _length = _sg_size * (_num_layers + 1);
    if (_permute == 2) _length += _num_layers;


    char cl[_length * _length + 1];
    int no[_length];
    int co[_length];

    Isomorphism::canonicalStrNauty(_mg, _rs, _current, cl, no, co);

    _gt->incrementString(cl);
    _gt->incrementOccurrences();
}


// This function is called when a single node is found
// It creates a canonical string to store in the gtrie
// size the current subgraph size
// gtn graph tree node to insert substring
// gtn_final final graph tree node of inserted substring (the same as return)
void Count::_go_fase(int size, GraphTreeNode *gtn, GraphTreeNode **gtn_final) {
    int _actual, k, i, l;
    int aux1 = 0; //indexes to construct label fase
    int aux2 = 0;
    char lf[size * _num_layers * 2 + 1]; //label fase

    _actual = _current[size - 1];
    aux2 = size * _num_layers;

    for (k = 0; k < _num_layers; k++) {
        if (_graph_def_layers) l = _layers[k];
        else l = k;

        for (i = 0; i < size; i++) {
            (_mg->isConnected(_actual, _current[i], l)) ? lf[aux1] = '1' : lf[aux1] = '0';
            (_mg->isConnected(_current[i], _actual, l)) ? lf[aux1 + aux2] = '1' : lf[aux1 + aux2] = '0';

            aux1++;
        }
    }
    lf[aux1 + aux2] = 0;

    //If it is final or not
    if (size == _sg_size) aux2 = 1;
    else aux2 = 0;

    _gt->addSubstring(gtn, lf, gtn_final, aux2);
    if (aux2)
        _gt->incrementOccurrences();
}


// Based on traditional ESU
void Count::_go(int n, int size, int next, int *ext, GraphTreeNode *gtn) {
    GraphTreeNode *gtn_final = NULL;
    _current[size++] = n;

    if (_fase && size <= _sg_size) {
        _go_fase(size, gtn, &gtn_final);
    }

    if (!_fase && size == _sg_size) {
        _go_esu();
    }

    //Expand the current subgraph
    if (size < _sg_size) {
        int i, j;
        int next2 = next;
        int ext2[_graph_nodes];

        for (i = 0; i < next; i++) {
            ext2[i] = ext[i];
        }

        int *v = _mg->arrayNeighboursTotal(_current[size - 1]);
        int num = _mg->numNeighboursTotal(_current[size - 1]);

        for (i = 0; i < num; i++) {
            if (v[i] <= _current[0]) continue; //condition to break
            for (j = 0; j + 1 < size; j++)
                if (_mg->isConnected(v[i], _current[j]))
                    break; //exclusive neighbours
            if (j + 1 == size) {
                ext2[next2++] = v[i];
            }
        }

        while (next2 > 0) {
            next2--;
            _go(ext2[next2], size, next2, ext2, gtn_final);
        }
    }
}


// It calls '_go' starting in each node
void Count::countSubgraphs(MultiplexGraph *mg, GraphTree *gt, ReducedSubgraph *rs, int sg, int permute, bool fase) {
    int i, v[1];

    _sg_size = sg;
    _graph_nodes = mg->numNodes();
    _graph_layers = mg->numLayers();
    _graph_def_layers = mg->numDefLayers();

    if (_graph_def_layers) {
        _num_layers = _graph_def_layers;
        _layers = new int[_graph_def_layers];
        for (i = 0; i < _graph_def_layers; i++) {
            _layers[i] = mg->defLayer(i);
        }
    }
    else _num_layers = _graph_layers;

    _current = new int[_sg_size];
    _mg = mg;
    _gt = gt;
    _rs = rs;
    _permute = permute;

    _fase = fase;

    _gt->zeroFrequency();

    printf("COUNTER PROGRESS      [");

    // This could be better if we use some heuristic
    // This status bar is not accurate
    int total = _graph_nodes;
    int actual = 0;

    bool printed[BAR_SIZE];
    for (i = 0; i < BAR_SIZE; i++) printed[i] = FALSE;

    // Starting in all nodes
    for (i = 0; i < _graph_nodes; i++) {
        actual = i * BAR_SIZE / total;
        if (!printed[actual]) {
            printed[actual] = TRUE;
            printf("#");
            fflush(stdout);
        }
        _go(i, 0, 0, v, _gt->root);
    }
    for (i = 0; i < BAR_SIZE; i++)
        if (!printed[i]) printf("#");
    printf("]\n");

    delete[] _current;
}