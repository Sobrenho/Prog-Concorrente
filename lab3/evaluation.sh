#!/bin/bash

# Function to run matrix multiplication with different thread counts
run_matmultconc () {
    local threads=$1
    local binary=$2
    echo "$threads threads $binary"
    for i in {1..5}; do
        if [ "$binary" = "transposto" ]; then
            ./matmultconctransp $threads m1.bin m2.bin m3.bin
        else
            ./matmultconc $threads m1.bin m2.bin m3.bin
        fi
        echo
    done
    echo "-----------------------------"
}

# Main loop
for j in 500 1000 2000; do
    echo "Gerando Matrizes $j $j"
    echo

    # Gerar primeira e segunda matriz
    ./geraMatriz $j $j m1.bin
    ./geraMatriz $j $j m2.bin

    echo "Sequencial"
    for i in {1..5}; do
        ./matmult m1.bin m2.bin m3.bin
        echo
    done
    echo "-----------------------------"

    # Run with different thread counts for normal and transposed binaries
    for threads in 1 2 4 8; do
        run_matmultconc $threads "normal"
        run_matmultconc $threads "transposto"
    done

done
