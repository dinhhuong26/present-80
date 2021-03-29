#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int IPAD[20] = {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1};
int OPAD[20] = { 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0 };
int Sbox[] = { 12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2 };
int RC[] = { 1, 3, 7, 14, 13, 11, 6, 12, 9, 2, 5, 10 };
int IC[] = { 0, 1, 3, 6, 4 };

int Mix[][5] = {
	{ 1,  2,  9,  9,  2},
	{ 2,  5,  3,  8, 13},
	{13, 11, 10, 12,  1},
	{ 1, 15,  2,  3, 14},
	{14, 14,  8,  5, 12}
};
int Mixcol[5] = { 1, 2, 9, 9, 2 };

int* inttobit(int a) {
	int* p = new int[8];
	p[0] = (a / 128) % 2;
	p[1] = (a / 64) % 2;
	p[2] = (a / 32) % 2;
	p[3] = (a / 16) % 2;
	p[4] = (a / 8) % 2;
	p[5] = (a / 4) % 2;
	p[6] = (a / 2) % 2;
	p[7] = a % 2;
	return p;
}
int bittoint(int* p) {
	int a;
	a =p[0]*128+ p[1] * 64 + p[2] * 32 + p[3] * 16 + p[4] * 8 + p[5] * 4 + p[6] * 2 + p[7];
	return a;
}
int tongdathuc(int a, int b) {
	int c;
	int* pa = new int[8];
	int* pb = new int[8];
	int* temp = new int[8];
	pa = inttobit(a);
	pb = inttobit(b);
	for (int i = 0; i < 8; i++) {
		temp[i] = (pa[i]) ^ (pb[i]);
	}
	c = bittoint(temp);
	delete[] pa, pb, temp;
	return c;
}

int Modulo(int a) {
	if (a < 16) {
		return a;
	}
	else {
		int temp;
		int* p = new int[8];
		p = inttobit(a);
		temp = (p[4] ^ p[0] ^ p[1]) * 8 + ((p[1] ^ p[5]) ^ p[2]) * 4 + ((p[2] ^ p[6]) ^ p[3] ^ p[0]) * 2 + (p[3] ^ p[0] ^ p[7]);
		delete[] p;
		return temp;
	}
}
// nhan da thuc 
int nhandathuc(int a, int b) {
	int temp;
	int* pa = new int[8];
	pa = inttobit(a);
	int* pb = new int[8];
	pb = inttobit(b);
	temp = (pa[4] * pb[4]) * 64
		+ ((pa[4] * pb[5]) ^ (pa[5] * pb[4])) * 32
		+ ((pa[4] * pb[6]) ^ (pa[5] * pb[5]) ^ (pa[6] * pb[4])) * 16
		+ ((pa[4] * pb[7]) ^ (pa[5] * pb[6]) ^ (pa[6] * pb[5]) ^ (pa[7] * pb[4])) * 8
		+ ((pa[5] * pb[7]) ^ (pa[6] * pb[6]) ^ (pa[7] * pb[5])) * 4
		+ ((pa[6] * pb[7]) ^ (pa[7] * pb[6])) * 2
		+ (pa[7] * pb[7]);
	return temp;
}

void dichhang(int s[], int k) {
	int temp = s[0];
	for (int i = 0; i < k - 1; i++) {
		s[i] = s[i + 1];
	}
	s[k - 1] = temp;
}

void addConstan(int** S, int d, int v) {
	for (int i = 0; i < d; i++) {
		S[i][0] = S[i][0] ^ RC[v] ^ IC[i];
	}
}

void subCells(int **S) {
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			S[i][j] = Sbox[S[i][j]];
}

void ShiftRows(int** S) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < i; j++) {
			dichhang(S[i], 5);
		}
	}
}

void  Mixcolum(int**S) {
	int temp1[5];
	int i, j, k;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			int sum = 0;
			for (k = 0; k < 5; k++) {
				sum ^= Modulo(nhandathuc(Mix[i][k], S[k][j]));
			}
			temp1[i] = Modulo(sum);
		}
		for (i = 0; i < 5; i++) {
			S[i][j] = temp1[i];
		}
	}
}



