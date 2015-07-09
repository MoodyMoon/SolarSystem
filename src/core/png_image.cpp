#include "stdafx.h"
#include "png_image.h"

PNGImage::PNGImage(const char *full_file_path)
{
    FILE *file = fopen(full_file_path, "rb");

    if(file == nullptr)
    {
        throw ReadFileException("PNGImage", "PNG file specified is not valid.");
    }

    png_byte png_header[8];

    fread(png_header, 1u, 8u, file);

    if(ferror(file) != 0 || feof(file) != 0)
    {
        fclose(file);
        throw ReadFileException("PNGImage", "Cannot read the PNG file.");
    }

    if(png_sig_cmp(png_header, 0, 8) != 0)
    {
        fclose(file);
        throw ReadFileException("PNGImage", "Please select a valid PNG file.");
    }

    png_structp png_read_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if(png_read_struct == nullptr)
    {
        fclose(file);
        throw ReadFileException("PNGImage", "Cannot initialize PNG read struct.");
    }

    png_infop png_info_struct = png_create_info_struct(png_read_struct);

    if(png_info_struct == nullptr)
    {
        png_destroy_read_struct(&png_read_struct, nullptr, nullptr);
        fclose(file);
        throw ReadFileException("PNGImage", "Cannot initialize PNG info struct.");
    }

    png_infop png_end_struct = png_create_info_struct(png_read_struct);

    if(png_info_struct == nullptr)
    {
        png_destroy_read_struct(&png_read_struct, &png_info_struct, nullptr);
        fclose(file);
        throw ReadFileException("PNGImage", "Cannot initialize PNG info struct.");
    }

    if(setjmp(png_jmpbuf(png_read_struct)))
    {
        png_destroy_read_struct(&png_read_struct, &png_info_struct, &png_end_struct);
        fclose(file);
        throw ReadFileException("PNGImage", "Error in libpng.");
    }

    png_init_io(png_read_struct, file);
    png_set_sig_bytes(png_read_struct, 8);
    png_read_info(png_read_struct, png_info_struct);

    int bit_depth, color_type;

    png_get_IHDR(png_read_struct, png_info_struct, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

    if(bit_depth == 16)
    {
        png_set_scale_16(png_read_struct);
        bit_depth = 8;
    }

    switch(color_type)
    {
        case PNG_COLOR_TYPE_RGB:
            color_space = GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            color_space = GL_RGBA;
            break;
        default:
            png_destroy_read_struct(&png_read_struct, &png_info_struct, &png_end_struct);
            fclose(file);
            throw ReadFileException("PNGImage", "Unsupported color space.");
    }

    int row_bytes = png_get_rowbytes(png_read_struct, png_info_struct);

    row_bytes += 3 - ((row_bytes-1) % 4);

    output_buffer.reset(new (std::nothrow)png_byte[row_bytes * height * sizeof(png_byte) + 15]);

    if(output_buffer.get() == nullptr)
    {
        png_destroy_read_struct(&png_read_struct, &png_info_struct, &png_end_struct);
        fclose(file);
        throw ReadFileException("PNGImage", "Cannot allocate required memory.");
    }

    std::unique_ptr<png_byte *> row_pointers(new png_byte *[height]);

    if(row_pointers.get() == nullptr)
    {
        png_destroy_read_struct(&png_read_struct, &png_info_struct, &png_end_struct);
        fclose(file);
        throw ReadFileException("PNGImage", "Cannot allocate required memory.");
    }

    png_byte **_row_pointers = row_pointers.get();
    unsigned char *_output_buffer = output_buffer.get();

    for(unsigned int i = 0; i < height; ++i)
    {
        _row_pointers[height - 1 - i] = _output_buffer + i * row_bytes;
    }

    png_read_image(png_read_struct, _row_pointers);
    png_destroy_read_struct(&png_read_struct, &png_info_struct, &png_end_struct);
    fclose(file);
}

unsigned int PNGImage::GetHeight()
{
    return height;
}

unsigned int PNGImage::GetWidth()
{
    return width;
}

GLint PNGImage::GetColorSpace()
{
    return color_space;
}

const unsigned char * PNGImage::GetDecodedImageBuffer()
{
    return output_buffer.get();
}