#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

void read_matrix(int m, int n, int *size_array, int ***v);

void alloc_matrix(int *line, int *col, int *size_array, int ***v)
{
	int m, n;
	scanf("%d%d", &m, &n);
	line[*size_array] = m;
	col[*size_array] = n;
	v[*size_array] = NULL;
	//se aloca m linii pt v[*size_array]
	v[*size_array] = (int **)malloc(m * sizeof(int *));
	if (!v[*size_array]) {
		fprintf(stderr, "Malloc failed\n");
		return;
	}
	for (int i = 0; i < m; i++) {
		// se aloca n elemente pt fiecare linie a lui v[*size_array]
		v[*size_array][i] = (int *)malloc(n * sizeof(int));
		if (!v[*size_array][i]) {
			fprintf(stderr, "Malloc failed\n");
			return;
		}
	}
	read_matrix(m, n, size_array, v);
}

// self-explanatory
void read_matrix(int m, int n, int *size_array, int ***v)
{
	// se citesc elementele matricei
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			scanf("%d", &v[*size_array][i][j]);
	}
	(*size_array)++;
}

void invalid(void)
{
	printf("No matrix with the given index\n");
}

void print_matrix(int *line, int *col, int ***v, int size_array)
{
	int index;
	scanf("%d", &index);
	if (index >= size_array || index < 0) {
		invalid();
		return;
	}
	// se afieaza matricea
	for (int i = 0; i < line[index]; i++) {
		for (int j = 0; j < col[index]; j++)
			printf("%d ", v[index][i][j]);
		printf("\n");
	}
}

void redimension_matrix(int ***v, int *n, int *m, int index, int size_array)
{
	int nr_lin, nr_col;
	int *new_lines, *new_colls;
	scanf("%d", &nr_lin);
	new_lines = (int *)malloc(nr_lin * sizeof(int));  // aloc noile linii
	for (int i = 0; i < nr_lin; i++)
		scanf("%d", &new_lines[i]); // citesc si retin liniile
	scanf("%d", &nr_col);
	new_colls = (int *)malloc(nr_col * sizeof(int));  /// aloc noile coloane
	for (int i = 0; i < nr_col; i++)
		scanf("%d", &new_colls[i]); // citesc si retin coloanele
	if (index >= size_array || index < 0) {
		invalid();
		free(new_lines);
		free(new_colls);
		return;
	}
	// aloc matrice noua
	int **new_mat = (int **)calloc(nr_lin, sizeof(int *));
	for (int i = 0; i < nr_lin; i++) {
		new_mat[i] = (int *)calloc(nr_col, sizeof(int));
		for (int j = 0; j < nr_col; j++)
			// pun in noua matrice doar numerele dorite
			new_mat[i][j] = v[index][new_lines[i]][new_colls[j]];
	}
	// eliberez memoria vechii matrice
	for (int i = 0; i < *n; i++)
		free(v[index][i]);
	// actualizez dimensiunile noii matrice
	*n = nr_lin;
	*m = nr_col;
	// eliberez memoria
	free(v[index]);
	free(new_lines);
	free(new_colls);
	// v[index] are acum doar valorile cerute
	v[index] = new_mat;
}

// self-explanatory
void read_values_C(void)
{
	int n, m, x;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf("%d", &x);
	scanf("%d", &m);
	for (int i = 0; i < m; i++)
		scanf("%d", &x);
	invalid();
}

void print_dimension(int *line, int *col, int size_array)
{
	int index;
	scanf("%d", &index);
	if (index >= size_array || index < 0)
		invalid();
	else
		printf("%d %d\n", line[index], col[index]);
}

int check_multiply(int ind1, int ind2, int size, int *line, int *col)
{
	// verific daca se poate realiza inmultirea a 2 matrice
	if (ind1 >= size || ind2 >= size || ind1 < 0 || ind2 < 0) {
		invalid();
		return 0;
	}
	if (col[ind1] != line[ind2]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}
	return 1;
}

