#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<vector<int>> QRcode;
vector<int> lb, cb, lt, ct, qb, db;
vector<vector<bool>> visited;
int k; // usar para o num de QRcodes gerados a partir do input
int N; // num linhas/ colunas do QRcode

void writeVector(vector<int> &vec, int n) {
    int num;
    while (n--) {
        cin >> num;
        vec.push_back(num);
    }
}

// possivelmente nao funciona porque nada garante que os pixeis pretos de uma linha nao fiquem todos num so quadrante
bool verify_quadrants() {
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

bool verify_args() {

    for (int i = 0; i < N; i++) {
        if (lb[i] < 0 || cb[i] < 0 || lt[i] < 0 || ct[i] < 0)
            return false;
    }

    for (int i = 0; i < 4; i++) {
        if (qb[i] < 0)
            return false;
    }

    for (int i = 0; i < 2; i++) {
        if (db[i] < 0)
            return false;
    }

    return true;
}

void update_pixel(int l, int c) {
    QRcode[l][c] = 1;

    // Atualizar linhas/ colunas
    lb[l]--;
    cb[c]--;

    // TODO: Atualizar transicoes

    // Atualizar quadrantes
    if (l < N / 2) {
        // 1 ou 2 Quadrante
        if (c < N / 2) {
            // 2Q
            qb[1]--;
        } else {
            // 1Q
            qb[0]--;
        }
    } else {
        // 3 ou 4 Quadrante
        if (c < N / 2) {
            // 3Q
            qb[2]--;
        } else {
            // 4Q
            qb[3]--;
        }
    }

    // Atualizar diagonais
    if (c == l)
        db[0]--;
    else if (N - l + 1 == c)
        db[1]--;
}

void revert_pixel(int l, int c) {
    QRcode[l][c] = 0;

    // Atualizar linhas/ colunas
    lb[l]++;
    cb[c]++;

    // TODO: Atualizar transicoes

    // Atualizar quadrantes
    if (l < N / 2) {
        // 1 ou 2 Quadrante
        if (c < N / 2) {
            // 2Q
            qb[1]++;
        } else {
            // 1Q
            qb[0]++;
        }
    } else {
        // 3 ou 4 Quadrante
        if (c < N / 2) {
            // 3Q
            qb[2]++;
        } else {
            // 4Q
            qb[3]++;
        }
    }

    // Atualizar diagonais
    if (c == l)
        db[0]++;
    else if (N - l + 1 == c)
        db[1]++;
}

bool encode(int lin, int c) {

    // DEFECT =============================
    // if (!verify_quadrants(lb, cb, qb))
    //     return k;

    // VALID / INVALID ====================

    // Rejection test
    if (!verify_args()) {
        return false;
    }

    // Base case
    if (c == N - 1 && lin == N - 1) {
        k++;
        return true;
    }

    // For all unvisited pixels
    // TODO: verificar o N-1, pus por causa do i+1 & j+1 mas pode nao estar a verificar o ultimo
    for (int i = 0; i < N - 1; i++) {
        if (!visited[lin][i]) {
            visited[lin][i] = true;
            update_pixel(lin, i);
            if (encode(lin, i + 1))
                return true;
            revert_pixel(lin, i);
            visited[lin][i] = false;
        }
    }
    
    return false;
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

        // Ler input ============================================
        cin >> N;

        writeVector(lb, N); // 1 < x < N
        writeVector(cb, N); // 1 < x < N
        writeVector(lt, N); // 1 < x < N-1
        writeVector(ct, N); // 1 < x < N-1
        writeVector(qb, 4); // 1 < x < (N/2+1)^2
        writeVector(db, 2); // 1 < x < N

        // Encoder ==============================================

        // Cria um vetor N*N
        QRcode = vector<vector<int>>(N, (vector<int>(N, 0)));
        visited = vector<vector<bool>>(N, (vector<bool>(N, false)));

        k = 0;
        encode(0, 0);

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
