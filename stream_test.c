#include <stdio.h>
#include <stdlib.h>

#ifdef AHO_CORASICK_TEST
#include "aho_corasick/aho_corasick.h"
#else
#include "dict_matching/dict_matching.h"
#endif

int main(int argc, char **argv) {
    FILE *p_file = fopen(argv[1], "r");
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

    FILE *t_file = fopen(argv[2], "r");
    fseek(t_file, 0, SEEK_END);
    long n = ftell(t_file);
    fseek(t_file, 0, SEEK_SET);

#ifdef AHO_CORASICK_TEST
    ac_state state = ac_build(patterns, m, num_patterns);
#else
    dict_matcher state = dict_matching_build(patterns, m, num_patterns, n, 0);
#endif

    for (i = 0; i < num_patterns; i++) {
        free(patterns[i]);
    }
    free(m);
    free(patterns);

    i = 0;
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

    fclose(t_file);

#ifdef AHO_CORASICK_TEST
    ac_free(state);
#else
    dict_matching_free(state);
#endif

    printf("%d\n", count);

    return 0;
}