#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include "core/object_callback_interface.h"

class CameraInterface : public ObjectCallbackInterface
{
    protected:
        CameraInterface() = default;

    public:
        CameraInterface(const CameraInterface &) = delete;
        CameraInterface & operator=(const CameraInterface &) = delete;

        virtual void MoveLeft(void) = 0;
        virtual void MoveRight(void) = 0;
        virtual void MoveUp(void) = 0;
        virtual void MoveDown(void) = 0;
        virtual void LookX(double cursor_x) = 0;
        virtual void LookY(double cursor_y) = 0;
        virtual void ZoomIn(void) = 0;
        virtual void ZoomOut(void) = 0;
        virtual glm::dvec3 GetPosition(void) = 0;
        virtual void Reset(double client_width, double client_height) = 0;

        virtual ~CameraInterface(void) {}
};

#endif