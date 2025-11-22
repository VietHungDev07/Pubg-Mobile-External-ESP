#include "Functions.h"
#include "Overlay.h"
#include <vector>
#include <d3d9types.h>
#include <thread>
#include "offset.hpp"
#include "Tools.h"
#include "SDKGame.h"
#include <algorithm>
D3DMATRIX ViewMatrix;
using namespace std;

namespace DirectX9Interface {
	IDirect3D9Ex* Direct3D9 = NULL;
	IDirect3DDevice9Ex* pDevice = NULL;
	D3DPRESENT_PARAMETERS pParams = { NULL };
	MARGINS Margin = { -1 };
	MSG Message = { NULL };
}



void Draw() {
	char FpsInfo[64];
	sprintf(FpsInfo, "Overlay FPS: %0.f", ImGui::GetIO().Framerate);
	RGBA White = { 255,255,255,255 };
	DrawStrokeText(30, 44, &White, FpsInfo);
}

void thread_aa()
{
	while (true)
	{


		Data::Ue4Base = ReadMemoryEx<UINT>(Offset::PointerUe4);
		Data::Gnames = ReadMemoryEx<DWORD>(Data::Ue4Base + Offset::Gnames) + 0x88;
		Data::Gworld = ReadMemoryEx<DWORD>(Data::Ue4Base + Offset::Gword) + 0x3C;
		Data::ViewMatrixBase = Data::Ue4Base + Offset::VIEWMATRIX;
		DWORD temp1 = ReadMemoryEx<DWORD>(Data::ViewMatrixBase);
		DWORD temp2 = ReadMemoryEx<DWORD>(temp1 + 0x20);
		ViewMatrix = ReadMemoryEx<D3DMATRIX>(temp2 + 0x200);
		Data::Uworld = ReadMemoryEx<DWORD>(Data::Gworld);
		if (Data::Uworld != 0)
		{
			Data::NetDriver = ReadMemoryEx<DWORD>(Data::Uworld + Offset::NetDriver);
			if (Data::NetDriver > 0)
			{
				Data::PersistentLevel = ReadMemoryEx<DWORD>(Data::Uworld + Offset::PersistentLevel);
				Data::ServerConnection = ReadMemoryEx<DWORD>(Data::NetDriver + Offset::ServerConnection);
				Data::PlayerController = ReadMemoryEx<DWORD>(Data::ServerConnection + Offset::PlayerController);
				Data::LocalPlayer = ReadMemoryEx<DWORD>(Data::PlayerController + Offset::AcknowledgedPawn);
				Data::LocalPlayerAddr = ReadMemoryEx<uint32_t>(Data::PlayerController + Offset::STExtraBaseCharacter);
				Data::MyTeamID = ReadMemoryEx<INT>(Data::LocalPlayerAddr + Offset::TeamId);
				Data::MyLocalRoot = ReadMemoryEx<DWORD>(Data::LocalPlayerAddr + Offset::RootComponent);
				Data::MyLocalPosition = ReadMemoryEx<VECTOR3>(Data::MyLocalRoot + Offset::Position);
				Data::PlayerCameraManager = ReadMemoryEx<DWORD>(Data::PlayerController + Offset::PlayerCameraManager);
				Data::CameraCache = ReadMemoryEx<CameraCacheEntry>(Data::PlayerCameraManager + Offset::CameraCache);

			}
		}
		Sleep(10);
	}
}

