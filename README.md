# Poisson Processes and Radioactive Decay
Radioactive decay is a fundamentally random process that continuously occurs in nature. Given that it meets certain conditions, it is possible to use random variables with a Poisson distribution to model the process. 

This project aims to program functions that computationally simulate various Poisson processes for radioactive disintegration. It features three C programs, each modeling a different type of radioactive decay process: 

* Simple Decay: Models the radioactive decay of radionuclides of the same species.
* Decay Chain: Simulates a decay process where a radionuclide decays into a daughter nucleus, which then decays into a stable end product.
* Branched Decay: Represents a decay process where a parent nucleus decays into two possible stable products (A or B).

For detailed implementation and theoretical background, check out the documentation in the `docs` directory (available in Spanish).

## Programs
Compile the programs using the provided `Makefile`. First, clone the repository and run:
```bash
cd poisson-disintegrations
make
```

After compilation, you can run the programs to simulate the decay processes. See usage details below:

1. **Simple Decay** (`simple_decay.c`). Usage: 
     ```bash
     ./simple_decay <N_0> <lambda> <deltaj> <M>
     ```
    Where `<N_0>` is the initial number of nuclei (integer), `<lambda>` is the decay constant (float), `<deltaj>` is the time step for recording data (integer), and `<M>` is the number of iterations (integer).

2. **Decay Chain** (`decay_chain.c`). Usage:
     ```bash
     ./decay_chain <N_0> <N_P0> <N_H0> <lambda_P> <lambda_H> <deltaj> <M>
     ```
    Where `<N_0>` is the total number of nuclei (integer), `<N_P0>` is the initial number of parent nuclei (integer), `<N_H0>` is the initial number of daughter nuclei (integer), `<lambda_P>` is the decay constant of parent nuclei (float), `<lambda_H>` is the decay constant of daughter nuclei (float), `<deltaj>` is the time step for recording data (integer), and `<M>` is the number of iterations (integer).

3. **Branched Decay** (`branched_decay.c`). Usage:
     ```bash
     ./branched_decay <N_0> <N_P0> <N_A0> <N_B0> <lambda_A> <lambda_B> <deltaj> <M>
     ```
Where `<N_0>` is the total number of nuclei (integer), `<N_P0>` is the initial number of parent nuclei (integer), `<N_A0>` is the initial number of A nuclei (integer), `<N_B0>` is the initial number of B nuclei (integer), `<lambda_A>` is the decay constant of A nuclei (float), `<lambda_B>` is the decay constant of B nuclei (float), `<deltaj>` is the time step for recording data (integer), and `<M>` is the number of iterations (integer).


## Acknowledgements
This project was developed as part of the course _Física Computacional I_ (61041094) from the Bachelor's degree in Physics at UNED.
