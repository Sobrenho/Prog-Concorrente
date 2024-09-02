#!/bin/bash

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

    for threads in 1 2 4 8; do
        run_matmultconc $threads "original"
        run_matmultconc $threads "transposto"
    done

done
