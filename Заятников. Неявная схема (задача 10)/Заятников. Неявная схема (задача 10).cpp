#include <iostream> 
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>
#include <fstream>
#define _WIN32_DCOM
#define pi 3.141592653589793238463

using namespace std;

double fi(double x) {
	return 1 - x * x;
}

double g(double x, double t) {
	return exp(-1 * t) * sin(7 * pi * x) + 1;
}

double m1(double t) {
	return cos(t);
}

double m2(double t) {
	return sin(4 * t);
}

vector <double> progonka(double C, double A, double B, vector <double> F, double h1, double h2, double m1, double m2) {
	int n = F.size() + 1;
	vector <double> alpha(n);
	vector <double> beta(n);
	alpha[0] = h1;
	for (int i = 0; i < n - 1; i++)
		alpha[i + 1] = B / (C - A * alpha[i]);
	beta[0] = m1;
	for (int i = 0; i < n - 1; i++)
		beta[i + 1] = (F[i] + A * beta[i]) / (C - A * alpha[i]);
	vector <double> y(n + 1);
	y[n] = (-1 * h2 * beta[n - 1] - m2) / (h2 * alpha[n - 1] - 1);
	for (int i = n - 1; i >= 0; i--)
		y[i] = alpha[i] * y[i + 1] + beta[i];
	return y;
}

int main() {
	setlocale(LC_ALL, "Russian");
	cout << "                               Задача №10 – Схемы для нестационарного уравнения теплопроводности (постоянные коэффициенты).\n";
	cout << "\n";
	cout << "Вариант 2. Команда №2. Выполнил: Заятников Антон. Командир команды: Воденеев Михаил.\n";
	cout << "Условия варианта: a^2=4; g(x, t)=e^(-t)sin(7pi*x)+1; фи(x)=1-x^2; мю1(t)=cost; мю2(t)=sin(4t).\n";
	cout << "Задача решается с помощью неявной схемы.\n";
	double sqra = 4, h1 = 0, h2 = 0;
	double n, m, T;
	cout << "введите граничное значение времени T=";
	cin >> T;
	cout << "введите количество шагов по координате длине отрезка n=";
	cin >> n;
	cout << "введите количество шагов по временному отрезку m=";
	cin >> m;
	double h = 1 / n;
	double tau = T / m;
	vector <double> X(n + 1);
	vector <double> ts(m + 1);
	for (int i = 0; i < n + 1; i++)
		X[i] = i * h;
	for (int j = 0; j < m + 1; j++)
		ts[j] = j * tau;
	double A = (sqra * tau) / (h * h);
	double B = A;
	double C = 1 + (2 * sqra * tau) / (h * h);
	vector <vector <double>> V(n + 1);
	vector <double> help(m + 1);
	for (int i = 0; i < n + 1; i++)
		V[i] = help;
	for (int i = 0; i < n + 1; i++)
		V[i][0] = fi(X[i]);
	for (int j = 1; j < m + 1; j++)
		V[0][j] = m1(ts[j]);
	for (int j = 1; j < m + 1; j++)
		V[n][j] = m2(ts[j]);
	for (int j = 1; j < m + 1; j++) {
		vector <double> F1(n - 1);
		for (int i = 0; i < n - 1; i++)
			F1[i] = V[i + 1][j - 1] + g(X[i + 1], ts[j]) * tau;
		vector <double> vcur = progonka(C, A, B, F1, h1, h2, m1(ts[j]), m2(ts[j]));
		for (int i = 0; i < n + 1; i++)
			V[i][j] = vcur[i];
	}
	if (m > 15) {
		ofstream fout;
		fout.open("file.txt");
		fout << "                ";
		for (int j = 0; j < m + 1; j++)
			fout << "t" << j << "=" << fixed << setprecision(2) << ts[j] << "    ";
		fout << "\n";
		for (int i = 0; i < n + 1; i++) {
			fout << "X" << i << "=" << fixed << setprecision(5) << X[i] << "  ";
			if (i < 10)
				fout << "   ";
			if ((i > 9) && (i < 100))
				fout << "  ";
			if ((i > 99) && (i < 1000))
				fout << " ";
			for (int j = 0; j < m + 1; j++) {
				if (V[i][j] >= 0)
					fout << " ";
				fout << fixed << V[i][j] << "   ";
				if (j >= 10)
					fout << "  ";
			}
			fout << "\n";
		}
	}
	else {
		cout << "                ";
		for (int j = 0; j < m + 1; j++)
			cout << "t" << j << "=" << fixed << setprecision(2) << ts[j] << "    ";
		cout << "\n";
		for (int i = 0; i < n + 1; i++) {
			cout << "X" << i << "=" << fixed << setprecision(5) << X[i] << "  ";
			if (i < 10)
				cout << "   ";
			if ((i > 9) && (i < 100))
				cout << "  ";
			if ((i > 99) && (i < 1000))
				cout << " ";
			for (int j = 0; j < m + 1; j++) {
				if (V[i][j] >= 0)
					cout << " ";
				cout << fixed << V[i][j] << "   ";
				if (j >= 10)
					cout << "  ";
			}
			cout << "\n";
		}
	}
	system("pause");
}