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
 * Graph Utilities
 *
 * Last Update: 04/2020
 *
 */


#include "MultiplexGraphUtils.h"
#include "Error.h"
#include "Common.h"
#include <stdio.h>
#include <vector>


void MultiplexGraphUtils::readFileTxt(MultiplexGraph *mg, const char *s, bool dir) {
    FILE *f = fopen(s, "r");
    if (!f) Error::msg(NULL);

    int i, a, b, l, size, maxN, maxL;
    vector<int> va, vb, vl;

    size = maxN = maxL = 0;
    while (fscanf(f, "%d %d %d", &l, &a, &b) == 3) {

        if (l < 1 || l > MAX_LAYERS)
            Error::msg("Invalid layer id (0 < ID < %d)", MAX_LAYERS);

        if (a < 1)
            Error::msg("Invalid node id (ID > 0)");

        if (b < 1)
            Error::msg("Invalid node id (ID > 0)");

        va.push_back(a);
        vb.push_back(b);
        vl.push_back(l);

        if (a > maxN) maxN = a;
        if (b > maxN) maxN = b;

        if (l > maxL) maxL = l;
        size++;
    }

    fclose(f);

    mg->createGraph(maxN, maxL);

    for (i = 0; i < size; i++) {
        if (va[i] == vb[i]) {
            fprintf(stderr, "Self-Loop on %d ignored\n", va[i]);
            continue; // discard self loops!
        }
        if (mg->isConnected(va[i] - 1, vb[i] - 1, vl[i] - 1))
            fprintf(stderr, "Repeated connection! (%d -- %d)(%d)\n", va[i], vb[i], vl[i]);
        else
            mg->addEdge(va[i] - 1, vb[i] - 1, vl[i] - 1);

        if (!dir) mg->addEdge(vb[i] - 1, va[i] - 1, vl[i] - 1);

    }
    va.clear();
    vb.clear();
    vl.clear();
}