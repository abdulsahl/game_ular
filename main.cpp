/*
List Kelompok:
Kelas B
1. 230411100003, Harits Putra Junaidi
2. 230411100086, Danendra Mahardhika
3. 230411100059, Achmad Lutfi Madhani
4. 230411100104 Moch Sigit Aringga
5. 230411100090, Abdullah Sahl
6. 230411100166, Dicky Prasetyo
*/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>

// Ukuran papan
#define COLS 7
#define ROWS 12

// Fungsi untuk menggambar teks
void drawText(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// Tambahkan fungsi untuk memeriksa apakah nomor sel mengandung digit NRP
int containsNRPDigit(int cellNum) {
    // Digit terakhir NRP: 0, 3, 4, 6, 9

    // Periksa setiap digit dalam nomor sel
    int temp = cellNum;
    while (temp > 0) {
        int digit = temp % 10;
        if (digit == 0 || digit == 3 || digit == 4 || digit == 6 || digit == 9) {
            return 1; // Mengandung digit NRP
        }
        temp /= 10;
    }

    // Kasus khusus untuk angka 0
    if (cellNum == 0) {
        return 1;
    }

    return 0;
}

// Fungsi untuk memeriksa apakah nomor sel bagian dari pasangan yang harus dihilangkan gap-nya
int isPartOfSpecialPair(int cellNum) {
    int specialPairs[] = {7, 8, 14, 15, 21, 22, 28, 29, 35, 36, 42, 43, 49, 50, 56, 57, 63, 64, 70, 71, 77, 78};
    int numSpecialPairs = sizeof(specialPairs) / sizeof(specialPairs[0]);

    for (int i = 0; i < numSpecialPairs; i++) {
        if (cellNum == specialPairs[i]) {
            return 1;
        }
    }
    return 0;
}

int checkIsUpOrDown(int num) {
    int special_num[] = {1, 2, 3, 4, 5, 6, 7, 78, 79, 80, 81, 82, 83, 84};

    // Perulangan untuk memeriksa apakah target ada dalam array
    for (int i = 0; i < sizeof(special_num) / sizeof(special_num[0]); ++i) {
        if (special_num[i] == num) {
            return 1;
        }
    }

    return 0;
}

// Fungsi untuk memeriksa apakah dua sel membentuk pasangan khusus
int formSpecialPair(int cell1, int cell2) {
    int specialPairs[][2] = {
        {7, 8}, {14, 15}, {21, 22}, {28, 29}, {35, 36},
        {42, 43}, {49, 50}, {56, 57}, {63, 64}, {70, 71}, {77, 78}
    };
    int numPairs = sizeof(specialPairs) / sizeof(specialPairs[0]);

    for (int i = 0; i < numPairs; i++) {
        if ((cell1 == specialPairs[i][0] && cell2 == specialPairs[i][1]) ||
            (cell1 == specialPairs[i][1] && cell2 == specialPairs[i][0])) {
            return 1;
        }
    }
    return 0;
}

// Fungsi untuk menggambar papan
void drawBoard() {
    float cellWidth = 2.0f / COLS;
    float cellHeight = 2.0f / ROWS; // Tinggi setiap sel
    float horizontalGapSize = 0.01f; // Ukuran gap horizontal antar baris

    // Gambar background hijau untuk gap
    glColor3f(0.0f, 1.0f, 0.0f); // Warna hijau untuk gap
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Array untuk menyimpan nomor sel untuk referensi
    int cellNumbers[ROWS][COLS];

    // isi array dengan nomor sel
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (row % 2 == 0) {
                // Baris genap (0, 2, 4, ...) - dari kiri ke kanan
                cellNumbers[row][col] = row * COLS + col + 1;
            } else {
                // Baris ganjil (1, 3, 5, ...) - dari kanan ke kiri
                cellNumbers[row][col] = row * COLS + (COLS - col);
            }
        }
    }

    // Gambar petak-petak
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int cellNum = cellNumbers[row][col];

            // Hitung posisi koordinat
            float x = -1.0f + col * cellWidth;
            float y = -1.0f + row * cellHeight;
            float topGap = horizontalGapSize;
            float bottomGap = horizontalGapSize;

            // Cek apakah perlu menghilangkan gap atas
            if (row > 0) {
                int cellAbove;
                if ((row-1) % 2 == 0) {
                    // Sel di atas berada di baris genap
                    cellAbove = cellNumbers[row-1][col];
                } else {
                    // Sel di atas berada di baris ganjil
                    cellAbove = cellNumbers[row-1][col];
                }

                if (formSpecialPair(cellNum, cellAbove)) {
                    topGap = 0.0f;
                }
            }

            // Cek apakah perlu menghilangkan gap bawah
            if (row < ROWS - 1) {
                int cellBelow;
                if ((row+1) % 2 == 0) {
                    // Sel di bawah berada di baris genap
                    cellBelow = cellNumbers[row+1][col];
                } else {
                    // Sel di bawah berada di baris ganjil
                    cellBelow = cellNumbers[row+1][col];
                }

                if (formSpecialPair(cellNum, cellBelow)) {
                    bottomGap = 0.0f;
                }
            }

            // Hilangkan gap bawah untuk cell 1 sampai 7
            if (cellNum >= 1 && cellNum <= 7) {
                topGap = 0.0f;
            }

            // Hilangkan gap atas untuk cell 78 sampai 84
            if (cellNum >= 78 && cellNum <= 84) {
                bottomGap = 0.0f;
            }



            // Sesuaikan posisi y dan tinggi sel berdasarkan gap
            y += topGap;
            float actualCellHeight = cellHeight - topGap - bottomGap;

            // Warna petak bergantian
            if ((row + col) % 2 == 0) {
                glColor3f(0.0f, 0.0f, 1.0f); // Putih
            } else {
                glColor3f(1.0f, 1.0f, 0.9f); // Biru
            }

            // Gambar petak menggunakan polygon
            glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x + cellWidth, y);
            glVertex2f(x + cellWidth, y + actualCellHeight);
            glVertex2f(x, y + actualCellHeight);
            glEnd();

            // Tulis nomor sel (hanya digit yang ada di NRP)
            char numStr[4];
            sprintf(numStr, "%d", cellNum);
            char filteredStr[4] = ""; // String untuk menyimpan hanya digit NRP
            int filteredIndex = 0;

            // Filter digit-digit yang ada di NRP
            for (int i = 0; numStr[i] != '\0'; i++) {
                char digit = numStr[i];
                if (digit == '0' || digit == '3' || digit == '4' || digit == '6' || digit == '9') {
                    filteredStr[filteredIndex++] = digit;
                }
            }
            filteredStr[filteredIndex] = '\0';

            // Tampilkan teks jika ada digit yang tersisa
            if (filteredIndex > 0) {
                // Warna nomor
                glColor3f(0.75f, 0.6f, 0.22f);
                drawText(x + cellWidth * 0.6f, y + actualCellHeight * 0.6f, filteredStr);
            }
        }
    }

    // Gambar pagar (border hijau lyar papan)
    glColor3f(0.0f, 1.0f, 0.0f); // Hijau untuk pagar
    float borderWidth = 0.02f;


    // Pagar atas
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f - borderWidth, 1.0f);
    glVertex2f(1.0f + borderWidth, 1.0f);
    glVertex2f(1.0f + borderWidth, 1.0f + borderWidth);
    glVertex2f(-1.0f - borderWidth, 1.0f + borderWidth);
    glEnd();

    // Pagar bawah
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f - borderWidth, -1.0f);
    glVertex2f(1.0f + borderWidth, -1.0f);
    glVertex2f(1.0f + borderWidth, -1.0f - borderWidth);
    glVertex2f(-1.0f - borderWidth, -1.0f - borderWidth);
    glEnd();

    // Pagar kiri
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f - borderWidth);
    glVertex2f(-1.0f, 1.0f + borderWidth);
    glVertex2f(-1.0f - borderWidth, 1.0f + borderWidth);
    glVertex2f(-1.0f - borderWidth, -1.0f - borderWidth);
    glEnd();

    // Pagar kanan
    glBegin(GL_POLYGON);
    glVertex2f(1.0f, -1.0f - borderWidth);
    glVertex2f(1.0f, 1.0f + borderWidth);
    glVertex2f(1.0f + borderWidth, 1.0f + borderWidth);
    glVertex2f(1.0f + borderWidth, -1.0f - borderWidth);
    glEnd();
}
//end



