#include <iostream>
#include <omp.h>
#include <time.h>
#include <math.h>
using namespace std;

// Функция параллельного умножения матриц с использованием технологии OpenMP
void umn_matr_parallel(float** A, float** B, float** C, int N) {
#pragma omp parallel for shared(A, B, C)
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0.0;
			for (int k = 0; k < N; k++) C[i][j] += A[i][k] * B[k][j];
		}
	}
}

// Функция стандартного умножения матриц
void umn_matr(float** A, float** B, float** C, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0.0;
			for (int k = 0; k < N; k++) 
				C[i][j] += A[i][k] * B[k][j];
		}
	}
}

// Функция ввода размерности квадратных матриц
int get_matrix_size() {
	int matrix_size;
	string input;
	bool exit = false;
	while (true) {
		cout << "Введите размерность квадратных матриц >> ";
		cin >> input;
		cin.clear();	
		while (cin.get() != '\n');
		for (int i = 0; i < input.size(); i++) {
			if (!isdigit(input[i])) {
				cout << "Некорректный ввод! Введено не число!" << endl;
				exit = false;
				break;
			}
			exit = true;
		}
		if (exit) {
			matrix_size = atoi(input.c_str());
			if (matrix_size <= 0) {
				cout << "Некорректный ввод! Размерность матрицы слишком маленькая!" << endl;
			}
			else {
				break;
			}
		}
	}
	return matrix_size;
}

// Функция ввода и установки количества потоков
void set_threads() {
	int num_of_threads;
	string input;
	bool exit = false;
	int max_threads = omp_get_num_procs();
	cout << "В системе доступно " << max_threads << " потоков" << endl;
	while (true) {
		cout << "Введите кол-во потоков для решения задачи >> ";
		cin >> input;
		cin.clear();
		for (int i = 0; i < input.size(); i++) {
			if (!isdigit(input[i])) {
				cout << "Некорректный ввод! Введено не число!" << endl;
				exit = false;
				break;
			}
			exit = true;
		}
		if (exit) {
			num_of_threads = atoi(input.c_str());
			if (num_of_threads <= 0) {
				cout << "Некорректный ввод! Введено слишком мало потоков!" << endl;
			}
			else if (num_of_threads > max_threads) {
				cout << "Некорректный ввод! Введено слишком потоков!" << endl;
			}
			else {
				omp_set_num_threads(num_of_threads);
				break;
			}
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	int i, j, k, N;

	N = get_matrix_size();
	cout << "" << endl;

	set_threads();
	cout << "" << endl;

	float** a, ** b, ** c;
	a = new float* [N];
	for (i = 0; i < N; i++)
		a[i] = new float[N];
	b = new float* [N];
	for (i = 0; i < N; i++)
		b[i] = new float[N];
	c = new float* [N];
	for (i = 0; i < N; i++)
		c[i] = new float[N];
	float t1, t2, tn, tk;

	// инициализация матриц
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			a[i][j] = b[i][j] = sin(i) * cos(j);

	cout << "Начато стандартное умножение матриц" << endl;
	tn = omp_get_wtime();
	umn_matr(a, b, c, N);
	tk = omp_get_wtime();
	cout << "Время стандартного умножения = " << tk - tn << endl;

	cout << "" << endl;

	cout << "Начато параллельное умножение матриц" << endl;
	t1 = omp_get_wtime();
	umn_matr_parallel(a, b, c, N);
	t2 = omp_get_wtime();
	cout << "Время параллельного умножения = " << t2 - t1 << endl;
}