/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "INCLUDE_GL.h"
#include "INCLUDE_GLUT.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbi_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <chrono>
#include <array>
#include <random>

#include "camera/Camera.h"
#include "shader/ShaderProgram.h"
#include "lighting/PointLight.h"
#include "renderer/LightRenderer.h"
#include "skybox/Skybox.h"
#include "renderer/BasicRenderer.h"
#include "renderer/TextureRenderer.h"
#include "mesh/Shapes.h"
#include "texture/Texture.h"
#include "mesh/Mesh.h"
#include "resources/ResourceManager.h"
#include "math/Math.h"
#include "Util.h"

/// Force nvidia optimus drivers to use GPU rather than IGPU
extern "C"
{
    __attribute((dllexport)) std::uint32_t NvOptimusEnablement = 0x00000001;
};

glm::mat4 ProjectionMat;
float CAMERA_ROTATE_SPEED = 0.2f;
float CAMERA_ZOOM_SPEED = 1.0f;
float CAMERA_MOVE_SPEED = 0.2f;
Camera* camera;

BasicRenderer* basicRenderer;
TextureRenderer* textureRenderer;
LightRenderer* lightRenderer;
Skybox* skybox;

/// Scene objects
RenderInfo* player;

OmniLight* outsideLight;

RenderInfo* house;
math::Box houseBounds {
    glm::vec3(-18.0f, 0.0f, -16.0f),
    glm::vec3(18.0f, 32.0f, 16.0f)
};

RenderInfo* fanBlades;
float FAN_VELOCITY = 10.0f;
float FAN_VEL_MAX = 10.0f;
float FAN_VEL_DELTA = (10.0f / 4.0f) / 10.0f;
RenderInfo* fanLightCap;
OmniLight* fanLight;

RenderInfo* mirror;

OmniLight* fireLight;
static constexpr glm::vec4 FireColor(0.886f, 0.345f, 0.133f, 1.0f);
static constexpr glm::vec3 FireLightBasePos(convBlenderVec_m(glm::vec3(-15.479, 0.44814, 2.1649)));
RenderInfo* fireLamp;

RenderInfo* windowBlinds;
float BLINDS_FOLD_SPEED = 0.2f;

RenderInfo* glassTank;

std::uint32_t MirrorFB;
std::uint32_t MirrorTex;
Camera* mirrorCam;
glm::mat4 MirrorProj;

/// Key register
bool Keys[UCHAR_MAX];

/// Timer Variables
auto lastTime = std::chrono::steady_clock::now();
// Maximum time that a frame can be displayed (~16ms = ~60FPS)
float MaxFrameDuration = 16.6666f;

glm::ivec2 WindowDimensions;
glm::vec4 ViewportDimensions;
constexpr float ASPECT_RATIO = 16.0f/9.0f;
bool cmpf(float a, float b, float e = 0.005f) { return (glm::abs(a-b) > e); }
void render();
void displayCallback();
void winReshapeCallback(int w, int h);
void keyCallback(unsigned char key, int mouseX, int mouseY);
void keyUpCallback(unsigned char key, int mouseX, int mouseY);
void mouseMoveCallback(int x, int y);
void mouseButtonCallback(int button, int state, int x, int y);
void update();
void idleCallback() { update(); }
void timerFunc(int) { update(); glutTimerFunc(1, timerFunc, 0); }
int main(int argc, char* argv[])
{
    std::srand(static_cast<unsigned>(time(0)));

    WindowDimensions = glm::ivec2(1280, 720);
    ViewportDimensions = glm::vec4(0, 0, 1280, 720);

    glutInit(&argc, argv);

#ifdef FREEGLUT
    glutSetOption(GLUT_MULTISAMPLE, 16);
#endif
    glutInitDisplayMode(
#if defined(OSX) || defined(__APPLE__)
        GLUT_3_2_CORE_PROFILE |
#endif
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH |
        GLUT_MULTISAMPLE
            );
    glutInitWindowSize(WindowDimensions.x, WindowDimensions.y);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WindowDimensions.x)/2, (glutGet(GLUT_SCREEN_HEIGHT)-WindowDimensions.y)/2);
    glutCreateWindow("Final Project");

#if !defined(OSX) && !defined(__APPLE__)
    glewInit();
