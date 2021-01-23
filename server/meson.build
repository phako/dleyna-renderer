renderer_service = executable(
    'dleyna-renderer-service',
    files('daemon.c'),
    dependencies : [
        glib,
        gio,
        dleyna_core,
        renderer_dep,
    ],
    install: true,
    install_dir: join_paths(get_option('prefix'), get_option('libexecdir'))
)

configure_file(
    input: 'com.intel.dleyna-renderer.service.in',
    output: 'com.intel.dleyna-renderer.service',
    configuration: conf,
    install: true,
    install_dir: join_paths(get_option('prefix'), get_option('datadir'), 'dbus-1/services')
)
