/*
*	Copyright (C) 2019 Jin Won. All right reserved.
*
*	파일명			: Constant.cpp
*	작성자			: 원진
*	최종 수정일		: 19.04.07
*/

#include "Constant.h"

using namespace std;

namespace Constant
{
	namespace Window
	{
		const char* const TITLE = "IntelliCAD";
	}

	namespace Volume
	{
		const int WIDTH = 256;
		const int HEIGHT = 256;
		const int DEPTH = 225;
		const Point3D PIVOT = {
			static_cast<float>(WIDTH / 2),
			static_cast<float>(HEIGHT / 2),
			static_cast<float>(DEPTH / 2) 
		};
	}

	namespace Material
	{
		const float SHININESS = 40.f;
		namespace TransferFunc
		{
			namespace Filter
			{
				const Range<int> SKIN = { 40, 100 };
				const Range<int> BONE = { 100, 140 };
			}
		}
	}

	namespace Light
	{
		namespace Position
		{
			const Point3D LEFT = (Camera::AT - 200.f);
			const Point3D RIGHT = (Camera::AT + 200.f);
			const Point3D FRONT = Camera::EYE;
			const Point3D BACK = ((Camera::AT * 2) - FRONT);
			const Point3D TOP = { Camera::AT.x, Camera::AT.y, Camera::AT.z + 200.f };
			const Point3D BOTTOM = { Camera::AT.x, Camera::AT.y, Camera::AT.z - 200.f };
		}
	}

	namespace Camera
	{
		const Vector3D EYE = { -300.f, -300.f, -300.f };
		const Vector3D AT = Volume::PIVOT;
		const Vector3D UP = { 0.f, 0.f, -1.f };
	}

	namespace DB
	{
		const tstring ROOT_DIR = _T("db/");
		const tstring CONFIG_SUBPATH = _T("config.xml");
	}

	namespace UI
	{
		const tstring TAB_NAMES[] =
		{
			_T("기본 정보"),
			_T("조작"),
			_T("클라우드"),
			_T("로그")
		};
	}
}