#endif

    glutDisplayFunc(displayCallback);
    glutReshapeFunc(winReshapeCallback);
    glutKeyboardFunc(keyCallback);
    glutKeyboardUpFunc(keyUpCallback);
    glutMotionFunc(mouseMoveCallback);
    glutMouseFunc(mouseButtonCallback);
    //glutIdleFunc(idleCallback);
    glutTimerFunc(1, timerFunc, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, WindowDimensions.x, WindowDimensions.y);

    ProjectionMat = glm::perspective(45.0f, ASPECT_RATIO, 0.1f, 100.0f);
    //camera = new Camera({10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    camera = new Camera({0.0f, 0.0f, 0.0f}, {-0.1f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    camera->moveVertLocked(5.0f);


    /// Create Renderers
    basicRenderer = new BasicRenderer();
    textureRenderer = new TextureRenderer();
    lightRenderer = new LightRenderer();
    lightRenderer->setAmbientLight(glm::vec4(0.1f));

    /// Create skybox
    std::array<const char*, 6> skyboxFaces =
            {
                "resources/skybox/posx.jpg",
                "resources/skybox/negx.jpg",
                "resources/skybox/posy.jpg",
                "resources/skybox/negy.jpg",
                "resources/skybox/posz.jpg",
                "resources/skybox/negz.jpg"
            };
    skybox = new Skybox(skyboxFaces);

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    Shapes::GenCube(vertices, indices);
    ResourceManager::Instance().NewMesh("cube", vertices, indices);

    std::uint32_t HouseAtlas = ResourceManager::Instance().NewTex2D("houseatlas", "resources/TextureAtlas.png", GL_REPEAT,
                                                                    GL_LINEAR, false);

/// "Player"
    std::uint32_t PlayerAtlas = ResourceManager::Instance().NewTex2D("playeratlas", "resources/ghost1.png", GL_REPEAT,
                                                                     GL_LINEAR, false);
    player = &ResourceManager::Instance().NewRenderInfo("campos");
    player->mesh = &ResourceManager::Instance().GetMesh("cube");
    player->texID = HouseAtlas;//PlayerAtlas;
    player->opacity = RenderInfo::Opaque;
    RenderInfo::GenVAO(*player);
    lightRenderer->addObject(player);

