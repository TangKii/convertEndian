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
    int numBytes = 4;

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

            // 检查 numBytes 是否是 2 的偶数
            if (numBytes < 2 || numBytes % 2 != 0) {
                printf("Error: numBytes must be at least 2 and an even number.\n");
                return 1;
            }

            i++;
        } else if (strcmp(argv[i], "-help") == 0) {
            printHelp();
            return 0;
        }
    }

    if (numBytes <= 0) {
        printf("Error: numBytes must be greater than 0.\n");
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

    // 计算需要读取的整数倍大小
    long readSize = (fileSize / numBytes) * numBytes;

    // 读取整个文件内容
    uint8_t *fileBuffer = malloc(readSize);
    fread(fileBuffer, 1, readSize, inputFile);

    // 关闭输入文件
    fclose(inputFile);

    // 进行大小端转换
    for (long i = 0; i < readSize; i += numBytes) {
        convertEndian(fileBuffer + i, numBytes);
    }

    // 将转换后的数据写入输出文件
    fwrite(fileBuffer, 1, readSize, outputFile);

    // 关闭输出文件
    fclose(outputFile);

    // 释放内存
    free(fileBuffer);

    return 0;
}
