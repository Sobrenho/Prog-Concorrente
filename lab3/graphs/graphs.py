import pandas as pd
import matplotlib.pyplot as plt



tempo_execucao = {
    'Threads': [0, 1, 2, 4, 8],
    '500x500': [0.3336684, 0.334812, 0.1710346, 0.1187786, 0.0716852],
    '1000x1000': [2.6622192, 2.679031, 1.3552448, 0.7984566, 0.5281418],
    '2000x2000': [22.0089842, 23.9701164, 11.174007, 6.222191, 4.0820018],
    '500x500 Transposto': [0.3323024, 0.3323024, 0.1728096, 0.098123, 0.069918],
    '1000x1000 Transposto': [2.6538046, 2.6538046, 1.422449, 0.7569292, 0.5238706],
    '2000x2000 Transposto': [21.2148462, 21.2148462, 10.65928, 6.054623, 3.952505]
}

df1 = pd.DataFrame(tempo_execucao)


aceleracao = {
    'Threads': [1, 2, 4, 8],
    '500x500': [0.996584352, 1.950882453, 2.809162593, 4.654634429],
    '1000x1000': [0.993724671, 1.964382523, 3.334206518, 5.040728077],
    '2000x2000': [0.918184285, 1.969659067, 3.537175924, 5.391713497],
    '500x500 Transposto': [1.004110714, 1.93084412, 3.400511603, 4.772281816],
    '1000x1000 Transposto': [1.003170768, 1.871574447, 3.517131061, 5.081825932],
    '2000x2000 Transposto': [1.037433125, 2.064772123, 3.635070953, 5.568363405]
}


df2 = pd.DataFrame(aceleracao)

eficiencia = {
    'Threads': [1, 2, 4, 8],
    '500x500': [0.996584352, 0.975441227, 0.702290648, 0.581829304],
    '1000x1000': [0.993724671, 0.982191262, 0.833551629, 0.63009101],
    '2000x2000': [0.918184285, 0.984829533, 0.884293981, 0.673964187],
    '500x500 Transposto': [1.004110714, 0.96542206, 0.850127901, 0.596535227],
    '1000x1000 Transposto': [1.003170768, 0.935787223, 0.879282765, 0.635228241],
    '2000x2000 Transposto': [1.037433125, 1.032386062, 0.908767738, 0.696045426]
}

df3 = pd.DataFrame(eficiencia)


def tempo_execucao_plot(matrix_size, title, df, filename):
    plt.figure(figsize=(8, 6))

    plt.plot(df['Threads'], df[matrix_size], label=f'{matrix_size} Original', marker='o')
    
    plt.plot(df['Threads'], df[f'{matrix_size} Transposto'], label=f'{matrix_size} Transposto', marker='o')
    
    plt.xlabel('Número de Threads')
    plt.ylabel('Tempo de Execução')
    plt.title(title)
    plt.legend()
    plt.savefig(f'./{filename}')
    plt.grid(True)
    plt.show()

tempo_execucao_plot('500x500', 'Matriz 500x500', df1, 'exec_500x500.png')
tempo_execucao_plot('1000x1000', 'Matriz 1000x1000', df1, 'exec_1000x1000.png')
tempo_execucao_plot('2000x2000', 'Matriz 2000x2000', df1, 'exec_2000x2000.png')




def comparacao_plot(title, filename, df, metrica):
    plt.figure(figsize=(10, 7))
    
    plt.plot(df['Threads'], df['500x500'], label='500x500 Original', marker='o')
    plt.plot(df['Threads'], df['500x500 Transposto'], label='500x500 Transposto', marker='o')
    plt.plot(df['Threads'], df['1000x1000'], label='1000x1000 Original', marker='o')
    plt.plot(df['Threads'], df['1000x1000 Transposto'], label='1000x1000 Transposto', marker='o')
    plt.plot(df['Threads'], df['2000x2000'], label='2000x2000 Original', marker='o')
    plt.plot(df['Threads'], df['2000x2000 Transposto'], label='2000x2000 Transposto', marker='o')
    
    plt.xlabel('Número de Threads')
    plt.ylabel(metrica)
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.savefig(f'./{filename}')
    plt.show()

comparacao_plot('Aceleração - Matriz 500x500, 1000x1000, 2000x2000', 'aceleracao_combinada.png', df2, 'Aceleração')
comparacao_plot('Eficiência - Matriz 500x500, 1000x1000, 2000x2000', 'eficiencia_combinada.png', df3, 'Eficiência')
