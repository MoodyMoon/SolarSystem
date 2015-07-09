#include "core/stdafx.h"
#include "solar_system_body.h"

SolarSystemBody::SolarSystemBody(GLdouble radius,
                                 GLdouble distance_from_parent_body,
                                 GLdouble rotation_inclination,
                                 const glm::dvec3 &rotation_inclination_vector,
                                 GLdouble rotation_degrees_per_second,
                                 GLdouble orbit_inclination,
                                 const glm::dvec3 &orbit_inclination_vector,
                                 GLdouble orbit_degrees_per_second,
                                 std::vector<std::weak_ptr<SolarSystemBody>> *satellites,
                                 GLTexture *texture) :
                                     texture(texture),
                                     radius(radius),
                                     distance_from_parent_body(distance_from_parent_body),
                                     rotation_inclination(rotation_inclination),
                                     rotation_inclination_vector(rotation_inclination_vector),
                                     rotation_degrees_per_second(rotation_degrees_per_second),
                                     orbit_inclination(orbit_inclination),
                                     orbit_inclination_vector(orbit_inclination_vector),
                                     orbit_degrees_per_second(orbit_degrees_per_second),
                                     satellites(satellites)
{
    sphere = gluNewQuadric();

    if(texture != nullptr)
    {
        gluQuadricTexture(sphere, GLU_TRUE);
    }

    if(satellites != nullptr)
    {
        std::weak_ptr<SolarSystemBody> satellite_weak_ptr;
        std::shared_ptr<SolarSystemBody> satellite_shared_ptr;

        std::vector<std::weak_ptr<SolarSystemBody>>::const_iterator satellite_index = satellites->begin();
        std::vector<std::weak_ptr<SolarSystemBody>>::const_iterator satellite_end = satellites->end();

        for(; satellite_index < satellite_end; ++satellite_index)
        {
            satellite_weak_ptr = *satellite_index;

            if(!satellite_weak_ptr.expired())
            {
                satellite_shared_ptr = satellite_weak_ptr.lock();
                satellite_shared_ptr->parent_body = this;
                satellite_shared_ptr.reset();
            }
        }
    }
}

glm::dvec3 SolarSystemBody::GetDistanceFromSolarSystemOrigin()
{
    return GetDistanceFromSolarSystemOrigin(glm::dvec3(0.0, 0.0, 0.0));
}

double SolarSystemBody::GetRadius()
{
    return radius;
}

void SolarSystemBody::OrbitSatellite(SolarSystemBody *satellite, double frame_swap_time)
{
    glPushMatrix();

    glm::dvec3 satellite_position;
    glm::dvec3 satellite_orbit_vector(0.0, 1.0, 0.0);

    /* Do satellite position calculations here and translate to the result */
    satellite_position.x = satellite->distance_from_parent_body;

    if(satellite->orbit_inclination > 0.0)
        glRotated(satellite->orbit_inclination, satellite->orbit_inclination_vector.x, satellite->orbit_inclination_vector.y, satellite->orbit_inclination_vector.z);

    if(satellite->distance_from_parent_body > 0.0)
    {
        satellite->DrawOrbit();
    }

    if(frame_swap_time > 0.0)
    {
        satellite->orbit_degrees += (satellite->orbit_degrees_per_second / (1.0 / frame_swap_time));

        if(satellite->orbit_degrees > 360.0)
            satellite->orbit_degrees -= 360.0;

        satellite_position = glm::rotate(satellite_position, M_PI * (satellite->orbit_degrees / 180.0), glm::dvec3(0.0, 1.0, 0.0));
    }

    glPushMatrix();
    glTranslated(satellite_position.x, satellite_position.y, satellite_position.z);
    satellite->Do(frame_swap_time);
    glPopMatrix();
    glPopMatrix();
}

