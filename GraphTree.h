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


#ifndef _GRAPHTREE_
#define _GRAPHTREE_

#include "Common.h"


class GraphTreeNode   {
 public:

  GraphTreeNode();
  ~GraphTreeNode();

  int frequency;
  GraphTreeNode *zero, *one;

  void zeroFrequency();
  void incrementString(int pos, char *s, int f);
  void incrementString(int pos, char *s, GraphTreeNode **final, int f);

  void populateMap(mapStringInt *m, int size, int pos, char *s);

  int countGraphs();
  double countOccurrences();

};


class GraphTree   {
 private:
    int occurrences;
    int leafs;

 public:

  GraphTreeNode *root;

  GraphTree();
  ~GraphTree();

  void zeroFrequency();
  void incrementString(char *s, int f);
  void incrementString(char *s);
  void addSubstring(GraphTreeNode *initial, char *s, GraphTreeNode **final, int f);

  void populateMap(mapStringInt *m, int maxsize);

  int countGraphs();
  double countOccurrences();

  int getOccurrences() { return occurrences; }
  void incrementOccurrences() { occurrences++; }
  void setOccurrences(int occ) { occurrences = occ; }

  int getLeafs() { return leafs; }
  void setLeafs(int l) { leafs = l; }
};

#endif