void ThreadGetTrueEntity()
{

	while (true)
	{

		if (Data::Uworld != 0)
		{

			if (Data::NetDriver <= 0)
			{
				continue;

			}


			if (Data::PersistentLevel != 0)
			{

				auto ActorsPointerAddress = DecryptActorsArrayZakaria(Data::PersistentLevel, Offset::EntityList, Offset::ActorsClass);


				DWORD ActorArray = ReadMemoryEx<uint32_t>(ActorsPointerAddress);
				int ActorArrayCount = ReadMemoryEx<int>(ActorsPointerAddress + 0x4);



				if (ActorArrayCount <= 0)
				{
					//dbug
					continue;

				}

				for (int i = 0; i < ActorArrayCount; i++)
				{
					DWORD EntityAddress = ReadMemoryEx<DWORD>(ActorArray + i * 4);
					if (!EntityAddress || EntityAddress == (DWORD)nullptr || EntityAddress == -1)
					{
						continue;
					}

					DWORD EntityStruct = ReadMemoryEx<DWORD>(EntityAddress + 0x10);
					if (EntityStruct == 0 || EntityStruct < 0)
					{
						continue;
					}

					std::string EntityType = GetEntities(Data::Gnames, EntityStruct);
					if (EntityType.empty() || EntityType == " " || EntityType == "None")
					{
						continue;
					}


					if (IsPlayer(EntityType))
					{

						Data::EntityList.push_back(EntityAddress);
					}

				}

				Sleep(1000);
				Data::EntityList.clear();
			}
		}
	}

}
ImColor GetHealthColor(float percentage) {
	if (percentage > 70) return ImColor(0, 255, 0, 255);   
	if (percentage > 30) return ImColor(255, 255, 0, 255); 
	return ImColor(255, 0, 0, 255);                        
}
ImColor RandomColor()
{
	return ImColor(
		rand() % 256,      
		rand() % 256,      
		rand() % 256,      
		255                
	);
}
ImColor Randomcol;
void threadRandomColor()
{
	while (true)
	{
		if (Setting::LGBTMode)
		{
			Randomcol = RandomColor();
		}
		Sleep(100);
	}
}
void ThreadESPMain()
{

	vector<INT> TeamIDList = {};


	vector<ImColor> TeamColors = {
		ImColor(255, 0, 0, 255),    
		ImColor(0, 255, 0, 255),    
		ImColor(0, 0, 255, 255),    
		ImColor(255, 255, 0, 255),  
		ImColor(255, 0, 255, 255),  
		ImColor(0, 255, 255, 255)   
	};


	for (int i = 0; i < Data::EntityList.size(); i++)
	{
		INT TeamID = ReadMemoryEx<INT>(Data::EntityList[i] + Offset::TeamId);

		if (std::find(TeamIDList.begin(), TeamIDList.end(), TeamID) == TeamIDList.end()
			&& TeamID != Data::MyTeamID &&
			TeamID > 0)
		{
			TeamIDList.push_back(TeamID);
		}

		bool isDead = ReadMemoryEx<bool>(Data::EntityList[i] + Offset::isDead);

		if (isDead || Data::EntityList[i] == Data::LocalPlayerAddr || TeamID == Data::MyTeamID)
		{

			continue;
		}

		Data::RootComponent = ReadMemoryEx<DWORD>(Data::EntityList[i] + Offset::RootComponent);
		VECTOR3 Postion = ReadMemoryEx<VECTOR3>(Data::RootComponent + Offset::Position);
		ImColor entityColor = ImColor(255, 255, 255, 255);
		if (Setting::LGBTMode == false)
		{

			auto it = std::find(TeamIDList.begin(), TeamIDList.end(), TeamID);
			if (it != TeamIDList.end())
			{
				int colorIndex = std::distance(TeamIDList.begin(), it) % TeamColors.size();
				entityColor = TeamColors[colorIndex];
			}
		}

		if (Setting::Sekeleton || Setting::Box || Setting::Health || Setting::pName)
		{
			VECTOR3 aHeadPos;
			DWORD MeshAddv = ReadMemoryEx<DWORD>(Data::EntityList[i] + Offset::Mesh);
			if (!MeshAddv)continue;
			DWORD BodyAddv = MeshAddv + Offset::BodyAddv;
			if (!BodyAddv)continue;
			DWORD BoneAddv = ReadMemoryEx<DWORD>(MeshAddv + Offset::MinLOD) + 48;
			if (!BoneAddv)continue;

			if (BoneAddv)
			{
				aHeadPos = ::GetBoneWorldPosition(BodyAddv, BoneAddv + BoneIndex::Head * 48);
				aHeadPos.Z += 7;
			}
			VECTOR3 HeadPos;
			VECTOR3 Head;
			HeadPos = aHeadPos;
			HeadPos.Z += 7;
			if (!WorldToScreenWithoutMatrix(HeadPos, Head)) continue;
			VECTOR3 LEFTFOOT;
			VECTOR3 RIGHTFOOT;

			if (!WorldToScreenWithoutMatrix(GetBoneWorldPosition(BodyAddv, BoneAddv + LeftAnkle * 48), LEFTFOOT)) continue;
			if (!WorldToScreenWithoutMatrix(GetBoneWorldPosition(BodyAddv, BoneAddv + RightAnkle * 48), RIGHTFOOT)) continue;


			if (Setting::Sekeleton)
			{

				VECTOR3 TempBone1;
				VECTOR3 TempBone2;

				for (auto& P : BonePairs)
				{


					if (!WorldToScreenWithoutMatrix(GetBoneWorldPosition(BodyAddv, BoneAddv + P.first * 48), TempBone1)) continue;
					if (!WorldToScreenWithoutMatrix(GetBoneWorldPosition(BodyAddv, BoneAddv + P.second * 48), TempBone2)) continue;

					if (Setting::LGBTMode)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(TempBone1.X, TempBone1.Y), ImVec2(TempBone2.X, TempBone2.Y), Randomcol, 1.0f);
					}
					else
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(TempBone1.X, TempBone1.Y), ImVec2(TempBone2.X, TempBone2.Y), entityColor, 1.0f);
					}
				
					TempBone1 = { 0,0,0 };
					TempBone2 = { 0,0,0 };


				}
			}

			if (Setting::Box)
			{
			
				if (!isfinite(Head.X) || !isfinite(Head.Y) || !isfinite(LEFTFOOT.X) || !isfinite(LEFTFOOT.Y) || !isfinite(RIGHTFOOT.X) || !isfinite(RIGHTFOOT.Y))
				{
					continue;
				}

				float height = abs(Head.Y - (LEFTFOOT.Y + RIGHTFOOT.Y) / 2.0f);
				float width = height * 0.6f;

			
				if (height <= 0 || width <= 0 || height > 5000 || width > 5000)
				{
					continue;
				}

				float headMargin = height * 0.1f;
				float footMargin = height * 0.05f;
				float leftOffset = width * 0.1f;

				float x = (LEFTFOOT.X + RIGHTFOOT.X) / 2.0f - width / 2.0f - leftOffset;
				float y = Head.Y - headMargin;
				float boxHeight = height + headMargin + footMargin;

		
				if (!isfinite(x) || !isfinite(y) || !isfinite(boxHeight) ||
					x < -10000 || x > 10000 || y < -10000 || y > 10000)
				{
					continue;
				}
				if (!Setting::LGBTMode)
				{
					ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
					draw_list->AddRect(
						ImVec2(x, y),
						ImVec2(x + width, y + boxHeight),
						entityColor
					);
				}
				else
				{
					ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
					draw_list->AddRect(
						ImVec2(x, y),
						ImVec2(x + width, y + boxHeight),
						Randomcol
					);
				}
				
			}

			if (Setting::Health)
			{
			
				
					float Health = ReadMemoryEx<float>(Data::EntityList[i] + Offset::Health);
					float HealthMax = ReadMemoryEx<float>(Data::EntityList[i] + Offset::HealthMax);
					if (Health <= 0 || HealthMax <= 0 || Health > HealthMax || !isfinite(Health) || !isfinite(HealthMax))
					{
						continue;
					}

					float healthPercentage = (Health / HealthMax) * 100.0f;

		
					float height = abs(Head.Y - (LEFTFOOT.Y + RIGHTFOOT.Y) / 2.0f);
					float width = height * 0.6f;

			
					if (height <= 0 || width <= 0 || height > 5000 || width > 5000)
					{
						continue;
					}

					float headMargin = height * 0.1f;
					float footMargin = height * 0.05f;
					float leftOffset = width * 0.1f;

					float boxX = (LEFTFOOT.X + RIGHTFOOT.X) / 2.0f - width / 2.0f - leftOffset;
					float boxY = Head.Y - headMargin;
					float boxHeight = height + headMargin + footMargin;

			
					if (!isfinite(boxX) || !isfinite(boxY) || !isfinite(boxHeight) ||
						boxX < -10000 || boxX > 10000 || boxY < -10000 || boxY > 10000)
					{
						continue;
					}

			
					float healthBarWidth = 4.0f;
					float healthBarHeight = boxHeight;
					float healthBarX = boxX - healthBarWidth - 2.0f;
					float healthBarY = boxY;

			
					if (!isfinite(healthBarX) || !isfinite(healthBarY) ||
						healthBarX < -10000 || healthBarX > 10000 || healthBarY < -10000 || healthBarY > 10000)
					{
						continue;
					}

					float healthFillHeight = (healthPercentage / 100.0f) * healthBarHeight;

			
					ImGui::GetBackgroundDrawList()->AddRectFilled(
						ImVec2(healthBarX, healthBarY),
						ImVec2(healthBarX + healthBarWidth, healthBarY + healthBarHeight),
						ImColor(50, 50, 50, 255)
					);

	
					ImGui::GetBackgroundDrawList()->AddRectFilled(
						ImVec2(healthBarX, healthBarY + (healthBarHeight - healthFillHeight)),
						ImVec2(healthBarX + healthBarWidth, healthBarY + healthBarHeight),
						GetHealthColor(healthPercentage)
					);

					ImGui::GetBackgroundDrawList()->AddRect(
						ImVec2(healthBarX, healthBarY),
						ImVec2(healthBarX + healthBarWidth, healthBarY + healthBarHeight),
						ImColor(0, 0, 0, 255)
					);
				
			}

			if (Setting::Distance)
			{
				int distance = Distance3D(Data::MyLocalPosition, Postion)/100.0f;

				float centerX = (LEFTFOOT.X + RIGHTFOOT.X) / 2.0f;
				float centerY = max(LEFTFOOT.Y, RIGHTFOOT.Y) + 5.0f;

				string distanceText = std::to_string(distance) + "m";
				ImVec2 textSize = ImGui::CalcTextSize(distanceText.c_str());
				if (!Setting::LGBTMode)
				{
					ImGui::GetBackgroundDrawList()->AddText(
						ImVec2(centerX - textSize.x / 2.0f, centerY),
						entityColor,
						distanceText.c_str()
					);
				}
				else
				{
					ImGui::GetBackgroundDrawList()->AddText(
						ImVec2(centerX - textSize.x / 2.0f, centerY),
						Randomcol,
						distanceText.c_str()
					);
				}
				

			}

			if (Setting::pName)
			{
				DWORD nameAddress = ReadMemoryEx<DWORD>(Data::EntityList[i] + Offset::Name);

			
				if (nameAddress && nameAddress > 0x10000 && nameAddress < 0x7FFFFFFFFFF)
				{
					wchar_t buffer[64] = { 0 };
					SIZE_T bytesRead = 0;

					if (ReadProcessMemory(Process::GameHandle,
						(LPVOID)nameAddress,
						buffer,
						sizeof(buffer) - sizeof(wchar_t),
						&bytesRead) && bytesRead > 0)
					{
		
						buffer[63] = L'\0';

						wstring Name = buffer;

						if (!Name.empty())
						{
			
							int bufferSize = WideCharToMultiByte(CP_UTF8, 0, Name.c_str(), -1, nullptr, 0, nullptr, nullptr);
							if (bufferSize > 0)
							{
								std::vector<char> multiByteBuffer(bufferSize);
								WideCharToMultiByte(CP_UTF8, 0, Name.c_str(), -1, multiByteBuffer.data(), bufferSize, nullptr, nullptr);
								string playerName = multiByteBuffer.data();

								ImVec2 textSize = ImGui::CalcTextSize(playerName.c_str());
								if (!Setting::LGBTMode)
								{
									ImGui::GetBackgroundDrawList()->AddText(
										ImVec2(Head.X - textSize.x / 2.0f, Head.Y - textSize.y - 5.0f),
										entityColor,
										playerName.c_str()
									);
								}
								else
								{
									ImGui::GetBackgroundDrawList()->AddText(
										ImVec2(Head.X - textSize.x / 2.0f, Head.Y - textSize.y - 5.0f),
										Randomcol,
										playerName.c_str()
									);
								}
							}
						}
					}
				}
			}
		}
	}
}


