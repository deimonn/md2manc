project(
    'md2manc', 'c',
    version: '0.1.1',
    license: 'MIT',
    license_files: 'LICENSE',
    meson_version: '>=1.1',
    default_options: [
        'c_std=c99',
        'warning_level=3'
    ]
)

diff = find_program('diff')
md4c = dependency('md4c', version: '>=0.5')
sigtrace = dependency('sigtrace', version: '>=0.1', required: false)

add_project_arguments(
    '-D_POSIX_C_SOURCE=200809L',
    '-D_PROGRAM_VERSION="' + meson.project_version() + '"',
    '-Wvla',
    '-Wno-empty-translation-unit',
    language: 'c'
)

if sigtrace.found()
    add_project_arguments('-D_HAS_SIGTRACE', language: 'c')
endif

sources = files(
    'src/block.c',
    'src/diagnostic.c',
    'src/main.c',
    'src/span.c',
    'src/text.c'
)

headers = files(
    'src/block.h',
    'src/diagnostic.h',
    'src/span.h',
    'src/text.h'
)

entities = custom_target(
    'entities.c',
    command: ['generate_named_entities.py', '@INPUT@'],
    input: 'src/entities.json',
    output: 'entities.c',
    capture: true
)

md2manc = executable(
    'md2manc',
    sources,
    entities,
    dependencies: [md4c, sigtrace],
    install: true
)

date = run_command('date', '+%Y-%m-%d', check: true).stdout().strip()

custom_target(
    'md2manc.1',
    command: [
        md2manc,
        '-d', date,
        '-S', 'md2manc ' + meson.project_version(),
        '@INPUT@'
    ],
    input: 'docs/md2manc.1.md',
    output: 'md2manc.1',
    capture: true,
    install: true,
    install_dir: get_option('mandir') + '/man1'
)

tests = [
    {
        'name': 'basic_conv',
        'in': files('tests/basic_conv.md')[0],
        'out': files('tests/basic_conv.out')[0]
    }
]

foreach entry : tests
    test_output = custom_target(
        entry['name'],
        command: [md2manc, '--no-comment', '@INPUT@'],
        input: entry['in'],
        output: 'test_' + entry['name'] + '.out',
        capture: true
    )

    test(entry['name'] + '_test', diff, args: [test_output, entry['out']])
endforeach

meson.add_postconf_script('compile_flags.py', 'md2manc')

run_target(
	'check-format',
	command: ['sh'] + files('check_format.sh') + sources + headers
)
