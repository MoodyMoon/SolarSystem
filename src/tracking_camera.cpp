#include "core/stdafx.h"
#include "tracking_camera.h"

TrackingCamera::TrackingCamera(std::vector<std::weak_ptr<SolarSystemBody>> *solar_system_bodies,
                               double client_width,
                               double client_height) : solar_system_bodies(solar_system_bodies)
{
    Reset(client_width, client_height);
    TrackBody();
}

void TrackingCamera::MoveLeft()
{
    if(solar_system_bodies != nullptr)
    {
        if(!solar_system_bodies->empty())
        {
            if(tracked_body_index == 0u)
            {
                tracked_body_index = solar_system_bodies->size() - 1u;
            }
            else
            {
                --tracked_body_index;
            }
            calculated_min_max_distance_from_body_surface = false;
        }
        else
        {
            tracked_body_index = 0u;
        }
    }
}

void TrackingCamera::MoveRight()
{
    if(solar_system_bodies != nullptr)
    {
        if(!solar_system_bodies->empty())
        {
            if(tracked_body_index == solar_system_bodies->size() - 1u)
            {
                tracked_body_index = 0u;
            }
            else
            {
                ++tracked_body_index;
            }
            calculated_min_max_distance_from_body_surface = false;
        }
        else
        {
            tracked_body_index = 0u;
        }
    }
}

void TrackingCamera::LookX(double cursor_x)
{
    if(cursor_x_init)
    {
        cursor_x_delta = cursor_x - this->cursor_x;

        if(cursor_x_delta > 0.0)
        {
            cursor_x_delta *= 0.01;
            x_rotation_degrees += cursor_x_delta;

            if(x_rotation_degrees > 360.0)
                x_rotation_degrees -= 360.0;

            TrackBody();
            this->cursor_x = cursor_x;
        }
        else if(cursor_x_delta < 0.0)
        {
            cursor_x_delta *= 0.01;
            x_rotation_degrees += cursor_x_delta;

            if(x_rotation_degrees < 0.0)
                x_rotation_degrees += 360.0;

            TrackBody();
            this->cursor_x = cursor_x;
        }
    }
    else
    {
        this->cursor_x = cursor_x;
        cursor_x_init = true;
    }
}

void TrackingCamera::ZoomIn()
{
    double distance_from_body_surface_temp = distance_from_body_surface - 0.1;

    if(distance_from_body_surface_temp < minimum_distance_from_body_surface)
    {
        distance_from_body_surface = minimum_distance_from_body_surface;
    }
    else
    {
        distance_from_body_surface = distance_from_body_surface_temp;
    }

    TrackBody();
}

void TrackingCamera::ZoomOut()
{
    double distance_from_body_surface_temp = distance_from_body_surface + 0.1;

    if(distance_from_body_surface_temp > maximum_distance_from_body_surface)
    {
        distance_from_body_surface = maximum_distance_from_body_surface;
    }
    else
    {
        distance_from_body_surface = distance_from_body_surface_temp;
    }

    TrackBody();
}

glm::dvec3 TrackingCamera::GetPosition()
{
    return eye_position;
}

void TrackingCamera::Reset(double client_width, double client_height)
{
    glViewport(0, 0, client_width, client_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, static_cast<GLdouble>(client_width) / static_cast<GLdouble>(client_height), 0.1, 2000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TrackingCamera::Do(double /* frame_swap_time */)
{
    TrackBody();
}

void TrackingCamera::TrackBody()
{
    if(solar_system_bodies != nullptr && !solar_system_bodies->empty())
    {
        try
        {
            solar_system_body_weak_ptr = solar_system_bodies->at(tracked_body_index);

            if(!solar_system_body_weak_ptr.expired())
            {
                solar_system_body_shared_ptr = solar_system_body_weak_ptr.lock();
                if(!calculated_min_max_distance_from_body_surface)
                {
                    minimum_distance_from_body_surface = solar_system_body_shared_ptr->GetRadius() * 5.0;
                    maximum_distance_from_body_surface = minimum_distance_from_body_surface + 5.0;
                    distance_from_body_surface = minimum_distance_from_body_surface + 2.5;
                    calculated_min_max_distance_from_body_surface = true;
                }

                tracked_body_position = solar_system_body_shared_ptr->GetDistanceFromSolarSystemOrigin();
                eye_position = tracked_body_position;
                vector_from_tracked_body_to_camera.x = 0.0;
                vector_from_tracked_body_to_camera.z = distance_from_body_surface;
                vector_from_tracked_body_to_camera = glm::rotate(vector_from_tracked_body_to_camera, M_PI * (x_rotation_degrees / 180.0), glm::dvec3(0.0, 1.0, 0.0));
                eye_position += vector_from_tracked_body_to_camera;

                solar_system_body_shared_ptr.reset();
                PositionCamera();
            }
            else
            {
                solar_system_bodies->erase(solar_system_bodies->begin() + tracked_body_index);
                calculated_min_max_distance_from_body_surface = false;

                std::size_t solar_system_body_count = solar_system_bodies->size();

                if(tracked_body_index >= solar_system_body_count)
                {
                    tracked_body_index = 0u;
                }
            }
        }
        catch(std::out_of_range &ex)
        {
            calculated_min_max_distance_from_body_surface = false;
            tracked_body_index = 0u;
        }
    }
}