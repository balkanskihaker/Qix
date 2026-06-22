#include "RgbImage.h"
#include <GLUT/glut.h>

RgbImage::RgbImage(int numRows, int numCols) {
    NumRows = numRows;
    NumCols = numCols;
    ImagePtr = new unsigned char[NumRows * GetNumBytesPerRow()];
    if (!ImagePtr) {
        fprintf(stderr, "Unable to allocate memory for %ld x %ld bitmap.\n", NumRows, NumCols);
        Reset();
        ErrorCode = MemoryError;
    }
    unsigned char* c = ImagePtr;
    int rowLen = GetNumBytesPerRow();
    for (int i = 0; i < NumRows; i++)
        for (int j = 0; j < rowLen; j++)
            *(c++) = 0;
}

bool RgbImage::LoadBmpFile(const char* filename) {
    Reset();
    FILE* infile = fopen(filename, "rb");
    if (!infile) {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        ErrorCode = OpenError;
        return false;
    }

    bool fileFormatOK = false;
    int bChar = fgetc(infile);
    int mChar = fgetc(infile);
    if (bChar == 'B' && mChar == 'M') {
        skipChars(infile, 4 + 4 + 4 + 4);
        NumCols = readLong(infile);
        NumRows = readLong(infile);
        skipChars(infile, 2);
        int bitsPerPixel = readShort(infile);
        skipChars(infile, 4 + 4 + 4 + 4 + 4 + 4);
        if (NumCols > 0 && NumCols <= 100000 && NumRows > 0 && NumRows <= 100000
            && bitsPerPixel == 24 && !feof(infile)) {
            fileFormatOK = true;
        }
    }
    if (!fileFormatOK) {
        Reset();
        ErrorCode = FileFormatError;
        fprintf(stderr, "Not a valid 24-bit bitmap file: %s.\n", filename);
        fclose(infile);
        return false;
    }

    ImagePtr = new unsigned char[NumRows * GetNumBytesPerRow()];
    if (!ImagePtr) {
        fprintf(stderr, "Unable to allocate memory for %ld x %ld bitmap: %s.\n", NumRows, NumCols, filename);
        Reset();
        ErrorCode = MemoryError;
        fclose(infile);
        return false;
    }

    unsigned char* cPtr = ImagePtr;
    for (int i = 0; i < NumRows; i++) {
        int j;
        for (j = 0; j < NumCols; j++) {
            *(cPtr + 2) = fgetc(infile);
            *(cPtr + 1) = fgetc(infile);
            *cPtr = fgetc(infile);
            cPtr += 3;
        }
        int k = 3 * NumCols;
        for (; k < GetNumBytesPerRow(); k++) {
            fgetc(infile);
            *(cPtr++) = 0;
        }
    }
    if (feof(infile)) {
        fprintf(stderr, "Premature end of file: %s.\n", filename);
        Reset();
        ErrorCode = ReadError;
        fclose(infile);
        return false;
    }
    fclose(infile);
    return true;
}

short RgbImage::readShort(FILE* infile) {
    unsigned char lowByte = fgetc(infile);
    unsigned char hiByte = fgetc(infile);
    short ret = hiByte;
    ret <<= 8;
    ret |= lowByte;
    return ret;
}

long RgbImage::readLong(FILE* infile) {
    unsigned char byte0 = fgetc(infile);
    unsigned char byte1 = fgetc(infile);
    unsigned char byte2 = fgetc(infile);
    unsigned char byte3 = fgetc(infile);
    long ret = byte3;
    ret <<= 8; ret |= byte2;
    ret <<= 8; ret |= byte1;
    ret <<= 8; ret |= byte0;
    return ret;
}

void RgbImage::skipChars(FILE* infile, int numChars) {
    for (int i = 0; i < numChars; i++) fgetc(infile);
}

bool RgbImage::WriteBmpFile(const char* filename) {
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        ErrorCode = OpenError;
        return false;
    }
    fputc('B', outfile); fputc('M', outfile);
    int rowLen = GetNumBytesPerRow();
    writeLong(40 + 14 + NumRows * rowLen, outfile);
    writeShort(0, outfile); writeShort(0, outfile);
    writeLong(40 + 14, outfile);
    writeLong(40, outfile);
    writeLong(NumCols, outfile);
    writeLong(NumRows, outfile);
    writeShort(1, outfile); writeShort(24, outfile);
    writeLong(0, outfile); writeLong(0, outfile);
    writeLong(0, outfile); writeLong(0, outfile);
    writeLong(0, outfile); writeLong(0, outfile);

    unsigned char* cPtr = ImagePtr;
    for (int i = 0; i < NumRows; i++) {
        for (int j = 0; j < NumCols; j++) {
            fputc(*(cPtr + 2), outfile);
            fputc(*(cPtr + 1), outfile);
            fputc(*(cPtr + 0), outfile);
            cPtr += 3;
        }
        int k = 3 * NumCols;
        for (; k < GetNumBytesPerRow(); k++) {
            fputc(0, outfile);
            cPtr++;
        }
    }
    fclose(outfile);
    return true;
}

void RgbImage::writeLong(long data, FILE* outfile) {
    fputc((unsigned char)(data & 0xff), outfile);
    fputc((unsigned char)((data >> 8) & 0xff), outfile);
    fputc((unsigned char)((data >> 16) & 0xff), outfile);
    fputc((unsigned char)((data >> 24) & 0xff), outfile);
}

void RgbImage::writeShort(short data, FILE* outfile) {
    fputc((unsigned char)(data & 0xff), outfile);
    fputc((unsigned char)((data >> 8) & 0xff), outfile);
}

void RgbImage::SetRgbPixelf(long row, long col, double red, double green, double blue) {
    SetRgbPixelc(row, col, doubleToUnsignedChar(red),
                 doubleToUnsignedChar(green), doubleToUnsignedChar(blue));
}

void RgbImage::SetRgbPixelc(long row, long col,
                              unsigned char red, unsigned char green, unsigned char blue) {
    assert(row < NumRows && col < NumCols);
    unsigned char* thePixel = GetRgbPixel(row, col);
    *(thePixel++) = red;
    *(thePixel++) = green;
    *(thePixel) = blue;
}

unsigned char RgbImage::doubleToUnsignedChar(double x) {
    if (x >= 1.0) return 255;
    if (x <= 0.0) return 0;
    return (unsigned char)(x * 255.0);
}

bool RgbImage::LoadFromOpenglBuffer() {
    int viewportData[4];
    glGetIntegerv(GL_VIEWPORT, viewportData);
    int vWidth = viewportData[2];
    int vHeight = viewportData[3];

    if (ImagePtr == 0) {
        NumRows = vHeight;
        NumCols = vWidth;
        ImagePtr = new unsigned char[NumRows * GetNumBytesPerRow()];
        if (!ImagePtr) {
            fprintf(stderr, "Unable to allocate memory for %ld x %ld buffer.\n", NumRows, NumCols);
            Reset();
            ErrorCode = MemoryError;
            return false;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, NumCols, NumRows, GL_RGB, GL_UNSIGNED_BYTE, ImagePtr);
    return true;
}