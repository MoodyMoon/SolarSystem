#include "stdafx.h"
#include "gltexture.h"

bool GLTexture::texture_2d_enabled = false;

GLTexture::GLTexture(std::size_t image_width, std::size_t image_height, const unsigned char *image_buffer, GLint image_color_space)
{
    if(!texture_2d_enabled)
    {
        glEnable(GL_TEXTURE_2D);
        texture_2d_enabled = true;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glGenTextures(1, &texture);
    CheckForError();

    glBindTexture(GL_TEXTURE_2D, texture);
    CheckForError();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, image_color_space, image_width, image_height, 0, image_color_space, GL_UNSIGNED_BYTE, image_buffer);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, image_color_space, image_width, image_height, image_color_space, GL_UNSIGNED_BYTE, image_buffer);
    CheckForError();
}

void GLTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void GLTexture::CheckForError()
{
    GLenum error = glGetError();

    if(error != GL_NO_ERROR)
    {
        throw GLEWException("GLTexture", std::to_string(error).c_str());
    }
}

GLTexture::~GLTexture()
{
    glBindTexture(GL_TEXTURE, 0u);
    glDeleteTextures(1, &texture);
}