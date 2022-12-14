#include "three/three.h"
#include "three/dependency_glad.h"
#include "three/render/opengl_utils.h"

namespace three
{

int run_main(MakeAppFunction make_app)
{
    constexpr int start_width = 800;
    constexpr int starth_height = 600;

    constexpr Uint32 flags = SDL_INIT_VIDEO;
    if(SDL_Init(flags) != 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    auto* sdl_window = SDL_CreateWindow
    (
        "three",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        start_width,
        starth_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if(sdl_window == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
        return -1;
    }

    auto sdl_glcontext = SDL_GL_CreateContext(sdl_window);

    if(sdl_glcontext == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());

        SDL_DestroyWindow(sdl_window);
        sdl_window = nullptr;

        return -1;
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize OpenGL context");

        SDL_GL_DeleteContext(sdl_glcontext);
        sdl_glcontext = nullptr;

        SDL_DestroyWindow(sdl_window);
        sdl_window = nullptr;

        return -1;
    }

    OpenglStates states;
    opengl_setup(&states);

    bool running = true;
    auto app = make_app();

    int window_width = start_width;
    int window_height = starth_height;

    glClearColor(0, 0, 0, 1.0f);

    auto last = SDL_GetPerformanceCounter();
    while(running)
    {
        const auto now = SDL_GetPerformanceCounter();
        const auto diff = static_cast<float>(now - last);
        const auto freq = static_cast<float>(SDL_GetPerformanceFrequency());
        const auto dt = diff / freq;
        last = now;

        // handle events
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
            case SDL_WINDOWEVENT:
                switch(e.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        window_width = e.window.data1;
                        window_height = e.window.data2;
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        running = false;
                        break;
                }
                break;
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }

        // update
        // render
        glViewport(0,0, window_width, window_height);
        glClear(GL_COLOR_BUFFER_BIT);
        opengl_set3d(&states);

        app->on_render(dt);

        SDL_GL_SwapWindow(sdl_window);
    }

    SDL_GL_DeleteContext(sdl_glcontext);
    sdl_glcontext = nullptr;
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    return 0;
}

// move to member function of app to access current window size
void render(const Scene& scene, const Camera& camera)
{
}

GeomPtr make_BoxGeometry(float boxWidth, float boxHeight, float boxDepth)
{
    return std::make_shared<CompiledGeom>();
}

MaterialPtr make_MeshBasicMaterial()
{
    return std::make_shared<Material>();
}

MeshPtr make_Mesh(GeomPtr geom, MaterialPtr material)
{
    auto mesh = std::make_shared<Mesh>();
    mesh->geom = geom;
    mesh->material = material;
    return mesh;
}


glm::vec3 color_from_hex(int hex)
{
    return {0, 0, 0};
}
        

void
Scene::add(MeshPtr mesh)
{
    meshes.emplace_back(mesh);
}


}

