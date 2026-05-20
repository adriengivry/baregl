# BareGL

*BareGL* is a fast, simple, permissive (MIT licensed), cross-platform **OpenGL 4.5+ wrapper** written in C++20.

It aims to reduce the boilerplate code required to get your modern OpenGL application up and running while enforcing concepts like [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) and making it significantly easier to create buffers, allocate memory, upload data to the GPU, and update the underlying OpenGL state.

## What It's Like
```cpp
// Creating an OpenGL context
baregl::Context context;

// Creating a vertex buffer
baregl::Buffer vb;
vb.Allocate(sizeof(vertices), baregl::types::EAccessSpecifier::STATIC_DRAW);
vb.Upload(vertices);

// Creating an index buffer
baregl::Buffer ib;
ib.Allocate(sizeof(indices), baregl::types::EAccessSpecifier::STATIC_DRAW);
ib.Upload(indices);

// Defining a vertex layout
baregl::VertexArray va;
va.SetLayout({
    { baregl::types::EDataType::FLOAT, 3 },
    { baregl::types::EDataType::FLOAT, 2 }
}, vb, ib);

// Loading and compiling shaders
baregl::ShaderStage vs(baregl::types::EShaderType::VERTEX);
vs.Upload(vertexShaderCode);
const auto vsCompilationResult = vs.Compile();
assert(vsCompilationResult.success);

baregl::ShaderStage fs(baregl::types::EShaderType::FRAGMENT);
fs.Upload(fragmentShaderCode);
const auto fsCompilationResult = fs.Compile();
assert(fsCompilationResult.success);

// Creating a shader program
baregl::ShaderProgram program;
program.Attach(vs);
program.Attach(fs);
const auto shaderLinkingResult = program.Link();
assert(shaderLinkingResult.success);

// Drawing
context.SetViewport(0, 0, width, height);
context.Clear(true, true, true);
program.Bind();
va.Bind();
context.DrawElements(baregl::types::EPrimitiveMode::TRIANGLES, 3);
va.Unbind();
program.Unbind();
```

## Features
- OpenGL Core 4.5+ (Windows & Linux)
- Modern C++20 code
- Fully documented public headers
- Zero dependencies on OpenGL headers required outside of BareGL (no GL headers are included in public headers)
- Direct State Access (DSA)
- Low overhead
- Easy integration with [CMake](https://cmake.org/)

## Dependencies
| BareGL (library) | Examples |
| - | - |
| [glad](https://github.com/Dav1dde/glad) (included in sources) | [glfw](https://github.com/glfw/glfw) & [glm](https://github.com/g-truc/glm) (included as submodules) |

## CMake Configuration
| Option | Allowed Values | Description |
| - | - | - |
| `BAREGL_GENERATE_EXAMPLES` |  `ON` / `OFF` (default) | Generate example projects (require [glfw](https://github.com/glfw/glfw), and [glm](https://github.com/g-truc/glm) submodules to be initialized) |

## Building Examples
> [!IMPORTANT]
> Example projects depend on [glfw](https://github.com/glfw/glfw) and [glm](https://github.com/g-truc/glm), which are included as Git submodules. Make sure to initialize submodules before generating projects.

**Windows:**
```powershell
git clone https://github.com/adriengivry/baregl --recurse-submodules
cd .\baregl
.\gen_proj_windows.bat
.\build\baregl.sln # (optional) open solution in Visual Studio
```

**Linux:**
```powershell
git clone https://github.com/adriengivry/baregl --recurse-submodules
cd ./baregl
./gen_proj_linux.sh
```

## CMake Quick Start
Create a new C++ project with `glfw`, `glm`, and `baregl` in a few seconds with this quick start `CMakeLists.txt`.

**Example project structure:**
```
my-project/
├── CMakeLists.txt
└── src
    └── Main.cpp (e.g. examples/1-triangle/Main.cpp) 
```

**Example CMakeLists.txt content:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(my-project)

set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS  OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL     OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)

set(GLM_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON  CACHE BOOL "" FORCE)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.3)

FetchContent_Declare(baregl GIT_REPOSITORY https://github.com/adriengivry/baregl.git GIT_TAG origin/main)

FetchContent_MakeAvailable(glfw glm baregl)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(my-project PRIVATE glfw glm baregl)
```

## Contributing
All contributions to *BareGL* are welcome — whether it's reporting bugs, suggesting new features, or submitting code improvements.

Feel free to open issues or submit pull requests (PRs) for review. Every bit of help makes a difference!

For more information on contributing, check out [CONTRIBUTING.md](CONTRIBUTING.md).

*By contributing, you agree that your contributions will be licensed under the [MIT License](LICENSE).*


## License
*BareGL* is licensed under the [MIT License](LICENSE).

You are free to use, modify, and distribute this project with proper attribution. See the license file for more details.
