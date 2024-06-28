// Coitu Sebastian-Teodor 314CA
#include <stdio.h>
#include <stdlib.h>

// Afișez matricea
void arr_print(int ***arr, int **dimensions, int index)
{
	for (int i = 0; i < dimensions[index][0]; i++) {
		for (int j = 0; j < dimensions[index][1]; j++)
			printf("%d ", arr[index][i][j]);
		printf("\n");
	}
}

// Eliberez memoria din vectorul de matrici și din matricea de dimensiuni
void mem_free(int ***arr, int **dimensions, int arrsize)
{
	int i, k;

	for (k = 0; k < arrsize; k++)
		for (i = 0; i < dimensions[k][0]; i++)
			free(arr[k][i]);
	for (i = 0; i < arrsize; i++)
		free(arr[i]);
	free(arr);

	for (i = 0; i < arrsize; i++)
		free(dimensions[i]);
	free(dimensions);
}

// Adaug o matrice nouă în memorie
void L(int **dimensions, int ***arr, int arrsize)
{
	int m, n, i, j;
	scanf("%d%d", &m, &n);
	dimensions[arrsize] = (int *)malloc(sizeof(int) * 2);
	if (!dimensions[arrsize]) {
		printf("Eroare la alocare\n");
		return;
	}
	dimensions[arrsize][0] = m;
	dimensions[arrsize][1] = n;

	arr[arrsize] = (int **)calloc(m, sizeof(int *));
	for (i = 0; i < m; i++)
		arr[arrsize][i] = (int *)calloc(n, sizeof(int));

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			scanf("%d", &arr[arrsize][i][j]);
}

