#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void writeVector(vector<int> &vec, int n) {
    int num;
    while (n--) {
        cin >> num;
        vec.push_back(num);
    }
}

int encode() {

    return 0;
}

void printQRcode(vector<vector<int>> QRcode, int N) {

    for (int i = 0; i < N; i++) {

        // Contruir borda de cima
        if (i == 0) {
            cout << "+";
            for (int k = 0; k < N; k++) {
                cout << "-";
            }
            cout << "+" << endl;
        }

        // Imprimir o QRcode
        for (int j = 0; j < N; j++) {
            // Borda da esquerda
            if (j == 0) {
                cout << "|";
            }

            // Imprimir QRcode concretamente
            if (QRcode[i][j] == 0)
                cout << " ";
            else
                cout << "#";
        }
        // Borda da direita
        cout << "|" << endl;
    }

    // Contruir borda de baixo
    cout << "+";
    for (int k = 0; k < N; k++) {
        cout << "-";
    }
    cout << "+" << endl;

}

int main() {
    int num_codes;
    cin >> num_codes;

    while (num_codes--) {
        int k = 1; // usar para o num de QRcodes gerados a partir do input

        // Ler input ============================================
        int N;
        cin >> N;

        vector<int> lb, cb, lt, ct, qb, db;

        writeVector(lb, N); // 1 < x < N
        writeVector(cb, N); // 1 < x < N
        writeVector(lt, N); // 1 < x < N-1
        writeVector(ct, N); // 1 < x < N-1
        writeVector(qb, 4); // 1 < x < (N/2+1)^2
        writeVector(db, 2); // 1 < x < N

        // Encoder ==============================================

        // Cria um vetor N*N
        vector<vector<int>> QRcode(N);
        for (int i = 0; i < N; i++)
            QRcode[i].resize(N);

        

        // Output ===============================================
        switch (k) {
        case 0:
            cout << "DEFECT: No QR Code generated!" << endl;
            break;
        case 1:
            cout << "VALID: 1 QR Code generated!" << endl;
            printQRcode(QRcode, N);
            break;
        default:
            cout << "INVALID: " << k << " QR Code generated!" << endl;
            break;
        }
    }

    return 0;
}