void printState(int** S) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << S[i][j] << " ";
		}
		cout << endl;
	}
}
void tach(char x,int &a, int& b) {
	int k = (int)x;
	a = ((k / 128) % 2) * 8 + ((k / 64) % 2) * 4 + ((k / 32) % 2) * 2 + ((k / 16) % 2);
	b = ((k / 8) % 2) * 8 + ((k / 4) % 2) * 4 + ((k / 2) % 2) * 2 + (k % 2);
}

int* KhoiP(int C[]) {
	int k = 0;
	int** S = new int* [5];
	for (int i = 0; i < 5; i++) {
		S[i] = new int[5];
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			S[i][j] = C[k];
			k++;
		}
	}
	int* D = new int[k];
	k = 0;
	for (int i = 0; i < 12; i++) {
		addConstan(S, 5, RC[i]);
		subCells(S);
		ShiftRows(S);
		Mixcolum(S);
	};
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			D[k] = S[i][j];
			k++;
		}
	}
	for (int i = 0; i < 5; i++) {
		delete[] S[i];
	}
	return D;
}

int* Hash() {
	string datachar;
	string K;
	int H[5];
	int  h, m = 1, n = 0, l = 0;
	int a, b;
	cout << "nhap K(5 ky tu ): ";
	getline(cin, K);
	for (int i = 0; i < 5; i++) {
		H[i] = (int)K[i] - '0';
	}
	getline(cin, datachar);
	int k = datachar.length();
	if ((k * 2) % 5 != 0) {
		h = ((k * 2) / 5 + 2) * 5;
	}
	else {
		h = k * 2 + 1;
	}
	int** ptr = new int* [h / 5];
	for (int i = 0; i < h / 5; i++) {
		ptr[i] = new int[25];
		for (int j = 0; j < 25; j++) {
			ptr[i][j] = 0;
		}
	}
	for (int i = 0; i < 5; i++) {
		H[i] = H[i] ^ 11;
		ptr[0][i] = H[i];
	}
	for (int i = 0; i < k; i++) {
		tach(datachar[i], a, b);
		ptr[m][n] = a;
		n++;
		if (n % 5 == 0) {
			m++;
			n = 0;
		}
		ptr[m][n] = b;
		n++;
		if (n % 5 == 0) {
			m++;
			n = 0;
		}
	}
	//tang 1
	int* mx = new int[25];
	for (int i = 0; i < h / 5; i++) {
		mx = KhoiP(ptr[i]);
		if (i + 1 == h / 5) {
			break;
		}
		for (int j = 0; j < 25; j++) {
			mx[j] = mx[j] ^ ptr[i + 1][j];
		}
	}
	int** Zx = new int* [7];
	for (int i = 0; i < 7; i++) {
		Zx[i] = new int[25];
	}
	for (int i = 0; i < 25; i++) {
		Zx[0][i] = mx[i];
	}
	delete[] mx;
	for (int i = 1; i < 6; i++) {
		Zx[i] = KhoiP(Zx[i - 1]);
	}
	for (int i = 1; i < 6; i++) {
		for (int j = 0; j < 25; j++) {
			Zx[i][j] = Zx[i - 1][j];
		}
	}
	//tang 2
	for (int i = 0; i < 5; i++) {
		H[i] = H[i] ^ 10;
		Zx[0][i] = H[i];
	}
	for (int i = 0; i < h / 5; i++) {
		mx = KhoiP(ptr[i]);
		if (i + 1 == h / 5) {
			break;
		}
		for (int j = 0; j < 25; j++) {
			mx[j] = mx[j] ^ ptr[i + 1][j];
		}
	}
	for (int i = 0; i < 6; i++) {
		Zx[i] = new int[25];
	}
	for (int i = 0; i < 25; i++) {
		Zx[0][i] = mx[i];
	}
	delete[] mx;
	for (int i = 1; i < 6; i++) {
		Zx[i] = KhoiP(Zx[i - 1]);
	}
	int* z = new int[30];
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			z[l] = Zx[i][j];
			l++;
		}
	}
	return z;
}

int  main() {
	char ch[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	int* M = new int[30];
	M = Hash();
	cout << "----------------------------------------------" << endl;
	for (int i = 0; i <30; i++) {
		cout <<ch[M[i]] ;
	}
}
