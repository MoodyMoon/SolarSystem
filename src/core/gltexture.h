#ifndef CORE_GLTEXTURE_H
#define CORE_GLTEXTURE_H

#include "exception.h"

class GLTexture
{
    private:
        static bool texture_2d_enabled;
        GLuint texture;

        void CheckForError(void);

    public:
        GLTexture(const GLTexture &) = delete;
        GLTexture & operator=(const GLTexture &) = delete;

        GLTexture(std::size_t image_width, std::size_t image_height, const unsigned char *image_buffer, GLint image_color_space);

        void Bind(void);

        ~GLTexture(void);
};

#endif