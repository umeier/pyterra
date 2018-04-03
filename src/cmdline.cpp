#include <cstdlib>
#include <fstream>
#include <cstring>
#include <getopt.h>
#include "terra.h"

Mesh *mesh;
Map *DEM;

static ImportMask default_mask; // NOLINT
ImportMask *MASK;


double error_threshold = 0.0;
int point_limit = -1;
double height_scale = 1.0;
FileFormat output_format;
char *output_filename = nullptr;
char *mask_filename = nullptr;
char *script_filename = nullptr;

static char *options = (char *) "e:p:h:o:m:s:";

static char *usage_string =
        (char *) "-e <thresh>      Sets the tolerable error threshold\n"
                "-p <count>       Sets the maximum number of allowable points\n"
                "-h <factor>      Sets the height scaling factor.  For example,\n"
                "                 if grid points are 25m apart, use a factor of 0.04\n"
                "-o <file> <type> When finished, output the approximation to <file>.\n"
                "                 Valid types:  tin, eps, dem, obj\n"
                "-m <file>        Load the importance mask from <file>\n"
                "-s <file>        Execute preinsertion script from <file>\n"
                "\n";

static void usage_error(char *msg = nullptr) {
    if (msg)
        cerr << msg << endl;

    cerr << "usage: terra <options> filename" << endl;
    cerr << "       where <options> is some combination of: " << endl;
    cerr << usage_string << endl;

    exit(1);
}

void process_cmdline(int argc, char **argv) {
    int c;

    while ((c = getopt(argc, argv, options)) != EOF) {
        int errflg = False;
        char *name;

        switch (c) {
            case 'e':
                error_threshold = strtod(optarg, nullptr);
                cerr << "    Setting error threshold to " << error_threshold << endl;
                break;

            case 'p':
                point_limit = (int)strtol(optarg, nullptr, 10);
                cerr << "    Setting point limit to " << point_limit << endl;
                break;

            case 'h':
                height_scale = strtod(optarg, nullptr);
                cerr << "    Setting height scaling factor to " << height_scale
                     << endl;
                break;

            case 'm':
                mask_filename = optarg;
                cerr << "    Input mask data from " << mask_filename << endl;
                break;

            case 's':
                script_filename = optarg;
                cerr << "    Will execute script from " << script_filename << endl;
                break;

            case 'o':
                output_filename = optarg;
                name = argv[optind++];

                if (!strcmp(name, "tin"))
                    output_format = TINfile;
                else if (!strcmp(name, "eps"))
                    output_format = EPSfile;
                else if (!strcmp(name, "dem"))
                    output_format = DEMfile;
                else if (!strcmp(name, "obj"))
                    output_format = OBJfile;
                else if (!strcmp(name, "rms"))
                    output_format = RMSfile;
                else
                    usage_error((char *) "Unknown output file type");

                cerr << "    Output file set to " << output_filename << endl;
                cerr << "    Output file type is " << name << endl;

                break;

            default:
                errflg++;
                break;

        }

        if (errflg)
            usage_error();
    }

    ////////////////////////////////////////////////////////////////

    if (optind == argc)
        usage_error();

    if (argv[optind][0] == '-')
        DEM = readPGM(cin);
    else {
        ifstream in(argv[optind]);
        DEM = readPGM(in);
        in.close();
    }

    if (!DEM) {
        cerr << "Unable to load height field." << endl;
        exit(1);
    }

    MASK = &default_mask;
    if (mask_filename) {
        ifstream in(mask_filename);
        MASK = readMask(in);
        in.close();

        if (!MASK) {
            cerr << "Unable to load mask data ... reverting to identity mask.";
            cerr << endl;
            MASK = &default_mask;
        } else if (MASK->width != DEM->width || MASK->height != DEM->height) {
            cerr << "Mask resolution does not match DEM resolution." << endl;
            cerr << "     ... reverting to identity mask." << endl;
            MASK = &default_mask;
        }
    }


    mesh = new Mesh(DEM);


    ////////////////////////////////////////////////////////////////

    if (point_limit < 0)
        point_limit = DEM->width * DEM->height;


    if (script_filename) {
        cerr << "Executing preinsertion script:" << endl;
        ifstream in(script_filename);
        scripted_preinsertion(in);
        in.close();
    }
}
