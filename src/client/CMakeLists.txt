add_executable(client
    main.cpp
    client_renderer_object_factory.cpp
    clear_screen_view.cpp
    presenter_mapper.cpp
    world/level_shader.cpp
    world/pov_mesh_node_visitor.cpp
    world/level_view.cpp
    world/thing_mesh_node_visitor.cpp
    places/action/action_presenter.cpp
    places/action/action_view.cpp
    application.cpp
    )

target_link_libraries(client
    game
    glew
    )
