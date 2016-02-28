#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sndfile.h>

#define BUFFER_LEN (1 << 16)

void usage (char *progname) {
    printf("\nUsage: %s <infile> <outfile>\n\n", progname);
    puts (
            "   Reverses the order of the frames in <infile> and writes\n"
            "   it to <outfile>\n"
         );
    exit(1);
}

void reverse_fp(SNDFILE* out, SNDFILE* in, int channels, int frames) {
    int len = frames * channels;
    double data[len];
    sf_readf_double(in, data, frames);

    int i, j;
    for (i = 0; i < len >> 1; i += channels) {
        for (j = 0; j < channels; j++) {
            int lower = i + j;
            int higher = len - i + j - channels;
            double temp = data[lower];
            data[lower] = data[higher];
            data[higher] = temp;
        }
    }

    sf_writef_double(out, data, frames);
}

void reverse_int(SNDFILE* out, SNDFILE* in, int channels, int frames) {
    int len = frames * channels;
    int data[len];
    sf_readf_int(in, data, frames);

    int i, j;
    for (i = 0; i < len >> 1; i += channels) {
        for (j = 0; j < channels; j++) {
            int lower = i + j;
            int higher = len - i + j - channels;
            int temp = data[lower];
            data[lower] = data[higher];
            data[higher] = temp;
        }
    }

    sf_writef_int(out, data, frames);
}

int main (int argc, char **argv) {
    if(argc != 3) usage(argv[0]);

    SNDFILE *outfile, *infile;
    SF_INFO sfinfo_out, sfinfo_in;
    memset(&sfinfo_in, 0, sizeof(sfinfo_in));

    if (NULL == (infile = sf_open(argv[1], SFM_READ, &sfinfo_in))) {
        printf("\nError: failed to open file '%s'.\n\n", argv[1]);
        exit(1);
    }

    sfinfo_out = sfinfo_in;

    if (NULL == (outfile = sf_open(argv[2], SFM_WRITE, &sfinfo_out))) {
        printf("\nError: failed to open file '%s'.\n\n", argv[2]);
        exit(1);
    }

    int format = sfinfo_out.format & SF_FORMAT_SUBMASK;
    int is_fp = format == SF_FORMAT_DOUBLE || format == SF_FORMAT_FLOAT;
    
    void (*func) (SNDFILE*, SNDFILE*, int, int);
    func = is_fp ? reverse_fp : reverse_int;

    func(outfile, infile, sfinfo_in.channels, sfinfo_in.frames);

    sf_close(outfile);
    sf_close(infile);

    return 0;
}
