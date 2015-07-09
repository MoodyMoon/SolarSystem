#include "core/stdafx.h"
#include "main_form.h"

MainForm::MainForm()
{
    int screen_width;
    int screen_height;
    Window::GetScreenResolution(screen_width, screen_height);

    client_width = screen_width / 1.1;
    client_height = screen_height / 1.1;

    int window_x = (screen_width - client_width) / 2;
    int window_y = (screen_height - client_height) / 2;

    window.reset(new Window(window_x, window_y, client_width, client_height, "Solar System", nullptr, this, this, this, &object_callback_interfaces));

    //Anything inserted here will have no effect. Do not insert anything. Do it in WindowEventHandlerInterface::OnInit() instead.
}

void MainForm::InitSolarSystem()
{
    /*
    rotation_inclination = axial tilt
    orbit_inclination = inclination to the sun's equator
    orbit_inclination_vector = longitude of ascending node
    */

    JPEGImage skybox_image("assets/skybox.jpg");
    JPEGImage pluto_image("assets/texture_pluto_fictional.jpg");
    JPEGImage neptune_image("assets/texture_neptune.jpg");
    JPEGImage uranus_image("assets/texture_uranus.jpg");
    JPEGImage saturn_image("assets/texture_saturn.jpg");
    JPEGImage europa_image("assets/texture_jupiter_europa.jpg");
    JPEGImage io_image("assets/texture_jupiter_io.jpg");
    JPEGImage jupiter_image("assets/texture_jupiter.jpg");
    JPEGImage mars_image("assets/texture_mars.jpg");
    JPEGImage moon_image("assets/texture_moon.jpg");
    JPEGImage earth_image("assets/texture_earth_clouds.jpg");
    JPEGImage venus_image("assets/texture_venus_atmosphere.jpg");
    JPEGImage mercury_image("assets/texture_mercury.jpg");
    JPEGImage sun_image("assets/texture_sun.jpg");

    PNGImage artificial_satellite_image("assets/satellite.png");

    GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat light_ambient[] = {0.01f, 0.01f, 0.01f, 1.0f};
	GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	skybox_texture.reset(new GLTexture(skybox_image.GetWidth(), skybox_image.GetHeight(), skybox_image.GetDecodedImageBuffer(), skybox_image.GetColorSpace()));
	skybox.reset(new Skybox(&camera, 1000.0, skybox_texture.get()));

	light.reset(new Light(GL_LIGHT0, light_position, light_ambient, light_diffuse, light_specular));

    glm::dvec3 rotation_inclination_vector(0.0, 0.0, 1.0);

    pluto_texture.reset(new GLTexture(pluto_image.GetWidth(), pluto_image.GetHeight(), pluto_image.GetDecodedImageBuffer(), pluto_image.GetColorSpace()));
    pluto.reset(new SolarSystemBody(0.163, 624.815, 119.591, rotation_inclination_vector, 28.1814, 11.88, glm::dvec3(1.0, 0.0, -0.3696), 248.1, nullptr, pluto_texture.get()));

    neptune_texture.reset(new GLTexture(neptune_image.GetWidth(), neptune_image.GetHeight(), neptune_image.GetDecodedImageBuffer(), neptune_image.GetColorSpace()));
    neptune.reset(new SolarSystemBody(3.264, 523.291, 28.32, rotation_inclination_vector, 268.1365, 6.43, glm::dvec3(1.0, 0.0, -0.8935), 164.8, nullptr, neptune_texture.get()));

    uranus_texture.reset(new GLTexture(uranus_image.GetWidth(), uranus_image.GetHeight(), uranus_image.GetDecodedImageBuffer(), uranus_image.GetColorSpace()));
    uranus.reset(new SolarSystemBody(3.3723, 406.214, 97.77, rotation_inclination_vector, 250.0592, 6.48, glm::dvec3(1.0, 0.0, 0.3249), 84.01, nullptr, uranus_texture.get()));

    saturn_texture.reset(new GLTexture(saturn_image.GetWidth(), saturn_image.GetHeight(), saturn_image.GetDecodedImageBuffer(), saturn_image.GetColorSpace()));
    saturn.reset(new SolarSystemBody(8.3653, 302.521, 26.73, rotation_inclination_vector, 422.5352, 5.51, glm::dvec3(1.0, 0.0, -0.4378), 29.46, nullptr, saturn_texture.get()));

    europa_texture.reset(new GLTexture(europa_image.GetWidth(), europa_image.GetHeight(), europa_image.GetDecodedImageBuffer(), europa_image.GetColorSpace()));
    europa.reset(new SolarSystemBody(0.2243, 14.5079, 4.91, rotation_inclination_vector, 50.6899, 0.47, glm::dvec3(1.0, 0.0, 0.6281), 415.923, nullptr, europa_texture.get()));

    io_texture.reset(new GLTexture(io_image.GetWidth(), io_image.GetHeight(), io_image.GetDecodedImageBuffer(), io_image.GetColorSpace()));
    io.reset(new SolarSystemBody(0.2617, 11.042, 2.21, rotation_inclination_vector, 101.7467, 0.05, glm::dvec3(1.0, 0.0, -0.1821), 400.102, nullptr, io_texture.get()));

    jupiter_satellites = {io, europa};
    jupiter_texture.reset(new GLTexture(jupiter_image.GetWidth(), jupiter_image.GetHeight(), jupiter_image.GetDecodedImageBuffer(), jupiter_image.GetColorSpace()));
    jupiter.reset(new SolarSystemBody(10.2725, 255.904, 3.13, rotation_inclination_vector, 438.9494, 6.09, glm::dvec3(1.0, 0.0, -0.1852), 11.86, &jupiter_satellites, jupiter_texture.get()));

    mars_texture.reset(new GLTexture(mars_image.GetWidth(), mars_image.GetHeight(), mars_image.GetDecodedImageBuffer(), mars_image.GetColorSpace()));
    mars.reset(new SolarSystemBody(0.4856, 216.368, 25.19, rotation_inclination_vector, 175.4386, 5.65, glm::dvec3(1.0, 0.0, 0.8522), 1.881, nullptr, mars_texture.get()));

    moon_texture.reset(new GLTexture(moon_image.GetWidth(), moon_image.GetHeight(), moon_image.GetDecodedImageBuffer(), moon_image.GetColorSpace()));
    moon.reset(new SolarSystemBody(0.2496, 1.8665, 1.5424, rotation_inclination_vector, 6.5882, 19.81, glm::dvec3(1.0, 0.0, 0.3912), 0.0748, nullptr, moon_texture.get()));

    earth_satellites = {moon};
    earth_texture.reset(new GLTexture(earth_image.GetWidth(), earth_image.GetHeight(), earth_image.GetDecodedImageBuffer(), earth_image.GetColorSpace()));
    earth.reset(new SolarSystemBody(0.9154, 210.745, 23.4333, rotation_inclination_vector, 180.0, 7.155, glm::dvec3(-1.0, 0.0, 5.0228), 1.0, &earth_satellites, earth_texture.get()));

    venus_texture.reset(new GLTexture(venus_image.GetWidth(), venus_image.GetHeight(), venus_image.GetDecodedImageBuffer(), venus_image.GetColorSpace()));
    venus.reset(new SolarSystemBody(0.8697, 207.767, 177.36, rotation_inclination_vector, 0.7407, 3.86, glm::dvec3(0.0, 0.0, 0.2368), 0.615, nullptr, venus_texture.get()));

    mercury_texture.reset(new GLTexture(mercury_image.GetWidth(), mercury_image.GetHeight(), mercury_image.GetDecodedImageBuffer(), mercury_image.GetColorSpace()));
    mercury.reset(new SolarSystemBody(0.3495, 204.163, 2.04, rotation_inclination_vector, 3.0693, 3.38, glm::dvec3(1.0, 0.0, 0.9208), 0.2408, nullptr, mercury_texture.get()));

    sun_satellites = {mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto};
    sun_texture.reset(new GLTexture(sun_image.GetWidth(), sun_image.GetHeight(), sun_image.GetDecodedImageBuffer(), sun_image.GetColorSpace()));
    sun.reset(new SolarSystemBody(100.0, 0.0, 0.0, glm::dvec3(0.0, 0.0, 0.0), 7.0922, 0.0, glm::dvec3(0.0, 0.0, 0.0), 0.0, &sun_satellites, sun_texture.get()));

    celestial_body_generator.reset(new CelestialBodyGenerator(1000u));

    solar_system_bodies = {mercury, venus, earth, moon, mars, jupiter, saturn};

    artificial_satellite_texture.reset(new GLTexture(artificial_satellite_image.GetWidth(), artificial_satellite_image.GetHeight(), artificial_satellite_image.GetDecodedImageBuffer(), artificial_satellite_image.GetColorSpace()));

    sprites = {std::shared_ptr<Billboard>(new Billboard(glm::dvec3(150.0, 0.0, 200.0), 20.0, 20.0, &camera, artificial_satellite_texture.get())),
               std::shared_ptr<Billboard>(new Billboard(glm::dvec3(100.0, 70.0, -100.0), 20.0, 20.0, &camera, artificial_satellite_texture.get())),
               std::shared_ptr<Billboard>(new Billboard(glm::dvec3(-200.0, -90.0, -150.0), 20.0, 20.0, &camera, artificial_satellite_texture.get()))};

    object_callback_interfaces = {sun, celestial_body_generator, light, skybox};

    for(sprites_iterator = sprites.begin(), sprites_end_iterator = sprites.end(); sprites_iterator < sprites_end_iterator; ++sprites_iterator)
        object_callback_interfaces.push_back(*sprites_iterator);
}

