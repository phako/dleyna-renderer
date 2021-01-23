xsltproc = find_program('xsltproc')

xsltproc_flags = [
    '--nonet',
    '--stringparam', 'man.output.quietly', '1',
    '--stringparam', 'funcsynopsis.style', 'ansi',
    '--stringparam', 'man.authors.section.enabled', '0',
    '--stringparam', 'man.copyright.section.enabled', '0',
]

stylesheet = 'http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl'

xsltproc_args = [
    xsltproc,
    xsltproc_flags,
    '-o', '@OUTPUT@',
    stylesheet,
    '@INPUT@',
]

# Check for stylesheets
r = run_command(
    xsltproc,
    xsltproc_flags,
    '--noout',
    stylesheet,
    'dleyna-renderer-service.conf.xml'
)

if r.returncode() != 0
    error('Cannot build man pages, failed to run xsltproc. Stylesheets missing?')
endif

custom_target(
    'man 5 pages',
    input: 'dleyna-renderer-service.conf.xml',
    output: 'dleyna-renderer-service.conf.5',
    command: xsltproc_args,
    install: true,
    install_dir: join_paths(get_option('mandir'), 'man5')
)
