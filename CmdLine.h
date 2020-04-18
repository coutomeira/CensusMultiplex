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
 * Command Line
 *
 * Last Update: 04/2020
 *
 */


#ifndef _CMDLINE_
#define _CMDLINE_

#include "MultiplexGraphMatrix.h"
#include "MultiplexGraphUtils.h"
#include "GraphTree.h"
#include "Count.h"
#include "Error.h"
#include "Common.h"
#include "Isomorphism.h"
#include "ReducedSubgraph.h"
#include "Timer.h"
#include "Results.h"


class CmdLine {
private:

    static char graph_file[MAX_BUF];
    static char folder_name[MAX_BUF];

    static int *def_layers;

    static bool dir;       //if it is directed
    static bool expand;    //representation
    static bool crescent;  //inverse order
    static bool one;       //show all in one page
    static bool esu;       //use this method
    static bool fase;      //use this method
    static bool edges;     //order by edges
    static bool method;    //if method was chosen

    static int sg_size;       //subgraph size
    static int permute;       //permutation type
    static int num_def_layers;//selected layers to consider
    static int per_page;      //subgraphs per page
    static int num_res;       //how many results
    static int matrix_size;   //supra adjacency matrix size
    static int num_layers;    //how many layers to use

    static MultiplexGraph *mg;
    static GraphTree *gt_final;
    static GraphTree *gt_fase;
    static ReducedSubgraph *rs;

    static void defaults();

    static void parse_cmdargs(int argc, char **argv);
    static void read_graph();
    static void run_esu();
    static void run_fase();
    static void write_results_html(Results_struct *res, int nres);
    static void write_results_cons();

    static int compare_results(const void *a, const void *b);
    static void layers_to_vector(char *s);

public:

    static void init(int argc, char **argv);
    static void middle();
    static void finish();
};

#endif
