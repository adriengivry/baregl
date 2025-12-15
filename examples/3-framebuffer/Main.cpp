/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/Buffer.h>
#include <baregl/Context.h>
#include <baregl/Framebuffer.h>
#include <baregl/Renderbuffer.h>
#include <baregl/ShaderProgram.h>
#include <baregl/ShaderStage.h>
#include <baregl/Texture.h>
#include <baregl/VertexArray.h>
#include <GLFW/glfw3.h>

#include <cassert>

int main(int, char**)
{
	// GLFW setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "3-framebuffer", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Graphics context
	baregl::Context context(true);

	// Triangle data
	const float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// Bottom left
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// Bottom right
		0.0f,  0.5f, 0.0f, 0.5f, 1.0f	// Top center
	};

	const uint32_t indices[] = { 0, 1, 2 };

	// Buffers
	baregl::Buffer vb;
	vb.Allocate(sizeof(vertices), baregl::types::EAccessSpecifier::STATIC_DRAW);
	vb.Upload(vertices);

	baregl::Buffer ib;
	ib.Allocate(sizeof(indices), baregl::types::EAccessSpecifier::STATIC_DRAW);
	ib.Upload(indices);

	baregl::VertexArray va;
	va.SetLayout({
		{ baregl::types::EDataType::FLOAT, 3 },
		{ baregl::types::EDataType::FLOAT, 2 }
	}, vb, ib);

	// Shaders
	baregl::ShaderStage vs(baregl::types::EShaderType::VERTEX);
	vs.Upload(R"(
#version 450 core
layout(location = 0) in vec3 geo_Pos;
layout(location = 1) in vec2 geo_UV;
out vec2 v_UV;
void main() {
	gl_Position = vec4(geo_Pos, 1.0);
	v_UV = geo_UV;
}
)");
	vs.Compile();

	baregl::ShaderStage fs(baregl::types::EShaderType::FRAGMENT);
	fs.Upload(R"(
#version 450 core
in vec2 v_UV;
out vec4 FragColor;
void main() {
	FragColor = vec4(v_UV, 0.5, 1.0);
}
)");
	fs.Compile();

	baregl::ShaderProgram program;
	program.Attach(vs);
	program.Attach(fs);
	program.Link();

	// Texture creation (framebuffer color attachment)
	std::shared_ptr<baregl::Texture> colorBuffer = std::make_shared<baregl::Texture>(
		baregl::types::ETextureType::TEXTURE_2D
	);
	colorBuffer->Allocate(baregl::data::TextureDesc{
		.width = 1,
		.height = 1,
		.internalFormat = baregl::types::EInternalFormat::RGBA32F,
		.useMipMaps = false,
		.mutableDesc = baregl::data::MutableTextureDesc{
			.format = baregl::types::EFormat::RGBA,
			.type = baregl::types::EPixelDataType::FLOAT
		}
	});

	// Renderbuffer creation (framebuffer depth attachment)
	std::shared_ptr<baregl::Renderbuffer> depthBuffer = std::make_shared<baregl::Renderbuffer>();
	depthBuffer->Allocate(1, 1, baregl::types::EInternalFormat::DEPTH_COMPONENT);

	baregl::Framebuffer framebuffer;
	framebuffer.Attach(colorBuffer, baregl::types::EFramebufferAttachment::COLOR);
	framebuffer.Attach(depthBuffer, baregl::types::EFramebufferAttachment::DEPTH);
	assert(framebuffer.Validate());

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Draw to framebuffer
		framebuffer.Bind();
		framebuffer.Resize(width, height);
		context.SetViewport(0, 0, width, height);
		context.Clear(true, true, true);
		program.Bind();
		va.Bind();
		context.DrawElements(baregl::types::EPrimitiveMode::TRIANGLES, 3);
		va.Unbind();
		program.Unbind();
		framebuffer.Unbind();

		// Blit framebuffer to backbuffer
		framebuffer.BlitToBackBuffer(width, height);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
