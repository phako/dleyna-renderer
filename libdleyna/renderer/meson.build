install_headers(
    files(
        'control-point-renderer.h'
    ),
    subdir: 'dleyna-1.0/libdleyna/renderer'
)

libdleyna_renderer = library(
    'dleyna-renderer-1.0',
    files(
        'async.c',
        'device.c',
        'gasync-task.c',
        'host-service.c',
        'manager.c',
        'server.c',
        'task.c',
        'upnp.c'
    ),
    version: '1.0.3',
    dependencies : [
        glib,
        gio,
        dleyna_core,
        gupnp,
        gupnp_av,
        gupnp_dlna,
        soup,
        math,
        config_h
    ],
    install: true,
    install_dir: library_path,
)

# FIXME: Change the .in file to directly take the values we put in config.h
config_conf = configuration_data()
config_conf.set('with_connector_name', get_option('connector'))
config_conf.set('with_log_type', log_types[get_option('log_type')])
config_conf.set('with_log_level', ','.join(get_option('log_level')))
config_conf.set('never_quit', get_option('never_quit').to_string().to_lower())
configure_file(
    input: 'dleyna-renderer-service.conf.in',
    output: 'dleyna-renderer-service.conf',
    configuration: config_conf,
    install: true,
    install_dir: get_option('sysconfdir')
)

renderer_dep = declare_dependency (
    link_with: libdleyna_renderer,
    include_directories: include_directories('../..')
)

pkg.generate(
    libdleyna_renderer,
    name: 'dleyna-renderer-service-1.0',
    description: 'UPnP & DLNA renderer library',
    version: meson.project_version(),
    requires: ['gupnp-1.2', 'glib-2.0', 'gio-2.0', 'dleyna-core-1.0'],
    install_dir: join_paths(get_option('prefix'), get_option('libdir'), 'pkgconfig')
)

meson.override_dependency('dleyna-renderer-1.0', renderer_dep)
