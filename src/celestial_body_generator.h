#ifndef CELESTIAL_BODY_GENERATOR_H
#define CELESTIAL_BODY_GENERATOR_H

#include "core/object_callback_interface.h"

class CelestialBodyGenerator : public ObjectCallbackInterface
{
    private:
        class CelestialBody;

        unsigned int distance_from_near_plane_to_origin;

        static bool seed_created;
        static constexpr size_t maximum_celestial_body_count = 20u;

        double elapsed_time = 0.0;
        double time_for_generating;

        std::size_t celestial_bodies_index;
        std::size_t celestial_body_count;
        std::vector<std::unique_ptr<CelestialBody>> celestial_bodies;

        inline double GenerateRandomDuration(void)
        {
            return static_cast<double>(rand() % 2 + 1);
        }

    public:
        CelestialBodyGenerator(const CelestialBodyGenerator &) = delete;
        CelestialBodyGenerator & operator=(const CelestialBodyGenerator &) = delete;

        CelestialBodyGenerator(unsigned int distance_from_near_plane_to_origin);

        virtual void Do(double frame_swap_time);
};

class CelestialBodyGenerator::CelestialBody : public ObjectCallbackInterface
{
    private:
        enum Relation
        {
            LARGER_THAN_OR_EQUAL_TO,
            LESSER_THAN
        };

        GLUquadric *sphere;

        bool reached_end_position = false;

        unsigned int distance_from_near_plane_to_origin;

        glm::dvec3 start_position;
        glm::dvec3 current_position;
        glm::dvec3 end_position;
        glm::dvec3 units_per_second;
        double radius;

        bool x_relation;
        bool y_relation;
        bool z_relation;

        CelestialBody(const CelestialBody &) = delete;
        CelestialBody & operator=(const CelestialBody &) = delete;

        inline double GenerateX(void)
        {
            //return static_cast<double>((rand() % (distance_from_near_plane_to_origin / 2u * 2u * 10u))) / 10.0 - (static_cast<double>(distance_from_near_plane_to_origin) / 2.0);
            return static_cast<double>((rand() % (distance_from_near_plane_to_origin * 10u))) / 10.0 - (static_cast<double>(distance_from_near_plane_to_origin) / 2.0);
        }

        inline double GenerateY(void)
        {
            //return static_cast<double>((rand() % (distance_from_near_plane_to_origin / 5u * 2u * 10u))) / 10.0 - (static_cast<double>(distance_from_near_plane_to_origin) / 5.0);
            return static_cast<double>((rand() % (distance_from_near_plane_to_origin / 5u * 20u))) / 10.0 - (static_cast<double>(distance_from_near_plane_to_origin) / 5.0);
        }

        inline double GenerateZ(void)
        {
            return GenerateX();
        }

        inline double GenerateSpeed(double start_position, double end_position)
        {
            double direction = end_position >= start_position ? 1.0 : -1.0;

            return static_cast<double>(rand() % 41 + 10) / 100.0 * direction;
        }

        inline double GenerateRadius(void)
        {
            return rand() % 5 + 8;
        }

    public:
        CelestialBody(unsigned int distance_from_near_plane_to_origin);

        bool HasReachedEndPosition(void);

        virtual void Do(double frame_swap_time);
        ~CelestialBody(void);
};

#endif