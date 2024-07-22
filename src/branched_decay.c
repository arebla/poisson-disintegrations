#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
 * Main function to simulate a branched decay process based on user input.
 *
 * Usage:
 *  ./branched_decay <N_0> <N_P0> <N_A0> <N_B0> <lambda_A> <lambda_B> <deltaj> <M>
 *
 * Arguments:
 *  - N_0 (int): Total number of nuclei (sum of parent, A, and B) (positive integer)
 *  - N_P0 (int): Initial number of parent nuclei (positive integer)
 *  - N_A0 (int): Initial number of A nuclei (positive integer)
 *  - N_B0 (int): Initial number of B nuclei (positive integer)
 *  - lambda_A (float): Decay constant for A nuclei (positive floating-point number)
 *  - lambda_B (float): Decay constant for B nuclei (positive floating-point number)
 *  - deltaj (int): Time step for recording data (positive integer)
 *  - M (int): Number of iterations (positive integer)
 *
 * Notes:
 * - Ensure all arguments are positive values.
 * - The total number of nuclei, <N_0>, should be the sum of <N_P0>, <N_A0>, and <N_B0>.
 */

int main(int argc, char** argv) {
    // Check for correct number of command-line arguments
    if (argc < 9) {
        fprintf(stderr, "Usage: %s <N_0> <N_P0> <N_A0> <N_B0> <lambda_A> <lambda_B> <deltaj> <M>\n", argv[0]);
        fprintf(stderr, "Arguments:\n");
        fprintf(stderr, "  <N_0>      (int): Total number of nuclei (sum of parent, A, and B) (positive integer)\n");
        fprintf(stderr, "  <N_P0>     (int): Initial number of parent nuclei (positive integer)\n");
        fprintf(stderr, "  <N_A0>     (int): Initial number of A nuclei (positive integer)\n");
        fprintf(stderr, "  <N_B0>     (int): Initial number of B nuclei (positive integer)\n");
        fprintf(stderr, "  <lambda_A> (float): Decay constant for A nuclei (positive floating-point number)\n");
        fprintf(stderr, "  <lambda_B> (float): Decay constant for B nuclei (positive floating-point number)\n");
        fprintf(stderr, "  <deltaj>   (int): Time step for recording data (positive integer)\n");
        fprintf(stderr, "  <M>        (int): Number of iterations (positive integer)\n");
        return 1;
    }

    // Initialize random number generator
    srand(time(NULL));

    // Convert command-line arguments to variables
    int N_0 = atoi(argv[1]);
    int N_P0 = atoi(argv[2]);
    int N_A0 = atoi(argv[3]);
    int N_B0 = atoi(argv[4]);
    float lambda_A = atof(argv[5]);
    float lambda_B = atof(argv[6]);
    int deltaj = atoi(argv[7]);
    int M = atoi(argv[8]);

    // Allocate memory for arrays and initialize to zero
    float *N = calloc(N_0, sizeof(float));
    float *N_P = calloc(N_0, sizeof(float));
    float *N_A = calloc(N_0, sizeof(float));
    float *N_B = calloc(N_0, sizeof(float));
    float *update_counts = calloc(N_0, sizeof(float));
    float *time = calloc(N_0, sizeof(float));

    // Check if memory allocation succeeded
    if (N == NULL || N_P == NULL || N_A == NULL || N_B == NULL || time == NULL) {
        perror("Error allocating memory");
        free(N);
        free(N_P);
        free(N_A);
        free(N_B);
        free(update_counts);
        free(time);
        return 1;
    }

    // Calculate decay probabilities 
    float P_A = lambda_A / (lambda_A + lambda_B);
    float P_B = lambda_B / (lambda_A + lambda_B);

    // Open output file for writing
    FILE *fout;
    fout = fopen("ejercicio_3.csv", "w");
    if (fout == NULL) {
        perror("Error opening file");
        free(N);
        free(N_P);
        free(N_A);
        free(N_B);
        free(update_counts);
        free(time);
        return 1;
    }

    // Print and write the header
    printf("N_P N_A N_B Time\n");
    fprintf(fout, "N_P\tN_A\tN_B\tTime\n");

    // Perform the simulation 'M' times
    for (int k = 0; k < M; k++) {
        // Initialize nuclei: 2 for parent, 1 for A and 0 for B
        for (int i = 0; i < N_P0; i++) {
            N[i] = 2;
        }
        for (int i = 0; i < N_A0; i++) {
            N[i + N_P0] = 1;
        }
        for (int i = 0; i < N_B0; i++) {
            N[i + N_P0 + N_A0] = 0;
        }

        // Simulate decay process
        int counts_P = 0; // Number of decayed parent nuclei
        int counts_A = 0;  // Number of parent nuclei decayed to A
        int counts_B = 0;  // Number of parent nuclei decayed to B
        int steps = 0;    // Number of time steps
        int j = 0;        // Index for storing results

        while (counts_P < N_P0) {
            // Save initial conditions and record data every 'deltaj' steps
            counts_P = counts_A + counts_B;
            if (steps == 0 || steps % deltaj == 0) {
                time[j] = steps * pow((lambda_A + lambda_B)  * N_0, -1);
                N_P[j] += N_P0 - counts_P;
                N_A[j] += N_A0 + counts_A;
                N_B[j] += N_B0 + counts_B;
                update_counts[j] += 1; // Track data collection
                j++;
            }
            steps++;

            // Randomly select a nucleus
            int random = (unsigned int)rand() % N_0;
            float U = (double)rand() / (RAND_MAX + 1.0);
            if (N[random] == 2) {
                if (U < P_A) {
                    N[random] = 1; // Parent decays to A
                    counts_A++;
                }else {
                    N[random] = 0; // Parent decays to B
                    counts_B++;
                }
            }

        }
    }

    // Count the number of cells with recorded simulation data
    int non_zero_size = 0;
    for (int i = 0; i < N_0; i++) {
        if (update_counts[i] != 0) {
            non_zero_size += 1;
        }
    }

    // Print and write results to file
    for (int i = 0; i < non_zero_size; i++) {
        N_P[i] /= M;
        N_A[i] /= update_counts[i];
        N_B[i] /= update_counts[i];
        printf("%.3f %.3f %.3f %.3f\n", N_P[i], N_A[i], N_B[i], time[i]);
        fprintf(fout,"%.3f\t %.3f\t %.3f\t %.3f\n", N_P[i], N_A[i], N_B[i], time[i]);
    }

    // Close output file and free allocated memory
    fclose(fout);
    free(N);
    free(N_P);
    free(N_A);
    free(N_B);
    free(update_counts);
    free(time);

    return 0;
}
