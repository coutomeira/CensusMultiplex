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
 * Command Line Client Functions
 *
 * Last Update: 04/2020
 *
 */


#include "CmdLine.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <limits.h>


/***************************************************/


char CmdLine::graph_file[MAX_BUF];
char CmdLine::folder_name[MAX_BUF];

int *CmdLine::def_layers;

bool CmdLine::dir;
bool CmdLine::expand;
bool CmdLine::crescent;
bool CmdLine::one;
bool CmdLine::esu;
bool CmdLine::fase;
bool CmdLine::edges;
bool CmdLine::method;

int CmdLine::per_page;
int CmdLine::num_res;
int CmdLine::permute;
int CmdLine::sg_size;
int CmdLine::num_def_layers;
int CmdLine::num_layers;
int CmdLine::matrix_size;

MultiplexGraph *CmdLine::mg;
GraphTree *CmdLine::gt_final;
GraphTree *CmdLine::gt_fase;
ReducedSubgraph *CmdLine::rs;


/***************************************************/


/**
 * Read original graph for computation
 */
void CmdLine::read_graph() {
    if (!strcmp(graph_file, INVALID_FILE))
        Error::msg("No graph file specified");
    mg = new MultiplexGraphMatrix();
    MultiplexGraphUtils::readFileTxt(mg, graph_file, dir);
}


//Run esu
void CmdLine::run_esu() {
    Timer::start(1);
    Count::countSubgraphs(mg, gt_final, rs, sg_size, permute, false);
    Timer::stop(1);
}


//Run fase
void CmdLine::run_fase() {
    Timer::start(1);
    Count::countSubgraphs(mg, gt_fase, rs, sg_size, permute, true);
    Timer::stop(1);
    //This is needed to maintain the correct values
    gt_final->setOccurrences(gt_fase->getOccurrences());
    gt_final->setLeafs(gt_fase->countGraphs());
    Timer::start(2);
    Isomorphism::canonicalFromTree(gt_fase, gt_final, rs);
    Timer::stop(2);
}


//Write to consola
void CmdLine::write_results_cons() {
    Results::write_consola(gt_final);
}


//Write to file HTML
void CmdLine::write_results_html(Results_struct *res, int nres) {
    Results::prepare_files(graph_file, nres);
    Results::write_html(gt_final, res, nres);
}


// Load default values
void CmdLine::defaults() {
    strcpy(graph_file, INVALID_FILE);
    strcpy(folder_name, DEFAULT_FOLDER);

    dir = false;
    expand = false;
    crescent = false;
    one = false;
    esu = false;
    fase = true;
    edges = false;
    method = false;

    sg_size = 2;
    permute = 1;
    per_page = 50;
    num_res = -1;
    num_def_layers = 0;
}


// Compare two different subgraphs by freq (sorting)
int CmdLine::compare_results(const void *a, const void *b) {
    Results_struct *r1 = (Results_struct *) a;
    Results_struct *r2 = (Results_struct *) b;

    if (edges) {
        int size_s = 0;
        int l1 = 0;
        int l2 = 0;
        while (r1->s[size_s] != '\0') {
            if (r1->s[size_s] == '1') {
                l1++;
            }
            if (r2->s[size_s] == '1') {
                l2++;
            }
            size_s++;
        }
        if (l1 > l2) {
            if (crescent) return 1;
            else return -1;
        } else if (l2 > l1) {
            if (crescent) return -1;
            else return 1;
        } else {
            if (r1->freq > r2->freq) {
                if (crescent) return 1;
                else return -1;
            }
            if (r1->freq < r2->freq) {
                if (crescent) return -1;
                else return 1;
            }
        }
    } else {
        if (r1->freq > r2->freq) {
            if (crescent) return 1;
            else return -1;
        }
        if (r1->freq < r2->freq) {
            if (crescent) return -1;
            else return 1;
        }
    }
    return 0;
}


// Convert string to vector
void CmdLine::layers_to_vector(char *s) {
    int size_l, size_s, i;
    bool valid;
    int num, digit;
    char c;

    size_s = 0;
    size_l = 1;
    while (s[size_s] != '\0') {
        size_s++;
        if (s[size_s] == ',') {
            size_l++;
        }
    }
    if ((s[0] != '[') || (s[size_s - 1] != ']'))
        Error::msg("Invalid format of def_layers (check README)");

    s[0] = '0';
    s[size_s - 1] = ',';

    num_def_layers = size_l;
    def_layers = new int[num_def_layers];

    size_l = 0;
    valid = false;
    num = 0;
    for (i = 1; i < size_s; i++) {
        c = s[i];
        if (c == ',') {
            if (!valid)
                Error::msg("Invalid format of def_layers (check README)");
            else {
                def_layers[size_l] = num - 1;
                size_l++;
                num = 0;
                valid = false;
            }
        } else if (c >= '0' && c <= '9') {
            digit = c - '0';
            num = num * 10;
            num += digit;
            valid = true;
        } else
            Error::msg("Invalid format of def_layers (check README)");
    }
}


