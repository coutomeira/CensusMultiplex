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
 * Graphs Implementation with Adj. Matrix and Adj. List
 *
 * Last Update: 04/2020
 *
 */


#include "MultiplexGraphMatrix.h"
#include "Common.h"
#include <stdio.h>
#include <algorithm>


MultiplexGraphMatrix::MultiplexGraphMatrix() {
    _init();
}


MultiplexGraphMatrix::~MultiplexGraphMatrix() {
    _delete();
}


void MultiplexGraphMatrix::_init() {
    _num_nodes = _num_edges = _num_layers = _num_def_layers = 0;

    _adjM = NULL;
    _adjMOverlap = NULL;

    _in = NULL;
    _out = NULL;
    _num_nei = NULL;

    _num_edges_layer = NULL;
    _num_nei_all_layers = NULL;

    _adjOut = NULL;
    _adjIn = NULL;
    _nei = NULL;
    _nei_all_layers = NULL;

    _def_layers = NULL;
    _array_nei_all_layers = NULL;
}


void MultiplexGraphMatrix::_delete() {
    int i, j;

    if (_adjMOverlap != NULL) {
        for (i = 0; i < _num_nodes; i++)
            if (_adjMOverlap[i] != NULL) delete[] _adjMOverlap[i];
        delete[] _adjMOverlap;
    }
    if (_adjM != NULL) {
        for (i = 0; i < _num_nodes; i++) {
            for (j = 0; j < _num_nodes; j++) {
                if (_adjM[i][j] != NULL)
                    delete[] _adjM[i][j];
            }
            if (_adjM[i] != NULL)
                delete[] _adjM[i];
        }
        delete[] _adjM;
    }
    if (_in != NULL) {
        for (i = 0; i < _num_nodes; i++)
            if (_in[i] != NULL) delete[] _in[i];
        delete[] _in;
    }
    if (_out != NULL) {
        for (i = 0; i < _num_nodes; i++)
            if (_out[i] != NULL) delete[] _out[i];
        delete[] _out;
    }
    if (_num_nei != NULL) {
        for (i = 0; i < _num_nodes; i++)
            if (_num_nei[i] != NULL) delete[] _num_nei[i];
        delete[] _num_nei;
    }

    if (_num_edges_layer != NULL) delete[] _num_edges_layer;

    if (_num_nei_all_layers != NULL) delete[] _num_nei_all_layers;

    if (_def_layers != NULL) delete[] _def_layers;


    if (_adjIn != NULL) {
        for (i = 0; i < _num_nodes; i++) {
            for (j = 0; j < _num_layers; j++) {
                _adjIn[i][j].clear();
            }
            if (_adjIn[i] != NULL) delete[] _adjIn[i];
        }
        delete[] _adjIn;
    }

    if (_adjOut != NULL) {
        for (i = 0; i < _num_nodes; i++) {
            for (j = 0; j < _num_layers; j++) {
                _adjOut[i][j].clear();
            }
            if (_adjOut[i] != NULL) delete[] _adjOut[i];

        }
        delete[] _adjOut;
    }

    if (_nei != NULL) {
        for (i = 0; i < _num_nodes; i++) {
            if (_nei[i] != NULL) delete[] _nei[i];

        }
        delete[] _nei;
    }

    if (_nei_all_layers != NULL) {
        for (i = 0; i < _num_nodes; i++) {
            _nei_all_layers[i].clear();
        }
        delete[] _nei_all_layers;
    }

    if (_array_nei_all_layers != NULL) {
        for (i = 0; i < _num_nodes; i++) {
            if (_array_nei_all_layers[i] != NULL)
                delete[] _array_nei_all_layers[i];
        }
        delete[] _array_nei_all_layers;
    }
}


void MultiplexGraphMatrix::zero() {
    int i, j, k;
    _num_edges = 0;

    for (i = 0; i < _num_nodes; i++) {
        for (k = 0; k < _num_layers; k++) {
            _in[i][k] = 0;
            _out[i][k] = 0;
            _num_nei[i][k] = 0;
            _adjIn[i][k].clear();
            _adjOut[i][k].clear();
            _nei[i][k].clear();
        }

        _nei_all_layers[i].clear();
        _num_nei_all_layers[i] = 0;

        for (j = 0; j < _num_nodes; j++) {
            _adjMOverlap[i][j] = false;
            for (k = 0; k < _num_layers; k++)
                _adjM[i][j][k] = false;
        }
    }
    for (i = 0; i < _num_layers; i++)
        _num_edges_layer[i] = 0;
}


