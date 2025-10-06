catch_includes = include_directories('thirdparty/catch')
catch = library('catch', 'src/tests_main.cpp', include_directories: catch_includes)

# Further below, the LLVM_PROFILE_FILE environment variable is set at various places in order to
# control the file paths to which raw coverage profiles are written to.
# For more information, see https://clang.llvm.org/docs/SourceBasedCodeCoverage.html

if get_option('src_based_coverage')
  llvm_profdata = find_program('llvm-profdata')
  llvm_cov = find_program('llvm-cov')

  # We will make coverage-report build targets for every binary added to this list
  coverage_report_binaries = []
endif


io_exe = executable('io', 'src/unit-io.cpp', link_with:catch, include_directories: [ ctbignum_includes, catch_includes])
if get_option('src_based_coverage')
  envment = {'LLVM_PROFILE_FILE': io_exe.full_path() + '.profdata/%p.profraw'}
  coverage_report_binaries += io_exe
else
  envment = {}
endif
test('io-test', io_exe, env: envment, is_parallel : true)

if get_option('src_based_coverage')

  # Per-binary targets for generating a (per-binary) coverage report
  # This target should be called after running the (instrumented) binary
  report_tgts = []
  foreach binary : coverage_report_binaries
    report_tgts += run_target(
      binary.name() + '_cov_report',
    command : [python, '@SOURCE_ROOT@' / 'prepare-code-coverage-artifact.py', llvm_profdata.full_path(), llvm_cov.full_path(), binary.full_path() + '.profdata', '@BUILD_ROOT@' / 'separate_coverage_reports', binary.full_path(), '--preserve-profiles'],
    )
  endforeach

  # Helper targets for copying the coverage-profiling data to a common directory for generating the combined coverage report
  copy_tgts = []
  foreach binary : coverage_report_binaries
    copy_tgts += run_target(
      binary.name() + '_copyfiles',
      command : [python, '@SOURCE_ROOT@' / 'copy_profdata_file.py', binary.full_path() + '.profdata', '@BUILD_ROOT@' / 'coverage_profdata'],
    )
  endforeach


  # Helper target for generating the combined coverage report
  combined_report_impl = run_target(
    'combined_cov_report_impl',
    command : [python, '../prepare-code-coverage-artifact.py', llvm_profdata.full_path(), llvm_cov.full_path(), '@BUILD_ROOT@' / 'coverage_profdata', '@BUILD_ROOT@' / 'combined_coverage_report', coverage_report_binaries, '--unified-report'],
  )


  # Alias target to generate per-binary HTML coverage reports
  separate_cov_reports = alias_target(
      'separate_cov_reports',
      report_tgts
  )


  # Alias target to generate one combined HTML coverage report
  combined_cov_report = alias_target(
      'combined_cov_report',
      report_tgts + copy_tgts + combined_report_impl
  )

endif
