#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<vector<int>> QRcode;
vector<vector<int>> QRcodecpy;

vector<int> lb, cb, lt, ct, qb, db, lt_copy, lb_copy;
vector<vector<bool>> visited;
int k; // usar para o num de QRcodes gerados a partir do input
int N; // num linhas/ colunas do QRcode

void writeVector(vector<int> &vec, int n) {
    int num;
    for (int i = 0; i < n; i++) {
        cin >> num;
        vec[i] = num;
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

bool verify_args(int lin, int c) {
    // TODO:
    // if (N - c + 1 < lb[lin] || (N - lin + 1 < cb[c])) {
    //     return false;
    // }

    for (int i = 0; i < N; i++) {
        if (lb[i] < 0 || cb[i] < 0 || (cb[i] == 0 && ct[i] != 0) || (lb[i] == 0 && lt[i] != 0)) {
            return false;
        }
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

bool verify_row(int lin) {
    if (lb[lin] != 0 || lt[lin] != 0)
        return false;
    int soma = 0;
    if (lin == N / 2 + 1) {
        // Ja se pode verificar o 1 e 2 quadrantes se faltam pixeis pretos

        // 1Q
        for (int i = 0; i < N / 2; i++) {
            for (int j = N / 2; j < N; j++) {
                soma += QRcode[i][j];
            }
        }
        if (soma < qb[0])
            return false;
        soma = 0;
        // 2Q
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N / 2; j++) {
                soma += QRcode[i][j];
            }
        }
        if (soma < qb[1])
            return false;
    }
    return true;
}

bool final_verify() {

    for (int i = 0; i < N; i++) {
        if (lb[i] != 0 || cb[i] != 0 || lt[i] != 0 || ct[i] != 0) {
            return false;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (qb[i] != 0)
            return false;
    }

    for (int i = 0; i < 2; i++) {
        if (db[i] != 0)
            return false;
    }

    k++;
    QRcodecpy = QRcode;
    return true;
}

void update_pixel(int l, int c) {
    QRcode[l][c] = 1;

    // Atualizar linhas/ colunas
    lb[l]--;
    cb[c]--;

    if (l >= 1 && l <= N - 2) {
        // se a primeira col estiver a 1 nao muda em nada
        if (QRcode[l + 1][c] == 0 && QRcode[l - 1][c] == 0) {
            ct[c] -= 2;
        } else if (QRcode[l + 1][c] == 1 && QRcode[l - 1][c] == 1) {
            ct[c] += 2;
        }
    } else if (l == 0 && QRcode[l + 1][c] == 0) { // primeiro
        ct[c]--;
    } else if (l == 0 && QRcode[l + 1][c] == 1) { // primeiro
        ct[c]++;
    } else if (l == N - 1 && QRcode[l - 1][c] == 0) { // ultima
        ct[c]--;
    } else if (l == N - 1 && QRcode[l - 1][c] == 1) { // ultima
        ct[c]++;
    }

    if (c >= 1 && c <= N - 2) {
        // se a primeira col estiver a 1 nao muda em nada
        if (QRcode[l][c + 1] == 0 && QRcode[l][c - 1] == 0) {
            lt[l] -= 2;
        } else if (QRcode[l][c + 1] == 1 && QRcode[l][c - 1] == 1) {
            lt[l] += 2;
        }
    } else if (c == 0 && QRcode[l][c + 1] == 0) { // primeiro
        lt[l]--;
    } else if (c == 0 && QRcode[l][c + 1] == 1) { // primeiro
        lt[l]++;
    } else if (c == N - 1 && QRcode[l][c - 1] == 0) { // primeiro
        lt[l]--;
    } else if (c == N - 1 && QRcode[l][c - 1] == 1) { // ultima
        lt[l]++;
    }

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
    if ((c + l) == (N - 1))
        db[1]--;
}

void revert_pixel(int l, int c) {
    QRcode[l][c] = 0;

    // Atualizar linhas/ colunas
    lb[l]++;
    cb[c]++;

    // TODO: Atualizar transicoes [verificar se para retirar tambem e assim]

    if (l >= 1 && l <= N - 2) {
        // se a primeira col estiver a 1 nao muda em nada
        if (QRcode[l + 1][c] == 0 && QRcode[l - 1][c] == 0) {
            ct[c] += 2;
        } else if (QRcode[l + 1][c] == 1 && QRcode[l - 1][c] == 1) {
            ct[c] -= 2;
        }
    } else if (l == 0 && QRcode[l + 1][c] == 0) { // primeiro
        ct[c]++;
    } else if (l == 0 && QRcode[l + 1][c] == 1) { // primeiro
        ct[c]--;
    } else if (l == N - 1 && QRcode[l - 1][c] == 0) { // ultima
        ct[c]++;
    } else if (l == N - 1 && QRcode[l - 1][c] == 1) { // ultima
        ct[c]--;
    }

    if (c >= 1 && c <= N - 2) {
        // se a primeira col estiver a 1 nao muda em nada
        if (QRcode[l][c + 1] == 0 && QRcode[l][c - 1] == 0) {
            lt[l] += 2;
        } else if (QRcode[l][c + 1] == 1 && QRcode[l][c - 1] == 1) {
            lt[l] -= 2;
        }
    } else if (c == 0 && QRcode[l][c + 1] == 0) { // primeiro
        lt[l]++;
    } else if (c == 0 && QRcode[l][c + 1] == 1) { // primeiro
        lt[l]--;
    } else if (c == N - 1 && QRcode[l][c - 1] == 0) { // ultima
        lt[l]++;
    } else if (c == N - 1 && QRcode[l][c - 1] == 1) { // ultima
        lt[l]--;
    }

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
    if ((c + l) == (N - 1))
        db[1]++;
}

bool preProcess() {

    // INVALID =====================================================

    // Verficar se a contagem de pixeis nas linhas concide com a contagem de pixeis nas colunas e quadrantes
    int sum_lb = 0;
    int sum_cb = 0;
    int sum_qb = 0;
    for (int i = 0; i < N; i++) {
        sum_lb += lb[i];
        sum_cb += cb[i];
        if (lb[i] > N || cb[i] > N || lt[i] > N - 1 || ct[i] > N - 1 || lt[i] > 2 * lb[i] || ct[i] > 2 * cb[i])
            return false;
    }
    if (qb[0] > (N / 2 * (N - N / 2))) {
        return false;
    }
    if (qb[1] > (N / 2 * N / 2)) {
        return false;
    }
    if (qb[2] > (N / 2 * (N - N / 2))) {
        return false;
    }
    if (qb[3] > ((N - N / 2) * (N - N / 2))) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        sum_qb += qb[i];
    }
    if (sum_lb != sum_cb || sum_cb != sum_qb)
        return false;

    // Verificar se o no de pixeis de metade das linhas/colunas coincide com o numero de pixeis desses quadrantes
    int soma1 = 0;
    int soma2 = 0;
    int soma3 = 0;
    int soma4 = 0;
    for (int i = 0; i < N; i++) {
        if (i < N / 2) {
            soma1 += lb[i];
            soma3 += cb[i];

        } else {
            soma2 += lb[i];
            soma4 += cb[i];
        }
    }
    if (soma1 != qb[0] + qb[1] || soma2 != qb[2] + qb[3] || soma3 != qb[1] + qb[2] || soma4 != qb[0] + qb[3])
        return false;

    // Tam diagonais maior que os quadrante
    if (qb[0] + qb[2] < db[1] || qb[1] + qb[3] < db[0])
        return false;

    // TODO: Transicoes impossiveis(nao haver conjucacao entre transicoes e no cel pretas)

    // VALID =====================================================

    vector<int> cb_copy = cb;
    vector<int> ct_copy = ct;
    vector<int> db_copy = db;
    vector<int> qb_copy = qb;

    // Quadrantes todos preenchidos
    int soma = (N / 2) * ((N + 1) / 2);
    // 1Q
    if (qb_copy[0] == soma) {
        for (int i = 0; i < N / 2; i++) {
            for (int j = N / 2; j < N; j++) {
                if (!visited[i][j]) {
                    visited[i][j] = true;
                    update_pixel(i, j);
                }
            }
        }
    } else if (qb[0] == 0) {
        for (int i = 0; i < N / 2; i++) {
            for (int j = N / 2; j < N; j++) {
                if (!visited[i][j])
                    visited[i][j] = true;
            }
        }
    }

    // 3Q
    if (qb[2] == soma) {
        for (int i = N / 2; i < N; i++) {
            for (int j = 0; j < N / 2; j++) {
                if (!visited[i][j]) {
                    visited[i][j] = true;
                    update_pixel(i, j);
                }
            }
        }
    } else if (qb[2] == 0) {
        for (int i = N / 2; i < N; i++) {
            for (int j = 0; j < N / 2; j++) {
                if (!visited[i][j])
                    visited[i][j] = true;
            }
        }
    }

    // 2Q
    soma = (N / 2) * (N / 2);
    if (qb_copy[1] == soma) {
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N / 2; j++) {
                if (!visited[i][j]) {
                    visited[i][j] = true;
                    update_pixel(i, j);
                }
            }
        }
    } else if (qb[1] == 0) {
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N / 2; j++) {
                if (!visited[i][j])
                    visited[i][j] = true;
            }
        }
    }
    // 4Q
    soma = ((N + 1) / 2) * ((N + 1) / 2);
    if (qb_copy[3] == soma) {
        for (int i = N / 2; i < N; i++) {
            for (int j = N / 2; j < N; j++) {
                if (!visited[i][j]) {
                    visited[i][j] = true;
                    update_pixel(i, j);
                }
            }
        }
    } else if (qb[3] == 0) {
        for (int i = N / 2; i < N; i++) {
            for (int j = N / 2; j < N; j++) {
                if (!visited[i][j])
                    visited[i][j] = true;
            }
        }
    }

    // Preencher linhas/ colunas completas
    int empty_lin = -1;
    int empty_col = -1;
    int full_lin = -1;
    int full_col = -1;

    for (int i = 0; i < N; i++) {
        // linhas
        if (lb_copy[i] == N) {
            full_lin = i;
            if (lt_copy[i] != 0)
                return false;
            for (int j = 0; j < N; j++) {
                if (!visited[i][j]) {
                    visited[i][j] = true;
                    update_pixel(i, j);
                }
            }
        } else if (lb_copy[i] == 0) {
            empty_lin = i;
            if (lt_copy[i] != 0)
                return false;
            for (int j = 0; j < N; j++)
                if (!visited[i][j])
                    visited[i][j] = true;
        }

        // colunas
        if (cb_copy[i] == N) {
            full_col = i;
            if (ct_copy[i] != 0)
                return false;
            for (int j = 0; j < N; j++) {
                if (!visited[j][i]) {
                    visited[j][i] = true;
                    update_pixel(j, i);
                }
            }
        } else if (cb_copy[i] == 0) {
            empty_col = i;
            if (ct_copy[i] != 0)
                return false;
            for (int j = 0; j < N; j++)
                if (!visited[j][i])
                    visited[j][i] = true;
        }
    }

    // Preencher diagonais completas
    if (db_copy[0] == N) {
        for (int i = 0; i < N; i++) {
            if (!visited[i][i]) {
                visited[i][i] = true;
                update_pixel(i, i);
            }
        }
    } else if (db_copy[0] == 0) {
        for (int i = 0; i < N; i++) {
            if (!visited[i][i])
                visited[i][i] = false;
        }
    }
    if (db_copy[1] == N) {
        for (int i = 0; i < N; i++) {
            if (!visited[i][N - i - 1]) {
                visited[i][N - i - 1] = true;
                update_pixel(i, N - i - 1);
            }
        }

    } else if (db_copy[1] == 0) {
        for (int i = 0; i < N; i++) {
            if (!visited[i][N - i - 1])
                visited[i][N - i - 1] = true;
        }
    }

    // Pintar linhas que so tenham uma transicao
    // Caso onde ha mais pixeis pretos na linha e existe uma coluna branca
    if (empty_col != -1) {
        for (int i = 0; i < N; i++) {
            if (lt_copy[i] == 1 && lb_copy[i] > N / 2) {
                if (empty_col + 1 > N / 2) {
                    // pintar a parte de cima
                    for (int j = 0; j < lb_copy[i]; j++) {
                        if (!visited[i][j]) {
                            visited[i][j] = true;
                            update_pixel(i, j);
                        }
                    }
                } else {
                    // pintar a parte de baixo
                    for (int j = N - lb_copy[i]; j < N; j++) {
                        if (!visited[i][j]) {
                            visited[i][j] = true;
                            update_pixel(i, j);
                        }
                    }
                }
            }
        }
    }

    // Caso onde ha mais pixeis pretos na linha e existe uma coluna preta
    if (full_col != -1) {
        for (int i = 0; i < N; i++) {
            if (lt_copy[i] == 1 && lb_copy[i] <= N / 2) {
                if (full_col + 1 > N / 2) {
                    // pintar a parte da direita
                    for (int j = N - lb_copy[i]; j < N; j++) {
                        if (!visited[i][j]) {
                            visited[i][j] = true;
                            update_pixel(i, j);
                        }
                    }
                } else {
                    // pintar a parte da esquerda
                    for (int j = 0; j < lb_copy[i]; j++) {
                        if (!visited[i][j]) {
                            visited[i][j] = true;
                            update_pixel(i, j);
                        }
                    }
                }
            }
        }
    }

    // Pintar colunas que so tenham uma transicao
    // Caso onde ha mais pixeis pretos na coluna e existe uma linha branca
    if (empty_lin != -1) {
        for (int i = 0; i < N; i++) {
            if (ct_copy[i] == 1 && cb_copy[i] > N / 2) {
                if (empty_lin + 1 > N / 2) {
                    // pintar a parte da esquerda
                    for (int j = 0; j < cb_copy[i]; j++) {
                        if (!visited[j][i]) {
                            visited[j][i] = true;
                            update_pixel(j, i);
                        }
                    }
                } else {
                    // pintar a parte da direita
                    for (int j = N - cb_copy[i]; j < N; j++) {
                        if (!visited[j][i]) {
                            visited[j][i] = true;
                            update_pixel(j, i);
                        }
                    }
                }
            }
        }
    }

    // Caso onde ha menos pixeis pretos na coluna e existe uma linha preta
    if (full_lin != -1) {
        for (int i = 0; i < N; i++) {
            if (ct_copy[i] == 1 && cb_copy[i] <= N / 2) {
                if (full_lin + 1 > N / 2) {
                    // pintar a parte de baixo
                    for (int j = N - cb_copy[i]; j < N; j++) {
                        if (!visited[j][i]) {
                            visited[j][i] = true;
                            update_pixel(j, i);
                        }
                    }
                } else {
                    // pintar a parte de cima
                    for (int j = 0; j < cb_copy[i]; j++) {
                        if (!visited[j][i]) {
                            visited[j][i] = true;
                            update_pixel(j, i);
                        }
                    }
                }
            }
        }
    }

    // Preto Branco Preto Branco nas colunas se N for impar
    if (N % 2) {
        for (int i = 0; i < N; i++) {
            if (ct_copy[i] == N - 1) {
                int j;
                if (cb_copy[i] == N / 2) {
                    j = 1; // 0 1 0 1 0
                } else if (cb_copy[i] == N / 2 + 1) {
                    j = 0; // 1 0 1 0 1
                }
                for (j; j < N; j += 2) {
                    if (!visited[j][i]) {
                        visited[j][i] = true;
                        update_pixel(j, i);
                    }
                }
            }
        }
    }

    return true;
}