// Afișez dimensiunile metricei
void D(int **dimensions, int arrsize)
{
	int index;
	scanf("%d", &index);

	if (index >= arrsize || index < 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", dimensions[index][0], dimensions[index][1]);
}

// Afișez matricea
void P(int ***arr, int **dimensions, int arrsize)
{
	int index;
	scanf("%d", &index);

	if (index >= arrsize || index < 0)
		printf("No matrix with the given index\n");
	else
		arr_print(arr, dimensions, index);
}

void vector_read(int n, int v[])
{
	for (int i = 0; i < n; i++)
		scanf("%d", &v[i]);
}

// Redimensionez matricea în funcție de liniile și coloanele date
void C(int ***arr, int **dimensions, int arrsize)
{
	int l, c, vl[10001] = {0}, vc[10001] = {0}, index, i, j;
	scanf("%d", &index);

	scanf("%d", &l);
	vector_read(l, vl);
	scanf("%d", &c);
	vector_read(c, vc);

	if (index < 0 || index >= arrsize) {
		printf("No matrix with the given index\n");
	} else {
		if (arrsize != index) {
			int **new_arr = (int **)calloc(l, sizeof(int *));
			if (!new_arr) {
				printf("Eroare la alocare\n");
				return;
			}
			for (i = 0; i < l; i++)
				new_arr[i] = (int *)calloc(c, sizeof(int));

			// Adaug într-o matrice auxiliară liniile și coloanele date
			for (i = 0; i < l; i++)
				for (j = 0; j < c; j++)
					new_arr[i][j] = arr[index][vl[i]][vc[j]];

			for (i = 0; i < dimensions[index][0]; i++)
				free(arr[index][i]);
			free(arr[index]);

			dimensions[index][0] = l;
			dimensions[index][1] = c;

			arr[index] = (int **)calloc(l, sizeof(int *));
			if (!arr[index]) {
				printf("Eroare la alocare\n");
				return;
			}
			for (i = 0; i < l; i++)
				arr[index][i] = (int *)calloc(c, sizeof(int));
			/*
			Copiez elementele din matricea auxiliară în cea principală,
			după ce îi modific dimensiunile
			*/
			for (i = 0; i < l; i++)
				for (j = 0; j < c; j++)
					arr[index][i][j] = new_arr[i][j];

			for (i = 0; i < l; i++)
				free(new_arr[i]);
			free(new_arr);
		}
	}
}

// Verific condițiile pentru înmulțirea a două matrice
int M_check(int index1, int index2, int arrsize, int **dimensions)
{
	if (index1 >= arrsize || index1 < 0 || index2 >= arrsize || index2 < 0) {
		printf("No matrix with the given index\n");
		return 0;
	} else if (dimensions[index1][1] != dimensions[index2][0]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}

	return 1;
}

// Înmulțesc două matrice
void M(int ***arr, int **dimensions, int arrsize, int i1, int i2)
{
	int i, j, k;
	arr[arrsize] = (int **)calloc(dimensions[i1][0], sizeof(int *));
	if (!arr[arrsize]) {
		printf("Eroare la alocare\n");
		return;
	}
	for (i = 0; i < dimensions[i1][0]; i++)
		arr[arrsize][i] = (int *)calloc(dimensions[i2][1], sizeof(int));

	// Adaug matricea rezultată pe ultima poziție din vectorul de matrici
	for (i = 0; i < dimensions[i1][0]; i++)
		for (j = 0; j < dimensions[i2][1]; j++) {
			for (k = 0; k < dimensions[i1][1]; k++)
				arr[arrsize][i][j] += (arr[i1][i][k] * arr[i2][k][j]) % 10007;
			if (arr[arrsize][i][j] % 10007 < 0)
				arr[arrsize][i][j] = (arr[arrsize][i][j] % 10007) + 10007;
			else
				arr[arrsize][i][j] = arr[arrsize][i][j] % 10007;
		}

	/*
	Adaug dimensiunile matricei rezultate pe ultima poziție a matricei de
	dimensiuni
	*/
	dimensions[arrsize] = (int *)malloc(sizeof(int) * 2);
	if (!dimensions[arrsize]) {
		printf("Eroare la alocare\n");
		return;
	}
	dimensions[arrsize][0] = dimensions[i1][0];
	dimensions[arrsize][1] = dimensions[i2][1];
}

// Fac suma elementelor matricei
int elem_sum(int ***arr, int index, int **dimensions)
{
	int s = 0, i, j;
	for (i = 0; i < dimensions[index][0]; i++)
		for (j = 0; j < dimensions[index][1]; j++)
			s = s + arr[index][i][j];

	if (s < 0)
		return (s % 10007 + 10007);
	else
		return s % 10007;
}

// Ordonez matricele
void O(int ***arr, int **dimensions, int arrsize)
{
	int i, j;

	for (i = 0; i < arrsize - 1; i++)
		for (j = i + 1; j < arrsize; j++) {
			int s1 = 0, s2 = 0;
			s1 = elem_sum(arr, i, dimensions);
			s2 = elem_sum(arr, j, dimensions);

			if (s1 > s2) {
				int **aux1;
				int aux2, aux3;

				// Interschimbarea matricelor și a dimensiunilor
				aux1 = arr[i];
				aux2 = dimensions[i][0], aux3 = dimensions[i][1];
				arr[i] = arr[j], arr[j] = aux1;
				dimensions[i][0] = dimensions[j][0];
				dimensions[i][1] = dimensions[j][1];
				dimensions[j][0] = aux2, dimensions[j][1] = aux3;
			}
		}
}

// Fac transpusa unei matrice
void T(int ***arr, int **dimensions, int arrsize)
{
	int index, i, j;
	scanf("%d", &index);
	if (index >= arrsize || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		int **new_arr = (int **)calloc(dimensions[index][1], sizeof(int *));
		if (!new_arr) {
			printf("Eroare la alocare\n");
			return;
		}
		for (i = 0; i < dimensions[index][1]; i++)
			new_arr[i] = (int *)calloc(dimensions[index][0], sizeof(int));

		// Copiez elementele matricei principale într-una auxiliară
		for (i = 0; i < dimensions[index][1]; i++)
			for (j = 0; j < dimensions[index][0]; j++)
				new_arr[i][j] = arr[index][j][i];

		for (i = 0; i < dimensions[index][0]; i++)
			free(arr[index][i]);
		free(arr[index]);

		// Modific dimensiunile matricei principale
		arr[index] = (int **)calloc(dimensions[index][1], sizeof(int *));
		if (!arr[index]) {
			printf("Eroare la alocare\n");
			return;
		}
		for (i = 0; i < dimensions[index][1]; i++)
			arr[index][i] = (int *)calloc(dimensions[index][0], sizeof(int));

		// Copiez elementele din matricea auxiliară în cea principală
		for (i = 0; i < dimensions[index][1]; i++)
			for (j = 0; j < dimensions[index][0]; j++)
				arr[index][i][j] = new_arr[i][j];

		// Actualizez dimensiunile în matricea de dimensiuni
		int aux;
		aux = dimensions[index][0];
		dimensions[index][0] = dimensions[index][1];
		dimensions[index][1] = aux;

		for (i = 0; i < dimensions[index][0]; i++)
			free(new_arr[i]);
		free(new_arr);
	}
}

// Fac înmulțirea unei matrice date cu o matrice din vectorul de matrici
void arr_multiplication(int ***arr, int **new_arr, int **dimensions, int index)
{
	int i, j, k;
	int **aux_arr = (int **)calloc(dimensions[index][0], sizeof(int *));
	if (!aux_arr) {
		printf("Eroare la alocare\n");
		return;
	}
	for (i = 0; i < dimensions[index][0]; i++)
		aux_arr[i] = (int *)calloc(dimensions[index][0], sizeof(int));

	// Memorez elementele rezultate într-o matrice auxiliară
	for (i = 0; i < dimensions[index][0]; i++)
		for (j = 0; j < dimensions[index][0]; j++) {
			for (k = 0; k < dimensions[index][0]; k++)
				aux_arr[i][j] += (new_arr[i][k] * arr[index][k][j]) % 10007;
			if (aux_arr[i][j] % 10007 < 0)
				aux_arr[i][j] = (aux_arr[i][j] % 10007) + 10007;
			else
				aux_arr[i][j] = aux_arr[i][j] % 10007;
		}
	// Copiez matricea auxiliară în matricea dată inițial
	for (i = 0; i < dimensions[index][0]; i++)
		for (j = 0; j < dimensions[index][0]; j++)
			new_arr[i][j] = aux_arr[i][j];

	for (i = 0; i < dimensions[index][0]; i++)
		free(aux_arr[i]);
	free(aux_arr);
}

/*
Fac puterea a doua a unei matrice din vectorul de matrici fără să o adaug la
sfărșitul vectorului
*/
void power_2(int ***arr, int **dimensions, int index)
{
	int i, j, k;
	int **aux1 = (int **)calloc(dimensions[index][0], sizeof(int *));
	if (!aux1) {
		printf("Eroare la alocare\n");
		return;
	}
	for (i = 0; i < dimensions[index][0]; i++)
		aux1[i] = (int *)calloc(dimensions[index][0], sizeof(int));
	int **aux2 = (int **)calloc(dimensions[index][0], sizeof(int *));
	if (!aux2) {
		printf("Eroare la alocare\n");
		return;
	}
	for (i = 0; i < dimensions[index][0]; i++)
		aux2[i] = (int *)calloc(dimensions[index][0], sizeof(int));

	// Două matrice auxiliare care conțin elementele matricei principale
	for (i = 0; i < dimensions[index][0]; i++)
		for (j = 0; j < dimensions[index][0]; j++) {
			aux1[i][j] = arr[index][i][j];
			aux2[i][j] = arr[index][i][j];
		}

	for (i = 0; i < dimensions[index][0]; i++)
		for (j = 0; j < dimensions[index][0]; j++)
			arr[index][i][j] = 0;
	/*
		Fac înmulțirea matricelor auxiliare, punând elementele rezultate în
		matricea principală
	*/
	for (i = 0; i < dimensions[index][0]; i++)
		for (j = 0; j < dimensions[index][0]; j++) {
			for (k = 0; k < dimensions[index][0]; k++)
				arr[index][i][j] += (aux1[i][k] * aux2[k][j]) % 10007;
			if (arr[index][i][j] % 10007 < 0)
				arr[index][i][j] = (arr[index][i][j] % 10007) + 10007;
			else
				arr[index][i][j] = arr[index][i][j] % 10007;
		}

	for (i = 0; i < dimensions[index][0]; i++) {
		free(aux1[i]);
		free(aux2[i]);
	}
	free(aux1);
	free(aux2);
}

// Înmulțirea a două matrice în timp logaritmic
void R(int ***arr, int **dimensions, int arrsize)
{
	int index, power, i, j;
	scanf("%d%d", &index, &power);

	if (index >= arrsize || index < 0) {
		printf("No matrix with the given index\n");
	} else if (power < 0) {
		printf("Power should be positive\n");
	} else if (dimensions[index][0] != dimensions[index][1]) {
		printf("Cannot perform matrix multiplication\n");
	// Dacă puterea este 0, rezultă matricea identitate
	} else if (power == 0) {
		for (i = 0; i < dimensions[index][0]; i++)
			for (j = 0; j < dimensions[index][1]; j++)
				if (i == j)
					arr[index][i][j] = 1;
				else
					arr[index][i][j] = 0;
	} else {
		int **new_arr = (int **)calloc(dimensions[index][0], sizeof(int *));
		if (!new_arr) {
			printf("Eroare la alocare\n");
			return;
		}
		for (i = 0; i < dimensions[index][0]; i++)
			new_arr[i] = (int *)calloc(dimensions[index][0], sizeof(int));

		// Fac o matrice identitate auxiliară
		for (i = 0; i < dimensions[index][0]; i++)
			for (j = 0; j < dimensions[index][0]; j++)
				if (i == j)
					new_arr[i][j] = 1;
				else
					new_arr[i][j] = 0;

		/*
		Dacă puterea este multiplu de 2, fac ridicarea la pătrat, iar dacă
		nu este multiplu de 2 fac înmulțirea cu matricea auxiliară
		*/
		while (power > 0) {
			if (power % 2 == 1)
				arr_multiplication(arr, new_arr, dimensions, index);

			power_2(arr, dimensions, index);
			power = power / 2;
		}

		// În matricea principală copiez matricea auxiliară
		for (i = 0; i < dimensions[index][0]; i++)
			for (j = 0; j < dimensions[index][0]; j++)
				arr[index][i][j] = new_arr[i][j];

		for (i = 0; i < dimensions[index][0]; i++)
			free(new_arr[i]);
		free(new_arr);
	}
}

// Elimin o matrice din vectorul de matrici
void F(int ***arr, int arrsize, int **dimensions, int index)
{
	int i;
	for (i = 0; i < dimensions[index][0]; i++)
		free(arr[index][i]);
	free(arr[index]);

	free(dimensions[index]);

	for (i = index; i < arrsize - 1; i++) {
		arr[i] = arr[i + 1];
		dimensions[i] = dimensions[i + 1];
	}
}

// Verific condițiile în care se poate aplica algoritmul lui Strassen
int S_check(int index1, int index2, int arrsize, int **dimensions)
{
	int size = dimensions[index1][0];
	if (index1 >= arrsize || index1 < 0 || index2 >= arrsize || index2 < 0) {
		printf("No mmatrix with the given index\n");
		return 0;
	} else if (dimensions[index1][1] == dimensions[index2][0]) {
		while (size != 1) {
			if (size % 2 != 0) {
				printf("Cannot perform matrix multiplication\n");
				return 0;
			}
			size = size / 2;
		}
		return 1;
	}
	printf("Cannot perform matrix multiplication\n");
	return 0;
}

// Aloc memorie pentru o noua matrice
int **arr_alloc(int size)
{
	int i;
	int **new_arr = (int **)calloc(size, sizeof(int *));
	for (i = 0; i < size; i++)
		new_arr[i] = (int *)calloc(size, sizeof(int));
	return new_arr;
}

// Dealoc o matrice din memorie
void arr_dealloc(int **arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
	free(arr);
}

// Adun două matrice
void arr_add(int **M1, int **M2, int **M3, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			M3[i][j] = (M1[i][j] + M2[i][j]) % 10007;
			if (M3[i][j] < 0)
				M3[i][j] += 10007;
		}
}

