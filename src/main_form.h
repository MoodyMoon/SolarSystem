#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include "core/gltexture.h"
#include "core/jpeg_image.h"
#include "core/png_image.h"
#include "core/window.h"
#include "billboard.h"
#include "camera_interface.h"
#include "celestial_body_generator.h"
#include "free_roam_camera.h"
#include "light.h"
#include "orthogonal_camera.h"
#include "solar_system_body.h"
#include "skybox.h"
#include "tracking_camera.h"

class MainForm : public WindowEventHandlerInterface, public MouseEventHandlerInterface, public KeyboardEventHandlerInterface
{
    private:
        enum CameraID
        {
            ORTHOGONAL_CAMERA,
            FREE_ROAM_CAMERA,
            TRACKING_CAMERA
        };

        CameraID current_camera_id;

        unsigned int client_width;
        unsigned int client_height;

        std::shared_ptr<CameraInterface> camera;

        std::shared_ptr<Light> light;
        std::unique_ptr<GLTexture> skybox_texture;
        std::shared_ptr<Skybox> skybox;

        std::unique_ptr<GLTexture> pluto_texture;
        std::shared_ptr<SolarSystemBody> pluto;

        std::unique_ptr<GLTexture> neptune_texture;
        std::shared_ptr<SolarSystemBody> neptune;

        std::unique_ptr<GLTexture> uranus_texture;
        std::shared_ptr<SolarSystemBody> uranus;

        std::unique_ptr<GLTexture> saturn_texture;
        std::shared_ptr<SolarSystemBody> saturn;

        std::unique_ptr<GLTexture> europa_texture;
        std::shared_ptr<SolarSystemBody> europa;

        std::unique_ptr<GLTexture> io_texture;
        std::shared_ptr<SolarSystemBody> io;

        std::vector<std::weak_ptr<SolarSystemBody>> jupiter_satellites;
        std::unique_ptr<GLTexture> jupiter_texture;
        std::shared_ptr<SolarSystemBody> jupiter;

        std::unique_ptr<GLTexture> mars_texture;
        std::shared_ptr<SolarSystemBody> mars;

        std::unique_ptr<GLTexture> moon_texture;
        std::shared_ptr<SolarSystemBody> moon;

        std::vector<std::weak_ptr<SolarSystemBody>> earth_satellites;
        std::unique_ptr<GLTexture> earth_texture;
        std::shared_ptr<SolarSystemBody> earth;

        std::unique_ptr<GLTexture> venus_texture;
        std::shared_ptr<SolarSystemBody> venus;

        std::unique_ptr<GLTexture> mercury_texture;
        std::shared_ptr<SolarSystemBody> mercury;

        std::vector<std::weak_ptr<SolarSystemBody>> sun_satellites;
        std::unique_ptr<GLTexture> sun_texture;
        std::shared_ptr<SolarSystemBody> sun;

        std::shared_ptr<CelestialBodyGenerator> celestial_body_generator;

        std::unique_ptr<GLTexture> artificial_satellite_texture;

        std::vector<std::shared_ptr<Billboard>>::const_iterator sprites_iterator;
        std::vector<std::shared_ptr<Billboard>>::const_iterator sprites_end_iterator;
        std::vector<std::shared_ptr<Billboard>> sprites;

        std::vector<std::weak_ptr<SolarSystemBody>> solar_system_bodies;
        std::vector<std::weak_ptr<ObjectCallbackInterface>> object_callback_interfaces;

        std::unique_ptr<Window> window;

        void InitSolarSystem(void);
        void LoadTexture(void);
        void SetUpCamera(CameraID camera_id);

    public:
        MainForm(const MainForm &) = delete;
        MainForm & operator=(const MainForm &) = delete;

        MainForm();

        virtual void OnInit(Window *window);
        virtual void OnFrameBufferResize(int width, int height);
        virtual void OnMove(double xpos, double ypos);
        virtual void OnScroll(double xoffset, double yoffset);
        virtual void OnTrigger(int key, int scancode, int action, int mods);
};

#endif