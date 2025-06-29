//1a.bitsuff
#include <stdio.h>
#include <string.h>

int main() {
    char ch;
    char array[400] = "01111110";
    char recd_array[400];
    int counter = 0, i = 8, j, k;

    printf("Enter a binary data stream (0s and 1s only):\n");

    while ((ch = getchar()) != '\n' && i < sizeof(array) - 9) {
        if (ch != '0' && ch != '1') continue;
        array[i++] = ch;
        if (ch == '1') {
            counter++;
            if (counter == 5) {
                array[i++] = '0';
                counter = 0;
            }
        } else {
            counter = 0;
        }
    }

    strcat(array, "01111110");

    printf("\nStuffed data:\n%s\n", array);

    counter = 0;
    k = 0;
    for (j = 8; j < strlen(array) - 8; j++) {
        recd_array[k++] = array[j];
        if (array[j] == '1') {
            counter++;
            if (counter == 5) {
                if (array[j + 1] == '0') {
                    j++;
                }
                counter = 0;
            }
        } else {
            counter = 0;
        }
    }

    recd_array[k] = '\0';

    printf("\nDestuffed data:\n%s\n", recd_array);

    return 0;
}












//1b.charcstuff

#include <stdio.h>

#define DLE 'P'
#define STX 'B'
#define ETX 'C'

int main() {
    char ch, array[200] = {DLE, STX};
    int i = 2, j;

    printf("Input: ");
    while ((ch = getchar()) != '\n') {
        if (ch == DLE) {
            array[i++] = DLE;
        }
        array[i++] = ch;
    }
    array[i++] = DLE;
    array[i++] = ETX;

    printf("\nStuffed stream:\n");
    for (j = 0; j < i; j++) {
        if (array[j] == DLE) printf("DLE ");
        else if (array[j] == STX) printf("STX ");
        else if (array[j] == ETX) printf("ETX ");
        else printf("%c ", array[j]);
    }

    printf("\n\nDestuffed data stream:\n");
    for (j = 2; j < i - 2; j++) {
        if (array[j] == DLE && array[j + 1] == DLE) {
            printf("DLE ");
            j++;
        } else if (array[j] == STX) {
            printf("STX ");
        } else if (array[j] == ETX) {
            printf("ETX ");
        } else {
            printf("%c ", array[j]);
        }
    }

    return 0;
}















//3. lbc
#include <iostream>
using namespace std;

char data[5];
int encoded[7], edata[7], syndrome[3];

int hmatrix[3][7] = {
    {1, 1, 1, 0, 1, 0, 0},
    {1, 1, 0, 1, 0, 1, 0},
    {1, 0, 1, 1, 0, 0, 1}
};

char gmatrix[4][8] = {
    "1000111",
    "0100110",
    "0010101",
    "0001011"
};

int main() {
    int i, j;

    cout << "Hamming Code --- Encoding\n";
    cout << "Enter 4 bit data: ";
    cin >> data;

    for (i = 0; i < 7; i++) encoded[i] = 0;

    cout << "Generator Matrix\n";
    for (i = 0; i < 4; i++)
        cout << "\t" << gmatrix[i] << "\n";

    cout << "Encoded Data: ";
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 4; j++)
            encoded[i] += (data[j] - '0') * (gmatrix[j][i] - '0');
        encoded[i] %= 2;
        cout << encoded[i];
    }
    cout << "\n";

    cout << "Hamming Code --- Decoding\n";
    cout << "Enter Encoded bits as received (7 bits one by one):\n";
    for (i = 0; i < 7; i++) {
        cin >> edata[i];
    }

    for (i = 0; i < 3; i++) {
        syndrome[i] = 0;
        for (j = 0; j < 7; j++)
            syndrome[i] += edata[j] * hmatrix[i][j];
        syndrome[i] %= 2;
    }

    int errorPos = -1;
    for (j = 0; j < 7; j++) {
        if (syndrome[0] == hmatrix[0][j] &&
            syndrome[1] == hmatrix[1][j] &&
            syndrome[2] == hmatrix[2][j]) {
            errorPos = j;
            break;
        }
    }

    if (syndrome[0] == 0 && syndrome[1] == 0 && syndrome[2] == 0) {
        cout << "Data is error free!!\n";
    } else if (errorPos != -1) {
        cout << "Error received at bit number " << (errorPos + 1) << " of the data\n";
        edata[errorPos] = !edata[errorPos];
        cout << "The Correct data Should be: ";
        for (i = 0; i < 7; i++)
            cout << edata[i];
        cout << "\n";
    } else {
        cout << "Error detected but position not identifiable (multiple errors)\n";
    }

    return 0;
}












