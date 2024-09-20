#ifndef UITEXTURELOADER_H
#define UITEXTURELOADER_H

#include <cstdio>
#include <d3d11.h>
#include "../ThirdParty/STB/stb_image.h"
#include "../ThirdParty/ImGui/imgui.h"

bool LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
bool LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

#endif //UITEXTURELOADER_H