int **mat_multi(int **a, int **b, int na, int m, int mb, int *size)
{
	// produsul a 2 matrice
	int **prod;
	// aloc na linii pentru produs
	prod = (int **)malloc(na * sizeof(int *));
	for (int i = 0; i < na; i++)
		// aloc mb elemente pe fiecare linie
		prod[i] = (int *)malloc(mb * sizeof(int));
	for (int i = 0; i < na; i++) {
		for (int k = 0; k < mb; k++) {
			prod[i][k] = 0;
			for (int j = 0; j < m; j++) {
				prod[i][k] += a[i][j] * b[j][k];
				prod[i][k] %= MOD;
			}
			prod[i][k] %= MOD;
			// daca valoarea este negativa atunci adun MOD
			if (prod[i][k] < 0)
				prod[i][k] = MOD + prod[i][k];
		}
	}
	// cresc dimensiunea lui v
	(*size)++;
	return prod;
}

int sum_matrix(int **a, int n, int m)
{
	long long sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			sum = (sum + a[i][j]) % MOD;
	}
	sum %= MOD;
	if (sum < 0)
		sum += MOD;
	return sum;
}

void sort_matrix(int ***v, int size_array, int *line, int *col)
{
	int *sum_array = NULL;
	// aloc vectorul de sume
	sum_array = (int *)malloc(size_array * sizeof(int));
	// calculez suma fiecarei matrice din v
	for (int index = 0; index < size_array; index++)
		sum_array[index] = sum_matrix(v[index], line[index], col[index]);

	for (int i = 0; i < size_array - 1; i++) {
		for (int j = i + 1; j < size_array; j++) {
			if (sum_array[i] > sum_array[j]) {
				int aux = sum_array[i];
				sum_array[i] = sum_array[j];
				sum_array[j] = aux;

				int **mat = v[i];
				v[i] = v[j];
				v[j] = mat;

				aux = line[i];
				line[i] = line[j];
				line[j] = aux;

				aux = col[i];
				col[i] = col[j];
				col[j] = aux;
			}
		}
	}
	free(sum_array);
}

int **trans_matrix(int **a, int n, int m, int *line, int *col, int index)
{
	int **mat = (int **)malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++) {
		mat[i] = (int *)malloc(n * sizeof(int));
		for (int j = 0; j < n; j++)
			mat[i][j] = a[j][i];
	}
	int aux = line[index]; // interschimb dimensiunile
	line[index] = col[index];
	col[index] = aux;

	int **copy = a;  // interschimb v[index] cu transpusa
	a = mat;
	mat = copy;
	// eliberez memoria
	for (int i = 0; i < n; i++)
		free(copy[i]);
	free(copy);
	return a;
}

int **putere(int **a, int **b, int dim)
{
	// tot inmultirea a 2 matrice, dar patratice cu aceeasi dimensiune
	int **c = NULL;
	// aloc matricea produs
	c = (int **)malloc(dim * sizeof(int *));
	for (int i = 0; i < dim; i++)
		c[i] = (int *)malloc(dim * sizeof(int));
	for (int i = 0; i < dim; i++) {
		for (int k = 0; k < dim; k++) {
			int val = 0;
			for (int j = 0; j < dim; j++)
				val = (val + b[i][j] * a[j][k]) % MOD;
			if (val < 0)
				val = MOD + val;
			c[i][k] = val;
		}
	}
	// copiez rezultatul in matricea a
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++)
			a[i][j] = c[i][j];
	}
	// eliberez memoria
	for (int i = 0; i < dim; i++)
		free(c[i]);
	free(c);
	return a;
}

int **pow_matrix(int **mat, int put, int dim)
{
	// exponentiere rapida de matrice
	if (put == 0) {
		int **cop = (int **)calloc(dim, sizeof(int *));
		for (int i = 0; i < dim; i++) {
			cop[i] = (int *)calloc(dim, sizeof(dim));
			cop[i][i] = 1;
		}
		return cop;
	}
	if (put % 2 == 1)
		return putere(pow_matrix(mat, put - 1, dim), mat, dim);
	int **cop = pow_matrix(mat, put / 2, dim);
	return putere(cop, cop, dim);
}