void SolarSystemBody::Rotate(double frame_swap_time)
{
    glPushMatrix();

    /* Do rotation and drawing here */
    if(frame_swap_time > 0.0)
    {
        rotation_degrees += rotation_degrees_per_second / (1.0 / frame_swap_time);

        if(rotation_degrees >= 360.0f)
        {
            rotation_degrees -= 360.0f;
        }
    }

    if(rotation_inclination > 0.0)
        glRotated(rotation_inclination, rotation_inclination_vector.x, rotation_inclination_vector.y, rotation_inclination_vector.z);

    glRotated(rotation_degrees, 0.0, 1.0, 0.0);
    glRotated(90.0, 1.0, 0.0, 0.0);

    if(texture != nullptr)
    {
        texture->Bind();
    }

    if(distance_from_parent_body > 0.0)
    {
        gluSphere(sphere, radius, 30, 30);
    }
    else
    {
        glDisable(GL_LIGHTING);
        gluSphere(sphere, radius, 30, 30);
        glEnable(GL_LIGHTING);
    }

//    glBegin(GL_TRIANGLES);
//    glColor3d(1.0, 1.0, 0.0);
//    glVertex3d(0.0, radius, 0.0);
//    glColor3d(1.0, 0.0, 1.0);
//    glVertex3d(-radius, -radius, 0.0);
//    glColor3d(0.0, 1.0, 1.0);
//    glVertex3d(radius, -radius, 0.0);
//    glEnd();

    glPopMatrix();
}

void SolarSystemBody::DrawOrbit()
{
    glm::dvec3 point(0.0, 0.0, 0.0);

    const static GLdouble two_pi = M_PI * 2.0;
    const static GLdouble radian_delta = two_pi / 360.0;

    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_LOOP);
    glColor3d(1.0, 1.0, 1.0);

    for(GLdouble orbit_radians = 0.0; orbit_radians < two_pi; orbit_radians += radian_delta)
    {
        point.x = std::cos(orbit_radians) * distance_from_parent_body;
        point.z = std::sin(orbit_radians) * distance_from_parent_body;
        glVertex3d(point.x, point.y, point.z);
    }

    glEnd();
    glEnable(GL_LIGHTING);
}

void SolarSystemBody::Do(double frame_swap_time)
{
    glPushMatrix();

    if(satellites != nullptr)
    {
        std::vector<std::weak_ptr<SolarSystemBody>>::const_iterator satellite_iterator;
        std::vector<std::weak_ptr<SolarSystemBody>>::const_iterator satellite_end_iterator;

        std::weak_ptr<SolarSystemBody> satellite_weak_pointer;
        std::shared_ptr<SolarSystemBody> satellite_shared_pointer;
        SolarSystemBody *satellite;

        satellite_iterator = satellites->begin();
        satellite_end_iterator = satellites->end();

        while(satellite_iterator < satellite_end_iterator)
        {
            satellite_weak_pointer = *satellite_iterator;

            if(satellite_weak_pointer.expired())
            {
                satellites->erase(satellite_iterator);
            }
            else
            {
                satellite_shared_pointer = satellite_weak_pointer.lock();
                satellite = satellite_shared_pointer.get();

                OrbitSatellite(satellite, frame_swap_time);

                satellite_shared_pointer.reset();
            }

            ++satellite_iterator;
        }
    }

    Rotate(frame_swap_time);

    glPopMatrix();
}

glm::dvec3 SolarSystemBody::GetDistanceFromSolarSystemOrigin(glm::dvec3 distance_to_satellite)
{
    if(parent_body == nullptr)
        return distance_to_satellite;

    glm::dvec3 vector_from_parent_body(distance_from_parent_body, 0.0, 0.0);
    vector_from_parent_body = glm::rotate(vector_from_parent_body, M_PI * (orbit_degrees / 180.0), glm::dvec3(0.0, 1.0, 0.0));
    vector_from_parent_body = glm::rotate(vector_from_parent_body, M_PI * (orbit_inclination / 180.0), orbit_inclination_vector);

    distance_to_satellite += vector_from_parent_body;

    return parent_body->GetDistanceFromSolarSystemOrigin(distance_to_satellite);
}

SolarSystemBody::~SolarSystemBody()
{
    gluDeleteQuadric(sphere);

    if(satellites != nullptr)
    {
        std::weak_ptr<SolarSystemBody> satellite_weak_ptr;
        std::shared_ptr<SolarSystemBody> satellite_shared_ptr;

        std::vector<std::weak_ptr<SolarSystemBody>>::const_iterator satellite_index = satellites->begin();
        std::vector<std::weak_ptr<SolarSystemBody>>::const_iterator satellite_end = satellites->end();

        for(; satellite_index < satellite_end; ++satellite_index)
        {
            satellite_weak_ptr = *satellite_index;

            if(!satellite_weak_ptr.expired())
            {
                satellite_shared_ptr = satellite_weak_ptr.lock();
                satellite_shared_ptr->parent_body = nullptr;
                satellite_shared_ptr.reset();
            }
        }
    }
}