//4 polynomial crc

#include <stdio.h>
#include <string.h>

#define DEGREE 16

int result[128];

int mod2add(int x, int y) {
    return (x == y ? 0 : 1);
}

int getnext(int array[], int pos, int length) {
    int i = pos;
    while (i < length && array[i] == 0)
        i++;
    return i;
}

void calc_CRC(int length) {
    int genpoly[DEGREE + 1] = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1};
    int pos = 0, i, newpos;
    while (pos <= length - DEGREE) {
        if (result[pos] == 1) {
            for (i = 0; i <= DEGREE; ++i)
                result[pos + i] = mod2add(result[pos + i], genpoly[i]);
        }
        newpos = getnext(result, pos + 1, length);
        if (newpos > pos + 1)
            pos = newpos;
        else
            ++pos;
    }
}

int main() {
    int array[128], length = 0, i;
    char ch;

    printf("Enter the data stream: ");
    while ((ch = getchar()) != '\n')
        array[length++] = ch - '0';

    int totalLength = length + DEGREE;

    for (i = length; i < totalLength; ++i)
        array[i] = 0;

    for (i = 0; i < totalLength; ++i)
        result[i] = array[i];

    calc_CRC(totalLength);

    printf("The transmitted frame is: ");
    for (i = 0; i < length; ++i)
        printf("%d", array[i]);
    for (i = length; i < totalLength; ++i)
        printf("%d", result[i]);
    printf("\n");

    length = 0;
    printf("Enter the stream for which CRC has to be checked: ");
    while ((ch = getchar()) != '\n')
        array[length++] = ch - '0';

    for (i = 0; i < length; ++i)
        result[i] = array[i];

    calc_CRC(length);

    printf("Checksum: ");
    for (i = length - DEGREE; i < length; ++i)
        printf("%d", result[i]);
    printf("\n");

    return 0;
}












//5. shortest path algo
#include <stdio.h>
#include <ctype.h>

#define NUM_OF_NODES 10
#define PERMANENT 1
#define TENTATIVE 0
#define INF 9999

struct node {
    unsigned int weight;
    int prev;
    int state;
};

int main() {
    int table[NUM_OF_NODES][NUM_OF_NODES] = {
        {0,1,0,0,0,4,0,0,0,0},
        {1,0,4,0,0,0,0,1,0,0},
        {0,4,0,3,2,0,0,0,3,0},
        {0,0,3,0,1,0,0,0,0,0},
        {0,0,2,1,0,3,0,0,0,1},
        {4,0,0,0,3,0,1,0,0,0},
        {0,0,0,0,0,1,0,2,0,2},
        {0,1,0,0,0,0,2,0,1,0},
        {0,0,3,0,0,0,0,1,0,2},
        {0,0,0,0,1,0,2,0,2,0}
    };

    int src, dest, i, working_node;
    unsigned int min;
    struct node nodes[NUM_OF_NODES];
    char ch;

    // Initialize all nodes
    for (i = 0; i < NUM_OF_NODES; i++) {
        nodes[i].state = TENTATIVE;
        nodes[i].weight = INF;
        nodes[i].prev = -1;
    }

    printf("Enter Source (A-J): ");
    src = toupper(getchar()) - 'A';
    while (getchar() != '\n');  // clear buffer

    printf("Enter Destination (A-J): ");
    dest = toupper(getchar()) - 'A';
    while (getchar() != '\n');

    nodes[src].weight = 0;
    working_node = src;

    do {
        nodes[working_node].state = PERMANENT;

        for (i = 0; i < NUM_OF_NODES; i++) {
            if (table[working_node][i] != 0 && nodes[i].state == TENTATIVE) {
                if (nodes[working_node].weight + table[working_node][i] < nodes[i].weight) {
                    nodes[i].weight = nodes[working_node].weight + table[working_node][i];
                    nodes[i].prev = working_node;
                }
            }
        }

        min = INF;
        for (i = 0; i < NUM_OF_NODES; i++) {
            if (nodes[i].state == TENTATIVE && nodes[i].weight < min) {
                min = nodes[i].weight;
                working_node = i;
            }
        }

    } while (working_node != dest);

    // Print shortest path
    printf("\nShortest Path got--->\n%c", dest + 'A');
    while (nodes[dest].prev != -1) {
        printf(" <- %c", nodes[dest].prev + 'A');
        dest = nodes[dest].prev;
    }

    printf("\nAt a total weight of: %d\n", nodes[working_node].weight);

    return 0;
}



