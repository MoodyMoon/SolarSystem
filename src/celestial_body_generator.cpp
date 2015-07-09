#include "core/stdafx.h"
#include "celestial_body_generator.h"

bool CelestialBodyGenerator::seed_created = false;
constexpr size_t CelestialBodyGenerator::maximum_celestial_body_count;

CelestialBodyGenerator::CelestialBodyGenerator(unsigned int distance_from_near_plane_to_origin) : distance_from_near_plane_to_origin(distance_from_near_plane_to_origin)
{
    if(!seed_created)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        seed_created = true;
    }

    time_for_generating = GenerateRandomDuration();
}

void CelestialBodyGenerator::Do(double frame_swap_time)
{
    for(celestial_bodies_index = 0u, celestial_body_count = celestial_bodies.size(); celestial_bodies_index < celestial_body_count;)
    {
        if(celestial_bodies.at(celestial_bodies_index)->HasReachedEndPosition())
        {
            celestial_bodies.erase(celestial_bodies.begin() + celestial_bodies_index);
            --celestial_body_count;
        }
        else
        {
            celestial_bodies.at(celestial_bodies_index)->Do(frame_swap_time);
            ++celestial_bodies_index;
        }
    }

    if(celestial_bodies.empty() || celestial_bodies.size() < maximum_celestial_body_count)
    {
        elapsed_time += frame_swap_time;

        if(elapsed_time > time_for_generating)
        {
            celestial_bodies.push_back(std::unique_ptr<CelestialBody>(new CelestialBody(distance_from_near_plane_to_origin)));
            elapsed_time = 0.0;
            time_for_generating = GenerateRandomDuration();
        }
    }
}

CelestialBodyGenerator::CelestialBody::CelestialBody(unsigned int distance_from_near_plane_to_origin) :
                                                     distance_from_near_plane_to_origin(distance_from_near_plane_to_origin),
                                                     start_position(GenerateX(), GenerateY(), GenerateZ()),
                                                     current_position(start_position),
                                                     end_position(GenerateX(), GenerateY(), GenerateZ()),
                                                     units_per_second(GenerateSpeed(start_position.x, end_position.x), GenerateSpeed(start_position.y, end_position.y), GenerateSpeed(start_position.z, end_position.z)),
                                                     radius(GenerateRadius())
{
    if(end_position.x >= start_position.x)
        x_relation = Relation::LARGER_THAN_OR_EQUAL_TO;
    else
        x_relation = Relation::LESSER_THAN;

    if(end_position.y >= start_position.y)
        y_relation = Relation::LARGER_THAN_OR_EQUAL_TO;
    else
        y_relation = Relation::LESSER_THAN;

    if(end_position.z >= start_position.z)
        z_relation = Relation::LARGER_THAN_OR_EQUAL_TO;
    else
        z_relation = Relation::LESSER_THAN;

    sphere = gluNewQuadric();
}

void CelestialBodyGenerator::CelestialBody::Do(double frame_swap_time)
{
    glPushMatrix();
    glTranslated(current_position.x, current_position.y, current_position.z);
    gluSphere(sphere, radius, 30, 30);
    glPopMatrix();

    if(!reached_end_position)
    {
        switch(x_relation)
        {
            case LARGER_THAN_OR_EQUAL_TO:
            {
                if(end_position.x > current_position.x)
                    current_position.x += units_per_second.x;
                else
                    reached_end_position = true;
                break;
            }
            case LESSER_THAN:
            {
                if(end_position.x < current_position.x)
                    current_position.x += units_per_second.x;
                else
                    reached_end_position = true;
                break;
            }
        }

        switch(y_relation)
        {
            case LARGER_THAN_OR_EQUAL_TO:
            {
                if(end_position.y > current_position.y)
                    current_position.y += units_per_second.y;
                else
                    reached_end_position = true;
                break;
            }
            case LESSER_THAN:
            {
                if(end_position.y < current_position.y)
                    current_position.y += units_per_second.y;
                else
                    reached_end_position = true;
                break;
            }
        }

        switch(z_relation)
        {
            case LARGER_THAN_OR_EQUAL_TO:
            {
                if(end_position.z > current_position.z)
                    current_position.z += units_per_second.z;
                else
                    reached_end_position = true;
                break;
            }
            case LESSER_THAN:
            {
                if(end_position.z < current_position.z)
                    current_position.z += units_per_second.z;
                else
                    reached_end_position = true;
                break;
            }
        }
    }
}

bool CelestialBodyGenerator::CelestialBody::HasReachedEndPosition()
{
    return reached_end_position;
}

CelestialBodyGenerator::CelestialBody::~CelestialBody()
{
    gluDeleteQuadric(sphere);
}