// Scad două matrice
void arr_sub(int **M1, int **M2, int **M3, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			M3[i][j] = (M1[i][j] - M2[i][j]) % 10007;
			if (M3[i][j] < 0)
				M3[i][j] += 10007;
		}
}

// Împart matricele A și B în 4 blocuri
void arr_split(int **A_11, int **A_12, int **A_21, int **A_22,
			   int **B_11, int **B_12, int **B_21, int **B_22,
			   int **A, int **B, int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			A_11[i][j] = A[i][j];
			A_12[i][j] = A[i][j + dim];
			A_21[i][j] = A[i + dim][j];
			A_22[i][j] = A[i + dim][j + dim];

			B_11[i][j] = B[i][j];
			B_12[i][j] = B[i][j + dim];
			B_21[i][j] = B[i + dim][j];
			B_22[i][j] = B[i + dim][j + dim];
		}
}

// Construiesc matricea rezultat
void arr_combin(int **M1, int **M2, int **M3, int **M4, int **M5, int **M6,
				int **M7, int **C, int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			// Blocul din stânga sus
			C[i][j] = (M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j]) % 10007;
			// Blocul din dreapta sus
			C[i][j + dim] = (M3[i][j] + M5[i][j]) % 10007;
			// Blocul din stânga jos
			C[i + dim][j] = (M2[i][j] + M4[i][j]) % 10007;
			// Blocul din dreapta jos
			C[i + dim][j + dim] = (M1[i][j] - M2[i][j] + M3[i][j] +
			M6[i][j]) % 10007;
			if (C[i][j] < 0)
				C[i][j] = C[i][j] + 10007;
			if (C[i][j + dim] < 0)
				C[i][j + dim] = C[i][j + dim] + 10007;
			if (C[i + dim][j] < 0)
				C[i + dim][j] = C[i + dim][j] + 10007;
			if (C[i + dim][j + dim] < 0)
				C[i + dim][j + dim] = C[i + dim][j + dim] + 10007;
		}
}

