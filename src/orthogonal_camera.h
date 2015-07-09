#ifndef ORTHOGONAL_CAMERA_H
#define ORTHOGONAL_CAMERA_H

#include "camera_interface.h"

class OrthogonalCamera : public CameraInterface
{
    private:
        static constexpr double distance_between_left_middle = 500.0;

    public:
        OrthogonalCamera(const OrthogonalCamera &) = delete;
        OrthogonalCamera & operator=(const OrthogonalCamera &) = delete;

        OrthogonalCamera(double client_width, double client_height);

        virtual void MoveLeft(void) {}
        virtual void MoveRight(void) {}
        virtual void MoveUp(void) {}
        virtual void MoveDown(void) {}
        virtual void LookX(double cursor_x) {}
        virtual void LookY(double cursor_y) {}
        virtual void ZoomIn(void) {}
        virtual void ZoomOut(void) {}
        virtual glm::dvec3 GetPosition(void);
        virtual void Reset(double client_width, double client_height);
        virtual void Do(double /* frame_swap_time */) {}
};

#endif