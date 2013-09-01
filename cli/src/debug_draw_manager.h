﻿// Ŭnicode please
#pragma once

#include "Typelist.h"
#include "template_lib.h"
#include "draw_attribute_list_mixin.h"

namespace kuuko {;

template<typename TList> class DrawAttributeListPolicy;

template<>
class DrawAttributeListPolicy<Loki::NullType> {
public:
	enum { HasNext = 0 };
	static void clear(kuuko::GenSimpleHierarchy<Loki::NullType> &obj) {}
	static void pop_back(kuuko::GenSimpleHierarchy<Loki::NullType> &obj) {}
	static size_t size(const kuuko::GenSimpleHierarchy<Loki::NullType> &obj) { return 0; }
	static bool validate(const kuuko::GenSimpleHierarchy<Loki::NullType> &obj) { return true; }
	static void swap(kuuko::GenSimpleHierarchy<Loki::NullType> &obj, int a, int b) {}
	static void copy_elem(kuuko::GenSimpleHierarchy<Loki::NullType> &obj,
		kuuko::GenSimpleHierarchy<Loki::NullType> *target, int idx) {}
};

template<typename T, typename U>
class DrawAttributeListPolicy< Loki::Typelist<T, U> > {
public:
	enum { HasNext = 1 };
	typedef DrawAttributeListPolicy<U> Next;
	static void clear(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		static_cast<T&>(obj).clear();
		Next::clear(obj);
	};
	static void pop_back(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		static_cast<T&>(obj).pop_back();
		Next::pop_back(obj);
	};
	static size_t size(const GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		return static_cast<const T&>(obj).size();
	}
	static bool validate(const GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		int leftSize = size(obj);
		int rightSize = Next::size(obj);
		if(Next::HasNext == 1 && leftSize != rightSize) {
			return false;
		}
		return Next::validate(obj);
	}
	static void swap(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj, int a, int b)
	{
		static_cast<T&>(obj).swap(a, b);
		Next::swap(obj, a, b);
	}
	static void copy_elem(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj, 
		GenSimpleHierarchy< Loki::Typelist<T, U> > *target, int idx) 
	{
		static_cast<T&>(obj).copy_elem(target, idx);
		Next::copy_elem(obj, target, idx);
	}
};

template<typename TList>
class DrawAttributeListT : public GenSimpleHierarchy<TList> {
public:
	typedef DrawAttributeListPolicy<TList> Policy;
	void clear() { Policy::clear(*this); }
	void pop_back() { Policy::pop_back(*this); }
	size_t size() const { return ((typename TList::Head*)this)->size(); }
	bool validate() const { return Policy::validate(*this); }
	void swap(int a, int b) { Policy::swap(*this, a, b); }
	void copy_elem(DrawAttributeListT *target, int idx) { Policy::copy_elem(*this, target, idx); }
};

class DrawAttributeList_Line2D : public DrawAttributeListT<DrawAttributeListMixin_Line2D> {};
class DrawAttributeList_Line3D : public DrawAttributeListT<DrawAttributeListMixin_Line3D> {};
class DrawAttributeList_Cross2D : public DrawAttributeListT<DrawAttributeListMixin_Cross2D> {};
class DrawAttributeList_Cross3D : public DrawAttributeListT<DrawAttributeListMixin_Cross3D> {};
class DrawAttributeList_String2D : public DrawAttributeListT<DrawAttributeListMixin_String2D> {};
class DrawAttributeList_String3D : public DrawAttributeListT<DrawAttributeListMixin_String3D> {};
class DrawAttributeList_Circle2D : public DrawAttributeListT<DrawAttributeListMixin_Circle2D> {};
class DrawAttributeList_Sphere3D : public DrawAttributeListT<DrawAttributeListMixin_Sphere3D> {};
class DrawAttributeList_Axis3D : public DrawAttributeListT<DrawAttributeListMixin_Axis3D> {};

template<class T>
class DrawAttributeListHolder {
public:
	typedef T value_type;

