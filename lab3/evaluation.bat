@echo off

for %%j in (500 1000 2000) do (
    echo Gerando Matrizes %%j %%j
    echo:

    :: Gerar primeira e segunda matrizes
    geraMatriz.exe %%j %%j m1.bin
    geraMatriz.exe %%j %%j m2.bin

    echo Sequencial
    echo:
    for /L %%i in (1, 1, 5) do (
        matmult.exe m1.bin m2.bin m3.bin
        echo:
    )
    echo -----------------------------

    :: Loop para diferentes números de threads
    for %%t in (1 2 4 8) do (
        echo %%t threads
        for /L %%i in (1, 1, 5) do (
            matmultconc.exe %%t m1.bin m2.bin m3.bin
            echo:
        )
        echo -----------------------------
    )

    echo Transposto
    :: Loop para diferentes números de threads com transposto
    for %%t in (1 2 4 8) do (
        echo %%t threads transposto
        for /L %%i in (1, 1, 5) do (
            matmultconctransp.exe %%t m1.bin m2.bin m3.bin
            echo:
        )
        echo -----------------------------
    )
)
