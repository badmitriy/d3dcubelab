// OpenGlCppCode.cpp: определяет экспортированные функции для приложения DLL.
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

		//изменено на отрисовку нескольких треугольников
		vector<array<double, 3>> xyz;
		std::ifstream fin(L"..\\cubevert.txt");
		
		array<array<int, 3>, 2> triangle;
		vector<array<double,2>> vp;
		double x, y, z, v, p;
		while(fin >> x)//не так конечно, будет
		{//из-за такого способа ввода входной файл может быть отчень чувствителен к лишним символам-разделителям 
			//особенно в конце файла
			fin >> y >>z >>v >>p;
			xyz.push_back({ x,y,z });
			vp.push_back({ v / 6, p });// /6, чтобы подогнать под текстурные координаты граней куба
		}
		fin.close();
		int k = -1;
		for (int i = 0; i < xyz.size()/3; i++)
		{
			triangle[0] = { ++k , ++k, ++k };
			triangle[1] = { 0,0,0 };
			triangles.push_back(triangle);
			vp[3 * i][0] += int(i / 2) / 6.0;
			vp[3 * i + 1][0] += int(i / 2) / 6.0;
			vp[3 * i + 2][0] += int(i / 2) / 6.0;// и прибавить  [i/2] /6.0,
							// т.к. каждый второй треугольник в текстурных координатах сдвигается
			//по y координате не растянуто
		}
		vector<array<double, 3>> normals(xyz.size(), {1,1,1});

		finded->second->RenderStart();

		auto unit = finded->second->CreateTriangleColorUnit(triangles, xyz, normals, vp);
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