bool encode(int lin, int c) {

    // Rejection test
    if (!verify_args(lin, c)) {
        return false;
    }

    // Base case
    if ((c == N || lb[lin] == 0) && lin >= N - 1) {
        final_verify();
        return false;
    } else if (lin > N - 1) {
        final_verify();
        return false;
    } else if (lb[lin] == 0 || c == N) {
        verify_row(lin) && encode(lin + 1, 0);
        return false;

    } else if (lt_copy[lin] == 1) {
        // pintar todas as celulas a direita ou a esquerda!!
        for (int i = 0; i < lb_copy[lin]; i++) {
            if (!visited[lin][i]) {
                update_pixel(lin, i);
            }
        }
        verify_row(lin) && encode(lin + 1, 0);
        // reverter os que pintei!
        for (int i = 0; i < lb_copy[lin]; i++) {
            if (!visited[lin][i]) {
                revert_pixel(lin, i);
            }
        }
        for (int i = 1; i < lb_copy[lin] + 1; i++) {
            if (!visited[lin][N - i]) {
                update_pixel(lin, N - i);
            }
        }
        verify_row(lin) && encode(lin + 1, 0);
        // reverter os que pintei!
        for (int i = 1; i < lb_copy[lin] + 1; i++) {
            if (!visited[lin][N - i]) {
                revert_pixel(lin, N - i);
            }
        }

        return false;
    } else if (lt_copy[lin] == N - 1) {
        // 0 1 0 1 0 || 1 0 1 0 1
        for (int j = 0; j < N; j += 2) {
            if (!visited[lin][j]) {
                // visited[lin][j] = true;
                update_pixel(lin, j);
            }
        }
        verify_row(lin) && encode(lin + 1, 0);
        for (int j = 0; j < N; j += 2) {
            if (!visited[lin][j]) {
                revert_pixel(lin, j);
            }
        }

        for (int j = 1; j < N; j += 2) {
            if (!visited[lin][j]) {
                // visited[lin][j] = true;
                update_pixel(lin, j);
            }
        }
        verify_row(lin) && encode(lin + 1, 0);
        for (int j = 1; j < N; j += 2) {
            if (!visited[lin][j]) {
                // visited[lin][j] = false;
                revert_pixel(lin, j);
            }
        }
        return false;
    } else {
        // For all unvisited pixels
        for (int i = c; i < N; i++) {
            if (!visited[lin][i]) {
                visited[lin][i] = true;
                update_pixel(lin, i);
                encode(lin, i + 1);
                revert_pixel(lin, i);
                visited[lin][i] = false;
            }

            // na ultima coluna passa para a proxima linha
            if (i == N - 1 && lin < N - 1) {
                verify_row(lin) && encode(lin + 1, 0);
                return false;
            } else if (lin == N - 1 && i == N - 1 && final_verify()) {
                return false;
            }
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
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    int num_codes;
    cin >> num_codes;

    while (num_codes--) {

        // Ler input ============================================
        cin >> N;
        lb = vector<int>(N);
        cb = vector<int>(N);
        lt = vector<int>(N);
        ct = vector<int>(N);
        qb = vector<int>(4);
        db = vector<int>(2);

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
        lt_copy = lt;
        lb_copy = lb;

        k = 0;

        if (preProcess()) {
            // printQRcode(QRcode, N);
            !final_verify() && encode(0, 0);
        }

        // Output =================================================
        switch (k) {
        case 0:
            cout << "DEFECT: No QR Code generated!" << endl;
            break;
        case 1:
            cout << "VALID: 1 QR Code generated!" << endl;
            printQRcode(QRcodecpy, N);
            break;
        default:
            cout << "INVALID: " << k << " QR Codes generated!" << endl;
            break;
        }
    }

    return 0;
}