/// Light outside window
    outsideLight = new OmniLight(util::convBlenderVec({-7.0f, -18.0f, 5.5f}), 60.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    lightRenderer->addLight(outsideLight);

/// Entire scene
    Mesh& houseMesh = ResourceManager::Instance().NewMesh("house", "resources/House.obj");
    house = &ResourceManager::Instance().NewRenderInfo("house");
    house->mesh = &houseMesh;
    house->texID = HouseAtlas;
    RenderInfo::GenVAO(*house);

    lightRenderer->addObject(house);

/// Rotating Fan Piece
    Mesh& fanBladeMesh = ResourceManager::Instance().NewMesh("ceilingfanblades", "resources/CeilingFanBlades.obj");
    fanBlades = &ResourceManager::Instance().NewRenderInfo("ceilingfanblades");
    fanBlades->mesh = &fanBladeMesh;
    fanBlades->texID = HouseAtlas;
    RenderInfo::GenVAO(*fanBlades);

    fanBlades->trans.setGlobalPosition(util::convBlenderVec({9.0f, 0.0f, 11.5f}));

    lightRenderer->addObject(fanBlades);

/// Fan light
    fanLight = new OmniLight(glm::vec3(0.0f, 0.0f, 0.0f), 40.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    lightRenderer->addLight(fanLight);

    fanLight->setPos(util::convBlenderVec({9.0f, 0.0f, 11.5f}));

/// Mirror
    Mesh& mirrorPlaneMesh = ResourceManager::Instance().NewMesh("mirrorplane", "resources/MirrorPlane.obj");
    mirror = &ResourceManager::Instance().NewRenderInfo("mirrorplane");
    mirror->mesh = &mirrorPlaneMesh;
    RenderInfo::GenVAO(*mirror);

    lightRenderer->addObject(mirror);

    /// Mirror fbo
    glGenTextures(1, &MirrorTex);
    mirror->texID = MirrorTex;
    glBindTexture(GL_TEXTURE_2D, MirrorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowDimensions.x, WindowDimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &MirrorFB);
    glBindFramebuffer(GL_FRAMEBUFFER, MirrorFB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, MirrorTex, 0);

    std::uint32_t MirrorRB;
    glGenRenderbuffers(1, &MirrorRB);
    glBindRenderbuffer(GL_RENDERBUFFER, MirrorRB);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowDimensions.x, WindowDimensions.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MirrorRB);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /// Mirror camera
    mirrorCam = new Camera({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.1f}, {0.0f, 1.0f, 0.0f});

    mirrorCam->setPosition(util::convBlenderVec({0.0f, 15.8f, 4.0f}));

    MirrorProj = glm::perspective(45.0f, ASPECT_RATIO, 0.1f, 100.0f);
    //MirrorProj = glm::ortho(-5.6f, 5.6f, -3.15f, 3.15f, 1.0f, 100.0f);

/// Light in fireplace
    /// Omni light
    fireLight = new OmniLight(
            FireLightBasePos,
            40.0f,
            FireColor,
            FireColor
            );
    lightRenderer->addLight(fireLight);

//    /// Lamp
//    fireLamp = &ResourceManager::Instance().NewRenderInfo("lamp");
//    fireLamp->mesh = &ResourceManager::Instance().GetMesh("cube");
//    RenderInfo::GenVAO(*fireLamp);
//    fireLamp->trans.setLocalPosition(fireLight->getPos());
//    basicRenderer->addObject(fireLamp);

/// Glass tank
    glassTank = &ResourceManager::Instance().NewRenderInfo("glasstank");
    glassTank->mesh = &ResourceManager::Instance().NewMesh("glasstank", "resources/GlassTank.obj");
    glassTank->texID = HouseAtlas;
    glassTank->opacity = RenderInfo::Translucent;
    RenderInfo::GenVAO(*glassTank);

    lightRenderer->addObject(glassTank);

/// Window blinds
    windowBlinds = &ResourceManager::Instance().NewRenderInfo("windowblinds");
    windowBlinds->mesh = &ResourceManager::Instance().NewMesh("windowblinds", "resources/WindowBlinds.obj");
    windowBlinds->texID = HouseAtlas;
    windowBlinds->trans.setGlobalPosition(util::convBlenderVec({-7.0f, -15.9f, 5.6f}));
    RenderInfo::GenVAO(*windowBlinds);
    lightRenderer->addObject(windowBlinds);

    atexit([]()
           {
                delete basicRenderer;
                delete textureRenderer;
                delete lightRenderer;

                Texture::FreeBasicColors();
           });

    glutMainLoop();
}

void render()
{
    player->trans.setLocalPosition(camera->getPosition());
    player->trans.setLocalOrientation(glm::inverse(camera->getOrientation()));

    mirrorCam->setOrientation(math::mirror::reflectOrient(*mirrorCam, *camera));

/// Render

    glm::mat4 PV = ProjectionMat * camera->getViewMatrix();
    glm::mat4 MirrorPV = MirrorProj * mirrorCam->getViewMatrix();

    // Mirror
    glBindFramebuffer(GL_FRAMEBUFFER, MirrorFB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    basicRenderer->render(MirrorPV);
    textureRenderer->render(MirrorPV);
    lightRenderer->render(MirrorPV, mirrorCam->getWorldEyePos(), ViewportDimensions, MirrorFB);
    skybox->render(MirrorProj, mirrorCam->getViewMatrix());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Normal scene
    basicRenderer->render(PV);
    lightRenderer->render(PV, camera->getWorldEyePos(), ViewportDimensions, 0);
    textureRenderer->render(PV);
    skybox->render(ProjectionMat, camera->getViewMatrix());

}

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();

    glutSwapBuffers();
}

void winReshapeCallback(int w, int h)
{
    // Maintain 16:9 aspect ratio
    if(!cmpf((float)w / (float)h, ASPECT_RATIO))
    {
        int x = glm::floor(glm::min((float)w / 16.0f, (float)h / 8.0f));
        glutReshapeWindow(x * 16, x * 9);
    }

    WindowDimensions.x = ViewportDimensions.z = w;
    WindowDimensions.y = ViewportDimensions.w = h;

    glViewport(0, 0, WindowDimensions.x, WindowDimensions.y);
}
void keyCallback(unsigned char key, int mouseX, int mouseY)
{
    if(key == 27)
    {
        //glutExit(); <- doesn't play nice on mac?
        exit(0);
    }
    else if(key >= 0 && key <= UCHAR_MAX)
    {
        Keys[key] = true;
    }
}

