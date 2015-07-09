#include "core/stdafx.h"
#include "light.h"

bool Light::lighting_enabled = false;

Light::Light(const GLenum light_index,
             const GLfloat light_position[4],
             const GLfloat light_ambient[4],
             const GLfloat light_diffuse[4],
             const GLfloat light_specular[4]) :
                 light_index(light_index)
{
    for(std::size_t index = 0; index < 4; ++index)
    {
        this->light_position[index] = light_position[index];
    }

    if(!lighting_enabled)
    {
        glEnable(GL_LIGHTING);
        lighting_enabled = true;
    }

    glEnable(light_index);

    glLightfv(light_index, GL_AMBIENT, light_ambient);
    glLightfv(light_index, GL_DIFFUSE, light_diffuse);
    glLightfv(light_index, GL_SPECULAR, light_specular);
}

void Light::Do(double /* frame_swap_time */)
{
    glLightfv(light_index, GL_POSITION, light_position);
}

Light::~Light()
{
    glDisable(light_index);
}