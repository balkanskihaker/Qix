#pragma once

#include <stdio.h>
#include <assert.h>

class RgbImage {
public:
    RgbImage();
    RgbImage(const char* filename);
    RgbImage(int numRows, int numCols);
    ~RgbImage();

    bool LoadBmpFile(const char* filename);
    bool WriteBmpFile(const char* filename);
    bool LoadFromOpenglBuffer();

    long GetNumRows() const { return NumRows; }
    long GetNumCols() const { return NumCols; }
    long GetNumBytesPerRow() const { return ((3 * NumCols + 3) >> 2) << 2; }
    const void* ImageData() const { return (void*)ImagePtr; }

    const unsigned char* GetRgbPixel(long row, long col) const;
    unsigned char* GetRgbPixel(long row, long col);

    void SetRgbPixelf(long row, long col, double red, double green, double blue);
    void SetRgbPixelc(long row, long col, unsigned char red, unsigned char green, unsigned char blue);

    int GetErrorCode() const { return ErrorCode; }
    enum {
        NoError = 0,
        OpenError = 1,
        FileFormatError = 2,
        MemoryError = 3,
        ReadError = 4,
        WriteError = 5
    };
    bool ImageLoaded() const { return (ImagePtr != 0); }
    void Reset();

private:
    unsigned char* ImagePtr;
    long NumRows;
    long NumCols;
    int ErrorCode;

    static short readShort(FILE* infile);
    static long readLong(FILE* infile);
    static void skipChars(FILE* infile, int numChars);
    static void writeLong(long data, FILE* outfile);
    static void writeShort(short data, FILE* outfile);
    static unsigned char doubleToUnsignedChar(double x);
};

inline RgbImage::RgbImage() {
    NumRows = 0; NumCols = 0; ImagePtr = 0; ErrorCode = 0;
}

inline RgbImage::RgbImage(const char* filename) {
    NumRows = 0; NumCols = 0; ImagePtr = 0; ErrorCode = 0;
    LoadBmpFile(filename);
}

inline RgbImage::~RgbImage() {
    delete[] ImagePtr;
}

inline void RgbImage::Reset() {
    NumRows = 0; NumCols = 0;
    delete[] ImagePtr;
    ImagePtr = 0; ErrorCode = 0;
}

inline const unsigned char* RgbImage::GetRgbPixel(long row, long col) const {
    assert(row < NumRows && col < NumCols);
    const unsigned char* ret = ImagePtr;
    long i = row * GetNumBytesPerRow() + 3 * col;
    ret += i;
    return ret;
}

inline unsigned char* RgbImage::GetRgbPixel(long row, long col) {
    assert(row < NumRows && col < NumCols);
    unsigned char* ret = ImagePtr;
    long i = row * GetNumBytesPerRow() + 3 * col;
    ret += i;
    return ret;
}