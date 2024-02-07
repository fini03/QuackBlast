#include "VEEventListenerPhysicsGUI.hpp"

using namespace ve::eventlisteners;

VEEventListenerPhysicsGUI::VEEventListenerPhysicsGUI(std::string name, VPEWorld* physics)
    : VEEventListener(name), m_physics{ physics } {}

VEEventListenerPhysicsGUI::~VEEventListenerPhysicsGUI() {}

void VEEventListenerPhysicsGUI::onDrawOverlay(veEvent event) {
    VESubrender_Nuklear* pSubrender = static_cast<VESubrender_Nuklear*>(getEnginePointer()->getRenderer()->getOverlay());
    if (pSubrender == nullptr) return;

    struct nk_context* ctx = pSubrender->getContext();

    if (!g_gameLost) {
        if (nk_begin(ctx, "", nk_rect(0, 0, 430, 200), NK_WINDOW_BORDER)) {
            char outbuffer[100];
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Colored Eggs:", NK_TEXT_LEFT);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Red: explodes one unit in every dir", NK_TEXT_LEFT);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Blue: explodes whole row", NK_TEXT_LEFT);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Green: explodes whole col", NK_TEXT_LEFT);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Yellow: explodes whole row + col", NK_TEXT_LEFT);
            
            nk_layout_row_dynamic(ctx, 45, 1);
            sprintf(outbuffer, "Time: %004.1lf", g_time);
            nk_label(ctx, outbuffer, NK_TEXT_LEFT);
        }
    } else {
        if (nk_begin(ctx, "", nk_rect(500, 500, 200, 170), NK_WINDOW_BORDER)) {
            nk_layout_row_dynamic(ctx, 45, 1);
            nk_label(ctx, "Game Over", NK_TEXT_LEFT);
        }
    }

    nk_end(ctx);
}