// Parse all command line arguments
void CmdLine::parse_cmdargs(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {

        // Graph file
        if (!strcmp("-g", argv[i]) || !strcmp("--graph", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            strcpy(graph_file, argv[i]);
        }

            // Directed graph
        else if (!strcmp("-d", argv[i]) || !strcmp("--dir", argv[i]) || !strcmp("--directed", argv[i])) {
            dir = true;
        }

            // Use method ESU
        else if (!strcmp("--esu", argv[i])) {
            if (!method) {
                esu = true;
                fase = false;
                method = true;
            } else {
                esu = false;
                fase = false;
            }
        }

            // Use method FASE
        else if (!strcmp("--fase", argv[i])) {
            if (!method) method = true;
            else {
                esu = false;
                fase = false;
            }
        }

            // Size of subgraphs to consider
        else if (!strcmp("-s", argv[i]) || !strcmp("--size", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            sg_size = atoi(argv[i]);
        }

            // Permute vertices and def_layers or only vertices
        else if (!strcmp("-i", argv[i]) || !strcmp("--isomorphism", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            permute = atoi(argv[i]);
        }

            // Set of def_layers to be considered
        else if (!strcmp("-l", argv[i]) || !strcmp("--def_layers", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            layers_to_vector(argv[i]);
        }

            // Expand when printing def_layers
        else if (!strcmp("-e", argv[i]) || !strcmp("--expand", argv[i])) {
            expand = true;
        }

            // Order in crescent order
        else if (!strcmp("-c", argv[i]) || !strcmp("--crescent", argv[i])) {
            crescent = true;
        }

            // Order by edges
        else if (!strcmp("--edges", argv[i])) {
            edges = true;
        }

            // Put results inside folder HTML
        else if (!strcmp("-f", argv[i]) || !strcmp("--folder", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            strcpy(folder_name, argv[i]);
        }

            // Number of subgraphs per page
        else if (!strcmp("-p", argv[i]) || !strcmp("--page", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            per_page = atoi(argv[i]);
        }

            // Number of results to show
        else if (!strcmp("-r", argv[i]) || !strcmp("--results", argv[i])) {
            i++;
            if (i >= argc) Error::msg("Invalid command argument (check README)");
            num_res = atoi(argv[i]);
        }

            // Show all the results in one HTML page
        else if (!strcmp("--one", argv[i]) || !strcmp("--all", argv[i])) {
            one = true;
        }

            // Invalid argument
        else {
            Error::msg("Invalid command argument (check README)");
        }
    }
}


// Initialize everything
void CmdLine::init(int argc, char **argv) {
    defaults();
    parse_cmdargs(argc, argv);
}


// Continue executing
void CmdLine::middle() {
    int i = 0;

    if (esu && fase)
        Error::msg("Select only one method ( esu v fase )");

    if (esu || fase) {

        Timer::start(0);
        read_graph();

        if (num_def_layers) num_layers = num_def_layers; //only selected def_layers
        else num_layers = mg->numLayers(); //all of them

        if (permute == 1) {
            matrix_size = sg_size * (num_layers + 1);
        } else if (permute == 2) {
            matrix_size = sg_size * (num_layers + 1) + num_layers;
        } else {
            matrix_size = 0; //Because of -wall, need to be initialized
            Error::msg("Invalid permutation mode ( 1 v 2 )");
        }

        if (num_def_layers) {
            int value;
            //Verify if def_layers ids are valids
            for (i = 0; i < num_def_layers; i++) {
                value = def_layers[i];
                if (value < 0 || value >= mg->numLayers())
                    Error::msg("Invalid layer id ( 1 <= ID <= %d )", mg->numLayers());
            }
            mg->createDefLayers(num_def_layers);
            for (i = 0; i < num_def_layers; i++)
                mg->addDefLayer(def_layers[i], i);
        }

        if (sg_size < MIN_SG_SIZE || sg_size > MAX_SG_SIZE)
            Error::msg("Invalid subgraph size ( %d <= SIZE <= %d )", MIN_SG_SIZE, MAX_SG_SIZE);


        gt_final = new GraphTree();
        if (fase) {
            gt_fase = new GraphTree();
        }
        rs = new ReducedSubgraph();

        mg->makeOverlapAndNeighbours();
        mg->sortNeighbours();
        mg->makeArrayNeighbours();

        Isomorphism::initNauty(matrix_size, sg_size, num_layers, dir, permute);
        rs->createSupra(sg_size, num_layers, matrix_size);

        Results::init_results(sg_size, num_res, per_page, permute, matrix_size, mg->numNodes(), mg->numEdges(),
                              mg->numLayers(), fase, expand, one, dir, folder_name, num_def_layers, def_layers);

        Timer::stop(0);


        if (esu) run_esu();
        if (fase) run_fase();

        Timer::start(3);
        vector<Results_struct> res;
        mapStringInt m_original;
        gt_final->populateMap(&m_original, matrix_size);

        mapStringInt::const_iterator iiend = m_original.end();
        mapStringInt::iterator ii = m_original.begin();
        Results_struct r_single;

        for (i = 0; ii != iiend; ii++, i++) {
            r_single.s = strdup((ii->first).c_str());
            r_single.freq = ii->second;
            res.push_back(r_single);
        }
        qsort(&res[0], res.size(), sizeof(Results_struct), compare_results);
        write_results_html(&res[0], res.size());
        Timer::stop(3);

        write_results_cons();

        for (int j = 0; j < i; j++) {
            if (res[j].s) free(res[j].s);
        }
    } else {
        Error::msg("Select one method ( esu v fase )");
    }
}


// Finish everything
void CmdLine::finish() {
    if (fase)
        delete gt_fase;
    delete gt_final;
    delete rs;
    delete mg;
    Isomorphism::finishNauty();
}