// gambar tangga
void tangga(float startX, float startY, float width, float height, int numRungs, float angle, float thickness) {
    float rungSpacing = height / (numRungs + 1);

    glPushMatrix();
    glTranslatef(startX + width / 2, startY, 0);
    glRotatef(angle, 0, 0, 1);
    glTranslatef(-startX - width / 2, -startY, 0);

    // Tiang vertikal kiri
    glColor3f(0.6f, 0.3f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(startX, startY);
    glVertex2f(startX + thickness, startY);
    glVertex2f(startX + thickness, startY + height);
    glVertex2f(startX, startY + height);
    glEnd();

    // Tiang vertikal kanan
    glBegin(GL_POLYGON);
    glVertex2f(startX + width - thickness, startY);
    glVertex2f(startX + width, startY);
    glVertex2f(startX + width, startY + height);
    glVertex2f(startX + width - thickness, startY + height);
    glEnd();

    // Anak tangga horizontal
    for (int i = 1; i <= numRungs; i++) {
        float rungY = startY + i * rungSpacing;
        glBegin(GL_POLYGON);
        glVertex2f(startX + thickness, rungY - thickness / 2);
        glVertex2f(startX + width - thickness, rungY - thickness / 2);
        glVertex2f(startX + width - thickness, rungY + thickness / 2);
        glVertex2f(startX + thickness, rungY + thickness / 2);
        glEnd();
    }
    glPopMatrix();
}
// end



//fungsi buat ular
void ular(float startX, float startY, float endX, float endY, float thickness, int numSegments) {
    // Warna merah
    glColor3f(1.0f, 0.0f, 0.0f);

    //titik awal ke titik akhir
    float dx = endX - startX;
    float dy = endY - startY;
    float length = sqrt(dx * dx + dy * dy);

    float nx = dx / length;
    float ny = dy / length;

    float perpX = -ny;
    float perpY = nx;

    // kurva
    float controlPoints[20][2];
    int numControls = numSegments + 1;

    // efek bergelombang
    for (int i = 0; i < numControls; i++) {
        float t = (float)i / (numControls - 1);
        controlPoints[i][0] = startX + t * dx;
        controlPoints[i][1] = startY + t * dy;
        if (i > 0 && i < numControls - 1) {
            float offset = (i % 2 == 0) ? 0.15f : -0.15f;
            controlPoints[i][0] += perpX * length * offset;
            controlPoints[i][1] += perpY * length * offset;
        }
    }

    // Gambar segmen ular
    for (int i = 0; i < numControls - 1; i++) {
        float segDx = controlPoints[i+1][0] - controlPoints[i][0];
        float segDy = controlPoints[i+1][1] - controlPoints[i][1];
        float segLen = sqrt(segDx * segDx + segDy * segDy);

        float segNx = segDx / segLen;
        float segNy = segDy / segLen;

        float segPerpX = -segNy;
        float segPerpY = segNx;

        glBegin(GL_POLYGON);
        // atas
        glVertex2f(controlPoints[i][0] + segPerpX * thickness/2,
                  controlPoints[i][1] + segPerpY * thickness/2);
        glVertex2f(controlPoints[i+1][0] + segPerpX * thickness/2,
                  controlPoints[i+1][1] + segPerpY * thickness/2);
        // bawah
        glVertex2f(controlPoints[i+1][0] - segPerpX * thickness/2,
                  controlPoints[i+1][1] - segPerpY * thickness/2);
        glVertex2f(controlPoints[i][0] - segPerpX * thickness/2,
                  controlPoints[i][1] - segPerpY * thickness/2);
        glEnd();
    }
}
//end


//persegi
void persegi(float x, float y, float size, float red, float green, float blue) {
    glColor3f(red, green, blue);

    glBegin(GL_POLYGON);
    glVertex2f(x, y + 0.01f);                     // Titik kiri bawah
    glVertex2f(x + (size + 0.01f), y + 0.01f);              // Titik kanan bawah
    glVertex2f(x + (size + 0.01f), y + size);       // Titik kanan atas
    glVertex2f(x , y + size);              // Titik kiri atas
    glEnd();
}
//end



//segitiga
void segitiga(float x, float y, float size, float red, float green, float blue) {
    glColor3f(red, green, blue);

    float height = size * sqrt(3) / 2;  // Tinggi segitiga

    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size / 2, y + height);
    glEnd();
}
//end


