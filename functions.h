#ifndef _MATRIX_H
#define _MATRIX_H
#define MOD 10007

// cele 2 structuri se folosesc pentru strassen matrix multiply
typedef struct {
	int **M11, **M12, **M21, **M22;
} quarters;

typedef struct {
	int **M1, **M2, **M3, **M4, **M5, **M6, **M7;
} strassen_prod;

typedef struct {
	int **M11_M12, **M12_M22, **M11_M22, **M21_M22, **M11_M21;
} first_op;

typedef struct {
	int **M11_M12, **M12_M22, **M11_M22, **M21_M22, **M21_M11;
} second_op;

int **alloc_new_matrix(int n, int m);
int **add_2_matrix(int **M1, int **M2, int n);
int **subtract(int **M1, int **M2, int n);
void copy_mat_C(int k, quarters mat_C, int **C);
void create_matrix(quarters mat_A, quarters mat_B, int k, int **A, int **B);
void free_strassen_aux(strassen_prod aux, int n);
void free_quarters(quarters mat, int n);
void free_first_op(first_op mat, int n);
void free_second_op(second_op mat, int n);
void alloc_quarters(quarters *A, int k);
int **strassen(int **A, int **B, int n);
void strassen_mat(int ***v, int *line, int *col, int *size, int ind1, int ind2);

#endif
