#ifndef TRACKING_CAMERA_H
#define TRACKING_CAMERA_H

#include "camera_interface.h"
#include "solar_system_body.h"

class TrackingCamera : public CameraInterface
{
    private:
        bool calculated_min_max_distance_from_body_surface = false;

        std::vector<std::weak_ptr<SolarSystemBody>> *solar_system_bodies;

        bool cursor_x_init = false;
        double cursor_x;
        double cursor_x_delta;

        double x_rotation_degrees = 0.0;
        double minimum_distance_from_body_surface;
        double maximum_distance_from_body_surface;
        double distance_from_body_surface;
        glm::dvec3 vector_from_tracked_body_to_camera;

        std::weak_ptr<SolarSystemBody> solar_system_body_weak_ptr;
        std::shared_ptr<SolarSystemBody> solar_system_body_shared_ptr;

        unsigned int tracked_body_index = 0u;
        glm::dvec3 tracked_body_position;

        glm::dvec3 eye_position;

        inline void PositionCamera(void)
        {
            glLoadIdentity();
            gluLookAt(eye_position.x, eye_position.y, eye_position.z, tracked_body_position.x, tracked_body_position.y, tracked_body_position.z, 0.0, 1.0, 0.0);
        }

        void TrackBody(void);

    public:
        TrackingCamera(const TrackingCamera &) = delete;
        TrackingCamera & operator=(const TrackingCamera &) = delete;

        TrackingCamera(std::vector<std::weak_ptr<SolarSystemBody>> *solar_system_bodies, double client_width, double client_height);

        virtual void MoveLeft(void);
        virtual void MoveRight(void);
        virtual void MoveUp(void) {}
        virtual void MoveDown(void) {}
        virtual void LookX(double cursor_x);
        virtual void LookY(double cursor_y) {}
        virtual void ZoomIn(void);
        virtual void ZoomOut(void);
        virtual glm::dvec3 GetPosition(void);
        virtual void Reset(double client_width, double client_height);
        virtual void Do(double frame_swap_time);
};

#endif