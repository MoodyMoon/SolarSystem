#ifndef SKYBOX_H
#define SKYBOX_H

#include "core/gltexture.h"
#include "core/object_callback_interface.h"
#include "camera_interface.h"

class Skybox : public ObjectCallbackInterface
{
    private:
        std::weak_ptr<CameraInterface> camera;
        std::shared_ptr<CameraInterface> camera_shared_pointer;
        glm::dvec3 camera_position;

        glm::dvec3 box_positive;
        glm::dvec3 box_negative;
        double box_half_width;
        double repeat = 5.0;
        GLTexture *texture;

    public:
        Skybox(std::shared_ptr<CameraInterface> *camera, double distance_between_left_middle, GLTexture *texture);
        Skybox(const Skybox &) = delete;
        Skybox & operator=(const Skybox &) = delete;

        void SetCamera(std::shared_ptr<CameraInterface> *camera);
        virtual void Do(double frame_swap_time);
};

#endif