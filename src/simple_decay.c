#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
 * Main function to simulate a decay chain based on user input.
 *
 * Usage:
 *  ./simple_decay <N_0> <lambda> <deltaj> <M>
 *
 * Arguments:
 *  - N_0 (int): Initial number of nuclei
 *  - lambda (float): Decay constant
 *  - deltaj (int): Time step for recording data
 *  - M (int): Number of iterations
 */

int main(int argc, char** argv) {
    // Check for correct number of command-line arguments
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <N_0> <lambda> <deltaj> <M>\n", argv[0]);
        fprintf(stderr, "Arguments:\n");
        fprintf(stderr, "  <N_0>    (int): Initial number of nuclei (positive integer)\n");
        fprintf(stderr, "  <lambda> (float): Decay constant (positive floating-point number)\n");
        fprintf(stderr, "  <deltaj> (int): Time step for recording data (positive integer)\n");
        fprintf(stderr, "  <M>      (int): Number of iterations (positive integer)\n");
        return 1;
    }
    
    // Initialize random number generator
    srand((unsigned int)time(NULL));

    // Convert command-line arguments to variables
    int N_0 = atoi(argv[1]);
    float lambda = atof(argv[2]);
    int deltaj = atoi(argv[3]);
    int M = atoi(argv[4]);

    // Allocate memory for arrays and initialize to zero
    float *N = calloc(N_0, sizeof(float));
    float *Nj = calloc(N_0, sizeof(float));
    float *time = calloc(N_0, sizeof(float));

    // Check if memory allocation succeeded
    if (N == NULL || Nj == NULL || time == NULL) {
        perror("Error allocating memory");
        free(N);
        free(Nj);
        free(time);
        return 1;
    }

    // Alternatively (for lower array sizes):
    // float N[N_0] = {0}, Nj[N_0] = {0}, time[N_0] = {0};

    // Open output file for writing
    FILE *fout;
    fout = fopen("ejercicio_1.csv", "w");
    if (fout == NULL) {
        perror("Error opening file");
        free(N);
        free(Nj);
        free(time);
        return 1;
    }

    // Print and write the header
    printf("N Time\n");
    fprintf(fout, "N\tTime\n");

    // Perform the simulation 'M' times
    for (int k = 0; k < M; k++) {
        // Activate all nuclei in the array
        for (int i = 0; i < N_0; i++) {
            N[i] = 1;
        }

        int counts = 0; // Number of decayed nuclei
        int steps = 0;  // Number of time steps
        int j = 0;      // Index for storing results

        // Simulate decay process
        while (counts < N_0) {
            int random = rand() % N_0;

            // Save initial conditions
            if (steps == 0) {
                time[j] = steps * pow(lambda * N_0, -1);
                Nj[j] += N_0;
                j++;
            }
            steps++;

            // Decay nucleus
            if (N[random] == 1) {
                N[random] = 0;
                counts++;
            }

            // Record data every 'deltaj' steps
            if (steps % deltaj == 0) {
                time[j] = steps * pow(lambda * N_0, -1);
                Nj[j] += (N_0 - counts);
                j++;
            }
        }
    }

    // Count the number of elements in the "time" array that have been updated
    int non_zero_size = 1; // Include time[0]
    for (int i = 0; i < N_0; i++) {
        if (time[i] != 0) {
            non_zero_size++;
        }
    }

    // Print and write results to file
    for (int i = 0; i < non_zero_size; i++) {
        Nj[i] /= M;
        printf("%.3f %.3f\n", Nj[i], time[i]);
        fprintf(fout,"%.3f\t%.3f\n", Nj[i], time[i]);
    }

    // Close output file and free allocated memory
    fclose(fout);
    free(N);
    free(Nj);
    free(time);

    return 0;
}
