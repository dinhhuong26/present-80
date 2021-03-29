#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
using namespace std;

int SBOX[] = { 12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2 };
int SBOX_inv[] = { 5,14,15,8,12,1,2,13,11,4,6,3,0,7,9,10 };

// sBoxLayer -----------------------------------------------------------------
void sBoxLayer(int data[], int n) {

	for (int i = 0; i < (n / 4); i++) {

		int sum = data[4 * i] * 8 + data[4 * i + 1] * 4 + data[4 * i + 2] * 2 + data[4 * i + 3];
		sum = SBOX[sum];
		data[4 * i] = (sum / 8) % 2;
		data[4 * i + 1] = (sum / 4) % 2;
		data[4 * i + 2] = (sum / 2) % 2;
		data[4 * i + 3] = (sum / 1) % 2;
	}
}


// pLayer -----------------------------------------------------------------
void pLayer(int data[]) {
	int a, i, temp[64];
	for (i = 0; i < 64; i++) {
		a = 16 * i;
		while (a > 63) {
			a -= 63;
		}
		temp[a] = data[i];
	}
	for (i = 0; i < 64; i++) {
		data[i] = temp[i];
	}
}


void generateRoundKeys(int RoundKeys[][80], int key[]) {
	for (int i = 0; i < 80; i++) {
		RoundKeys[0][i] = key[i];
	}
	for (int i = 1; i <= 31; i++) {
		int x;
		int temp[80];
		for (int j = 0; j < 80; j++) {
			if (j > 60) {
				x = j - 61;
			}
			else {
				x = 19 + j;
			}
			temp[j] = RoundKeys[i - 1][x];
		}
		for (int j = 0; j < 80; j++) {
			RoundKeys[i][j] = temp[j];
		}
		//4 bit cuoi qua S box
		for (int j = 0; j < 4; j++) {
			temp[j] = RoundKeys[i][79 - j];
		}
		sBoxLayer(temp, 0);
		for (int j = 0; j < 4; j++) {
			RoundKeys[i][79 - j] = temp[j];
		}
		//phep xox 5 bit
		for (int j = 0; j < 5; j++) {
			temp[j] = RoundKeys[i][19 - j];
		}
		int iround[5];
		iround[0] = (i / 16) % 2;
		iround[1] = (i / 8) % 2;
		iround[2] = (i / 4) % 2;
		iround[3] = (i / 2) % 2;
		iround[4] = i % 2;
		for (int j = 0; j < 5; j++) {
			RoundKeys[i][19 - j] = iround[j] ^ temp[j];
		}
	}
}

// Add round key 
void addRoundKey(int data[], int key[]) {
	for (int i = 0; i < 64; i++) {
		data[i] = data[i] ^ key[i];
	}
};

// Input data
void inputdata(int data[]) {
	char a[9];
	cout << "nhap du lieu: ";
	cin.getline(a, 9);
	for (int i = 0; i < 8; i++) {
		data[8 * i] = ((int)a[i] / 128) % 2;
		data[8 * i + 1] = ((int)a[i] / 64) % 2;
		data[8 * i + 2] = ((int)a[i] / 32) % 2;
		data[8 * i + 3] = ((int)a[i] / 16) % 2;
		data[8 * i + 4] = ((int)a[i] / 8) % 2;
		data[8 * i + 5] = ((int)a[i] / 4) % 2;
		data[8 * i + 6] = ((int)a[i] / 2) % 2;
		data[8 * i + 7] = ((int)a[i]) % 2;
	}
}

void inputkey(int key[]) {
	char a[11];
	cout << "nhap key (10 ky tu): ";
	cin.getline(a, 11);
	for (int i = 0; i < 10; i++) {
		key[8 * i] = ((int)a[i] / 128) % 2;
		key[8 * i + 1] = ((int)a[i] / 64) % 2;
		key[8 * i + 2] = ((int)a[i] / 32) % 2;
		key[8 * i + 3] = ((int)a[i] / 16) % 2;
		key[8 * i + 4] = ((int)a[i] / 8) % 2;
		key[8 * i + 5] = ((int)a[i] / 4) % 2;
		key[8 * i + 6] = ((int)a[i] / 2) % 2;
		key[8 * i + 7] = ((int)a[i]) % 2;
	}
}
// Output data 
void outputdata(int data[], char datachar[]) {
	for (int i = 0; i < 8; i++) {
		datachar[i] = data[i * 8] * 128 + data[1 + i * 8] * 64 + data[i * 8 + 2] * 32 + data[i * 8 + 3] * 16 + data[i * 8 + 4] * 8 + data[i * 8 + 5] * 4 + data[i * 8 + 6] * 2 + data[i * 8 + 7];
	}
}
//
void sBoxLayer_inv(int data[], int n) {
	for (int i = 0; i < (n / 4); i++) {
		int sum = data[4 * i] * 8 + data[4 * i + 1] * 4 + data[4 * i + 2] * 2 + data[4 * i + 3];
		sum = SBOX_inv[sum];
		data[4 * i] = (sum / 8) % 2;
		data[4 * i + 1] = (sum / 4) % 2;
		data[4 * i + 2] = (sum / 2) % 2;
		data[4 * i + 3] = (sum / 1) % 2;
	}
}
void pLayer_inv(int data[]) {
	int a, i, temp[64];
	for (i = 0; i < 64; i++) {
		a = 4 * i;
		while (a > 63) {
			a -= 63;
		}
		temp[a] = data[i];
	}
	for (i = 0; i < 64; i++) {
		data[i] = temp[i];
	}
}
void inputdatas(char datachar[], int data[]) {
	for (int i = 0; i < 8; i++) {
		data[8 * i] = ((int)datachar[i] / 128) % 2;
		data[8 * i + 1] = ((int)datachar[i] / 64) % 2;
		data[8 * i + 2] = ((int)datachar[i] / 32) % 2;
		data[8 * i + 3] = ((int)datachar[i] / 16) % 2;
		data[8 * i + 4] = ((int)datachar[i] / 8) % 2;
		data[8 * i + 5] = ((int)datachar[i] / 4) % 2;
		data[8 * i + 6] = ((int)datachar[i] / 2) % 2;
		data[8 * i + 7] = ((int)datachar[i]) % 2;
	}
}

