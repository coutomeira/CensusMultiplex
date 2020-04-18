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
 * Write Results
 *
 * Last Update: 04/2020
 *
 */


#include "Results.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <map>
#include <cmath>


/***************************************************/


FILE **Results::f_output;

char *Results::_graph_file;
char *Results::_folder_name;
char *Results::_input_file;

int *Results::_def_layers;

bool Results::_expand;
bool Results::_one;
bool Results::_dir;
bool Results::_fase;

int Results::_num_res;
int Results::_permute;
int Results::_matrix_size;
int Results::_sg_size;
int Results::_num_pages;
int Results::_num_nodes;
int Results::_num_edges;
int Results::_num_layers;
int Results::_per_page;
int Results::_num_def_layers;


/***************************************************/


void Results::init_results(int sg_size, int total, int per_page, int permute, int matrix_size, int nodes, int edges,
                           int num_layers, bool fase, bool expand, bool all, bool dir, char *folder_name,
                           int num_def_layers, int *layers) {

    _sg_size = sg_size;
    _num_res = total;
    _permute = permute;
    _matrix_size = matrix_size;
    _num_nodes = nodes;
    _num_edges = edges;
    _num_layers = num_layers;
    _per_page = per_page;
    _fase = fase;
    _expand = expand;
    _one = all;
    _dir = dir;
    _folder_name = strdup(folder_name);
    _num_def_layers = num_def_layers;
    _def_layers = new int[_num_def_layers];
    for (int i = 0; i < _num_def_layers; i++)
        _def_layers[i] = layers[i];
}


void Results::prepare_files(char *s, int nres) {

    _input_file = strdup(s);

    if (_dir) _expand = true;

    if (_num_res < 0) _num_res = MAX_RES;
    if (_num_res >= MAX_RES) _num_res = MAX_RES;
    if (_num_res >= nres) _num_res = nres;

    if (_per_page > _num_res) _per_page = _num_res;

    _num_pages = _num_res / _per_page;
    if (_num_res % _per_page) _num_pages++;//Final page
    _num_pages++;//Initial page to show numbers

    if (_one) {
        _num_pages = 2;
        _per_page = _num_res;
    }

    f_output = new FILE *[_num_pages];

    _graph_file = strdup(DEFAULT_OUTPUT);

    for (int i = 0; i < _num_pages; i++) {

        size_t needed = snprintf(NULL, 0, "%s%s%d%s", _folder_name, DEFAULT_OUTPUT, i, DEFAULT_EXTENSION) + 2;

        char *name = (char *) malloc(needed);

        sprintf(name, "%s%s%s%d%s", _folder_name, "/", DEFAULT_OUTPUT, i, DEFAULT_EXTENSION);

        f_output[i] = fopen(name, "w");
        if (f_output[i] == NULL)
            Error::msg("Unable to open output file \"%s\"", name);
        free(name);
    }
}


void Results::_write_menu(int page, int width) {
    fprintf(f_output[page], "<div style=\"width: %dpx; height: 105px; position:relative;\">\n", width);
    if (page < _num_pages - 1) {
        fprintf(f_output[page], "<div class=\"infoSmall\"");
        fprintf(f_output[page], "style=\"float: right; margin-right: 0;\">\n");
        fprintf(f_output[page], "<p class=\"center\" "
                                "style=\"margin-top: 45px;\">"
                                "<a href=\""
                                "%s%d.html"
                                "\">"
                                "NEXT</a></p>\n", _graph_file, page + 1);
        fprintf(f_output[page], "</div>\n");
    }
    if (page > 0) {
        fprintf(f_output[page], "<div class=\"infoSmall\"");
        fprintf(f_output[page], "style=\"float: left;\">\n");
        fprintf(f_output[page], "<p class=\"center\" "
                                "style=\"margin-top: 45px;\">"
                                "<a href=\""
                                "%s%d.html"
                                "\">"
                                "PREVIOUS</a></p>\n", _graph_file, page - 1);
        fprintf(f_output[page], "</div>\n");
    }
    if (page > 1) {
        fprintf(f_output[page], "<a style=\"position: absolute; "
                                "bottom: 0px;"
                                "left: 0;"
                                "right: 0;"
                                "text-align: center;\""
                                "href=\""
                                "%s%d.html"
                                "\">"
                                "MENU</a>\n", _graph_file, 0);
    }
    fprintf(f_output[page], "</div>\n");
}


