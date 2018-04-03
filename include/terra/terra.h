#ifndef TERRA_TERRA_H // -*- C++ -*-
#define TERRA_TERRA_H

#include "GreedyInsert.h"
#include "Map.h"
#include "Mask.h"


extern Mesh *mesh;
extern Map *DEM;
extern ImportMask *MASK;

extern double error_threshold;
extern int point_limit;
extern double height_scale;
enum FileFormat {
    NULLfile, TINfile, EPSfile, DEMfile, OBJfile, RMSfile
};
extern FileFormat output_format;
extern char *output_filename;
extern char *script_filename;

extern int goal_not_met();

extern void greedy_insertion();

extern void scripted_preinsertion(istream &);

extern void generate_output(char *filename = nullptr,
                            FileFormat format = NULLfile);

extern void output_tin(ostream &);

extern void output_eps(ostream &);

extern void output_obj(ostream &);

extern void output_dem(ostream &);

extern void process_cmdline(int argc, char **argv);


#endif
