#include "pocsag.h"

enum {
    PocsagSubmenuIndexReceive,
    PocsagSubmenuIndexSettings,
};


uint32_t pocsag_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}


void pocsag_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    PocsagApp* app = context;
    UNUSED(app);
    UNUSED(index);
}


PocsagApp* pocsag_app_alloc() {
    PocsagApp* app = malloc(sizeof(PocsagApp));

    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    app->submenu = submenu_alloc();
    submenu_add_item(app->submenu, "Receive", PocsagSubmenuIndexReceive, pocsag_submenu_callback, app);
    submenu_add_item(app->submenu, "Settings", PocsagSubmenuIndexSettings, pocsag_submenu_callback, app);
    view_set_previous_callback(submenu_get_view(app->submenu), pocsag_exit);
    view_dispatcher_add_view(
            app->view_dispatcher, PocsagViewSubmenu, submenu_get_view(app->submenu));

    app->view_id = PocsagViewSubmenu;
    view_dispatcher_switch_to_view(app->view_dispatcher, app->view_id);
    return app;
}


void pocsag_app_free(PocsagApp* app) {
    furi_assert(app);
    view_dispatcher_remove_view(app->view_dispatcher, PocsagViewSubmenu);
    submenu_free(app->submenu);
    furi_record_close(RECORD_GUI);
    app->gui = NULL;
    free(app);
}


int32_t pocsag(void* p) {
    UNUSED(p);

    PocsagApp* app = pocsag_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    pocsag_app_free(app);
    return 0;
}
