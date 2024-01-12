#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef union {
    uint64_t value;
    uint8_t bytes[8];
} EndianConverter;

uint64_t convertEndian(uint64_t value, int numBytes) {
    EndianConverter converter;
    converter.value = value;

    // 交换字节顺序
    for (int i = 0; i < numBytes / 2; ++i) {
        uint8_t temp = converter.bytes[i];
        converter.bytes[i] = converter.bytes[numBytes - 1 - i];
        converter.bytes[numBytes - 1 - i] = temp;
    }

    return converter.value;
}

void printHelp() {
    printf("Usage: your_program [-i input_file] [-o output_file] [-n numBytes] [-help]\n");
    printf("Options:\n");
    printf("  -i   Input file name (default: input.bin)\n");
    printf("  -o   Output file name (default: output.bin)\n");
    printf("  -n   Number of bytes for endian conversion (default: 4)\n");
    printf("  -help   Display this help message\n");
}

int main(int argc, char *argv[]) {
    const char *inputFileName = "input.bin";
    const char *outputFileName = "output.bin";
    int numBytes = 8;

    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            inputFileName = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputFileName = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            numBytes = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "-help") == 0) {
            printHelp();
            return 0;
        }
    }

    // 判断 numBytes 是否为偶数
    if (numBytes % 2 != 0) {
        printf("Error: numBytes must be an even number.\n");
        return 1;
    }

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

    // 读取整个文件内容
    uint8_t *fileBuffer = malloc(fileSize);
    fread(fileBuffer, 1, fileSize, inputFile);

    // 关闭输入文件
    fclose(inputFile);

    // 进行大小端转换
    for (long i = 0; i < fileSize; i += numBytes) {
        uint64_t *currentData = (uint64_t *)(fileBuffer + i);
        *currentData = convertEndian(*currentData, numBytes);
    }

    // 将转换后的数据写入输出文件
    fwrite(fileBuffer, 1, fileSize, outputFile);

    // 关闭输出文件
    fclose(outputFile);

    // 释放内存
    free(fileBuffer);

    return 0;
}