void Results::_write_header(int page) {
    fprintf(f_output[page], "<html>\n"
                            "<head>\n"
                            "<style>\n"
                            "html *\n"
                            "{\n"
                            "\tfont-family: Courier !important;\n"
                            "}\n"
                            ".node1 {\n"
                            "\tfill: #ffeaa7;\n"
                            "}\n"
                            ".node2 {\n"
                            "\tfill: white;\n"
                            "}\n"
                            ".line {\n"
                            "\tstroke: black;\n"
                            "\tstroke-width: 2;\n"
                            "}\n"
                            ".layer1 {\n"
                            "\tfill: #bfbfbf;\n"
                            "}\n"
                            ".layer2 {\n"
                            "\tfill: #6c7a89;\n"
                            "}\n"
                            ".infoSmall {\n"
                            "\twidth: 100px;\n"
                            "\theight: 100px;\n"
                            "\tbackground-color: #ffeaa7;\n"
                            "\tdisplay: inline-block;\n"
                            "\tcolor: gray;\n"
                            "\tfloat: left;\n"
                            "\tmargin-right: 5px;\n"
                            "\tmargin-bottom: 5px;\n"
                            "\tmargin-top: 5px;\n"
                            "}\n"
                            ".infoLarge {\n"
                            "\twidth: 415px;\n"
                            "\theight: 100px;\n"
                            "\tbackground-color: #ffeaa7;\n"
                            "\tdisplay: block;\n"
                            "\tcolor: gray;\n"
                            "\tmargin-right: 5px;\n"
                            "\tclear: both;\n"
                            "\tposition: relative;\n"
                            "}\n"
                            ".center {\n"
                            "\twidth: 100%%;\n"
                            "\ttext-align: center;\n"
                            "}\n"
                            ".big {\n"
                            "font-weight: bold;\n"
                            "}\n"
                            ".bottom {\n"
                            "\tmargin-bottom: 0;\n"
                            "\tmargin-top: 0;\n"
                            "}\n"
                            "a:link, a:hover, a:visited, a:active {\n"
                            "\tcolor: gray;\n"
                            "}\n"
                            "</style>\n"
                            "</head>\n"
                            "<body>\n\n");
    fprintf(f_output[page], "<div style=\"width: 415px;\n"
                            "margin-left: auto;\n"
                            "margin-right: auto;\">\n");
}


void Results::write_consola(GraphTree *gt) {
    int i = 0;
    puts(SEPARATOR);
    printf("%s  |  version  %s\n", PROGRAM_NAME, VERSION);
    puts(SEPARATOR);
    printf("(P%d) ", _permute);
    if (_permute == 1 && !_fase) printf("PERMUTE ONLY VERTICES  |            ESU\n");
    if (_permute == 1 && _fase)  printf("PERMUTE ONLY VERTICES  |           FASE\n");
    if (_permute == 2 && !_fase) printf("PERMUTE NODES & LAYERS |            ESU\n");
    if (_permute == 2 && _fase)  printf("PERMUTE NODES & LAYERS |           FASE\n");

    if (_num_def_layers) {
        puts(SEPARATOR);
        printf(" Layers Considered          :[");
        for (i = 0; i < _num_def_layers - 1; i++) {
            printf("%d,", _def_layers[i] + 1);
        }
        printf("%d]\n", _def_layers[i] + 1);
    }

    puts(SEPARATOR);
    printf(" >> Subgraphs size          :%d\n", _sg_size);
    if(_num_def_layers == 0)
        printf(" >> Layers                  :%d\n", _num_layers);
    puts(SEPARATOR);
    printf(" >> Subgraphs found         :%d\n", gt->getOccurrences());
    printf(" >> Different types         :%d\n", gt->countGraphs());
    if (_fase)//it is the same as occurrences in esu
        printf(" >> Nauty calls             :%d\n", Isomorphism::getNautyCalls());
    puts(SEPARATOR);
    printf(" >> Time elapsed            (seconds)\n");
    printf(" >>> Pre-method             :%.6f\n", Timer::elapsed(0));
    if (!_fase) {
        printf(" >>>> Method ESU            :%.6f\n", Timer::elapsed(1));
    } else {
        printf(" >>>> Enumeration           :%.6f\n", Timer::elapsed(1));
        printf(" >>>> Isomorphism           :%.6f\n", Timer::elapsed(2));
    }
    printf(" >>> Pos-method             :%.6f\n", Timer::elapsed(3));
    puts(SEPARATOR);
    while (_input_file[i] != '\0') {
        printf("%c", _input_file[i++]);
        if (i % WIDTH == 0) printf("\n");
    }
    printf("\n");
    puts(SEPARATOR);
    if (_num_layers == 1 || _num_def_layers == 1) {
        printf(" Hey! Sorry, but this is not the best method\n for analyze only one layer! We have better!\n Check in www.dcc.fc.up.pt/gtries/\n Thank you! Have a nice day!\n");
        puts(SEPARATOR);
    }
}


