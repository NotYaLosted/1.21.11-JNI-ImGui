#include "gui.h"

static bool is_init = false;
static bool do_draw = false;
static bool unload_cheat = false;

bool GUI::init(HWND wnd_handle)
{
	if (is_init)
		return true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;

	ImGui_ImplWin32_Init(wnd_handle);
	ImGui_ImplOpenGL3_Init("#version 120");

	ImGui::StyleColorsDark();

	is_init = true;
	do_draw = true;
	unload_cheat = false; 

	return true;
}

void GUI::shutdown()
{
	if (!is_init)
		return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	is_init = false;
	do_draw = false;
}

void GUI::draw()
{
	if (!is_init || !do_draw)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(250, 100));

	ImGui::Begin("JNI 1.21.11 ImGui (opengl)", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("insert to toggle");
	ImGui::Separator();

	float window_width = ImGui::GetWindowWidth();

	ImGui::Dummy(ImVec2(0, 10));
	

	if (ImGui::Button("Unload Cheat", ImVec2(120, 35)))
	{
		unload_cheat = true;
	}

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Unloads the cheat completely");

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::get_is_init()
{
	return is_init;
}

bool GUI::get_do_draw()
{
	return do_draw;
}

void GUI::set_do_draw(bool new_value)
{
	do_draw = new_value;
}

bool GUI::get_unload()
{
	return unload_cheat;
}

void GUI::reset_unload_flag()
{
	unload_cheat = false;
}