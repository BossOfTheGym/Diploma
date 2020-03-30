#include <iostream>
#include <memory>

#include "Components/Camera.h"
#include "Solvers/NumSolvers.h"
#include "Functions/Functions.h"
#include "Simulator/GLWindow.h"
#include "GraphicsEngine/GraphicsEngine.h"

#include "Meshes/SphereMesh.h"
#include "Meshes/SphereMeshBuilder.h"
#include "Meshes/BoxMeshBuilder.h"

#include <glWrap/Framebuffer/Framebuffer.h>
#include <glWrap/Shader/Shader.h>
#include <glWrap/Shader/ShaderProgram.h>
#include <Math/MathLib.h>

using math::Vec2f64;
using math::Vec3f32;
using math::Mat4f32;

class MyWindow : public sim::GLWindow
{
public:
	MyWindow(const context::CreationInfo& info) 
		: GLWindow(nullptr, info, true)
	{
		graphics::ShaderProgramBuilder programBuilder;
		graphics::SimpleShaderLoader   shaderLoader;

		m_vert = shaderLoader.shaderFromFile(gl::ShaderType::Vertex, "assets/shaders/test.vert");
		m_frag = shaderLoader.shaderFromFile(gl::ShaderType::Fragment, "assets/shaders/test.frag");
		m_program = programBuilder.buildProgram("box_program", m_vert, m_frag);
		m_uPVM = m_program.getUniformLocation("uPVM");

		meshes::BoxMeshBuilder sphereBuilder({0});
		m_sphere.reset(sphereBuilder.buildMesh());

		auto size = getWindowSize();
		m_camera.setProjection(glm::perspective(glm::radians(60.0f), 1.0f * size.x / size.y, 0.1f, 100.0f));
		m_last = getCursorPos();
	}


public:
	void render()
	{
		auto fb = gl::Framebuffer::getDefaultFramebuffer();
		fb.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
		fb.clear(gl::ClearMask::ColorDepth);

		gl::state::enable(gl::Capability::DepthTest);
		gl::state::enable(gl::Capability::CullFace);
		gl::state::cullFace(gl::Face::Back);

		m_program.bind();
		m_program.setUniformMat4f32(m_uPVM, m_camera.projMat() * m_camera.viewMat() * glm::scale(Mat4f32(1.0f), Vec3f32(0.7f)));
		auto& drawInfo = m_sphere->getDrawInfo();
		auto& vao      = m_sphere->getVertexArray();
		vao.bind();
		if (m_sphere->usingIndices())
		{
			vao.drawElements(drawInfo);
		}
		else
		{
			vao.drawArrays(drawInfo);
		}

		m_program.unbind();

		swapBuffers();
	}


public:
	virtual void mouseEvent(double xPos, double yPos) override
	{
		m_camera.rotateAround(2 * math::PI * (xPos - m_last.x) / m_info.width);
		m_camera.rotatePitch(2 * math::PI * (yPos - m_last.y) / m_info.height);
		m_last.x = xPos;
		m_last.y = yPos;
	}

	virtual void mouseButtonEvent(int button, int action, int mods) override
	{
		
	}

	virtual void scrollEvent(double xOffset, double yOffset) override
	{
		m_camera.changeDistance(yOffset / 100);
	}

	virtual void keyEvent(int key, int scancode, int action, int mods) override
	{
		
	}


private:
	gl::Shader m_vert;
	gl::Shader m_frag;
	gl::ShaderProgram m_program;

	GLint m_uPVM{};

	std::unique_ptr<mesh::IMesh> m_sphere;

	comp::Camera3rdPerson m_camera;
	Vec2f64 m_last;
};


int main()
{
	context::initialize();

	MyWindow w(
		context::CreationInfo{
			  1600
			, 1000
			, "MyWindow"
			, {
				  {GLFW_DOUBLEBUFFER, GLFW_TRUE}
				, {GLFW_CENTER_CURSOR, GLFW_TRUE}
				, {GLFW_DEPTH_BITS  , 24}
				, {GLFW_STENCIL_BITS, 8}
				, {GLFW_CONTEXT_VERSION_MAJOR, 4}
				, {GLFW_CONTEXT_VERSION_MINOR, 5}
				, {GLFW_VISIBLE  , GL_FALSE}
				, {GLFW_RESIZABLE, GL_FALSE}
			}
		}
	);

	w.showWindow();
	while(!w.shouldClose())
	{
		context::pollEvents();

		w.render();
	}

	return 0;
}