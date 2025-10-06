#!/usr/bin/env python3

# This is a script, to be called from meson, for copying Clang's source-based profiler data.
# The idea is that two kinds of coverage reports are generated: a separate report per
# binary (where this binary is typically a unit test, or the vdlserver binary),
# and a combined coverage report. We want to generate both kinds of reports based on the same
# profiling data. Hence, we first generate the per-binary reports, from data that is stored in a
# prof-data directory per binary. Next, we copy (using this script), all .profraw files to a common
# directory (named 'coverage_profdata') and generate the combined report.

import pathlib
import shutil
import sys

input_path = pathlib.Path(sys.argv[1])
output_path = pathlib.Path(sys.argv[2])

# make sure destination directory exists
output_path.mkdir(exist_ok=True)

for file in input_path.glob("*.profraw"):
    shutil.copy(file, output_path)
