#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "core/gltexture.h"
#include "core/object_callback_interface.h"
#include "camera_interface.h"

class Billboard : public ObjectCallbackInterface
{
    private:
        double half_width;
        double half_height;
        glm::dvec3 right_vector;
        glm::dvec3 look_at_camera_vector;
        double rotation_radians_about_y;
        double rotation_radians_about_right_vector;

        glm::dvec3 center_coordinates;
        glm::dvec3 top_left_vector;
        glm::dvec3 bottom_left_vector;
        glm::dvec3 bottom_right_vector;
        glm::dvec3 top_right_vector;
        glm::dvec3 resultant_vector;

        std::weak_ptr<CameraInterface> camera;
        std::shared_ptr<CameraInterface> camera_shared_pointer;
        GLTexture *texture;

        void CalculateRotationAboutY(void);
        void CalculateRotationAboutRightVector(void);
        void ResetQuadCoordinates(void);

    public:
        Billboard(const Billboard &) = delete;
        Billboard & operator=(const Billboard &) = delete;

        Billboard(glm::dvec3 center_coordinates, double width, double height, std::shared_ptr<CameraInterface> *camera, GLTexture *texture);
        void SetCamera(std::shared_ptr<CameraInterface> *camera);

        virtual void Do(double frame_swap_time);
};

#endif