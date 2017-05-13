#include <iostream>
#include <regex>
#include <fstream>
#include <set>

#include "huffman.h"

#define __length_of_char_counts__ (UCHAR_MAX + 1)
#define CHCNTSLEN __length_of_char_counts__

typedef struct char_count {
    char c;
    long n;
} char_count;

void help_info();

char_count *analysis(std::string filename, bool show);

int compare_char_count(const void *count_a, const void *count_b) {
    return (int) (((char_count *)(count_a))->n - ((char_count *)(count_b))->n);
}

int main(const int argc, const char **argv) {
    std::string fnm;
    if (argc > 2) fnm = (argv[2]);
    else fnm = ("");
    if (argc > 1) {
        if (argv[1][0] != '-')
            std::cerr << "Err: Wrong arguments!" << std::endl;
        else {
            switch (argv[1][1]) {
                case 'v':
                    std::cout << "Huffman compressor by Chen Runze." << std::endl;
                    break;
                case 'h':
                    help_info();
                    break;
                case 'a':
                    if (argc == 3)
                        analysis(fnm, true);
                    break;
                case 'c':
                    if (argc == 4)
                        huffman_encode_file(fopen(argv[2], "rb"), fopen(argv[3], "wb"));
                    break;
                case 'x':
                    if (argc == 4)
                        huffman_decode_file(fopen(argv[2], "rb"), fopen(argv[3], "wb"));
                    break;
                case 'l':

                default:
                    break;
            }
        }
    }
}

void help_info() {
    std::cout << "huffman -ahv [filename]" << std::endl;
    std::cout << "-a: Analysis of the file." << std::endl;
}

char_count *analysis(std::string filename, bool show = false) {
    FILE *inFile = fopen(filename.c_str(), "rb");
    if (!inFile) {
        std::cerr << "ERR: BAD filename." << std::endl;
        return nullptr;
    }
    char_count *counts = new char_count[CHCNTSLEN];
    for (int i = 0; i <= UCHAR_MAX; ++i) {
        counts[i].c = (char) (CHAR_MIN + i);
        counts[i].n = 0;
    }
    while (!feof(inFile)) {
        char ch = (char) fgetc(inFile);
        for (int i = 0; i <= UCHAR_MAX; ++i) {
            if (counts[i].c == ch) {
                counts[i].n++;
                break;
            }
        }
    }
    fclose(inFile);
    qsort(counts, CHCNTSLEN, sizeof(char_count), compare_char_count);
    FILE *outFile = fopen("stat.csv", "w");
    if (show) {
        for (int u = 0; u <= UCHAR_MAX; ++u) {
            fprintf(outFile, "%d,%ld\n", counts[u].c, counts[u].n);
            printf("<%d, %ld>", counts[u].c, counts[u].n);
            if (u % 5 == 4 || u == UCHAR_MAX)
                putchar('\n');
        }
    }
    fclose(outFile);
    return counts;
}