void MainForm::OnInit(Window *window)
{
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    SetUpCamera(CameraID::FREE_ROAM_CAMERA);

    InitSolarSystem();
}

void MainForm::OnFrameBufferResize(int width, int height)
{
    camera->Reset(width, height);
}

void MainForm::OnMove(double xpos, double ypos)
{
    camera->LookX(xpos);
    camera->LookY(ypos);
}

void MainForm::OnScroll(double xoffset, double yoffset)
{
    int _yoffset = static_cast<int>(yoffset);

    switch(_yoffset)
    {
        case 1:
            camera->ZoomIn();
            break;
        case -1:
            camera->ZoomOut();
            break;
    }
}

void MainForm::OnTrigger(int key, int scancode, int action, int mods)
{
    if(action != GLFW_RELEASE)
    {
        switch(key)
        {
            case GLFW_KEY_1:
                SetUpCamera(CameraID::ORTHOGONAL_CAMERA);
                break;
            case GLFW_KEY_2:
                SetUpCamera(CameraID::FREE_ROAM_CAMERA);
                break;
            case GLFW_KEY_3:
                SetUpCamera(CameraID::TRACKING_CAMERA);
                break;
            case GLFW_KEY_A:
                camera->MoveLeft();
                break;
            case GLFW_KEY_W:
                camera->MoveUp();
                break;
            case GLFW_KEY_D:
                camera->MoveRight();
                break;
            case GLFW_KEY_S:
                camera->MoveDown();
                break;
        }
    }
}

void MainForm::SetUpCamera(CameraID camera_id)
{
    if(current_camera_id != camera_id)
    {
        switch(camera_id)
        {
            case ORTHOGONAL_CAMERA:
                camera.reset();
                camera.reset(new OrthogonalCamera(client_width, client_height));
                break;
            case FREE_ROAM_CAMERA:
                camera.reset();
                camera.reset(new FreeRoamCamera(client_width, client_height));
                break;
            case TRACKING_CAMERA:
                camera.reset();
                camera.reset(new TrackingCamera(&solar_system_bodies, client_width, client_height));
                object_callback_interfaces.insert(object_callback_interfaces.begin(), camera);
                break;
        }

        if(skybox.get() != nullptr)
            skybox->SetCamera(&camera);

        for(sprites_iterator = sprites.begin(), sprites_end_iterator = sprites.end(); sprites_iterator < sprites_end_iterator; ++sprites_iterator)
            (*sprites_iterator)->SetCamera(&camera);

        current_camera_id = camera_id;
    }
}