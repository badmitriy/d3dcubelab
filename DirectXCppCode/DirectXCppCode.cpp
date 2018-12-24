// OpenGlCppCode.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include <exception>
#include <map>
#include <memory>
#include <fstream>
#include "D3DRender.h"

#pragma comment(lib, "D3d11.lib")
#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")

#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif
using namespace std; 
extern map<HWND, shared_ptr<DX::Directx11>>* device = NULL;
CPP_API void InitDirectX(int handle)
{
	(*device)[(HWND) handle] = make_shared<DX::Directx11>((HWND)handle);
}

CPP_API void PrepareScene(int handle,int w,int h)
{
	auto finded = device->find((HWND) handle);
	if (finded != device->end())
	{
		finded->second->CreateTarget(w, h);
		finded->second->ClearAll();
		vector<array<array<int, 3>, 2>> triangles;

		//�������� �� ��������� ���������� �������������
		vector<array<double, 3>> xyz;
		std::ifstream fin(L"..\\cubevert.txt");
		double x, y, z;
		while(fin >> x)//�� ��� �������, �����
		{
		fin >> y;
		fin >> z;
		xyz.push_back({ x,y,z });
		}
		fin.close();
		//xyz.resize(3);
		//xyz[0] = { 0,0,0 };
		//xyz[1] = { 0,1,0 };
		//xyz[2] = { 1,0,0 };
		vector<array<double, 3>> normals(xyz.size(), {1,1,1});
		array<array<int, 3>, 2> triangle;
		triangle[0] = { 0, 1, 2 };
		triangle[1] = { 0, 255, 255 };
		for (int i = 0; i < xyz.size() / 3; i++)
		{
			triangles.push_back(triangle);
		}
		finded->second->RenderStart();

		auto unit = finded->second->CreateTriangleColorUnit(triangles, xyz, normals);
		finded->second->AddToSaved(unit);
		finded->second->RenderSavedData();
		finded->second->EndRender();
	}
}
CPP_API void RenderScene(int handle)
{
	auto finded = device->find((HWND) handle);
	if (finded != device->end())
	{
		DirectX::XMStoreFloat4x4(&(finded->second->ModelViewMatrix), DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&(finded->second->ProjectionMatrix), DirectX::XMMatrixIdentity());

		finded->second->RenderSavedData();
		finded->second->EndRender();
	}
}
