#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef AHO_CORASICK_TEST
#include "aho_corasick/aho_corasick.h"
#else
#include "dict_matching/dict_matching.h"
#endif

int main(int argc, char **argv) {
    int alpha;
    if (argc == 4) {
        printf("%s\n", argv[3]);
        sscanf(argv[3], "%d", &alpha);
    }
    else alpha = 0;
#ifdef PROFILE
    FILE *p_file = fopen("patterns.txt", "r");
#else
    FILE *p_file = fopen(argv[1], "r");
#endif
    int num_patterns;
    fscanf(p_file, "%d\n", &num_patterns);
    int *m = malloc(sizeof(int) * num_patterns);
    char **patterns = malloc(sizeof(char*) * num_patterns);
    int i;
    for (i = 0; i < num_patterns; i++) {
        fscanf(p_file, "%d", &(m[i]));
        fgetc(p_file);
        patterns[i] = malloc(sizeof(char) * (m[i] + 1));
        fread(patterns[i], sizeof(char), m[i], p_file);
        patterns[i][m[i]] = '\0';
    }
    fclose(p_file);

#ifdef PROFILE
    FILE *t_file = fopen("dna.50MB", "r");
#else
    FILE *t_file = fopen(argv[2], "r");
#endif
    fseek(t_file, 0, SEEK_END);
    long n = ftell(t_file);
    fseek(t_file, 0, SEEK_SET);

    clock_t start = clock();
#ifdef AHO_CORASICK_TEST
    ac_state state = ac_build(patterns, m, num_patterns);
#else
    dict_matcher state = dict_matching_build(patterns, m, num_patterns, n, alpha);
#endif
    double build_time = (double)(clock() - start) / (CLOCKS_PER_SEC/1000);

    for (i = 0; i < num_patterns; i++) {
        free(patterns[i]);
    }
    free(m);
    free(patterns);

    i = 0;
    start = clock();
    char T_i;
    int result;
    int count = 0;
    T_i = fgetc(t_file);
    while (T_i != EOF) {
#ifdef AHO_CORASICK_TEST
        result = ac_stream(state, T_i, i);
#else
        result = dict_matching_stream(state, T_i, i);
#endif
        if (result != -1) {
            count++;
        }
        i++;
        T_i = fgetc(t_file);
    }
    double time = (double)(clock() - start) / (CLOCKS_PER_SEC/1000000);

    fclose(t_file);

    printf("%d\n", count);
#ifdef AHO_CORASICK_TEST
    printf("%d\n", ac_size(state));
#else
    printf("%d,%d,%d\n", dict_matching_size(state), short_dict_matching_size(state->short_matcher), periodic_dict_matching_size(state->periodic_matcher));
#endif
    printf("%f\n", build_time);
    printf("%f\n", time/i);
    printf("%f\n", time);

#ifdef AHO_CORASICK_TEST
    ac_free(state);
#else
    dict_matching_free(state);
#endif


    return 0;
}
