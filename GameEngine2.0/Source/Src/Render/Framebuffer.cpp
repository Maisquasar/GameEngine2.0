#include "stdafx.h"
#include "Include/Render/Framebuffer.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Utils/Utils.h"

Render::FrameBuffer::FrameBuffer() {
}

Render::FrameBuffer::~FrameBuffer()
{
	if (_VAO != 0)
		glDeleteVertexArrays(1, &_VAO);
	if (_VBO != 0)
		glDeleteBuffers(1, &_VBO);
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &_RBO);
}

void Render::FrameBuffer::Initialize(Math::IVec2 size)
{
	if (!shader)
	{
		this->shader = Application.GetResourceManager()->Get<Resources::Shader>("Assets/Default/Shaders/DefaultScreenShader");
	}
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

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Texture.
	Tex = new Resources::Texture();
	Tex->NewTexture("FrameBuffer");
	auto path = Utils::StringFormat("Framebuffer%d", Tex->GetIndex());
	Tex->SetPath(path);
	Application.GetResourceManager()->Add(path, Tex);
	glActiveTexture(GL_TEXTURE0 + Tex->GetIndex());
	glGenTextures(1, Tex->GetDataPtr());
	glBindTexture(GL_TEXTURE_2D, Tex->GetData());

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
	glBindTextureUnit(Tex->GetIndex(), Tex->GetData());

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex->GetData(), 0);
	// Set texture to resourcesManager.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		PrintLog("Framebuffer %d Complete !", FBO);
	Tex->Loaded = true;

	glGenRenderbuffers(1, &_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y); // use a single render buffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::FrameBuffer::Draw()
{
	// Force Fill Mode.
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers

	
	glUseProgram(shader->Program);
	glUniform1i(shader->GetLocation(Resources::Location::L_TEXTURE), Tex->GetIndex());
	glBindVertexArray(_VAO);
	glActiveTexture(GL_TEXTURE0 + Tex->GetIndex());
	glBindTexture(GL_TEXTURE_2D, Tex->GetData());
	// Draw The Quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
