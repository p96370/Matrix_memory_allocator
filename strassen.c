#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int **alloc_new_matrix(int n, int m)
{
	// aloc n linii pentru pointerul dublu
	int **mat = (int **)malloc(n * sizeof(int *));
	if (!mat) {
		fprintf(stderr, "Malloc failed\n");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		// pentru fiecare linie aloc m elemente
		mat[i] = (int *)malloc(m * sizeof(int));
		if (!mat[i]) {
			fprintf(stderr, "Malloc failed\n");
			return NULL;
		}
	}
	return mat;
}

// adun 2 matrice cu aceeasi dimensiune n
int **add_2_matrix(int **M1, int **M2, int n)
{
	int **aux = alloc_new_matrix(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			aux[i][j] = (M1[i][j] + M2[i][j]) % MOD;
			if (aux[i][j] < 0)
				aux[i][j] += MOD;
		}
	return aux;
}

// scad 2 matrice cu aceeasi dimensiune n
int **subtract(int **M1, int **M2, int n)
{
	int **aux = alloc_new_matrix(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			aux[i][j] = (M1[i][j] - M2[i][j]) % MOD;
			if (aux[i][j] < 0)
				aux[i][j] += MOD;
		}
	return aux;
}

// copiez in matricea C valorile din structura mat_C
void copy_mat_C(int k, quarters mat_C, int **C)
{
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			C[i][j] = mat_C.M11[i][j];
			C[i][j + k] = mat_C.M12[i][j];
			C[i + k][j] = mat_C.M21[i][j];
			C[i + k][j + k] = mat_C.M22[i][j];
		}
}

void create_matrix(quarters mat_A, quarters mat_B, int k, int **A, int **B)
{
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			mat_A.M11[i][j] = A[i][j], mat_A.M12[i][j] = A[i][j + k];
			mat_A.M21[i][j] = A[i + k][j], mat_A.M22[i][j] = A[i + k][j + k];
			mat_B.M11[i][j] = B[i][j], mat_B.M12[i][j] = B[i][j + k];
			mat_B.M21[i][j] = B[i + k][j], mat_B.M22[i][j] = B[i + k][j + k];
		}
}

// self explanatory
void free_strassen_aux(strassen_prod aux, int n)
{
	for (int i = 0; i < n; i++) {
		free(aux.M1[i]);
		free(aux.M2[i]);
		free(aux.M3[i]);
		free(aux.M4[i]);
		free(aux.M5[i]);
		free(aux.M6[i]);
		free(aux.M7[i]);
	}
	free(aux.M1);
	free(aux.M2);
	free(aux.M3);
	free(aux.M4);
	free(aux.M5);
	free(aux.M6);
	free(aux.M7);
}

// self explanatory
void free_quarters(quarters mat, int n)
{
	for (int i = 0; i < n; i++) {
		free(mat.M11[i]);
		free(mat.M12[i]);
		free(mat.M21[i]);
		free(mat.M22[i]);
	}
	free(mat.M11);
	free(mat.M12);
	free(mat.M21);
	free(mat.M22);
}

void free_first_op(first_op mat, int n)
{
	for (int i = 0; i < n; i++) {
		free(mat.M11_M12[i]);
		free(mat.M11_M21[i]);
		free(mat.M11_M22[i]);
		free(mat.M12_M22[i]);
		free(mat.M21_M22[i]);
	}
	free(mat.M11_M12);
	free(mat.M11_M21);
	free(mat.M11_M22);
	free(mat.M12_M22);
	free(mat.M21_M22);
}

void free_second_op(second_op mat, int n)
{
	for (int i = 0; i < n; i++) {
		free(mat.M11_M12[i]);
		free(mat.M11_M22[i]);
		free(mat.M12_M22[i]);
		free(mat.M21_M22[i]);
		free(mat.M21_M11[i]);
	}
	free(mat.M11_M12);
	free(mat.M11_M22);
	free(mat.M12_M22);
	free(mat.M21_M22);
	free(mat.M21_M11);
}

void alloc_quarters(quarters *A, int k)
{
	A->M11 = alloc_new_matrix(k, k); // (*A).M11
	A->M12 = alloc_new_matrix(k, k);
	A->M21 = alloc_new_matrix(k, k);
	A->M22 = alloc_new_matrix(k, k);
}

