﻿// Ŭnicode please 
#pragma once

#include "Typelist.h"
#include <string>
#include <vector>

namespace kuuko {;

template<typename T>
void swapVectorElement(T &vec, int a, int b)
{
	OVR_ASSERT(a >= 0 && a < static_cast<int>(vec.size()));
	OVR_ASSERT(b >= 0 && b < static_cast<int>(vec.size()));
	typename T::value_type tmp = vec[a];
	vec[a] = vec[b];
	vec[b] = tmp;
}

class DrawAttributeListMixin_Color {
public:
	typedef DrawAttributeListMixin_Color mixin_type;
public:
    std::vector<OVR::Color> colorList;	
	void clear() { colorList.clear(); }
	void pop_back() { colorList.pop_back(); }
	size_t size() const { return colorList.size(); }
	void swap(int a, int b) { swapVectorElement(colorList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->colorList.push_back(colorList[idx]);
	}
};

class DrawAttributeListMixin_3D {
public:
	typedef DrawAttributeListMixin_3D mixin_type;
public:
	std::vector<bool> depthEnableList;
	void clear() { depthEnableList.clear(); }
	void pop_back() { depthEnableList.pop_back(); }
	size_t size() const { return depthEnableList.size(); }
	void swap(int a, int b) { swapVectorElement(depthEnableList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->depthEnableList.push_back(depthEnableList[idx]);
	}
};

template<typename T>
class DrawAttributeListMixin_Scale {
public:
	typedef DrawAttributeListMixin_Scale<T> mixin_type;
public:
	std::vector<T> scaleList;
	void clear() { scaleList.clear(); }
	void pop_back() { scaleList.pop_back(); }
	size_t size() const { return scaleList.size(); }
	void swap(int a, int b) { swapVectorElement(scaleList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->scaleList.push_back(scaleList[idx]);
	}
};

class DrawAttributeListMixin_Transform {
public:
	typedef DrawAttributeListMixin_Transform mixin_type;
public:
    std::vector<OVR::Matrix4f> xfList;
	void clear() { xfList.clear(); }
	void pop_back() { xfList.pop_back(); }
	size_t size() const { return xfList.size(); }
	void swap(int a, int b) { swapVectorElement(xfList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->xfList.push_back(xfList[idx]);
	}
};

template<typename T>
class DrawAttributeListMixin_Line {
public:
	typedef DrawAttributeListMixin_Line mixin_type;
public:
	std::vector<T> p1List;
	std::vector<T> p2List;

	void clear()
	{
		p1List.clear();
		p2List.clear();
	}
	void pop_back()
	{
		p1List.pop_back();
		p2List.pop_back();
	}
	size_t size() const
	{
		OVR_ASSERT(p1List.size() == p2List.size());
		return p1List.size();
	}
	void swap(int a, int b)
	{
		swapVectorElement(p1List, a, b);
		swapVectorElement(p2List, a, b);
	}
	void copy_elem(mixin_type *target, int idx)
	{
		target->p1List.push_back(p1List[idx]);
		target->p2List.push_back(p2List[idx]);
	}
};

template<typename T>
class DrawAttributeListMixin_Pos {
public:
	typedef DrawAttributeListMixin_Pos<T> mixin_type;
public:
	std::vector<T> posList;
	void clear() { posList.clear(); }
	void pop_back() { posList.pop_back(); }
	size_t size() const { return posList.size(); }
	void swap(int a, int b) { swapVectorElement(posList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->posList.push_back(posList[idx]);
	}
};

class DrawAttributeListMixin_String {
public:
	typedef DrawAttributeListMixin_String mixin_type;
public:
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;

	std::vector<string_type> textList;

	void clear() { textList.clear(); }
	void pop_back() { textList.pop_back(); }
	size_t size() const { return textList.size(); }
	void swap(int a, int b) { swapVectorElement(textList, a, b); }

	void copy_elem(mixin_type *target, int idx)
	{
		target->textList.push_back(textList[idx]);
	}
};

//mixin으로 실제로 사용하는 mixin 조합 구성
typedef TYPELIST_3(
    DrawAttributeListMixin_Line<OVR::Vector2i>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color
	) DrawAttributeListMixin_Line2D;

typedef TYPELIST_4(
    DrawAttributeListMixin_Line<OVR::Vector3f>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Line3D;

typedef TYPELIST_3(
	DrawAttributeListMixin_Pos<OVR::Vector2i>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color
	) DrawAttributeListMixin_Cross2D;

typedef TYPELIST_4(
    DrawAttributeListMixin_Pos<OVR::Vector3f>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Cross3D;

typedef TYPELIST_4(
    DrawAttributeListMixin_Pos<OVR::Vector2i>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_String
	) DrawAttributeListMixin_String2D;

typedef TYPELIST_5(
    DrawAttributeListMixin_Pos<OVR::Vector3f>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_String,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_String3D;

typedef TYPELIST_3(
	DrawAttributeListMixin_Pos<OVR::Vector2i>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color
	) DrawAttributeListMixin_Circle2D;

typedef TYPELIST_4(
	DrawAttributeListMixin_Pos<OVR::Vector3f>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Sphere3D;

typedef TYPELIST_3(
	DrawAttributeListMixin_Transform,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Axis3D;

}   // namespace kuuko