void check_power_matrix(int ***v, int size_array, int *line, int *col)
{
	// verific daca se pot inmulti matricele
	int index, put;
	scanf("%d%d", &index, &put);
	if (index >= size_array || index < 0) {
		invalid();
		return;
	}
	if (put < 0) {
		printf("Power should be positive\n");
		return;
	}
	if (line[index] != col[index]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	int dim = line[index];
	// copiez matricea in mat
	int **mat = v[index];
	v[index] = pow_matrix(mat, put, dim);
	// eliberez memoria
	for (int i = 0; i < dim; i++)
		free(mat[i]);
	free(mat);
}

void free_matrix(int ***v, int *size_array, int *line, int *col)
{
	// verific daca indexul e valid
	int index;
	scanf("%d", &index);
	if (index >= *size_array || index < 0) {
		invalid();
		return;
	}
	// eliberez liniile matricei cerute
	for (int i = 0; i < line[index]; i++)
		free(v[index][i]);
	// eliberez toata matricea
	free(v[index]);
	// mut toate matricele din vector cu o pozitie la stanga
	for (int i = index; i < *size_array - 1; i++) {
		v[i] = v[i + 1];
		line[i] = line[i + 1];
		col[i] = col[i + 1];
	}
	// scad dimensiunea lui v
	(*size_array)--;
}

int check_strassen(int index1, int index2, int size, int *line, int *col)
{
	// verific indecsii sunt valizi si pot realiza algoritmul
	if (index1 >= size || index2 >= size) {
		printf("No matrix with the given index\n");
		return 0;
	}
	if (index1 < 0 || index2 < 0) {
		printf("No matrix with the given index\n");
		return 0;
	}
	if (line[index1] != col[index1] || line[index2] != col[index2]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}
	if (line[index1] != line[index2]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}
	return 1;
}

void quit(int ***v, int *line, int *col, int size_array)
{
	for (int i = 0; i < size_array; i++) {
		for (int j = 0; j < line[i]; j++)
			free(v[i][j]);
		free(v[i]);
	}
	free(v);
	free(line);
	free(col);
}

int main(void)
{
	int ***v = NULL, *line = NULL, *col = NULL;
	v = (int ***)calloc(1, sizeof(int **));
	line = (int *)calloc(1, sizeof(int));
	col = (int *)calloc(1, sizeof(int));
	char lit, com[] = {"LDPCMOTRFSQ"};
	int m, n, size_array = 0, index;
	do {
		scanf("%c", &lit);
		if (lit == 'L') {
			v = (int ***)realloc(v, (size_array + 1) * sizeof(int **));
			line = (int *)realloc(line, (size_array + 1) * sizeof(int));
			col = (int *)realloc(col, (size_array + 1) * sizeof(int));
			alloc_matrix(line, col, &size_array, v);
		}
		if (lit == 'D')
			print_dimension(line, col, size_array);
		if (lit == 'P')
			print_matrix(line, col, v, size_array);
		if (lit == 'C') {
			scanf("%d", &index);
			if (index < size_array && index >= 0) {
				int n = line[index], m = col[index];
				redimension_matrix(v, &n, &m, index, size_array);
				line[index] = n;
				col[index] = m;
			} else {
				read_values_C();
			}
		}
		if (lit == 'M') {
			int index1, index2;
			scanf("%d%d", &index1, &index2);
			if (check_multiply(index1, index2, size_array, line, col)) {
				n = line[index1];
				int k = col[index1];
				m = col[index2];
				int **prod = NULL;
				prod = mat_multi(v[index1], v[index2], n, k, m, &size_array);
				v = (int ***)realloc(v, size_array * sizeof(int **));
				v[size_array - 1] = prod;
				line = (int *)realloc(line, size_array * sizeof(int));
				col = (int *)realloc(col, size_array * sizeof(int));
				line[size_array - 1] = line[index1];
				col[size_array - 1] = col[index2];
			}
		}
		if (lit == 'O')
			sort_matrix(v, size_array, line, col);
		if (lit == 'T') {
			scanf("%d", &index);
			if (index >= size_array || index < 0) {
				invalid();
			} else {
				n = line[index];
				m = col[index];
				v[index] = trans_matrix(v[index], n, m, line, col, index);
			}
		}
		if (lit == 'R')
			check_power_matrix(v, size_array, line, col);
		if (lit == 'F')
			free_matrix(v, &size_array, line, col);
		if (lit == 'S') {
			int index1, index2;
			scanf("%d%d", &index1, &index2);
			if (check_strassen(index1, index2, size_array, line, col)) {
				v = (int ***)realloc(v, (size_array + 1) * sizeof(int **));
				line = (int *)realloc(line, (size_array + 1) * sizeof(int));
				col = (int *)realloc(col, (size_array + 1) * sizeof(int));
				strassen_mat(v, line, col, &size_array, index1, index2);
			}
		}
		if (lit == 'Q')
			quit(v, line, col, size_array);
		if (!strchr(com, lit))
			printf("Unrecognized command\n");
		getchar();
	} while (lit != 'Q');
	return 0;
}