int currentPlayer = 0; // 0 = segitiga (hijau), 1 = persegi (kuning)
int playerPositions[2] = {1, 1}; // Posisi awal kedua pemain (cell 1)
int diceResult = 0;
int gameOver = 0;

// Koordinat pemain
float triangleX = -0.93f;
float triangleY = -0.93f;
float squareX = -0.85f;
float squareY = -0.95f;


float playerCoordinates[ROWS][COLS][2];
int tanggaMulai[] = {13, 36, 71};
int tanggaAkhir[] = {44, 62, 75};
int ularMulai[] = {35, 69, 76};
int ularAkhir[] = {9, 31, 12};


//lempar dadu
int rollDice() {
    return (rand() % 6) + 1; // Angka 1-6
}

// Fungsi untuk mengkonversi nomor sel ke koordinat baris dan kolom
void getCellPosition(int cellNum, int* row, int* col) {
    // Sel dimulai dari 1, jadi kurangi 1 untuk mendapatkan indeks yang benar
    cellNum = cellNum - 1;
    *row = cellNum / COLS;

    // Arah baris bergantian (serpentine pattern)
    if (*row % 2 == 0) {
        // Baris genap (dari kiri ke kanan)
        *col = cellNum % COLS;
    } else {
        // Baris ganjil (dari kanan ke kiri)
        *col = COLS - 1 - (cellNum % COLS);
    }
}

