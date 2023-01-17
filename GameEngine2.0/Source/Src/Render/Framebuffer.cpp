#include "Include/Render/Framebuffer.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Utils/Utils.h"

Render::FrameBuffer::FrameBuffer() {}

Render::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &_RBO);
}

void Render::FrameBuffer::Initialize(Math::IVec2 size)
{
	this->shader = Application.GetResourceManager()->Get<Resources::Shader>("DefaultScreenShader");
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Texture.
	Tex = new Resources::Texture();
	Tex->NewTexture("FrameBuffer");
	auto name = Utils::StringFormat("Framebuffer%d", Tex->GetIndex());
	Application.GetResourceManager()->Add(name, Tex);
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
	// Draw The Quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
