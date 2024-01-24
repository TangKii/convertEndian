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

    // 解析命令行参数
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

        // 检查 numBytes 是否是 2 的偶数
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

    // 获取文件大小
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // 计算需要读取的整数倍大小
    long alignSize = (fileSize / numBytes) * numBytes;
    long leftover = fileSize - alignSize;

    //printf("file align ize %d, file leftover size %d\n",alignSize, leftover);
    // 读取整个文件内容
    uint8_t *fileBuffer = malloc(fileSize);
    fread(fileBuffer, 1, fileSize, inputFile);

    // 关闭输入文件
    fclose(inputFile);

    // 进行大小端转换
    for (long i = 0; i < alignSize; i += numBytes) {
        convertEndian(fileBuffer + i, numBytes);
    }
    convertEndian(fileBuffer + alignSize, leftover);

    // 将转换后的数据写入输出文件
    fwrite(fileBuffer, 1, fileSize, outputFile);

    // 关闭输出文件
    fclose(outputFile);

    // 释放内存
    free(fileBuffer);

    return 0;
}
