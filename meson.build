project('ilar', 'cpp',
    version : 'v0.1',
    default_options : ['cpp_std=c++20']
)

sources = [
    'src/main.cpp',
    'src/cmds/create.cpp',
    'src/cmds/extract.cpp',
    'src/cmds/list.cpp',
    'src/tools/compress.cpp'
]

includes = include_directories('src')
conflict_dep = dependency('conflict')

executable('ilar',
    dependencies : [conflict_dep],
    sources : sources,
    include_directories : includes,
    link_args : ['-lboost_iostreams']
)