// Fungsi untuk mendapatkan koordinat gambar di sel tertentu
void getDrawCoordinatesForCell(int cellNum, float* x, float* y, int playerId) {
    if (cellNum < 1 || cellNum > ROWS * COLS) {
        *x = -0.9f;
        *y = -0.9f;
        return;
    }

    int row, col;
    getCellPosition(cellNum, &row, &col);

    float cellWidth = 2.0f / COLS;
    float cellHeight = 2.0f / ROWS;

    // Titik dasar tengah sel
    float baseX = -1.0f + col * cellWidth + cellWidth * 0.25f;
    float baseY = -1.0f + row * cellHeight + cellHeight * 0.25f;

    // Offset agar pemain tidak tumpang tindih
    if (playerId == 0) { // segitiga
        *x = baseX;
        *y = baseY + cellHeight * 0.15f; // geser sedikit ke atas
    } else { // kotak
        *x = baseX + cellWidth * 0.30f;  // geser ke kanan
        *y = baseY;
    }
}


// Fungsi untuk memeriksa apakah pemain berada di awal tangga atau ular
void checkLaddersAndSnakes(int playerIndex) {
    int position = playerPositions[playerIndex];

    // Periksa apakah pemain ada di tangga
    for (int i = 0; i < 3; i++) {
        if (position == tanggaMulai[i]) {
            playerPositions[playerIndex] = tanggaAkhir[i];
            printf("Player %d naik tangga dari %d ke %d\n", playerIndex + 1, position, tanggaAkhir[i]);
            return;
        }
    }

    // Periksa apakah pemain ada di ular
    for (int i = 0; i < 3; i++) {
        if (position == ularMulai[i]) {
            playerPositions[playerIndex] = ularAkhir[i];
            printf("Player %d turun ular dari %d ke %d\n", playerIndex + 1, position, ularAkhir[i]);
            return;
        }
    }
}



