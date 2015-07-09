#include "core/stdafx.h"
#include "billboard.h"

Billboard::Billboard(glm::dvec3 center_coordinates,
                     double width,
                     double height,
                     std::shared_ptr<CameraInterface> *camera,
                     GLTexture *texture) : half_width(width / 2.0),
                                           half_height(height / 2.0),
                                           right_vector(-1.0, 0.0, 0.0),
                                           center_coordinates(center_coordinates),
                                           camera(*camera),
                                           texture(texture) {}

void Billboard::SetCamera(std::shared_ptr<CameraInterface> *camera)
{
    this->camera = *camera;
    Do(0.0);
}

void Billboard::Do(double frame_swap_time)
{
    if(!camera.expired())
    {
        camera_shared_pointer = camera.lock();
        look_at_camera_vector = camera_shared_pointer->GetPosition() - center_coordinates;
        camera_shared_pointer.reset();

        CalculateRotationAboutY();
        CalculateRotationAboutRightVector();

        ResetQuadCoordinates();

        top_left_vector = glm::rotateY(top_left_vector, rotation_radians_about_y);
        bottom_left_vector = glm::rotateY(bottom_left_vector, rotation_radians_about_y);
        bottom_right_vector = glm::rotateY(bottom_right_vector, rotation_radians_about_y);
        top_right_vector = glm::rotateY(top_right_vector, rotation_radians_about_y);

        top_left_vector = glm::rotate(top_left_vector, rotation_radians_about_right_vector, right_vector);
        bottom_left_vector = glm::rotate(bottom_left_vector, rotation_radians_about_right_vector, right_vector);
        bottom_right_vector = glm::rotate(bottom_right_vector, rotation_radians_about_right_vector, right_vector);
        top_right_vector = glm::rotate(top_right_vector, rotation_radians_about_right_vector, right_vector);

        glPushMatrix();

        glDisable(GL_LIGHTING);

        if(texture != nullptr)
            texture->Bind();

        glBegin(GL_QUADS);
        resultant_vector = center_coordinates + top_left_vector;
        glTexCoord2d(0.0, 1.0); glVertex3d(resultant_vector.x, resultant_vector.y, resultant_vector.z);
        resultant_vector = center_coordinates + bottom_left_vector;
        glTexCoord2d(0.0, 0.0); glVertex3d(resultant_vector.x, resultant_vector.y, resultant_vector.z);
        resultant_vector = center_coordinates + bottom_right_vector;
        glTexCoord2d(1.0, 0.0); glVertex3d(resultant_vector.x, resultant_vector.y, resultant_vector.z);
        resultant_vector = center_coordinates + top_right_vector;
        glTexCoord2d(1.0, 1.0); glVertex3d(resultant_vector.x, resultant_vector.y, resultant_vector.z);
        glEnd();

        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}

void Billboard::CalculateRotationAboutY()
{
    rotation_radians_about_y = atan2(look_at_camera_vector.x, look_at_camera_vector.z);
    right_vector = {-1.0, 0.0, 0.0};
    right_vector = glm::rotateY(right_vector, rotation_radians_about_y);
}

void Billboard::CalculateRotationAboutRightVector()
{
    rotation_radians_about_right_vector = atan2(look_at_camera_vector.y, sqrt(pow(look_at_camera_vector.x, 2.0) + pow(look_at_camera_vector.z, 2.0)));
}

void Billboard::ResetQuadCoordinates()
{
    top_left_vector = {-half_width, half_height, 0.0};
    bottom_left_vector = {-half_width, -half_height, 0.0};
    bottom_right_vector = {half_width, -half_height, 0.0};
    top_right_vector = {half_width, half_height, 0.0};
}