// nhap key64
void inputkey64(int key64[]) {
	char a[9];
	cout << "nhap key64 (8 ky tu): ";
	cin.getline(a, 9);
	for (int i = 0; i < 8; i++) {
		key64[8 * i] = ((int)a[i] / 128) % 2;
		key64[8 * i + 1] = ((int)a[i] / 64) % 2;
		key64[8 * i + 2] = ((int)a[i] / 32) % 2;
		key64[8 * i + 3] = ((int)a[i] / 16) % 2;
		key64[8 * i + 4] = ((int)a[i] / 8) % 2;
		key64[8 * i + 5] = ((int)a[i] / 4) % 2;
		key64[8 * i + 6] = ((int)a[i] / 2) % 2;
		key64[8 * i + 7] = ((int)a[i]) % 2;
	}
}

// Main code 
int main() {
	//khai bao
	int data[64];
	int key[80];
	char datachar[8];
	int RoundKeys[32][80];
	int dem = 0, n, k = 0, h = 0;
	int key0[64];
	char ch;
	//nhan du lieu tu file
	ifstream input ("datain.txt");
	while (!input.eof()) {
		input.get(ch);
		dem++;
	}
	input.close();
	ifstream input1("datain.txt");
	//input.seekg(0 ,0);
	char* p = new char[dem];
	for (int i = 0; i <dem; i++) {
		input1.get(ch);
		p[i] = ch;
	}
	input1.close();
	for (int i = 0; i < dem; i++) {
		cout << p[i];
	}
	if ((dem % 8) == 0) {
		n = dem;
	}
	else {
		n = 8 * ((dem / 8) + 1);
	}
	char* t = new char[n];
	for (int i = 0; i < n; i++) {
		t[i] = '0';
	}
	for (int i = 0; i < dem; i++) {
		t[i] = p[i];
	}
	delete[] p;
	char** datachar1;
	datachar1 = new char* [n / 8];
	for (int i = 0; i < n / 8; i++) {
		datachar1[i] = new char[8];
	}
	for (int i = 0; i < n / 8; i++) {
		for (int j = 0; j < 8; j++) {
			datachar1[i][j] = t[k];
			k += 1;
		}
	}
	cout << endl;
	int** datas;
	datas = new int* [n / 8];
	for (int i = 0; i < n / 8; i++) {
		datas[i] = new int[64];
	}

	//nhap key
	inputkey(key);
	inputkey64(key0);

	//ma hoa presen
	generateRoundKeys(RoundKeys, key);
	for (int i = 0; i < n / 8; i++) {
		inputdatas(datachar1[i], datas[i]);
		for (int j = 0; j < 31; j++) {
			addRoundKey(datas[i], RoundKeys[j]);
			sBoxLayer(datas[i], 64);
			pLayer(datas[i]);
		}
		addRoundKey(datas[i], RoundKeys[31]);
		outputdata(datas[i], datachar1[i]);
	}

	//ma hoa moc xich
	for (int i = 0; i < 64; i++) {
		datas[0][i] = key0[i] ^ datas[0][i];
	}
	for (int i = 1; i < (n / 8); i++) {
		for (int j = 0; j < 64; j++) {
			datas[i][j] = datas[i - 1][j] ^ datas[i][j];
		}
	}

	//giai ma moc xich
	for (int i = (n / 8) - 1; i > 0; i--) {
		for (int j = 0; j < 64; j++) {
			datas[i][j] = datas[i][j] ^ datas[i - 1][j];
		}
	}
	for (int i = 0; i < 64; i++) {
		datas[0][i] = datas[0][i] ^ key0[i];
	}
	for (int i = 0; i < n / 8; i++) {
		outputdata(datas[i], datachar1[i]);
	}

	cout << "ban ma hoa: " << endl;
	for (int i = 0; i < n / 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << datachar1[i][j];
		}
	}
	cout << endl << endl;
	cout << "----------------------------------------" << endl << endl;

	//giai ma
	for (int i = 0; i < n / 8; i++) {
		addRoundKey(datas[i], RoundKeys[31]);
		for (int j = 30; j >= 0; j--) {
			pLayer_inv(datas[i]);
			sBoxLayer_inv(datas[i], 64);
			addRoundKey(datas[i], RoundKeys[j]);
		}
		outputdata(datas[i], datachar1[i]);
	}
	for (int i = 0; i < n / 8; i++) {
		delete[] datas[i];
	}
	delete[] datas;
	cout << "ban giai ma: " << endl;
	char* banro = new char[n];
	for (int i = 0; i < (n / 8); i++) {
		for (int j = 0; j < 8; j++) {
			banro[h] = datachar1[i][j];
			h++;
		}
	}
	for (int i = 0; i < dem; i++) {
		cout << banro[i];
	}
	 fstream dataout("dataout.txt");
	for (int i = 0; i < dem; i++) {
		dataout << banro[i];
	}

	return 0;
}