void Results::write_html(GraphTree *gt, Results_struct *res, int nres) {
    int i, j, k, l;

    _write_header(0);

    //block 1
    fprintf(f_output[0], "<div class=\"infoLarge\">\n");
    fprintf(f_output[0], "<h1 class=\"center\" style=\"padding-top: 25px;\">MULTIPLEX NETWORK</h1>\n");
    fprintf(f_output[0], "<p class=\"center\" style=\"direction: rtl; overflow: hidden; font-size: small;\">%s</p>\n",
            _input_file);
    fprintf(f_output[0], "</div>\n\n");


    //block 2
    fprintf(f_output[0], "<div style=\"width: 420px;\">\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">%s</p>\n", _dir ? "Directed" : "Undirected");
    fprintf(f_output[0], "<svg width=\"100\" height=\"65\">\n"
                         "<line x1=\"15\" y1=\"20\" x2=\"85\" y2=\"20\" stroke=\"gray\"/>\n"
                         "<circle cx=\"15\" cy=\"20\" r=\"10\" fill=\"gray\"/>\n"
                         "<circle cx=\"85\" cy=\"20\" r=\"10\" fill=\"gray\"/>\n");
    if (_dir) fprintf(f_output[0], "<polyline points=\"74 20,64 30,75 20,64 10, 74 20\" stroke=\"gray\" />\n");
    fprintf(f_output[0], "</svg>\n");
    fprintf(f_output[0], "</div>\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">NODES</p>\n");
    fprintf(f_output[0], "<p class=\"center big\">%d</p>\n", _num_nodes);
    fprintf(f_output[0], "</div>\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">EDGES</p>\n");
    fprintf(f_output[0], "<p class=\"center big\">%d</p>\n", _num_edges);
    fprintf(f_output[0], "</div>\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">LAYERS</p>\n");
    fprintf(f_output[0], "<p class=\"center big\">%d</p>\n", _num_layers);
    fprintf(f_output[0], "</div>\n");
    fprintf(f_output[0], "</div>\n");


    //block 3
    fprintf(f_output[0], "<div class=\"infoLarge\">\n");
    fprintf(f_output[0], "<h3 style=\"padding-top: 25px;\" >");

    if (_permute == 1) {
        fprintf(f_output[0], "[P1] NODE ISOMORPHISM</h3>\n");
        fprintf(f_output[0],
                "<p class=\"bottom\">Permutations of node labels are allowed, but not permutations of layer labels</p>\n");
    } else if (_permute == 2) {
        fprintf(f_output[0], "[P2] PERMUTE NODES AND LAYERS</h3>\n");
        fprintf(f_output[0], "<p class=\"bottom\">Both node and layer labels are allowed to be permuted</p>\n");
    }
    fprintf(f_output[0], "</div>\n");


    //block (4)?
    if (_num_def_layers) {
        fprintf(f_output[0], "<div class=\"infoLarge\">\n");
        fprintf(f_output[0], "<h4 style=\"padding-top: 25px; margin-top: 5px;\" >LAYERS CONSIDERED</h4>\n");
        fprintf(f_output[0], "<p class=\"bottom\">");
        fprintf(f_output[0], "Layers: [");
        for (i = 0; i < _num_def_layers - 1; i++) {
            fprintf(f_output[0], "%d,", _def_layers[i] + 1);
        }
        fprintf(f_output[0], "%d]</p>\n", _def_layers[i] + 1);
        fprintf(f_output[0], "</div>\n");
    }


    //block (4v5)
    fprintf(f_output[0], "<div style=\"width: 420px;\">\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">SIZE</p>\n");
    fprintf(f_output[0], "<p class=\"center big\">%d</p>\n", _sg_size);
    fprintf(f_output[0], "</div>\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">SUBGRAPHS</p>\n");
    fprintf(f_output[0], "<p class=\"center big\">%.0f</p>\n", gt->countOccurrences());
    fprintf(f_output[0], "</div>\n");
    fprintf(f_output[0], "<div class=\"infoSmall\">\n");
    fprintf(f_output[0], "<p class=\"center\">TYPES</p>\n");
    fprintf(f_output[0], "<p class=\"center big\">%d</p>\n", gt->countGraphs());
    fprintf(f_output[0], "</div>\n");
    //block to move
    if (_num_res > 0 && nres > 0) {
        fprintf(f_output[0], "<div class=\"infoSmall\" style=\"");
        fprintf(f_output[0], "position: relative;\">\n");
        fprintf(f_output[0], "<p class=\"center\">"
                             "<a style=\"margin-top: 15px;"
                             "margin-bottom: 0px;"
                             "height: 70px;"
                             "top: 0;"
                             "left: 0;"
                             "right: 0;"
                             "position: absolute;\""
                             "href=\""
                             "%s%d.html"
                             "\">"
                             "RESULTS</a></p>\n", _graph_file, 1);
        fprintf(f_output[0], "<svg width=\"100\" height=\"50\" style=\"margin-top:20px\">\n"
                             "<line x1=\"15\" y1=\"20\" x2=\"85\" y2=\"20\" stroke=\"gray\"/>\n"
                             "<polyline points=\"84 20,74 30,85 20,74 10, 84 20\" stroke=\"gray\"/>\n"
                             "</svg>");

        fprintf(f_output[0], "</div>\n");
    }
    fprintf(f_output[0], "</div>\n");

    //END OF INITIAL PAGE
    fprintf(f_output[0], "<div>\n</body>\n</html>");

    //REPRESENTATION OF SUBGRAPHS
    int pos_x[_sg_size];
    int pos_y[_sg_size];
    int nl, initial_x, initial_y;
    double step = ((3.14159265 * 2) / _sg_size);
    double x, y = 0;
    double current = 3.14159265 / 4; //pi/4
    double radius = _sg_size * 10; //radius of big circle
    int radius_n; //radius of nodes
    int center_x, center_y; // center of big circle
    int height, width; //def_layers size
    int width_aux; //give perspective of real layer
    int aux;
    int aux_x, aux_y; //move the end points of edges
    int edges; //count in each subgraph

    //nl -> number of def_layers to display
    if (_num_def_layers) nl = _num_def_layers;
    else nl = _num_layers;

    int r[nl]; //R from RGB
    int g[nl]; //G from RGB
    int b[nl]; //B from RGB
    int order_n_x[nl * _sg_size];
    int order_n_y[nl * _sg_size];
    int order_l[nl * _sg_size];

    height = 3 * radius;
    width = 415;
    center_x = width / 2;
    center_y = 1.5 * radius;

    width_aux = 20;
    if (_expand) radius_n = 6;//node radius
    else radius_n = 2;

    // GENERATE POSITIONS
    for (i = 0; i < _sg_size; i++) {
        x = sin(current) * radius;
        y = cos(current) * radius;
        pos_x[i] = (int) x + center_x;
        pos_y[i] = (int) y + center_y;
        current += step;
    }
    srand(time(NULL));
    if (!_expand) {
        if (nl >= 1) {
            //rgb(30, 55, 153)
            r[0] = 30;
            g[0] = 55;
            b[0] = 153;
        }
        if (nl >= 2) {
            //rgb(255, 71, 87)
            r[1] = 255;
            g[1] = 71;
            b[1] = 87;
        }
        if (nl >= 3) {
            //rgb(236, 204, 104)
            r[2] = 236;
            g[2] = 204;
            b[2] = 104;
        }
        if (nl >= 4) {
            //rgb(46, 213, 115)
            r[3] = 46;
            g[3] = 213;
            b[3] = 115;
        }
        if (nl >= 5) {
            //rgb(30, 144, 255)
            r[4] = 30;
            g[4] = 144;
            b[4] = 255;
        }
        if (nl >= 6) {
            //rgb(30, 39, 46)
            r[5] = 30;
            g[5] = 39;
            b[5] = 46;
        }
        if (nl >= 7) {
            //rgb(197, 108, 240)
            r[6] = 197;
            g[6] = 108;
            b[6] = 240;
        }
        if (nl >= 8) {
            //rgb(255, 184, 184)
            r[7] = 255;
            g[7] = 184;
            b[7] = 184;
        }
        if (nl >= 9) {
            //rgb(255, 255, 255)
            r[8] = 255;
            g[8] = 255;
            b[8] = 255;
        }

    }
    int rest;
    if (!_expand) rest = 8;
    else rest = 0;

    for (l = rest; l < nl; l++) {
        if (!_expand) {
            r[l] = rand() % 255;
            g[l] = rand() % 255;
            b[l] = rand() % 255;
        } else {
            r[l] = 255;
            g[l] = 255;
            b[l] = 255;
        }
    }


    int shown;
    int shown_p = 0;
    int p = 1;
    _write_header(p);

    for (k = 0, shown = 0; k < nres && shown < _num_res; k++) {
        shown++;
        if (shown_p == _per_page && p < _num_pages - 1) {
            _write_menu(p, width - width_aux);
            fprintf(f_output[p], "</div>\n</body>\n</html>");
            fclose(f_output[p]);
            p++;
            _write_header(p);
            shown_p = 0;
        }
        shown_p++;
        edges = 0;

        if (_expand)
            aux = nl;
        else
            aux = 1;
        fprintf(f_output[p], "<div class=\"graph\" style=\"margin-top: 5px\">\n");
        fprintf(f_output[p], "<svg width=\"%d\" height=\"%d\" >\n", width, height * aux);
        if (_dir) {
            fprintf(f_output[p],
                    "<defs>\n"
                    "<marker id=\"head\" markerWidth=\"8\" markerHeight=\"8\" refX=\"2\" refY=\"4\" orient=\"auto\" markerUnits=\"strokeWidth\">\n"
                    "<path d=\"M 2,0 L 2,8 L 8,4 L 2,0\" style=\"fill: #FFFFFF;\"/>\n"
                    "</marker>\n"
                    "</defs>\n");
        }

        //ORDER OF NODES
        initial_x = nl * _sg_size * _matrix_size;
        initial_y = nl * _sg_size;
        for (i = 0; i < _sg_size; i++) {
            for (j = 0; j < _sg_size * nl; j++) {
                if (res[k].s[initial_x + j + i * _matrix_size] == '1')
                    order_n_x[j] = i;
                if (res[k].s[initial_y + j * _matrix_size + i] == '1')
                    order_n_y[j] = i;
            }
        }
        if (_permute == 1) {
            aux = 0;
            for (i = 0; i < _sg_size * nl; i++) {
                if (i > 0 && i % _sg_size == 0) aux++;
                order_l[i] = aux;
            }
        } else if (_permute == 2) {
            initial_x = (nl + 1) * _sg_size * _matrix_size;
            for (i = 0; i < nl; i++)
                for (j = 0; j < _sg_size * nl; j++)
                    if (res[k].s[initial_x + j + i * _matrix_size] == '1')
                        order_l[j] = i;
        }

        //ADD LAYERS
        for (l = 0; l < nl; l++) {
            if (l == 0 || _expand) {
                fprintf(f_output[p],
                        "<polygon points=\"%d %d,%d %d,%d %d,%d %d\" class=\"layer%d\"/>\n", width_aux,
                        l * height, 0,
                        l * height + height, width - width_aux,
                        l * height + height, width,
                        l * height, l % 2 == 0 ? 2 : 1);
            }
            if (_expand) { //layer labels
                if (_num_def_layers)
                    fprintf(f_output[p],
                            "<text x=\"%d\" y=\"%d\" fill=\"white\">%d</text>\n", width_aux,
                            l * height + height, _def_layers[l] + 1);
                else
                    fprintf(f_output[p],
                            "<text x=\"%d\" y=\"%d\" fill=\"white\">%d</text>\n", width_aux,
                            l * height + height, l + 1);
            }
        }


        //COUNTER OF EDGES BECAUSE REPRESENTATION
        int edges_counter[_sg_size][_sg_size];
        int edges_added[_sg_size][_sg_size];
        for (i = 0; i < _sg_size; i++) {
            for (j = 0; j < _sg_size; j++) {
                edges_counter[i][j] = 0;
                edges_added[i][j] = 0;
            }
        }
        for (i = 0; i < _sg_size * nl; i++) {
            for (j = 0; j < _sg_size * nl; j++) {
                if (res[k].s[i * _matrix_size + j] == '1') {
                    int pos_a = order_n_y[j];
                    int pos_b = order_n_x[i];
                    edges_counter[pos_a][pos_b]++;
                }
            }
        }

        //CLEAN HALF MATRIX
        for (i = 0; i < _sg_size; i++) {
            for (j = i; j < _sg_size; j++) {
                edges_counter[i][j] = 0;
            }
        }


        //ADD EDGES
        for (i = 0; i < _sg_size * nl; i++) {
            for (j = 0; j < _sg_size * nl; j++) {

                if (res[k].s[i * _matrix_size + j] == '1') {
                    if (order_l[i] == order_l[j])
                        l = order_l[i];
                    else Error::msg("Error in conversion\nReport to the author");

                    edges++;

                    if (_expand) {
                        aux_y = l * height;
                        aux_x = 0;
                    } else {
                        aux_y = 0;
                        aux_x = 0;
                    }
                    int pos_a = order_n_y[j];
                    int pos_b = order_n_x[i];

                    if (_dir) {//ALWAYS EXPANDED
                        int pos_1_x, pos_1_y, pos_2_x, pos_2_y, pos_3_x, pos_3_y;
                        pos_1_x = pos_x[pos_a] + aux_x;
                        pos_1_y = pos_y[pos_a] + aux_y;
                        pos_3_x = pos_x[pos_b] + aux_x;
                        pos_3_y = pos_y[pos_b] + aux_y;
                        pos_2_x = (pos_1_x + pos_3_x) / 2;
                        pos_2_y = (pos_1_y + pos_3_y) / 2;
                        fprintf(f_output[p],
                                "<polyline points=\"%d,%d %d,%d %d,%d\" "
                                "fill=\"none\" "
                                "stroke=\"white\" "
                                "marker-mid=\"url(#head)\""
                                "style=\"stroke: rgb(%d,%d,%d);"
                                "stroke-width:1;\"/>",
                                pos_1_x, pos_1_y, pos_2_x,
                                pos_2_y, pos_3_x, pos_3_y,
                                r[l], g[l], b[l]);
                    } else {
                        if (!_expand && edges_counter[pos_a][pos_b] > 0) {
                            float size_x, size_y;
                            float new_pos_x_a, new_pos_y_a;
                            float new_pos_x_b, new_pos_y_b;

                            if (pos_x[pos_a] > pos_x[pos_b])
                                size_x = (float) pos_x[pos_a] - pos_x[pos_b];
                            else
                                size_x = (float) pos_x[pos_b] - pos_x[pos_a];
                            size_x /= (float) edges_counter[pos_a][pos_b];

                            if (pos_y[pos_a] > pos_y[pos_b])
                                size_y = (float) pos_y[pos_a] - pos_y[pos_b];
                            else
                                size_y = (float) pos_y[pos_b] - pos_y[pos_a];
                            size_y /= (float) edges_counter[pos_a][pos_b];

                            int diff = edges_added[pos_a][pos_b];

                            if (pos_x[pos_a] > pos_x[pos_b]) {
                                new_pos_x_a = pos_x[pos_a] - diff * size_x;
                                new_pos_x_b = pos_x[pos_a] - (diff + 1) * size_x;
                            } else {
                                new_pos_x_a = pos_x[pos_a] + diff * size_x;
                                new_pos_x_b = pos_x[pos_a] + (diff + 1) * size_x;
                            }
                            if (pos_y[pos_a] > pos_y[pos_b]) {
                                new_pos_y_a = pos_y[pos_a] - diff * size_y;
                                new_pos_y_b = pos_y[pos_a] - (diff + 1) * size_y;
                            } else {
                                new_pos_y_a = pos_y[pos_a] + diff * size_y;
                                new_pos_y_b = pos_y[pos_a] + (diff + 1) * size_y;
                            }
                            edges_added[pos_a][pos_b]++;

                            fprintf(f_output[p],
                                    "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" "
                                    "style=\"stroke: rgb(%d,%d,%d);"
                                    "stroke-width:3;\"/>",
                                    new_pos_x_a, new_pos_y_a,
                                    new_pos_x_b, new_pos_y_b,
                                    r[l], g[l], b[l]);
                        } else if (edges_counter[pos_a][pos_b] > 0) {
                            aux_y = l * height;
                            aux_x = 0;

                            int pos_a = order_n_y[j];
                            int pos_b = order_n_x[i];

                            fprintf(f_output[p],
                                    "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke: rgb(%d,%d,%d);"
                                    "stroke-width:3;\"/>\n",
                                    pos_x[pos_a] + aux_x, pos_y[pos_a] + aux_y,
                                    pos_x[pos_b] + aux_x, pos_y[pos_b] + aux_y,
                                    r[l], g[l], b[l]);
                        }
                    }
                }
            }
        }


        //ADD NODES
        for (l = 0; l < nl; l++) {
            for (i = 0; i < _sg_size; i++) {
                if (l == 0 && _expand) {
                    fprintf(f_output[p],
                            "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" class=\"node1\"/>\n", pos_x[i], pos_y[i], radius_n);
                }
                if (l == 0 && !_expand) {
                    fprintf(f_output[p],
                            "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" class=\"node2\"/>\n", pos_x[i], pos_y[i], radius_n);
                }
                if (_expand && l > 0) {
                    fprintf(f_output[p],
                            "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" class=\"node1\"/>\n", pos_x[i],
                            pos_y[i] + (l - 1) * height + height, radius_n);
                }
            }
        }


        // INFO ABOUT EACH SUBGRAPH
        fprintf(f_output[p],
                "<div class=\"infoLarge\" style=\"height: auto; width: %dpx; padding-top: 25px;\">\n",
                width - width_aux);
        float conc = ((double) res[k].freq / gt->getOccurrences()) * 100;
        fprintf(f_output[p],
                "<p class=\"bottom\" style=\"height: auto; font-size: 14px;\">"
                "Subgraph %d (%d edges) : %d (%.2f%%)</p>\n",
                k + 1, _dir ? edges : edges / 2, res[k].freq, conc);

        fprintf(f_output[p], "</div>\n");
        fprintf(f_output[p], "</svg>\n");
        fprintf(f_output[p], "</div>\n");

    }//END OF CYCLE K ANSWERS

    //LAST PAGE
    _write_menu(p, width - width_aux);
    fprintf(f_output[p], "</div>\n</body>\n</html>");
    fclose(f_output[p]);
}