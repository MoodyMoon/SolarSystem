#include "core/stdafx.h"
#include "free_roam_camera.h"

constexpr double FreeRoamCamera::radians;

FreeRoamCamera::FreeRoamCamera(double client_width,
                               double client_height) : eye_position(0.0, 0.0, 500.0),
                                                       look_at_unit_vector(0.0, 0.0, -1.0)
{
    Reset(client_width, client_height);
    look_at_right_rotate_axis = glm::rotateY(look_at_unit_vector, -radians);
    PositionCamera();
}

void FreeRoamCamera::MoveLeft()
{
    eye_position.x -= look_at_right_rotate_axis.x;
    eye_position.z -= look_at_right_rotate_axis.z;
    PositionCamera();
}

void FreeRoamCamera::MoveRight()
{
    eye_position.x += look_at_right_rotate_axis.x;
    eye_position.z += look_at_right_rotate_axis.z;
    PositionCamera();
}

void FreeRoamCamera::MoveUp()
{
    eye_position += look_at_unit_vector;
    PositionCamera();
}

void FreeRoamCamera::MoveDown()
{
    eye_position -= look_at_unit_vector;
    PositionCamera();
}

void FreeRoamCamera::LookX(double cursor_x)
{
    if(cursor_x_init)
    {
        cursor_x_delta = cursor_x - this->cursor_x;

        if(cursor_x_delta > 0.0  || cursor_x_delta < 0.0)
        {
            look_x_rotate_radians = -(cursor_x_delta * 0.001);
            look_at_unit_vector = glm::rotateY(look_at_unit_vector, look_x_rotate_radians);
            look_at_right_rotate_axis = glm::rotateY(look_at_right_rotate_axis, look_x_rotate_radians);
            PositionCamera();
        }
    }
    else
    {
        cursor_x_init = true;
    }

    this->cursor_x = cursor_x;
}

void FreeRoamCamera::LookY(double cursor_y)
{
    if(cursor_y_init)
    {
        cursor_y_delta = cursor_y - this->cursor_y;

        if(cursor_y_delta > 0.0  || cursor_y_delta < 0.0)
        {
            look_at_unit_vector_temp = glm::rotate(look_at_unit_vector, cursor_y_delta * 0.001, -look_at_right_rotate_axis);

            if(look_at_unit_vector_temp.y > -0.95 && look_at_unit_vector_temp.y < 0.95)
            {
                look_at_unit_vector = look_at_unit_vector_temp;
                PositionCamera();
            }
        }
    }
    else
    {
        cursor_y_init = true;
    }

    this->cursor_y = cursor_y;
}

glm::dvec3 FreeRoamCamera::GetPosition()
{
    return eye_position;
}

void FreeRoamCamera::Reset(double client_width, double client_height)
{
    glViewport(0, 0, client_width, client_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, static_cast<GLdouble>(client_width) / static_cast<GLdouble>(client_height), 0.1, 2000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}