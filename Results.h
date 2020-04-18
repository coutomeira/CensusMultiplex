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


#ifndef _RESULTS_
#define _RESULTS_

#include "Common.h"
#include "GraphTree.h"
#include "Isomorphism.h"
#include "Timer.h"
#include "Error.h"

class Results {
private:

    static FILE **f_output;

    static bool _expand;
    static bool _one;
    static bool _dir;
    static bool _fase;

    static int _permute;
    static int _matrix_size;
    static int _sg_size;
    static int _num_pages;
    static int _num_nodes;
    static int _num_edges;
    static int _num_layers;
    static int _num_res;
    static int _per_page;
    static int _num_def_layers;

    static int *_def_layers;

    static char *_graph_file;
    static char *_folder_name;
    static char *_input_file;

    static void _write_header(int page);

    static void _write_menu(int page, int width);

public:

    static void write_consola(GraphTree *gt);

    static void write_html(GraphTree *gt, Results_struct *res, int nres);

    static void
    init_results(int sg_size, int total, int per_page, int permute, int matrix_size, int nodes, int edges,
                 int num_layers, bool fase, bool expand, bool all, bool dir, char *folder_name, int num_def_layers,
                 int *def_layers);

    static void prepare_files(char *s, int nres);

};

#endif
