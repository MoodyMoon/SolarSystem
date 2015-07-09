#ifndef CORE_JPEG_IMAGE_H
#define CORE_JPEG_IMAGE_H

#include "file_reader.h"

class JPEGImage
{
    private:
        int height;
        int width;

        std::unique_ptr<unsigned char> output_buffer;

    public:
        JPEGImage(const JPEGImage &) = delete;
        JPEGImage & operator=(const JPEGImage &) = delete;

        JPEGImage(const char *full_file_path);

        unsigned int GetHeight(void);
        unsigned int GetWidth(void);
        GLint GetColorSpace(void);

        const unsigned char * GetDecodedImageBuffer(void);
};

#endif