#include <Cipher/CipherUtils.h>
#include <imgui/imgui.h>
#include <misc/Logger.h>

#include "resedit.hpp"
#include "resedit_app.hpp"


typedef void (*func)(bool*);
void Menu(bool *_pOpen);
void Init();
extern "C" void __attribute__((visibility("default"))) InitLate();

extern "C" func __attribute__((visibility ("default"))) Start() {
    Init();
    return &Menu;
}

// UI related functions
void Menu(bool *_pOpen) {
    resedit::ResEditApp* app = resedit::ResEditApp::get_instance();

    if (!app)
        return;

    if (*_pOpen)
    {
        app->set_window_open(true);

        app->pre_render();
        app->render();

        *_pOpen = app->is_window_open();
    }
}

// Called in a later stage of game initialisation
void InitLate() {
    modui::set_screen_density(CipherUtils::get_DeviceInfo().density);
    modui::set_ui_scale(0.8f);
    resedit::ResEditApp::initialize();
}

// Called at the start of the game
void Init() {
    resedit::initialize();
}