void keyUpCallback(unsigned char key, int mouseX, int mouseY)
{
    if(key >= 0 && key <= UCHAR_MAX)
    {
        Keys[key] = false;
    }

    if(key == 'o')
    {
        static bool folded = false;
        if(folded)
        {
            static void (*unfoldBlinds)(int) = [](int steps)
            {
                if(!folded)
                {
                    float stepSpeed = static_cast<float>(steps) * BLINDS_FOLD_SPEED;
                    windowBlinds->trans.setLocalScale({1.0f, stepSpeed / 10.0f, 1.0f});
                    windowBlinds->trans.setLocalPosition({0.0f, 2.0f - (stepSpeed / 5.0f), 0.0f});
                    if(windowBlinds->trans.getLocalScale().y < 1.0f)
                    {
                        glutTimerFunc(MaxFrameDuration, unfoldBlinds, steps+1);
                    }
                }
            };

            glutTimerFunc(MaxFrameDuration, unfoldBlinds, 10);
        }
        else
        {
            static void (*foldBlinds)(int) = [](int steps)
            {
                if(folded)
                {
                    float stepSpeed = static_cast<float>(steps) * BLINDS_FOLD_SPEED;
                    windowBlinds->trans.setLocalScale({1.0f, 1.0f - (stepSpeed / 10.0f), 1.0f});
                    windowBlinds->trans.setLocalPosition({0.0f, stepSpeed / 5.0f, 0.0f});

                    if(windowBlinds->trans.getLocalScale().y > BLINDS_FOLD_SPEED)
                    {
                        glutTimerFunc(MaxFrameDuration, foldBlinds, steps+1);
                    }
                }
            };
            glutTimerFunc(MaxFrameDuration, foldBlinds, 1);

        }
        folded = !folded;
    }
    else if(key == 'f')
    {
        static bool on = true;
        if(on)
        {
            static void (*slowFan)(int) = [](int)
            {
                // Protect against fan being turned on while slowing down
                if(!on)
                {
                    FAN_VELOCITY -= FAN_VEL_DELTA;

                    if(FAN_VELOCITY > 0.0f)
                    {
                        glutTimerFunc(100, slowFan, 0);
                    }
                    else
                    {
                        FAN_VELOCITY = 0.0f;
                    }
                }
            };

            glutTimerFunc(100, slowFan, 0);
        }
        else
        {
            static void (*speedFan)(int) = [](int)
            {
                // Protect against fan being turned off while speeding up
                if(on)
                {
                    FAN_VELOCITY += FAN_VEL_DELTA;

                    if(FAN_VELOCITY < FAN_VEL_MAX)
                    {
                        glutTimerFunc(100, speedFan, 0);
                    }
                    else
                    {
                        FAN_VELOCITY = FAN_VEL_MAX;
                    }
                }
            };

            glutTimerFunc(100, speedFan, 0);
        }

        on = !on;
    }
    else if(key == 'l')
    {
        static bool on = true;
        if(on)
        {
            lightRenderer->removeLight(fanLight);
        }
        else
        {
            lightRenderer->addLight(fanLight);
        }
        on = !on;
    }
    else if(key == 'p')
    {
        static bool on = true;
        if(on)
        {
            lightRenderer->removeLight(fireLight);
            std::cout << "The room grows cold..." << std::endl;
        }
        else
        {
            lightRenderer->addLight(fireLight);
            std::cout << "A pleasant warmth appears" << std::endl;
        }

        on = !on;
    }
}

