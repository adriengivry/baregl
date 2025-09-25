# BareGL

*BareGL* is a fast, simple, permissive (MIT licensed), cross-platform **OpenGL 4.5+ wrapper** written in C++20.

It aims to reduce the boilerplate code required to get your modern OpenGL application up and running while enforcing concepts like [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) and making it significantly easier to create buffers, allocate memory, upload data to the GPU, and update the underlying OpenGL state.

## What It's Like
```cpp
// Creating an OpenGL context
baregl::Context context;
context.Init(true);

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
va.SetLayout(
    std::to_array<baregl::data::VertexAttribute>({
        { baregl::types::EDataType::FLOAT, 3 },
        { baregl::types::EDataType::FLOAT, 2 }
    }), vb, ib
);

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
.\gen_proj_win32.bat
.\build\baregl.sln # (optional) open solution in Visual Studio
```

**Linux:**
```powershell
git clone https://github.com/adriengivry/baregl --recurse-submodules
cd ./baregl
./gen_proj_linux.sh
```

## Contributing
All contributions to *BareGL* are welcome — whether it's reporting bugs, suggesting new features, or submitting code improvements.

Feel free to open issues or submit pull requests (PRs) for review. Every bit of help makes a difference!

For more information on contributing, check out [CONTRIBUTING.md](CONTRIBUTING.md).

*By contributing, you agree that your contributions will be licensed under the [MIT License](LICENSE).*


## License
*BareGL* is licensed under the [MIT License](LICENSE).

You are free to use, modify, and distribute this project with proper attribution. See the license file for more details.