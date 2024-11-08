#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
 * Main function to simulate a decay chain involving parent and daughter nuclei 
 * based on user input.
 *
 * Usage:
 *  ./simple_decay <N_0> <N_P0> <N_H0> <lambda_P> <lambda_H> <deltaj> <M>
 *
 * Arguments:
 *  - N_0 (int): Total number of nuclei (sum of parent and daughter)
 *  - N_P0 (int): Initial number of parent nuclei
 *  - N_H0 (int): Initial number of daughter nuclei
 *  - lambda_P (float): Decay constant for parent nuclei
 *  - lambda_H (float): Decay constant for daughter nuclei
 *  - deltaj (int): Time step for recording data
 *  - M (int): Number of iterations
 *
 * Notes:
 * - Ensure all arguments are positive values.
 * - The total number of nuclei, <N_0>, should be the sum of <N_P0> and <N_H0>.
 */

int main(int argc, char** argv) {
    // Check for correct number of command-line arguments
    if (argc < 8) {
        fprintf(stderr, "Usage: %s <N_0> <N_P0> <N_H0> <lambda_P> <lambda_H> <deltaj> <M>\n", argv[0]);
        fprintf(stderr, "Arguments:\n");
        fprintf(stderr, "  <N_0>      (int): Total number of nuclei (sum of parent and daughter) (positive integer)\n");
        fprintf(stderr, "  <N_P0>     (int): Initial number of parent nuclei (positive integer)\n");
        fprintf(stderr, "  <N_H0>     (int): Initial number of daughter nuclei (positive integer)\n");
        fprintf(stderr, "  <lambda_P> (float): Decay constant for parent nuclei (positive floating-point number)\n");
        fprintf(stderr, "  <lambda_H> (float): Decay constant for daughter nuclei (positive floating-point number)\n");
        fprintf(stderr, "  <deltaj>   (int): Time step for recording data (positive integer)\n");
        fprintf(stderr, "  <M>        (int): Number of iterations (positive integer)\n");
        return 1;
    }

    // Initialize random number generator
    srand(time(NULL));

    // Convert command-line arguments to variables
    int N_0 = atoi(argv[1]);
    int N_P0 = atoi(argv[2]);
    int N_H0 = atoi(argv[3]);
    float lambda_P = atof(argv[4]);
    float lambda_H = atof(argv[5]);
    int deltaj = atoi(argv[6]);
    int M = atoi(argv[7]);

    // Allocate memory for arrays and initialize to zero
    float *N = calloc(N_0, sizeof(float));
    float *N_P = calloc(N_0, sizeof(float));
    float *N_H = calloc(N_0, sizeof(float));
    float *time = calloc(N_0, sizeof(float));

    // Check if memory allocation succeeded
    if (N == NULL || N_P == NULL || N_H == NULL || time == NULL) {
        perror("Error allocating memory");
        free(N);
        free(N_P);
        free(N_H);
        free(time);
        return 1;
    }

    // Calculate decay probabilities 
    float P_P = lambda_P / (lambda_P + lambda_H);
    float P_H = lambda_H / (lambda_P + lambda_H);

    // Open output file for writing
    FILE *fout;
    fout = fopen("ejercicio_2.csv", "w");
    if (fout == NULL) {
        perror("Error opening file");
        free(N);
        free(N_P);
        free(N_H);
        free(time);
        return 1;
    }

    // Print and write the header
    printf("N_P N_H Time\n");
    fprintf(fout, "N_P\tN_H\tTime\n");

    // Perform the simulation 'M' times
    for (int k = 0; k < M; k++) {
        // Initialize nuclei: 2 for parent and 1 for daughter
        for (int i = 0; i < N_P0; i++) {
            N[i] = 2;
        }
        for (int i = 0; i < N_H0; i++) {
            N[i + N_P0] = 1;
        }

        // Simulate decay process
        int counts_P = 0; // Number of decayed parent nuclei
        int counts_H = 0; // Number of decayed daughter nuclei
        int steps = 0;    // Number of time steps
        int j = 0;        // Index for storing results

        while (counts_H < N_0) {
            // Save initial conditions and record data every 'deltaj' steps
            if (steps == 0 || steps % deltaj == 0) {
                time[j] = steps * pow((lambda_P + lambda_H) * N_0, -1);
                N_P[j] += (N_P0 - counts_P);
                N_H[j] += (N_H0 + counts_P - counts_H);
                j++;
            }
            steps++;

            // Randomly select a nucleus and decide whether it decays
            int random = (unsigned int)rand() % N_0;
            float U = (double)rand() / (RAND_MAX + 1.0);
            if (N[random] == 2 && U < P_P) {
                N[random] = 1;
                counts_P++;
            }else {
                if (N[random] == 1 && U < P_H) {
                    N[random] = 0;
                    counts_H++;
                }
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
        N_P[i] /= M;
        N_H[i] /= M;
        printf("%.3f %.3f %.3f\n", N_P[i], N_H[i], time[i]);
        fprintf(fout,"%.3f\t %.3f\t %.3f\n", N_P[i], N_H[i], time[i]);
    }


    // Close output file and free allocated memory
    fclose(fout);
    free(N);
    free(N_P);
    free(N_H);
    free(time);

    return 0;
}