void Render() {
	if (GetAsyncKeyState(VK_INSERT) & 1) ShowMenu = !ShowMenu;
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.WindowRounding = 11.5f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
	style.FrameRounding = 11.89999961853027f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
	style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
	style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
	style.IndentSpacing = 0.0f;
	style.ColumnsMinSpacing = 4.900000095367432f;
	style.ScrollbarSize = 11.60000038146973f;
	style.ScrollbarRounding = 15.89999961853027f;
	style.GrabMinSize = 3.700000047683716f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09250493347644806f, 0.100297249853611f, 0.1158798336982727f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1120669096708298f, 0.1262156516313553f, 0.1545064449310303f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.971993625164032f, 1.0f, 0.4980392456054688f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7953379154205322f, 0.4980392456054688f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1821731775999069f, 0.1897992044687271f, 0.1974248886108398f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1545050293207169f, 0.1545048952102661f, 0.1545064449310303f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1414651423692703f, 0.1629818230867386f, 0.2060086131095886f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1072951927781105f, 0.107295036315918f, 0.1072961091995239f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1293079704046249f, 0.1479243338108063f, 0.1931330561637878f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1459212601184845f, 0.1459220051765442f, 0.1459227204322815f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.999999463558197f, 1.0f, 0.9999899864196777f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1249424293637276f, 0.2735691666603088f, 0.5708154439926147f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8841201663017273f, 0.7941429018974304f, 0.5615870356559753f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9570815563201904f, 0.9570719599723816f, 0.9570761322975159f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9356134533882141f, 0.9356129765510559f, 0.9356223344802856f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.266094446182251f, 0.2890366911888123f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	Draw();
	if (Setting::ESP)
	{
		ThreadESPMain();
	}
	if (ShowMenu)
	{
	

		ImGui::Begin(u8"The Blade Dancerr", &ShowMenu);

		ImGui::Checkbox("Enable ESP", &Setting::ESP);
		if (Setting::ESP)
		{
			ImGui::Separator();
			ImGui::Text("ESP Settings:");
			ImGui::Checkbox("Box", &Setting::Box);
			ImGui::Checkbox("Health Bar", &Setting::Health);
			ImGui::Checkbox("Distance", &Setting::Distance);
			ImGui::Checkbox("Skeleton", &Setting::Sekeleton);
			ImGui::Checkbox("Player Name", &Setting::pName);
			ImGui::Checkbox("LGBT ESP", &Setting::LGBTMode);

			ImGui::Separator();
			ImGui::Text("ESP Range: MAX");
		}

		ImGui::End();
	}
	//if (ShowMenu == true) {
	//	ImGui::ShowDemoWindow();
	//}
	ImGui::EndFrame();

	DirectX9Interface::pDevice->SetRenderState(D3DRS_ZENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

	DirectX9Interface::pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9Interface::pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		DirectX9Interface::pDevice->EndScene();
	}

	HRESULT result = DirectX9Interface::pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9Interface::pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void MainLoop()
{
	static RECT OldRect;
	ZeroMemory(&DirectX9Interface::Message, sizeof(MSG));

	while (DirectX9Interface::Message.message != WM_QUIT) 
	{
		if (PeekMessage(&DirectX9Interface::Message, OverlayWindow::Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9Interface::Message);
			DispatchMessage(&DirectX9Interface::Message);
		}
		HWND ForegroundWindow = GetForegroundWindow();
		if (ForegroundWindow == Process::Hwnd) {
			HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
			SetWindowPos(OverlayWindow::Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT TempRect;
		POINT TempPoint;
		ZeroMemory(&TempRect, sizeof(RECT));
		ZeroMemory(&TempPoint, sizeof(POINT));

		GetClientRect(Process::Hwnd, &TempRect);
		ClientToScreen(Process::Hwnd, &TempPoint);

		Process::Width = TempRect.right;
		Process::Height = TempRect.bottom;

		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = Process::Hwnd;

		POINT TempPoint2;
		GetCursorPos(&TempPoint2);
		io.MousePos.x = TempPoint2.x - TempPoint.x;
		io.MousePos.y = TempPoint2.y - TempPoint.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else {
			io.MouseDown[0] = false;
		}

		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
			OldRect = TempRect;
			Process::WindowWidth = TempRect.right;
			Process::WindowHeight = TempRect.bottom;
			DirectX9Interface::pParams.BackBufferWidth = Process::WindowWidth;
			DirectX9Interface::pParams.BackBufferHeight = Process::WindowHeight;
			SetWindowPos(OverlayWindow::Hwnd, (HWND)0, TempPoint.x, TempPoint.y, Process::WindowWidth, Process::WindowHeight, SWP_NOREDRAW);
			DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		}

		Render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (DirectX9Interface::pDevice != NULL) {
		DirectX9Interface::pDevice->EndScene();
		DirectX9Interface::pDevice->Release();
	}
	if (DirectX9Interface::Direct3D9 != NULL) {
		DirectX9Interface::Direct3D9->Release();
	}
	DestroyWindow(OverlayWindow::Hwnd);
	UnregisterClass(OverlayWindow::WindowClass.lpszClassName, OverlayWindow::WindowClass.hInstance);
}

bool DirectXInit() {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
		return false;
	}

	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = OverlayWindow::Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = Process::WindowWidth;
	Params.BackBufferHeight = Process::WindowHeight;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if (FAILED(DirectX9Interface::Direct3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayWindow::Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9Interface::pDevice))) {
		DirectX9Interface::Direct3D9->Release();
		return false;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12.0f);
	io.FontDefault = font;
	io.Fonts->Build();
	ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::SetNextWindowPos(ImVec2(88, 39), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(194, 256), ImGuiCond_Always);

	ImGui_ImplWin32_Init(OverlayWindow::Hwnd);
	ImGui_ImplDX9_Init(DirectX9Interface::pDevice);
	DirectX9Interface::Direct3D9->Release();
	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message) {
	case WM_DESTROY:
		if (DirectX9Interface::pDevice != NULL) {
			DirectX9Interface::pDevice->EndScene();
			DirectX9Interface::pDevice->Release();
		}
		if (DirectX9Interface::Direct3D9 != NULL) {
			DirectX9Interface::Direct3D9->Release();
		}
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (DirectX9Interface::pDevice != NULL && wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9Interface::pParams.BackBufferWidth = LOWORD(lParam);
			DirectX9Interface::pParams.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void SetupWindow() {
	OverlayWindow::WindowClass = {
		sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, OverlayWindow::Name, LoadIcon(nullptr, IDI_APPLICATION)
	};

	RegisterClassEx(&OverlayWindow::WindowClass);
	if (Process::Hwnd){
		static RECT TempRect = { NULL };
		static POINT TempPoint;
		GetClientRect(Process::Hwnd, &TempRect);
		ClientToScreen(Process::Hwnd, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		Process::WindowWidth = TempRect.right;
		Process::WindowHeight = TempRect.bottom;
	}

	OverlayWindow::Hwnd = CreateWindowEx(NULL, OverlayWindow::Name, OverlayWindow::Name, WS_POPUP | WS_VISIBLE, Process::WindowLeft, Process::WindowTop, Process::WindowWidth, Process::WindowHeight, NULL, NULL, 0, NULL);
	DwmExtendFrameIntoClientArea(OverlayWindow::Hwnd, &DirectX9Interface::Margin);
	SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	ShowWindow(OverlayWindow::Hwnd, SW_SHOW);
	UpdateWindow(OverlayWindow::Hwnd);
}

DWORD WINAPI ProcessCheck(LPVOID lpParameter) {
	while (true) {
		if (Process::Hwnd != NULL) {
			if (GetProcessId(TargetProcess) == 0) {
				exit(0);
			}
		}
	}
}


int main() 
{
	SetConsoleTitleA("External Cheat");
	cout << R"(
 ________   _________ ______ _____  _   _          _      
 |  ____\ \ / /__   __|  ____|  __ \| \ | |   /\   | |     
 | |__   \ V /   | |  | |__  | |__) |  \| |  /  \  | |     
 |  __|   > <    | |  |  __| |  _  /| . ` | / /\ \ | |     
 | |____ / . \   | |  | |____| | \ \| |\  |/ ____ \| |____ 
 |______/_/ \_\  |_|  |______|_|  \_\_| \_/_/    \_\______|
                                                                                                                     
)" << endl;
	Sleep(1000);
	DWORD pidgame = GetTrueProcessId("aow_exe.exe");

	if (pidgame != 0)
	{
	
		cout << "Found PID: " << pidgame << endl;
		Process::GameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pidgame);
		setupchitchit(Process::GameHandle);
		if (Process::GameHandle)
		{
			
			cout << "Attack Memory SuccessFully" << endl; 

		}
		else
		{
			goto sigmaboy;
		}
	}
	else
	{
	sigmaboy:
		cout << "failed find||open mem" << endl;
	}

	Sleep(2000);
	if (CreateConsole == false)
		ShowWindow(GetConsoleWindow(), SW_HIDE); 

	bool WindowFocus = false;
	while (WindowFocus == false) 
	{
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (GetProcessId(TargetProcess) == ForegroundWindowProcessID) {
			Process::ID = GetCurrentProcessId();
			Process::Handle = GetCurrentProcess();
			Process::Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(Process::Hwnd, &TempRect);
			Process::WindowWidth = TempRect.right - TempRect.left;
			Process::WindowHeight = TempRect.bottom - TempRect.top;
			Process::WindowLeft = TempRect.left;
			Process::WindowRight = TempRect.right;
			Process::WindowTop = TempRect.top;
			Process::WindowBottom = TempRect.bottom;

			char TempTitle[MAX_PATH];
			GetWindowText(Process::Hwnd, TempTitle, sizeof(TempTitle));
			Process::Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process::Hwnd, TempClassName, sizeof(TempClassName));
			Process::ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process::Handle, NULL, TempPath, sizeof(TempPath));
			Process::Path = TempPath;

			WindowFocus = true;
		}
		Sleep(100);
	}
	OverlayWindow::Name = RandomString(10).c_str();
	SetupWindow();
	DirectXInit();
	CreateThread(0, 0, ProcessCheck, 0, 0, 0);
	thread t(thread_aa);
	t.detach();
	thread a(ThreadGetTrueEntity);
	a.detach();
	thread b(threadRandomColor);
	b.detach();
	while (TRUE) {
		MainLoop();
	}
}