// Algoritmul lui Strassen
void S_multi(int **A, int **B, int **C, int dim)
{
	/*
		Dacă dimensiunea este 1, atunci matricele au doar un element pe
		poziția [0][0]
	*/
	if (dim == 1) {
		C[0][0] = (A[0][0] * B[0][0]) % 10007;
		if (C[0][0] < 0)
			C[0][0] += 10007;
		return;
	}
	// Aloc memorie pentru matricele blocuri
	int new_dim = dim / 2;
	int **A_11 = arr_alloc(new_dim);
	int **A_12 = arr_alloc(new_dim);
	int **A_21 = arr_alloc(new_dim);
	int **A_22 = arr_alloc(new_dim);
	int **B_11 = arr_alloc(new_dim);
	int **B_12 = arr_alloc(new_dim);
	int **B_21 = arr_alloc(new_dim);
	int **B_22 = arr_alloc(new_dim);
	// T1 și T2 sunt matrici care ajută la realizarea operațiilor
	int **T1 = arr_alloc(new_dim);
	int **T2 = arr_alloc(new_dim);

	arr_split(A_11, A_12, A_21, A_22, B_11, B_12, B_21, B_22, A, B,
			  new_dim);

	int **M1 = arr_alloc(new_dim);
	int **M2 = arr_alloc(new_dim);
	int **M3 = arr_alloc(new_dim);
	int **M4 = arr_alloc(new_dim);
	int **M5 = arr_alloc(new_dim);
	int **M6 = arr_alloc(new_dim);
	int **M7 = arr_alloc(new_dim);

	// M1 = (A_11 + A_22) * (B_11 + B_22)
	arr_add(A_11, A_22, T1, new_dim);
	arr_add(B_11, B_22, T2, new_dim);
	S_multi(T1, T2, M1, new_dim);
	// M2 = (A_21 + A_22) * B_11
	arr_add(A_21, A_22, T1, new_dim);
	S_multi(T1, B_11, M2, new_dim);
	// M3 = A_11 * (B_12 - B_22);
	arr_sub(B_12, B_22, T1, new_dim);
	S_multi(A_11, T1, M3, new_dim);
	// M4 = A_22 * (B_21 - B_11)
	arr_sub(B_21, B_11, T1, new_dim);
	S_multi(A_22, T1, M4, new_dim);
	// M5 = (A_11 + A_12) * B_22
	arr_add(A_11, A_12, T1, new_dim);
	S_multi(T1, B_22, M5, new_dim);
	// M6 = (A_21 - A_11) * (B_11 + B_12)
	arr_sub(A_21, A_11, T1, new_dim);
	arr_add(B_11, B_12, T2, new_dim);
	S_multi(T1, T2, M6, new_dim);
	// M7 = (A_12 - A_22) * (B_21 + B_22)
	arr_sub(A_12, A_22, T1, new_dim);
	arr_add(B_21, B_22, T2, new_dim);
	S_multi(T1, T2, M7, new_dim);

	arr_combin(M1, M2, M3, M4, M5, M6, M7, C, new_dim);
	// Eliberez memoria
	arr_dealloc(A_11, new_dim);
	arr_dealloc(A_12, new_dim);
	arr_dealloc(A_21, new_dim);
	arr_dealloc(A_22, new_dim);
	arr_dealloc(B_11, new_dim);
	arr_dealloc(B_12, new_dim);
	arr_dealloc(B_21, new_dim);
	arr_dealloc(B_22, new_dim);
	arr_dealloc(T1, new_dim);
	arr_dealloc(T2, new_dim);
	arr_dealloc(M1, new_dim);
	arr_dealloc(M2, new_dim);
	arr_dealloc(M3, new_dim);
	arr_dealloc(M4, new_dim);
	arr_dealloc(M5, new_dim);
	arr_dealloc(M6, new_dim);
	arr_dealloc(M7, new_dim);
}

