#ifndef FREE_ROAM_CAMERA_H
#define FREE_ROAM_CAMERA_H

#include "camera_interface.h"

class FreeRoamCamera : public CameraInterface
{
    private:
        static constexpr double radians = 90.0 / 180.0 * M_PI;

        bool cursor_x_init = false;
        bool cursor_y_init = false;
        double cursor_x;
        double cursor_y;
        double cursor_x_delta;
        double cursor_y_delta;

        glm::dvec3 eye_position;
        glm::dvec3 look_at_unit_vector;
        glm::dvec3 look_at_unit_vector_temp;
        glm::dvec3 look_at_right_rotate_axis;
        glm::dvec3 up_unit_vector;

        double look_x_rotate_radians;
        double look_y_rotate_radians;

        inline void PositionCamera(void)
        {
            glLoadIdentity();
            gluLookAt(eye_position.x, eye_position.y, eye_position.z, eye_position.x + look_at_unit_vector.x, eye_position.y + look_at_unit_vector.y, eye_position.z + look_at_unit_vector.z, 0.0, 1.0, 0.0);
        }

    public:
        FreeRoamCamera(const FreeRoamCamera &) = delete;
        FreeRoamCamera & operator=(const FreeRoamCamera &) = delete;

        FreeRoamCamera(double client_width, double client_height);

        virtual void MoveLeft(void);
        virtual void MoveRight(void);
        virtual void MoveUp(void);
        virtual void MoveDown(void);
        virtual void LookX(double cursor_x);
        virtual void LookY(double cursor_y);
        virtual void ZoomIn(void) {}
        virtual void ZoomOut(void) {}
        virtual glm::dvec3 GetPosition(void);
        virtual void Reset(double client_width, double client_height);
        virtual void Do(double /* frame_swap_time */) {}
};

#endif