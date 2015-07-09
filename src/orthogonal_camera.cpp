#include "core/stdafx.h"
#include "orthogonal_camera.h"

constexpr double OrthogonalCamera::distance_between_left_middle;

OrthogonalCamera::OrthogonalCamera(double client_width, double client_height)
{
    Reset(client_width, client_height);
}

glm::dvec3 OrthogonalCamera::GetPosition()
{
    static glm::dvec3 camera_position(0.0, 1000.0, 0.0);
    return camera_position;
}

void OrthogonalCamera::Reset(double client_width, double client_height)
{
    glViewport(0, 0, client_width, client_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double distance_between_top_middle = distance_between_left_middle / client_width * client_height;

    glOrtho(-distance_between_left_middle, distance_between_left_middle, -distance_between_top_middle, distance_between_top_middle, -1000.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotated(90.0, 1.0, 0.0, 0.0);
}