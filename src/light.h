#ifndef LIGHT_H
#define LIGHT_H

#include "core/object_callback_interface.h"

class Light : public ObjectCallbackInterface
{
    private:
        static bool lighting_enabled;

        GLenum light_index;
        GLfloat light_position[4];

    public:
        Light(const Light &) = delete;
        Light & operator=(const Light &) = delete;

        Light(const GLenum light_index,
              const GLfloat light_position[4],
              const GLfloat light_ambient[4],
              const GLfloat light_diffuse[4],
              const GLfloat light_specular[4]);

        virtual void Do(double frame_swap_time);

        ~Light(void);
};

#endif