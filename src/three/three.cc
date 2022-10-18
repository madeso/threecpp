#include "three/three.h"

#include <iostream>

#include "three/dependency_sokol.h"
#include "three/dependency_opengl.h"
#include "three/render/opengl_utils.h"


namespace
{

struct SokolApp
{
    three::MakeAppFunction make_app;
    std::unique_ptr<three::App> app;
    three::OpenglStates states;

    void on_init()
    {
        three::opengl_setup(&states);
        three::opengl_set3d(&states);

        // const auto* renderer = glGetString(GL_RENDERER); // get renderer string
        // const auto* version = glGetString(GL_VERSION); // version as a string
        // std::cout << "Renderer: " << renderer << "\n";
        // std::cout << "Version: " << version << "\n";
        app = make_app();
    }

    void on_frame()
    {
        three::opengl_set3d(&states);

        const double dt = sapp_frame_duration();
        app->on_render(static_cast<float>(dt));
    }

    void on_event(const sapp_event& e)
    {
    }

    static void on_static_init(void* userdata)
    {
        SokolApp* app = static_cast<SokolApp*>(userdata);
        app->on_init();
    }
    static void on_static_frame(void* userdata)
    {
        SokolApp* app = static_cast<SokolApp*>(userdata);
        app->on_frame();
    }
    static void on_static_cleanup(void* userdata)
    {
        SokolApp* app = static_cast<SokolApp*>(userdata);
        delete app;
    }
    static void on_static_event(const sapp_event* e, void* userdata)
    {
        SokolApp* app = static_cast<SokolApp*>(userdata);
        app->on_event(*e);
    }
};

}

namespace three
{

sapp_desc run_main(MakeAppFunction make_app)
{
    auto app = std::make_unique<SokolApp>();
    app->make_app = std::move(make_app);

    sapp_desc desc;
    
    desc.user_data = app.get();
    desc.width = 800;
    desc.height = 600;
    desc.init_userdata_cb = SokolApp::on_static_init;
    desc.frame_userdata_cb = SokolApp::on_static_frame;
    desc.cleanup_userdata_cb = SokolApp::on_static_cleanup;
    // desc.event_userdata_cb = SokolApp::on_static_event;

    app.release();
    return desc;
}

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