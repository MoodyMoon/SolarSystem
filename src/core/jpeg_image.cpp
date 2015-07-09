#include "stdafx.h"
#include "jpeg_image.h"

JPEGImage::JPEGImage(const char *full_file_path)
{
    tjhandle turbo_decompressor = tjInitDecompress();

    if(turbo_decompressor == nullptr)
    {
        throw ReadFileException("Image", tjGetErrorStr());
    }

    FileReader file_reader(full_file_path, false);

    uint64_t file_size = file_reader.GetFileLength();

    std::unique_ptr<unsigned char> input_buffer(new unsigned char[file_size]);
    unsigned char *_input_buffer = input_buffer.get();

    file_reader.Read(reinterpret_cast<char *>(_input_buffer), file_size);

    if(tjDecompressHeader(turbo_decompressor, _input_buffer, file_size, &width, &height) == -1)
    {
        ASSERT_METHOD(tjDestroy(turbo_decompressor), ==, 0);
        throw ReadFileException("Image", tjGetErrorStr());
    }

    output_buffer.reset(new unsigned char[width * height * 3]);

    tjDecompress2(turbo_decompressor, _input_buffer, file_size, output_buffer.get(), width, 0, height, TJPF_RGB, TJFLAG_ACCURATEDCT | TJFLAG_BOTTOMUP);

    ASSERT_METHOD(tjDestroy(turbo_decompressor), ==, 0);
}

unsigned int JPEGImage::GetHeight()
{
    return height;
}

unsigned int JPEGImage::GetWidth()
{
    return width;
}

GLint JPEGImage::GetColorSpace(void)
{
    return GL_RGB;
}

const unsigned char * JPEGImage::GetDecodedImageBuffer()
{
    return output_buffer.get();
}