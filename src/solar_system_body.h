#ifndef SOLAR_SYSTEM_BODY_H
#define SOLAR_SYSTEM_BODY_H

#include "core/gltexture.h"
#include "core/object_callback_interface.h"

class SolarSystemBody : public ObjectCallbackInterface
{
    private:
        GLUquadric *sphere;
        GLTexture *texture;

        GLdouble radius;
        GLdouble distance_from_parent_body = 0.0;

        GLdouble rotation_inclination = 0.0;
        glm::dvec3 rotation_inclination_vector;
        GLdouble rotation_degrees = 0.0;
        GLdouble rotation_degrees_per_second = 0.0;

        GLdouble orbit_inclination = 0.0;
        glm::dvec3 orbit_inclination_vector;
        GLdouble orbit_degrees = 0.0;
        GLdouble orbit_degrees_per_second = 0.0;

        SolarSystemBody *parent_body = nullptr;
        std::vector<std::weak_ptr<SolarSystemBody>> *satellites = nullptr;

        /* Do satellite position calculations here and translate to the result */
        void OrbitSatellite(SolarSystemBody *satellite, double frame_swap_time);

        /* Do rotation here after moving all satellites and do drawing here */
        void Rotate(double frame_swap_time);

        void DrawOrbit(void);

        glm::dvec3 GetDistanceFromSolarSystemOrigin(glm::dvec3 distance_to_satellite);

    public:
        SolarSystemBody(const SolarSystemBody &) = delete;
        SolarSystemBody & operator=(const SolarSystemBody &) = delete;

        SolarSystemBody(GLdouble radius,
                        GLdouble distance_from_parent_body,
                        GLdouble rotation_inclination,
                        const glm::dvec3 &rotation_inclination_vector,
                        GLdouble rotation_degrees_per_second,
                        GLdouble orbit_inclination,
                        const glm::dvec3 &orbit_inclination_vector,
                        GLdouble orbit_degrees_per_second,
                        std::vector<std::weak_ptr<SolarSystemBody>> *satellites,
                        GLTexture *texture);

        virtual void Do(double frame_swap_time);

        glm::dvec3 GetDistanceFromSolarSystemOrigin(void);
        double GetRadius(void);

        ~SolarSystemBody(void);
};

#endif