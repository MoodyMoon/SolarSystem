#include "core/stdafx.h"
#include "skybox.h"

Skybox::Skybox(std::shared_ptr<CameraInterface> *camera,
               double distance_between_left_middle,
               GLTexture *texture) : camera(*camera),
                                     box_half_width(distance_between_left_middle + 100.0),
                                     texture(texture) {}

void Skybox::SetCamera(std::shared_ptr<CameraInterface> *camera)
{
    this->camera = *camera;
    Do(0.0);
}

void Skybox::Do(double frame_swap_time)
{
    if(!camera.expired())
    {
        camera_shared_pointer = camera.lock();
        camera_position = camera_shared_pointer->GetPosition();
        camera_shared_pointer.reset();

        glPushMatrix();

        glDisable(GL_LIGHTING);

        if(texture != nullptr)
        {
            texture->Bind();
        }

        glBegin(GL_QUADS);
        glTexCoord2d(0.0, repeat); glVertex3d(camera_position.x - box_half_width, camera_position.y + box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(0.0, 0.0); glVertex3d(camera_position.x - box_half_width, camera_position.y - box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, 0.0); glVertex3d(camera_position.x + box_half_width, camera_position.y - box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, repeat); glVertex3d(camera_position.x + box_half_width, camera_position.y + box_half_width, camera_position.z - box_half_width);

        glTexCoord2d(0.0, repeat); glVertex3d(camera_position.x + box_half_width, camera_position.y + box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(0.0, 0.0); glVertex3d(camera_position.x + box_half_width, camera_position.y - box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, 0.0); glVertex3d(camera_position.x + box_half_width, camera_position.y - box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(repeat, repeat); glVertex3d(camera_position.x + box_half_width, camera_position.y + box_half_width, camera_position.z + box_half_width);

        glTexCoord2d(0.0, repeat); glVertex3d(camera_position.x + box_half_width, camera_position.y + box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(0.0, 0.0); glVertex3d(camera_position.x + box_half_width, camera_position.y - box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(repeat, 0.0); glVertex3d(camera_position.x - box_half_width, camera_position.y - box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(repeat, repeat); glVertex3d(camera_position.x - box_half_width, camera_position.y + box_half_width, camera_position.z + box_half_width);

        glTexCoord2d(0.0, repeat); glVertex3d(camera_position.x - box_half_width, camera_position.y + box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(0.0, 0.0); glVertex3d(camera_position.x - box_half_width, camera_position.y - box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(repeat, 0.0); glVertex3d(camera_position.x - box_half_width, camera_position.y - box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, repeat); glVertex3d(camera_position.x - box_half_width, camera_position.y + box_half_width, camera_position.z - box_half_width);

        glTexCoord2d(0.0, repeat); glVertex3d(camera_position.x - box_half_width, camera_position.y + box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(0.0, 0.0); glVertex3d(camera_position.x - box_half_width, camera_position.y + box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, 0.0); glVertex3d(camera_position.x + box_half_width, camera_position.y + box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, repeat); glVertex3d(camera_position.x + box_half_width, camera_position.y + box_half_width, camera_position.z + box_half_width);

        glTexCoord2d(0.0, repeat); glVertex3d(camera_position.x - box_half_width, camera_position.y - box_half_width, camera_position.z + box_half_width);
        glTexCoord2d(0.0, 0.0); glVertex3d(camera_position.x - box_half_width, camera_position.y - box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, 0.0); glVertex3d(camera_position.x + box_half_width, camera_position.y - box_half_width, camera_position.z - box_half_width);
        glTexCoord2d(repeat, repeat); glVertex3d(camera_position.x + box_half_width, camera_position.y - box_half_width, camera_position.z + box_half_width);
        glEnd();
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}