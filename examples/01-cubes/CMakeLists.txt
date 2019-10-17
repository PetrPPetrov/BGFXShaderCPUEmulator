# Copyright (c) 2019 Petr Petrov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

project(01-cubes)

cmake_minimum_required(VERSION 2.8)

include_directories(${01-cubes_SOURCE_DIR})

set(shaders
${01-cubes_SOURCE_DIR}/vs_cubes.sc
${01-cubes_SOURCE_DIR}/fs_cubes.sc
${01-cubes_SOURCE_DIR}/varying.def.sc
)

add_executable(01-cubes
${BGFXShaderEmulation}
${shaders}
${01-cubes_SOURCE_DIR}/main.cpp
)

set_source_files_properties(${BGFXShaderEmulation} PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(${shaders} PROPERTIES HEADER_FILE_ONLY TRUE)
set_target_properties(01-cubes PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