int lx = 0;
int ly = 0;
void mouseMoveCallback(int x, int y)
{
    // Rotate yaw only around world up
    if(lx != x)
        camera->rotateWorldYaw((WindowDimensions.x/2.0f - (float)x) * -CAMERA_ROTATE_SPEED);
    // Rotate pitch
    if(ly != y)
        // Rotate speed multiplied by aspect ratio to make movement speed consistent
        // TODO: There still is a weird feeling to vertical mouse movement
        camera->rotatePitch((WindowDimensions.y/2.0f - (float)y) * -CAMERA_ROTATE_SPEED*(16.0f/9.0f));


    if(lx != x || ly != y)
    {
        // Reset mouse to center of screen
        glutWarpPointer(WindowDimensions.x / 2, WindowDimensions.y / 2);
    }

    lx = x;
    ly = y;
}
void mouseButtonCallback(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN && button != 3 && button != 4)
    {
        glutWarpPointer(WindowDimensions.x/2, WindowDimensions.y/2);
        glutSetCursor(GLUT_CURSOR_NONE);
    }
    else if(button == 3)
    {
        camera->zoom(CAMERA_ZOOM_SPEED);
    }
    else if(button == 4)
    {
        camera->zoom(-CAMERA_ZOOM_SPEED);
    }
    else
    {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }

}
void update()
{
    // Get current and elapsed time
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastTime).count();
    // If the elapsed time is greater than or equal to the 1 second / calculations per second perform calculations and update screen
    if((float)elapsed >= MaxFrameDuration)
    {
        lastTime = now;

        fanBlades->trans.rotateLocal(FAN_VELOCITY, glm::vec3(0.0f, 1.0f, 0.0f));

        // Fire should flicker only every 4 frames
        static int flickerBuffer = 0;
        if(flickerBuffer > 4)
        {
            static std::random_device rd;
            static std::mt19937 randEngine(rd());
            static std::uniform_real_distribution<float> randLight(0.7f, 1.3f);


            fireLight->setDiffuse(FireColor*randLight(randEngine));


            static std::uniform_real_distribution<float> randPos(0.999f, 1.001f);
            fireLight->setPos(FireLightBasePos*randPos(randEngine));

            flickerBuffer = 0;
        }
        else
        {
            ++flickerBuffer;
        }

        if(Keys['w'] || Keys['W'])
        {
            camera->moveLongLocked(-CAMERA_MOVE_SPEED);
        }
        else if(Keys['s'] || Keys['S'])
        {
            camera->moveLongLocked(CAMERA_MOVE_SPEED);
        }

        if(Keys['a'] || Keys['A'])
        {
            camera->moveLatLocked(CAMERA_MOVE_SPEED);
        }
        else if(Keys['d'] || Keys['D'])
        {
            camera->moveLatLocked(-CAMERA_MOVE_SPEED);
        }

        if(Keys['z'] || Keys['Z'])
        {
            camera->moveVertLocked(CAMERA_MOVE_SPEED);
        }
        else if(Keys['x'] || Keys['X'])
        {
            camera->moveVertLocked(-CAMERA_MOVE_SPEED);
        }

        if(Keys['+'] || Keys['='])
        {
            CAMERA_MOVE_SPEED *= 1.1f;
        }
        else if(Keys['-'] || Keys['_'])
        {
            CAMERA_MOVE_SPEED /= 1.1f;
        }

        // TODO: This should only be checked when the camera is moved
        glm::vec3 camPos = camera->getPosition();
        if(camPos.x < houseBounds.min.x)
        {
            camera->setPosition({camPos.x + CAMERA_MOVE_SPEED, camPos.y, camPos.z});
        }
        else if(camPos.x > houseBounds.max.x)
        {
            camera->setPosition({camPos.x - CAMERA_MOVE_SPEED, camPos.y, camPos.z});
        }

        if(camPos.y < houseBounds.min.y)
        {
            camera->setPosition({camPos.x, camPos.y + CAMERA_MOVE_SPEED, camPos.z});
        }
        else if(camPos.y > houseBounds.max.y)
        {
            camera->setPosition({camPos.x, camPos.y - CAMERA_MOVE_SPEED, camPos.z});
        }

        if(camPos.z < houseBounds.min.z)
        {
            camera->setPosition({camPos.x, camPos.y, camPos.z + CAMERA_MOVE_SPEED});
        }
        else if(camPos.z > houseBounds.max.z)
        {
            camera->setPosition({camPos.x, camPos.y, camPos.z - CAMERA_MOVE_SPEED});
        }

        glutPostRedisplay();
    }

}