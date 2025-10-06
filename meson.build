project('ctbignum', 'cpp',
        default_options : [
          'cpp_std=c++23',
          'default_library=static',
          'warning_level=everything',
          'werror=true',
        ])

cxx = meson.get_compiler('cpp')

add_project_arguments('-fsafe-buffer-usage-suggestions', language: 'cpp')

if cxx.get_id() == 'clang'
  if get_option('warning_level') in ['2', '3', 'everything']
    add_project_arguments(
      '-Wno-unused-parameter',
      language : 'cpp')
  endif
  if get_option('warning_level') == 'everything'
    add_project_arguments(
      '-Wno-c++20-compat',
      '-Wno-c++98-compat',
      '-Wno-c++98-compat-pedantic',
      #'-Wno-cast-function-type-strict',
      #'-Wno-covered-switch-default',
      #'-Wno-ctad-maybe-unsupported',
      #'-Wno-deprecated-copy-with-dtor',
      #'-Wno-documentation',
      #'-Wno-documentation-html',
      #'-Wno-documentation-unknown-command',
      #'-Wno-double-promotion',
      #'-Wno-exit-time-destructors',
      #'-Wno-float-conversion',
      #'-Wno-float-equal',
      #'-Wno-global-constructors',
      #'-Wno-implicit-fallthrough',
      #'-Wno-implicit-int-conversion',
      '-Wno-pre-c++20-compat-pedantic',
      #'-Wno-padded',
      #'-Wno-shadow',
      #'-Wno-shadow-field-in-constructor',
      #'-Wno-shadow-uncaptured-local',
      #'-Wno-shorten-64-to-32',
      #'-Wno-sign-conversion',
      #'-Wno-suggest-destructor-override',
      #'-Wno-switch-default',
      #'-Wno-switch-enum',
      #'-Wno-tautological-unsigned-zero-compare',
      #'-Wno-undefined-func-template',
      #'-Wno-unused-template',
      #'-Wno-unsafe-buffer-usage',
      #'-Wno-weak-vtables',
      #'-Wno-gnu-line-marker',
      language : 'cpp')
  endif

  #add_global_arguments('-ftime-trace', language : 'cpp')
  # for measuring compilation time (currently disabled)

  add_global_arguments('-fconstexpr-steps=9999999', language : 'cpp')
elif cxx.get_id() == 'gcc'
  if get_option('warning_level') in ['0', '1', '2', '3', 'everything']
    add_project_arguments(
      '-Wno-return-type',
      '-Wno-virtual-move-assign',
      language : 'cpp')
  endif
  if get_option('warning_level') in ['1', '2', '3', 'everything']
    add_project_arguments(
      '-Wno-catch-value',
      '-Wno-dangling-pointer',
      '-Wno-dangling-reference',
      '-Wno-maybe-uninitialized',
      language : 'cpp')
  endif
  if get_option('warning_level') in ['2', '3', 'everything']
    add_project_arguments(
      '-Wno-type-limits',
      '-Wno-unused-parameter',
      language : 'cpp')
  endif
  if get_option('warning_level') in ['3', 'everything'] and get_option('optimization') == '3'
    add_project_arguments(
      '-Wno-array-bounds',
      '-Wno-restrict',
      '-Wno-stringop-overflow',
      language : 'cpp')
  endif
  if get_option('warning_level') == 'everything'
    add_project_arguments(
      '-Wno-abi-tag',
      '-Wno-arith-conversion',
      '-Wno-cast-align',
      '-Wno-conversion',
      '-Wno-date-time',
      '-Wno-deprecated-copy-dtor',
      '-Wno-double-promotion',
      '-Wno-effc++',
      '-Wno-float-conversion',
      '-Wno-float-equal',
      '-Wno-inline',
      '-Wno-missing-declarations',
      '-Wno-multiple-inheritance',
      '-Wno-noexcept',
      '-Wno-null-dereference',
      '-Wno-padded',
      '-Wno-redundant-decls',
      '-Wno-redundant-tags',
      '-Wno-shadow',
      '-Wno-sign-promo',
      '-Wno-strict-overflow',
      '-Wno-suggest-attribute=cold',
      '-Wno-suggest-attribute=const',
      '-Wno-suggest-attribute=noreturn',
      '-Wno-suggest-attribute=pure',
      '-Wno-switch-default',
      '-Wno-switch-enum',
      '-Wno-unused-const-variable',
      '-Wno-useless-cast',
      '-Wno-virtual-inheritance',
      language : 'cpp')
  endif
  if get_option('warning_level') == 'everything' and get_option('optimization') == '3'
    add_project_arguments(
      '-Wno-alloc-zero',
      '-Wno-suggest-final-methods',
      '-Wno-suggest-final-types',
      language : 'cpp')
  endif
endif


# Clang's source-based coverage
if get_option('src_based_coverage')
  if get_option('b_coverage')
    error('project option \'src_based_coverage\' is mutually exclusive with \'b_coverage\'')
  endif

  if cxx.get_id() == 'clang'
    add_project_arguments('-fprofile-instr-generate', language : 'cpp')
    add_project_arguments('-fcoverage-mapping', language : 'cpp')
    add_project_link_arguments('-fprofile-instr-generate', language : 'cpp')
    add_project_link_arguments('-fcoverage-mapping', language : 'cpp')
  else
    error('project option \'src_based_coverage\' not supported for compiler \'', cxx.get_id(), '\'')
  endif
endif

ctbignum_includes = include_directories('include')

python_module = import('python')
python = python_module.find_installation('python3')

subdir('test')
