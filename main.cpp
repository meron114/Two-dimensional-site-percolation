#include<iostream>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<Eigen/Eigenvalues>
#include<cstdio>
#include<Windows.h>

using namespace std;
using namespace Eigen;

double Uniform() {
	int r = rand();
	return (r) / ((double)RAND_MAX + 1.0); //　一様乱数生成(0 ≦ x ＜ 1)
}

void setCursorPos(int x, int y)
{
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hCons, pos);
}

int main() {
	srand(time(NULL));
	int count = 1, x = 0, y = 0, k;
	bool flag = false;

	const string ResultOfConnectionPath = "D:\\ResultOfConnection.txt";
	const string EachClusterPath = "D:\\EachCluster.txt";
	const string NumberOfTrialsPath = "D:\\NumberOfTrials.txt";

	ofstream outputfileA(ResultOfConnectionPath);
	ofstream outputfileC(EachClusterPath);
	ofstream outcount(NumberOfTrialsPath);

	cout << "正方行列大きさ : ";
	cin >> k;
	cout << endl;

	double m = 1.0, pre;
	MatrixXd A = MatrixXd::Zero(k, k), B(k, k), C = MatrixXd::Zero(k, k);

	for (;;) {
		for (;;) {
			B = A;
			x = Uniform() * k;
			y = Uniform() * k;
			A(x, y) = 1.0;
			if (A != B)
				break;
		}

		for (int j = 0; j < k; j++) {
			for (int i = 0; i < k; i++) {  //1列目
				if (j == 0) {
					if (A(0, 0) == 1.0) {
						C(0, 0) = m;
						m = m + 1.0;
					}
					if (i > 0) {
						if (A(i, 0) == 1.0 && A(i - 1, 0) == 1.0) {
							C(i, 0) = C(i - 1, 0);
						}
						else if (A(i, 0) == 1.0 && A(i - 1) == 0.0) {
							C(i, 0) = m;
							m = m + 1.0;
						}
					}
				}
				if (j > 0) {               //2列目以降　　　　　　　　　
					if (A(0, j) == 1.0) {
						if (A(0, j - 1) == 1.0) {
							C(0, j) = C(0, j - 1);
						}
						else {
							C(0, j) = m;
							m = m + 1.0;
						}
					}
					if (i > 0 && A(i, j) != 0) {
						if (A(i, j - 1) == 0.0 && A(i - 1, j) == 0.0) {
							C(i, j) = m;
							m = m + 1.0;
						}
						else if (A(i, j - 1) == 1.0 && A(i - 1, j) == 0.0) {
							C(i, j) = C(i, j - 1);
						}
						else if (A(i, j - 1) == 0.0 && A(i - 1, j) == 1.0) {
							C(i, j) = C(i - 1, j);
						}
						else if (A(i, j - 1) == 1.0 && A(i - 1, j) == 1.0) {
							if (C(i, j - 1) == C(i - 1, j)) {
								C(i, j) = C(i, j - 1);
							}
							else{
								if (C(i, j - 1) > C(i - 1, j)) {
									C(i, j) = C(i - 1, j);
									pre = C(i, j - 1);
								}
								else if (C(i, j - 1) < C(i - 1, j)) {
									C(i, j) = C(i, j - 1);
									pre = C(i - 1, j);
								}
								for (int q = j; q >= 0; q--) {
									for (int p = 0; p < k; p++) {
										if (C(p, q) == pre)
											C(p, q) = C(i, j);
									}
								}
							}
						}
					}
				}
			}
		}

		cout << A;
		setCursorPos(0, 2);
		
		for (int j = 0; j < k; j++) {
			for (int i = 0; i < k; i++) {
				if ((C(0, j) == C(k - 1, i) && C(0, j) != 0) || (C(j, 0) == C(i, k - 1) && C(j, 0) != 0)) {
					flag = true;
				}
				if (flag) {
					A(x, y) = -A(x, y);
					C(x, y) = -C(x, y);
					break;
				}
			}
			if (flag) {
				break;
			}
		}

		if (flag) {
			break;
		}

		C = MatrixXd::Zero(k, k);  //初期化
		m = 1.0;                   //初期化
		count = count++;
	}

	outputfileA << "結果(最後の値はマイナス)：" << endl << A << endl;
	outputfileC << "行列内の各クラスター(最後の値はマイナス)：" << endl << C << endl;
	outcount << "カウント回数：" << endl << count << endl;
	cout << A << endl << endl;
	cout << C << endl << endl;


	cout << "試行回数：" << count << endl << endl;
	cout << "出力結果1：" << ResultOfConnectionPath << endl;
	cout << "出力結果2：" << EachClusterPath << endl;
	cout << "出力結果3：" << NumberOfTrialsPath << endl;

	outputfileA.close();
	outputfileC.close();
	outcount.close();
	
	rewind(stdin);
	getchar();
}