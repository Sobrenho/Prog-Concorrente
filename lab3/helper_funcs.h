#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H

typedef struct {
	float* matriz;
	int N;
	int M;

}FloatMatriz;

void escrever_matriz(FloatMatriz *m, char* filename);
FloatMatriz* pegar_matriz(char* filename);
void free_floatMatriz(FloatMatriz* fm);

#endif