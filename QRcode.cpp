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

// possivelmente nao funciona porque nada garante que os pixeis pretos de uma linha nao fiquem todos num so quadrante
bool verify_quadrants(vector<int> lb, vector<int> cb, vector<int> qb) {
    int soma = 0;
    int size = lb.size();

    // Quadrante 1 ======================================
    for (int i = 0; i < size / 2; i++) {
        soma += lb[i] / 2;
    }

    if (soma > qb[0] || soma > qb[1])
        return false;

    soma = 0;
    for (int i = size / 2; i < size; i++) {
        soma += cb[i] / 2;
    }

    if (soma > qb[0] || soma > qb[3])
        return false;

    // Quadrante 2 ======================================
    // ja foi feito o caso das linhas

    soma = 0;
    for (int i = 0; i < size / 2; i++) {
        soma += cb[i] / 2;
    }

    if (soma > qb[1] || soma > qb[2])
        return false;

    // Quadrante 3 ======================================
    soma = 0;
    for (int i = size / 2; i < size; i++) {
        soma += lb[i] / 2;
    }

    if (soma > qb[2] || soma > qb[3])
        return false;

    // ja foi feito o caso das colunas

    // Quadrante 4 ======================================
    // ja foram feitos ambos os casos

    return true;
}

int encode(vector<vector<int>> &QRcode, vector<int> lb, vector<int> cb, vector<int> lt, vector<int> ct, vector<int> qb, vector<int> db) {
    int k = 0; // num de QRcodes gerados a partir do encode

    // DEFECT =============================
    if (!verify_quadrants(lb, cb, qb))
        return k;

    // VALID / INVALID
    k = 1;

    return k;
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
        int k; // usar para o num de QRcodes gerados a partir do input

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

        k = encode(QRcode, lb, cb, lt, ct, qb, db);

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
