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
 * Reduction Utilities
 *
 * Last Update: 04/2020
 *
 */


#include "ReducedSubgraph.h"


ReducedSubgraph::ReducedSubgraph() {
    _supra = NULL;
}


ReducedSubgraph::~ReducedSubgraph() {
    int i;
    if (_supra!= NULL) {
        for (i = 0; i < _matrix_size; i++)
            if (_supra[i] != NULL) delete[] _supra[i];
        delete[] _supra;
    }
}

void ReducedSubgraph::createSupra(int nodes, int layers, int matrix_size) {
    int i;

    _num_layers = layers;
    _num_nodes = nodes;
    _matrix_size = matrix_size;

    _supra = new bool *[_matrix_size];
    for (i = 0; i < _matrix_size; i++) {
        _supra[i] = new bool[_matrix_size];
    }
}


void ReducedSubgraph::makeSupraEsu(MultiplexGraph *mg, int *v, int permute) {
    int i, j, k, l;

    _zeroSupra();
    static bool ***adjM = mg->adjacencyMatrices();
    static int num_def_layers = mg->numDefLayers();

    static int *layers = new int[num_def_layers];
    for (i = 0; i < num_def_layers; i++) {
        layers[i] = mg->defLayer(i);
    }

    for (k = 0; k < _num_layers; k++) {
        if (num_def_layers) l = layers[k];
        else l = k;
        for (i = 0; i < _num_nodes; i++) {
            for (j = 0; j < _num_nodes; j++) {
                if (adjM[v[i]][v[j]][l]){
                    _supra[i + k * _num_nodes][j + k * _num_nodes] = true;
                }
            }
        }
    }

    _superNodes(permute);
}


void ReducedSubgraph::makeSupraFase(char *s, int nodes, int layers, int permute) {
    int i, j, k, a, b, l;
    int aux = 0;
    _zeroSupra();

    for (i = 0; i < nodes; i++) {
        k = (i + 1) * layers;
        for (j = 0; j < k * 2; j++) {
            if (s[aux++] == '1') {
                a = i;
                b = j % (i + 1);
                if (j < k) l = ((j + 1) / (i + 1));
                else l = ((j - k + 1) / (i + 1));
                l = l * nodes;

                if (j < k) _supra[a + l][b + l] = true;
                else _supra[b + l][a + l] = true;
            }
        }
    }

    _superNodes(permute);
}


void ReducedSubgraph::_zeroSupra() {
    int i, j;

    for (i = 0; i < _matrix_size; i++) {
        for (j = 0; j < _matrix_size; j++) {
            _supra[i][j] = false;
        }
    }
}


void ReducedSubgraph::_superNodes(int permute) {
    int i, k;

    for (k = 0; k < _num_layers; k++) {
        for (i = 0; i < _num_nodes; i++) {
            //super nodes for nodes
            _supra[i + k * _num_nodes][i + _num_layers * _num_nodes] = true;
            if (permute == 2) //super nodes for def_layers
                _supra[i + k * _num_nodes][k + (_num_layers + 1) * _num_nodes] = true;
        }
    }
    //super nodes for nodes
    for (k = 0; k < _num_layers; k++) {
        for (i = 0; i < _num_nodes; i++) {
            _supra[i + _num_layers * _num_nodes][i + k * _num_nodes] = true;
        }
    }
    //super nodes for def_layers
    if (permute == 2) {
        for (k = 0; k < _num_layers; k++) {
            for (i = 0; i < _num_nodes; i++) {
                _supra[k + (_num_layers + 1) * _num_nodes][i + k * _num_nodes] = true;
            }
        }
    }
}
