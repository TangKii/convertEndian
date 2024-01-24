#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void convertEndian(uint8_t *data, int numBytes) {
    for (int i = 0; i < numBytes / 2; ++i) {
        uint8_t temp = data[i];
        data[i] = data[numBytes - 1 - i];
        data[numBytes - 1 - i] = temp;
    }
}

void printHelp() {
    printf("\nconvertEndian [-n numBytes] input_file output_file\n");
    printf("Options:\n");
    printf("  -n   Number of bytes for endian conversion (default: 8)\n");
}

int main(int argc, char *argv[]) {
    const char *inputFileName = "input.bin";
    const char *outputFileName = "output.bin";
    int numBytes = 8;

    // ���������в���
    if(argc == 3) {
        inputFileName = argv[1];
        outputFileName = argv[2];
    }
    else if(argc == 5) {

        if (strcmp(argv[1], "-n") == 0) {
            numBytes = atoi(argv[2]);
        }
        else {
            printHelp();
            return 0;
        }

        inputFileName = argv[3];
        outputFileName = argv[4];

        // ��� numBytes �Ƿ��� 2 ��ż��
        if (numBytes < 2 || numBytes % 2 != 0) {
            printf("Error: numBytes must be at least 2 and an even number.\n");
            return 1;
        }
    }
    else {
        printHelp();
        return 0;
    }
    //printf("inputfile: %s, outputfile %s, numBytes %d\n",inputFileName, outputFileName,numBytes);

    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "rb");
    outputFile = fopen(outputFileName, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    // ��ȡ�ļ���С
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // ������Ҫ��ȡ����������С
    long alignSize = (fileSize / numBytes) * numBytes;
    long leftover = fileSize - alignSize;

    //printf("file align ize %d, file leftover size %d\n",alignSize, leftover);
    // ��ȡ�����ļ�����
    uint8_t *fileBuffer = malloc(fileSize);
    fread(fileBuffer, 1, fileSize, inputFile);

    // �ر������ļ�
    fclose(inputFile);

    // ���д�С��ת��
    for (long i = 0; i < alignSize; i += numBytes) {
        convertEndian(fileBuffer + i, numBytes);
    }
    convertEndian(fileBuffer + alignSize, leftover);

    // ��ת���������д������ļ�
    fwrite(fileBuffer, 1, fileSize, outputFile);

    // �ر�����ļ�
    fclose(outputFile);

    // �ͷ��ڴ�
    free(fileBuffer);

    return 0;
}
