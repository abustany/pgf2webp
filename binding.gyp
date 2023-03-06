{
  'targets': [
    {
      'target_name': 'pgf2webp_node',
      'sources': [ 'node.cpp' ],
      'include_dirs': [
        '<!@(node -p "require(\'node-addon-api\').include")'
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'dependencies': [ 'pgf2webp' ],
      'conditions': [
      	[ 'OS=="linux"', {
          'libraries': ['<!@(pkg-config --libs libpgf libwebp)']
        }]
      ]
    },
    {
      # compiling this in a separate target to use c++11 and exceptions
      'target_name': 'pgf2webp',
      'type': 'static_library',
      'sources': [ 'pgf2webp.cpp' ],
      'conditions': [
      	[ 'OS=="linux"', {
          'cflags_cc': [ '-std=c++11', '-Wall', '-Wno-deprecated', '-fexceptions' ],
          'include_dirs+': ['<!@(pkg-config --cflags-only-I libpgf libwebp | sed s/-I//g)'],
        }]
      ]
    }
  ]
}