void MultiplexGraphMatrix::createGraph(int n, int l) {
    int i, j;

    _num_nodes = n;
    _num_layers = l;

    _delete();

    _adjMOverlap = new bool *[n];
    _adjM = new bool **[n];
    _adjIn = new vector<int> *[n];
    _adjOut = new vector<int> *[n];
    _nei = new vector<int> *[n];
    _nei_all_layers = new vector<int>[n];
    _num_nei_all_layers = new int[n];

    for (i = 0; i < n; i++) {
        _adjMOverlap[i] = new bool[n];
        _adjM[i] = new bool *[n];

        _adjIn[i] = new vector<int>[l];
        _adjOut[i] = new vector<int>[l];
        _nei[i] = new vector<int>[l];
    }

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            _adjM[i][j] = new bool[l];

    _in = new int *[n];
    _out = new int *[n];
    _num_nei = new int *[n];
    _num_edges_layer = new int[l];

    for (i = 0; i < n; i++) {
        _in[i] = new int[l];
        _out[i] = new int[l];
        _num_nei[i] = new int[l];
    }

    zero();
}


void MultiplexGraphMatrix::sortNeighbours() {
    int i;
    for (i = 0; i < _num_nodes; i++)
        sort(_nei_all_layers[i].begin(), _nei_all_layers[i].begin() + _nei_all_layers[i].size());
}


void MultiplexGraphMatrix::makeArrayNeighbours() {
    int i, j;
    vector<int>::iterator ii;

    _array_nei_all_layers = new int *[_num_nodes];

    for (i = 0; i < _num_nodes; i++) {
        _array_nei_all_layers[i] = new int[_nei_all_layers[i].size()];
        for (ii = _nei_all_layers[i].begin(), j = 0; ii != _nei_all_layers[i].end(); ++ii, j++) {
            _array_nei_all_layers[i][j] = *ii;
        }
        _nei_all_layers[i].clear();
    }
}


void MultiplexGraphMatrix::makeOverlapAndNeighbours() {
    int i, j, k, layer, num;

    for (i = 0; i < _num_nodes; i++)
        for (j = 0; j < _num_nodes; j++)
            _adjMOverlap[i][j] = false;

    if (_num_def_layers) num = _num_def_layers;
    else num = _num_layers;

    for (k = 0; k < num; k++) {
        if (_num_def_layers) layer = _def_layers[k];
        else layer = k;

        for (i = 0; i < _num_nodes; i++)
            for (j = 0; j < _num_nodes; j++)
                if (_adjM[i][j][layer]) {
                    _num_edges++;
                    _num_edges_layer[layer]++;
                    _adjMOverlap[i][j] = true;
                    _addNeighbours(i, j, layer);
                }
    }
}


void MultiplexGraphMatrix::_addNeighbours(int a, int b, int l) {
    _adjOut[a][l].push_back(b);
    _adjIn[b][l].push_back(a);

    _out[a][l]++;
    _in[b][l]++;

    int count;
    count = std::count(_nei_all_layers[a].begin(), _nei_all_layers[a].end(), b);
    if (!count) {
        _nei_all_layers[a].push_back(b);
        _num_nei_all_layers[a]++;
    }

    count = std::count(_nei_all_layers[b].begin(), _nei_all_layers[b].end(), a);
    if (!count) {
        _nei_all_layers[b].push_back(a);
        _num_nei_all_layers[b]++;
    }

    count = std::count(_nei[a][l].begin(), _nei[a][l].end(), b);
    if (!count) {
        _nei[a][l].push_back(b);
        _num_nei[a][l]++;
    }

    count = std::count(_nei[b][l].begin(), _nei[b][l].end(), a);
    if (!count) {
        _nei[b][l].push_back(a);
        _num_nei[b][l]++;
    }
}


void MultiplexGraphMatrix::createDefLayers(int n) {
    _num_def_layers = n;
    _def_layers = new int[n];
}


void MultiplexGraphMatrix::addDefLayer(int l, int pos) {
    _def_layers[pos] = l;
}