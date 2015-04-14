#include <stdio.h>
#include <stdlib.h>

#include "aho_corasick/aho_corasick.h"

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

    ac_state state = ac_build(patterns, m, num_patterns);

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
        result = ac_stream(state, T_i, i);
        if (result != -1) {
            printf("%d %d %d\n", result, state->state, state->has_match[state->state]);
            count++;
        }
        i++;
        T_i = fgetc(t_file);
    }

    fclose(t_file);

    ac_free(state);

    printf("%d\n", count);

    return 0;
}