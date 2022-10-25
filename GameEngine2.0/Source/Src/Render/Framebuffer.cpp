#include "Include/Render/Framebuffer.h"
#include "Include/Resources/ResourceManager.h"

Render::FrameBuffer::FrameBuffer() {}

Render::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &_RBO);
	glDeleteBuffers(1, &_VBO);
	glDeleteVertexArrays(1, &_VAO);
}

void Render::FrameBuffer::Initialize()
{
	this->shader = Resources::ResourceManager::Get<Resources::Shader>("DefaultScreenShader");
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
	};

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Texture.
	Tex = new Resources::Texture();
	Tex->NewTexture("FrameBuffer");
	Resources::ResourceManager::Add("FrameBuffer", Tex);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, Tex->GetDataPtr());
	glBindTexture(GL_TEXTURE_2D, Tex->GetData());

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
	glBindTextureUnit(Tex->GetIndex(), Tex->GetData());

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex->GetData(), 0);
	// Set texture to resourcesManager.
	Tex->Loaded = true;

	glGenRenderbuffers(1, &_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::FrameBuffer::Draw()
{
	// Force Fill Mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(this->ClearColor.Value.x, this->ClearColor.Value.y, this->ClearColor.Value.z, this->ClearColor.Value.w);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader->Program);
	glUniform1i(glGetUniformLocation(shader->Program, "Tex"), Tex->GetIndex());
	glUniform1f(shader->GetLocation(Resources::Location::L_TIME), (float)glfwGetTime());
	glBindVertexArray(_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Tex->GetData());

	if (ImGui::Begin("Scene"))
	{
		auto size = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)static_cast<uintptr_t>(Tex->GetData()), ImVec2(size.x, size.y - 35));
	}
	ImGui::End();

	// Draw The Quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
}