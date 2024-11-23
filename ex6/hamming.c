#include <stdio.h>
#include <string.h>
#include <math.h>

void charToBinary(char ch, int binary[], int *index) {
    for (int i = 7; i >= 0; i--) {
        binary[(*index)++] = (ch >> i) & 1;
    }
}

void calculateParityBits(int hammingCode[], int n, int r) {
    for (int i = 0; i < r; i++) {
        int parityPos = (int)pow(2, i);
        int parity = 0;

        for (int j = parityPos; j <= n; j += (2 * parityPos)) {
            for (int k = j; k < j + parityPos && k <= n; k++) {
                parity ^= hammingCode[k];
            }
        }
        hammingCode[parityPos] = parity;
    }
}

int generateHammingCode(int dataBits[], int m, int hammingCode[]) {
    int r = 0;
    int n = m;

    while (n + r + 1 > pow(2, r)) {
        r++;
    }

    n = m + r;

    for (int i = 1, j = 0, k = 0; i <= n; i++) {
        if (i == (int)pow(2, k)) {
            hammingCode[i] = 0;
            k++;
        } else {
            hammingCode[i] = dataBits[j++];
        }
    }

    calculateParityBits(hammingCode, n, r);

    return n;
}

int detectAndCorrectError(int hammingCode[], int n, int r) {
    int errorPos = 0;

    for (int i = 0; i < r; i++) {
        int parityPos = (int)pow(2, i);
        int parity = 0;

        for (int j = parityPos; j <= n; j += (2 * parityPos)) {
            for (int k = j; k < j + parityPos && k <= n; k++) {
                parity ^= hammingCode[k];
            }
        }

        if (parity != 0) {
            errorPos += parityPos;
        }
    }

    return errorPos;
}

void binaryToChar(int binary[], int length, char output[]) {
    int index = 0;
    for (int i = 0; i < length; i += 8) {
        char ch = 0;
        for (int j = 0; j < 8; j++) {
            ch |= (binary[i + j] << (7 - j));
        }
        output[index++] = ch;
    }
    output[index] = '\0';
}

int main() {
    char inputString[32];
    int binary[256];
    int dataBits[256];
    int hammingCode[512];

    printf("Enter the input string: ");
    scanf("%s", inputString);

    int index = 0;
    for (int i = 0; i < strlen(inputString); i++) {
        charToBinary(inputString[i], binary, &index);
    }

    for (int i = 0; i < index; i++) {
        dataBits[i] = binary[i];
    }

    int n = generateHammingCode(dataBits, index, hammingCode);

    printf("Generated Hamming code: ");
    for (int i = 1; i <= n; i++) {
        printf("%d ", hammingCode[i]);
    }
    printf("\n");

    printf("Enter the position to simulate error (0 for no error): ");
    int errorPos;
    scanf("%d", &errorPos);

    if (errorPos > 0 && errorPos <= n) {
        hammingCode[errorPos] = !hammingCode[errorPos];
        printf("Hamming code with error: ");
        for (int i = 1; i <= n; i++) {
            printf("%d ", hammingCode[i]);
        }
        printf("\n");
    }

    int detectedErrorPos = detectAndCorrectError(hammingCode, n, log2(n + 1));
    if (detectedErrorPos == 0) {
        printf("No error detected.\n");
    } else {
        printf("Error detected at position: %d\n", detectedErrorPos);
        int originalBit = !hammingCode[detectedErrorPos];
        hammingCode[detectedErrorPos] = originalBit;  // Correct the error
        printf("Corrected Hamming code: ");
        for (int i = 1; i <= n; i++) {
            printf("%d ", hammingCode[i]);
        }
        printf("\n");
        printf("Corrected bit at position %d: %d\n", detectedErrorPos, originalBit);
    }

    // Extract the data bits from the corrected Hamming code
    int correctedDataBits[256];
    int j = 0, k = 0;
    for (int i = 1; i <= n; i++) {
        if (i != (int)pow(2, k)) {
            correctedDataBits[j++] = hammingCode[i];
        } else {
            k++;
        }
    }

    // Convert the corrected data bits back to string
    char correctedString[32];
    binaryToChar(correctedDataBits, j, correctedString);
    printf("Corrected string: %s\n", correctedString);

    return 0;
}