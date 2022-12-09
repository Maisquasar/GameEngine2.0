#include "Include/Render/Framebuffer.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"

Render::FrameBuffer::FrameBuffer() {}

Render::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &_RBO);
}

void Render::FrameBuffer::Initialize(Math::Integer2 size)
{
	this->shader = Resources::ResourceManager::Get<Resources::Shader>("DefaultScreenShader");
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Texture.
	Tex = new Resources::Texture();
	Tex->NewTexture("FrameBuffer");
	Resources::ResourceManager::Add("FrameBuffer", Tex);
	glActiveTexture(GL_TEXTURE0);
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
	Tex->Loaded = true;

	glGenRenderbuffers(1, &_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::FrameBuffer::Draw()
{
	// Force Fill Mode.
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(this->ClearColor.Value.x, this->ClearColor.Value.y, this->ClearColor.Value.z, this->ClearColor.Value.w);
	if (ImGui::Begin("Scene", (bool*)true, ImGuiWindowFlags_MenuBar))
	{
		if (!Window)
			Window = ImGui::GetCurrentWindow();


		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Transform")) {
				ImGui::Checkbox("Local", &App::GetSettings()->LocalTransform);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		auto size = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)static_cast<uintptr_t>(Tex->GetData()), ImVec2(size.x - 16, size.y - 60), ImVec2(0, 1), ImVec2(1, 0));
		_size = Math::Vector2(size.x - 16, size.y - 60);
		IsHovered = ImGui::IsItemHovered();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
			UpdateCameraEditor = true;
		if (UpdateCameraEditor && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
			UpdateCameraEditor = false;
	}
	ImGui::End();

	// Draw The Quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Math::Vector2 Render::FrameBuffer::GetSize()
{
	return _size;
}

Math::Vector2 Render::FrameBuffer::GetPos()
{
	if (Window)
		return  Math::Vector2(Window->Pos.x + 7, Window->Pos.y + 46);
	return  Math::Vector2(0, 0);
}

Math::Vector2 Render::FrameBuffer::GetMousePosition()
{
	auto mouse = ImGui::GetMousePos();
	return Math::Vector2(mouse.x, mouse.y) - GetPos();
}
