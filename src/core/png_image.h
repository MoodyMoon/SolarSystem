#ifndef CORE_PNG_IMAGE_H
#define CORE_PNG_IMAGE_H

#include "exception.h"

class PNGImage
{
    private:
        uint32_t height;
        uint32_t width;
        GLint color_space;

        std::unique_ptr<unsigned char> output_buffer;

    public:
        PNGImage(const PNGImage &) = delete;
        PNGImage & operator=(const PNGImage &) = delete;

        PNGImage(const char *full_file_path);

        unsigned int GetHeight(void);
        unsigned int GetWidth(void);
        GLint GetColorSpace(void);

        const unsigned char * GetDecodedImageBuffer(void);
};

#endif