// Aici apelez funcția S_multi
void S(int ***arr, int index1, int index2, int arrsize, int **dimensions)
{
	// Creez niște copii ale matricelor din vectorul de matrici
	int **A = arr_alloc(dimensions[index1][0]);
	int **B = arr_alloc(dimensions[index1][0]);
	int **C = arr_alloc(dimensions[index1][0]);
	if (!A || !B || !C) {
		printf("Eroare la alocare\n");
		return;
	}
	int i, j;
	for (i = 0; i < dimensions[index1][0]; i++)
		for (j = 0; j < dimensions[index1][0]; j++) {
			A[i][j] = arr[index1][i][j];
			B[i][j] = arr[index2][i][j];
		}

	S_multi(A, B, C, dimensions[index1][0]);

	// Adaug la sfârșitul vectorului matricea C
	arr[arrsize] = arr_alloc(dimensions[index1][0]);
	if (!arr[arrsize]) {
		printf("Eroare la alocare\n");
		return;
	}
	for (i = 0; i < dimensions[index1][0]; i++)
		for (j = 0; j < dimensions[index1][0]; j++)
			arr[arrsize][i][j] = C[i][j];
	// Adaug dimensiunile noii matrice
	dimensions[arrsize] = (int *)malloc(2 * sizeof(int));
	if (!dimensions[arrsize]) {
		printf("Eroare la alocare\n");
		return;
	}
	dimensions[arrsize][0] = dimensions[index1][0];
	dimensions[arrsize][1] = dimensions[index1][0];

	for (i = 0; i < dimensions[index1][0]; i++) {
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	free(A);
	free(B);
	free(C);
}

int main(void)
{
	int maxsize = 1, arrsize = 0;
	// Inițializez vectorul de matrici
	int ***arr = (int ***)malloc(sizeof(int **) * maxsize);
	// Inițializez matricea de dimensiuni
	int **dimensions = (int **)malloc(sizeof(int *));
	char input;

	while (scanf("%c", &input)) {
		if (input == 'Q') {
			mem_free(arr, dimensions, arrsize);
			break;
		} else if (input == 'L') {
			if (maxsize == arrsize) {
				maxsize = maxsize * 2;
				arr = realloc(arr, maxsize * sizeof(int **));
				dimensions = realloc(dimensions, maxsize * sizeof(int *));
			}
			L(dimensions, arr, arrsize);
			arrsize++;
		} else if (input == 'D') {
			D(dimensions, arrsize);
		} else if (input == 'P') {
			P(arr, dimensions, arrsize);
		} else if (input == 'C') {
			C(arr, dimensions, arrsize);
		} else if (input == 'M') {
			int index1, index2;
			scanf("%d%d", &index1, &index2);
			if (M_check(index1, index2, arrsize, dimensions) == 1) {
				if (maxsize == arrsize) {
					maxsize = maxsize * 2;
					arr = realloc(arr, maxsize * sizeof(int **));
					dimensions = realloc(dimensions, maxsize * sizeof(int *));
				}
				M(arr, dimensions, arrsize, index1, index2);
				arrsize++;
			}
		} else if (input == 'O') {
			O(arr, dimensions, arrsize);
		} else if (input == 'T') {
			T(arr, dimensions, arrsize);
		} else if (input == 'R') {
			R(arr, dimensions, arrsize);
		} else if (input == 'F') {
			int index;
			scanf("%d", &index);
			if (index >= arrsize || index < 0) {
				printf("No matrix with the given index\n");
			} else {
				F(arr, arrsize, dimensions, index);
				arrsize--;
			}
			if (arrsize < (maxsize / 2)) {
				maxsize = maxsize / 2;
				arr = realloc(arr, maxsize * sizeof(int **));
				dimensions = realloc(dimensions, maxsize * sizeof(int *));
			}
		} else if (input == 'S') {
			int index1, index2;
			scanf("%d%d", &index1, &index2);
			if (S_check(index1, index2, arrsize, dimensions) == 1) {
				if (maxsize == arrsize) {
					maxsize = maxsize * 2;
					arr = realloc(arr, maxsize * sizeof(int **));
					dimensions = realloc(dimensions, maxsize * sizeof(int *));
				}
				S(arr, index1, index2, arrsize, dimensions);
				arrsize++;
			}
		} else if ('A' <= input && input <= 'Z') {
			printf("Unrecognized command\n");
		}
	}
	return 0;
}
