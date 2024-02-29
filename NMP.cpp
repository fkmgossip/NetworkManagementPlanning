#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int N;
vector<int> critWay, workTime, Works, Sorted;
vector<vector<int>> MainMatrix;

void matrixRead()
{
	ifstream file;

	vector<vector<int>> matrix;

	file.open("matrix.txt");
	if (file.is_open())
	{
		vector<int> row;
		string s1, s2;

		while (getline(file, s1, '\n'))
		{
			stringstream line(s1);

			while (getline(line, s2, ' '))
				row.push_back(stoi(s2));

			matrix.push_back(row);
			row.clear();
		}
	}

	N = matrix[0].size();
	Works = matrix[N];
	matrix.erase(matrix.begin() + N);
	MainMatrix = matrix;
}

//------------------------------------------------
void topSort(int m, vector<int>& visited)
{
	visited[m] = 1;
	for (int i = 0; i < N; i++) {
		if (MainMatrix[m][i] == 1 && visited[i] != 1)
			topSort(i, visited);
	}

	Sorted.push_back(m);
}

void cycleCheck()
{
	bool cycles;
	vector<vector<int>> M1 = MainMatrix, M2(N, vector<int>(N));

	for (int l = 1; l < N; l++)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				M2[i][j] = 0;
				for (int k = 0; k < N; k++)
					M2[i][j] = (int)(M2[i][j] || M1[i][k] && MainMatrix[k][i]);
			}
			if (M2[i][i] != 0) cycles = false;
		}
		M1 = M2;
	}
	cycles = true;

	if (!cycles)
	{
		cout << "Cycle found.\n";
		exit(-1);
	}
	else cout << "No cycles.\n";
}

int sourceFind()
{
	int i, j, k = 0, result;
	bool flag;
	vector<int> source(N);

	for (i = 0; i < N; i++)
	{
		flag = true;
		for (j = 0; j < N; j++)
			if (MainMatrix[j][i] == 1) flag = false;
		if (flag) { source[i] = 1; k++; }
	}
	i = 0;

	if (k == 1)
	{
		while (source[i] == 0) i++;
		result = i;
	}
	else if (k == 0) result = -1;
	else
	{
		vector<vector<int>> newMatrix(N + 1, vector<int>(N + 1));

		for (i = 0; i < N; i++)
		{
			MainMatrix[0][i + 1] = source[i];
			for (j = 0; j < N; j++)
				newMatrix[i + 1, j + 1] = MainMatrix[i, j];
		}

		MainMatrix = newMatrix;
		N++;

		result = 0;
	}


	if (result < 0)
	{
		cout << "No source.";
		exit(-2);
	}
	else cout << "Source: " << result << "\n\n";

	return result;
}

void transpos()
{
	vector<int> newWorkLength(N);
	vector<vector<int>> newMatrix(N, vector<int>(N));

	for (int i = 0; i < N; i++)
	{
		int l = 0;

		while (Sorted[l] != i)
			l++;

		newWorkLength[l] = Works[i];
		for (int j = 0; j < N; j++)
			if (MainMatrix[i][j] == 1)
			{
				int k = 0;
				while (Sorted[k] != j) k++;
				newMatrix[l][k] = 1;
			}
	}

	MainMatrix = newMatrix;
	Works = newWorkLength;
}

void critWayFind()
{
	int critWayForward, critWayBack;
	vector<int> T(N), T1(N);

	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			if (MainMatrix[i][j] == 1 && T[j] < T[i] + Works[i])
				T[j] = T[i] + Works[i];
	critWayForward = T[N - 1] + Works[N - 1];

	for (int i = N - 1; i >= 0; i--)
		for (int j = i - 1; j >= 0; j--)
			if (MainMatrix[j][i] == 1 && T1[j] < T1[i] + Works[i])
				T1[j] = T1[i] + Works[i];
	critWayBack = T1[0] + Works[0];

	cout << "Critical way (forward): " << critWayForward << "\n";
	cout << "Critical way (back): " << critWayBack << "\n";

	cout << "Vertexes: ";
	for (int i = 0; i < N; i++)
	{
		int buff = Works[i] + T[i] + T1[i];
		critWay.push_back(buff);
		if (buff == critWayForward)
			cout << i << " ";
		workTime.push_back(critWayForward - buff);
	}
	cout << "\n";

	cout << "Rest of time: ";
	for (int i = 0; i < N; i++)
		cout << workTime[i] << " ";
	cout << "\n";
}

void NPnM_Alg()
{
	matrixRead();

	cout << "Input matrix:\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << MainMatrix[i][j] << " ";
		cout << "\n";
	}

	cout << "\nLength of work:\n";
	for (int i = 0; i < N; i++)
		cout << Works[i] << " ";
	cout << "\n\n";


	cycleCheck();

	int source = sourceFind();
	vector<int> visitedVrtx(N);

	topSort(source, visitedVrtx);
	reverse(Sorted.begin(), Sorted.end());

	cout << "Old numbering:\n";
	for (int i = 0; i < N; i++)
		cout << Sorted[i] << " ";

	cout << "\nNew numbering:\n";
	for (int i = 0; i < N; i++)
		cout << i << " ";
	cout << "\n";

	transpos();

	cout << "\nNew matrix:\n";
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << MainMatrix[i][j] << " ";
		cout << "\n";
	}

	cout << "\nNew length:\n";
	for (int i = 0; i < N; i++)
		cout << Works[i] << " ";
	cout << "\n\n";

	critWayFind();
}

//------------------------------------------------
int main()
{
	NPnM_Alg();
}