	value_type immediateDrawList;
	value_type durationDrawList;
	std::vector<int> durationList;	//millisecond

	const value_type &getList(int duration) const
	{
		const value_type *drawList = nullptr;
		OVR_ASSERT(duration >= 0);
		if(duration == 0) {
			drawList = &immediateDrawList;
		} else {
			drawList = &durationDrawList;
			
		}
		return *drawList;
	}

	value_type &getListAndPushDuration(int duration)
	{
		value_type &drawList = const_cast<value_type&>(getList(duration));
		if(duration > 0) {
			durationList.push_back(duration);
		}
		return drawList;
	}

	bool runValidateOnce(int duration) const
	{
		static bool immediateValidateRun = false;
		static bool durationValidateRun = false;

		const value_type &drawList = getList(duration);
		if(duration > 0 && durationValidateRun) {
			durationValidateRun = true;
			bool validateResult = drawList.validate();
			OVR_ASSERT(validateResult == true);
			
			if(drawList.size() == durationDrawList.size()) {
				validateResult = false;
			}
			OVR_ASSERT(validateResult == true);

			return validateResult;
		} else if(duration == 0 && immediateValidateRun) {
			immediateValidateRun = true;
			bool validateResult = drawList.validate();
			OVR_ASSERT(validateResult == true);
			return validateResult;
		} else {
			return true;
		}
	}
};


// 로직 돌릴 순서대로 쓰기
typedef TYPELIST_9(
	DrawAttributeList_Line3D,
	DrawAttributeList_Cross3D,
	DrawAttributeList_Sphere3D,
	DrawAttributeList_Axis3D,
	DrawAttributeList_String3D,
	DrawAttributeList_Line2D,
	DrawAttributeList_Cross2D,
	DrawAttributeList_String2D,
	DrawAttributeList_Circle2D
	) DrawAttributeElemList;

class DebugDrawManager : public Loki::GenScatterHierarchy<DrawAttributeElemList, DrawAttributeListHolder> {
public:
	DebugDrawManager();
	~DebugDrawManager();

public:
	void UpdateAll(int ms);
	void Clear();
	size_t Size() const;

	//add 3d
public:
	void AddLine(const OVR::Vector3f &p1, const OVR::Vector3f &p2,
        const OVR::Color &color,
		float lineWidth = 1.0f,
		int duration = 0,
		bool depthEnable = true);
	
	void AddCross(const OVR::Vector3f &pos, 
		const OVR::Color &color,
		float size,
		int duration = 0,
		bool depthEnable = true);

	void AddSphere(const OVR::Vector3f &pos, 
		float radius,
		const OVR::Color &color,
		int duration = 0,
		bool depthEnable = true);

    void AddAxis(const OVR::Matrix4f &xf,
		float size = 1.0f, 
		int duration = 0,
		bool depthEnable = true);

	void AddString(const OVR::Vector3f &pos, 
		const DrawAttributeListMixin_String::string_type &msg,
		const OVR::Color &color,
		float scale = 1.0f,
		int duration = 0,
		bool depthEnable = true);
	
	//add 2d
public:
	void AddLine(const OVR::Vector2i &p1, const OVR::Vector2i &p2,
		const OVR::Color &color,
		float lineWidth = 1.0f,
		int duration = 0);
	
	void AddCross(const OVR::Vector2i &pos, 
		const OVR::Color &color,
		float size,
		int duration = 0);

	void AddString(const OVR::Vector2i &pos, 
		const DrawAttributeListMixin_String::string_type &msg,
		const OVR::Color &color,
		float scale = 1.0f,
		int duration = 0);

	void AddCircle(const OVR::Vector2i &pos, float radius,
		const OVR::Color &color,
		int duration = 0);
};

// 주력으로 사용할것을 전역변수로 걸어놔야 속편하다
extern DebugDrawManager *g_debugDrawMgr;

}   // namespace kuuko