// Fungsi input yang dimodifikasi
void input(unsigned char key, int x, int y) {
    if (key == ' ' && !gameOver) {
        diceResult = rollDice();
        printf("Player %d melempar dadu: %d\n", currentPlayer + 1, diceResult);

        // Pindahkan pemain
        playerPositions[currentPlayer] += diceResult;

        // Periksa jika pemain mencapai atau melewati sel terakhir (menang)
        if (playerPositions[currentPlayer] >= ROWS * COLS) {
            playerPositions[currentPlayer] = ROWS * COLS; // Tetapkan ke sel terakhir
            printf("Player %d menang!\n", currentPlayer + 1);
            gameOver = 1;
        } else {
            // Periksa tangga dan ular
            checkLaddersAndSnakes(currentPlayer);
        }

        // Update posisi visual pemain
        if (currentPlayer == 0) {
            // Update posisi segitiga
            getDrawCoordinatesForCell(playerPositions[0], &triangleX, &triangleY, 0);

        } else {
            // Update posisi persegi
            getDrawCoordinatesForCell(playerPositions[1], &squareX, &squareY, 1);
        }

        // Ganti giliran pemain
        currentPlayer = 1 - currentPlayer; // Switch between 0 and 1


        // Redraw the board
        glutPostRedisplay();
    } else if (key == 'r' || key == 'R') {
        // Reset game
        playerPositions[0] = 1;
        playerPositions[1] = 1;
        currentPlayer = 0;
        gameOver = 0;

        // Reset posisi visual
        triangleX = -0.93f;
        triangleY = -0.93f;
        squareX = -0.85f;
        squareY = -0.95f;

        printf("Game direset\n");
        glutPostRedisplay();
    }
}



// Fungsi display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawBoard();
     // Tangga
    tangga(-0.68f, -0.78f, 0.22f, 0.85f, 4, 0, 0.018f); //ver
    tangga(0.2f, 0.75f, 0.13f, 1.1f, 4, 90, 0.018f); //horizon
    tangga(0.77f, -0.1f, 0.22f, 0.6f, 4, 30, 0.018f); //doag
    //ular
    ular(0.5f, 0.72f, -0.2f, -0.75f, 0.025f, 3); // tengah
    ular(-0.6f, 0.55f, -0.3f, -0.2f, 0.025f, 3); //kiri
    ular(0.8f, -0.3f, 0.6f, -0.75f, 0.025f, 3); // kanan
    segitiga(triangleX, triangleY, 0.05f, 0.0f, 1.0f, 0.0f); // Player 1
    persegi(squareX, squareY, 0.05f, 1.0f, 1.0f, 0.0f); // Player 2

    glutSwapBuffers();
}
//end


// Fungsi inisialisasi
void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.1f, 1.1f, -1.1f, 1.1f);
    glMatrixMode(GL_MODELVIEW);
    glutKeyboardFunc(input);

    // Seed random number generator
    srand(time(NULL));
}
//end


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 700);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Ular Tangga");

    printf("===== PERMAINAN TANGGA & ULAR =====\n");
    printf("Petunjuk:\n");
    printf("- Tekan SPASI untuk melempar dadu dan bermain\n");
    printf("- Pemain 1: Segitiga (Hijau)\n");
    printf("- Pemain 2: Persegi (Kuning)\n");
    printf("Selamat bermain!\n\n");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