int **strassen(int **A, int **B, int n)
{
	if (n == 1) {
		int **C = alloc_new_matrix(n, n);
		C[0][0] = A[0][0] * B[0][0];
		return C;
	}
	// C este matricea rezultat
	int **C = alloc_new_matrix(n, n), k = n / 2;
	// impart matricele A, B, C in 4 cadrane: M11, M12, M21, M22
	quarters mat_A, mat_B, mat_C;
	// aloc noile matrice-cadrane
	alloc_quarters(&mat_A, k);
	alloc_quarters(&mat_B, k);
	create_matrix(mat_A, mat_B, k, A, B);
	// Pentru algoritmul Strassen folosesc mai multe matrice mai mici
	// Care ar fi putut si calculate direct daca foloseam alocare statica.
	// Dar pentru ca folosesc alocare dinamica am facut variabile pentru
	// aceste matrice cu un singur scop: sa le pastrez adresa
	// undeva in memorie, pentru a le putea elibera ulterior.
	first_op A2;
	A2.M11_M12 = add_2_matrix(mat_A.M11, mat_A.M12, k);
	A2.M12_M22 = subtract(mat_A.M12, mat_A.M22, k);
	A2.M11_M22 = add_2_matrix(mat_A.M11, mat_A.M22, k);
	A2.M21_M22 = add_2_matrix(mat_A.M21, mat_A.M22, k);
	A2.M11_M21 = subtract(mat_A.M11, mat_A.M21, k);

	second_op B2;
	B2.M11_M12 = add_2_matrix(mat_B.M11, mat_B.M12, k);
	B2.M12_M22 = subtract(mat_B.M12, mat_B.M22, k);
	B2.M11_M22 = add_2_matrix(mat_B.M11, mat_B.M22, k);
	B2.M21_M22 = add_2_matrix(mat_B.M21, mat_B.M22, k);
	B2.M21_M11 = subtract(mat_B.M21, mat_B.M11, k);
	// Produse auxiliare Strassen, de exemplu:
	strassen_prod aux;
	aux.M1 = strassen(mat_A.M11, B2.M12_M22, k);
	aux.M2 = strassen(A2.M11_M12, mat_B.M22, k);
	aux.M3 = strassen(A2.M21_M22, mat_B.M11, k);
	aux.M4 = strassen(mat_A.M22, B2.M21_M11, k);
	aux.M5 = strassen(A2.M11_M22, B2.M11_M22, k);
	aux.M6 = strassen(A2.M12_M22, B2.M21_M22, k);
	aux.M7 = strassen(A2.M11_M21, B2.M11_M12, k);

	// 4 matrice auxiliare cu aceleasi scop ca cele de mai sus
	int **mat1 = add_2_matrix(aux.M5, aux.M4, k);
	int **mat2 = add_2_matrix(mat1, aux.M6, k);
	int **mat3 = add_2_matrix(aux.M5, aux.M1, k);
	int **mat4 = subtract(mat3, aux.M3, k);
	mat_C.M11 = subtract(mat2, aux.M2, k);
	mat_C.M12 = add_2_matrix(aux.M1, aux.M2, k);
	mat_C.M21 = add_2_matrix(aux.M3, aux.M4, k);
	mat_C.M22 = subtract(mat4, aux.M7, k);

	// Le eliberez pe toate :)
	copy_mat_C(k, mat_C, C);
	free_quarters(mat_A, k);
	free_quarters(mat_B, k);
	free_quarters(mat_C, k);
	free_strassen_aux(aux, k);
	free_first_op(A2, k);
	free_second_op(B2, k);

	for (int i = 0; i < k; i++) {
		free(mat1[i]);
		free(mat2[i]);
		free(mat3[i]);
		free(mat4[i]);
	}
	free(mat1);
	free(mat2);
	free(mat3);
	free(mat4);
	return C;
}

// Creez un nou element cu elemente normale cu exceptia faptului
// ca matricea este calculata folosind algoritmul lui Strassen
void strassen_mat(int ***v, int *line, int *col, int *size, int ind1, int ind2)
{
	// elementele "normale"
	line[*size] = line[ind1];
	col[*size] = col[ind2];
	// matricea se calculeaza folosind o alta functie
	v[*size] = strassen(v[ind1], v[ind2], line[*size]);
	(*size)++;
}
