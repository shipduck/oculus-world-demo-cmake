// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

using namespace Loki;

namespace kuuko {;

//singleton
DebugDrawManager debugDrawMgrLocal;
DebugDrawManager *g_debugDrawMgr = &debugDrawMgrLocal;

template<typename TList> struct DebugDrawListFunctor;
template<>
struct DebugDrawListFunctor<NullType> {
	static int Size(const DebugDrawManager &mgr) { return 0; }
	static void Clear(DebugDrawManager &mgr) { return; }
	static void Update(int ms, DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct DebugDrawListFunctor< Typelist<T, U> > {
	static int Size(const DebugDrawManager &mgr)
	{
		int sum = 0;
		sum += Field<T>(mgr).immediateDrawList.size();
		sum += Field<T>(mgr).durationDrawList.size();
		sum += DebugDrawListFunctor<U>::Size(mgr);
		return sum;
	}
	static void Clear(DebugDrawManager &mgr)
	{
		Field<T>(mgr).durationList.clear();
		Field<T>(mgr).durationDrawList.clear();
		Field<T>(mgr).immediateDrawList.clear();
		DebugDrawListFunctor<U>::Clear(mgr);
	}
	static void Update(int ms, DebugDrawManager &mgr)
	{
		Field<T>(mgr).immediateDrawList.clear();
		
		auto &durationList = Field<T>(mgr).durationList;
		auto &durationDrawList = Field<T>(mgr).durationDrawList;
		OVR_ASSERT(durationList.size() == durationDrawList.size());

		//경과 시간 처리
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			durationList[i] -= ms;
		}
		while(durationList.empty() == false) {
			int duration = durationList.back();
			if(duration > 0) {
				break;
			}
			durationList.pop_back();
		}

		typedef std::tuple<int, int> DurationIndexTuple;
		std::vector<DurationIndexTuple> durationIndexList(durationList.size());
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			DurationIndexTuple &dataTuple = durationIndexList[i];
			std::get<0>(dataTuple) = durationList[i];
			std::get<1>(dataTuple) = i;
		}
		//durationDrawList-durationList 시간 내림차순으로 정렬
		std::sort(durationIndexList.begin(),
			durationIndexList.end(),
			[](const DurationIndexTuple &a, const DurationIndexTuple &b) {
				return std::get<0>(a) > std::get<0>(b);
		});
		std::sort(durationList.begin(), 
			durationList.end(),
			[](int a, int b) { return a > b; });

		//index 순서대로 복사, 경과시간 고려해서 지나친항목은 무시
		typedef typename std::remove_reference<decltype(durationDrawList)>::type DurationDrawList;
		DurationDrawList nextDurationDrawList;
		for(const auto &dataTuple : durationIndexList) {
			int idx = std::get<1>(dataTuple);
			int duration = std::get<0>(dataTuple);
			if(duration > 0) {
				durationDrawList.copy_elem(&nextDurationDrawList, idx);
			}
		}
		durationDrawList = std::move(nextDurationDrawList);

		OVR_ASSERT(durationList.size() == durationDrawList.size());
		DebugDrawListFunctor<U>::Update(ms, mgr);
	}
};

size_t DebugDrawManager::Size() const
{
	int sum = DebugDrawListFunctor<DrawAttributeElemList>::Size(*this);	
	return sum;
}

DebugDrawManager::DebugDrawManager()
{
}
DebugDrawManager::~DebugDrawManager()
{
	Clear();
}

void DebugDrawManager::Clear() 
{
	DebugDrawListFunctor<DrawAttributeElemList>::Clear(*this);
}

void DebugDrawManager::UpdateAll(int ms)
{
	DebugDrawListFunctor<DrawAttributeElemList>::Update(ms, *this);
}

void DebugDrawManager::AddLine(const OVR::Vector3f &p1, const OVR::Vector3f &p2,
		const OVR::Color &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Line3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.p1List.push_back(p1);
	drawList.p2List.push_back(p2);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(lineWidth);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddCross(const OVR::Vector3f &pos, 
		const OVR::Color &color,
		float size,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Cross3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(size);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddSphere(const OVR::Vector3f &pos, 
		float radius,
		const OVR::Color &color,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Sphere3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(radius);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddAxis(const OVR::Matrix4f &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Axis3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.xfList.push_back(xf);
	drawList.scaleList.push_back(size);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddString(const OVR::Vector3f &pos, 
		const DrawAttributeListMixin_String::string_type &msg,
		const OVR::Color &color,
		float scale,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_String3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(scale);
	drawList.textList.push_back(msg);
	drawList.depthEnableList.push_back(depthEnable);
	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddLine(const OVR::Vector2i &p1, const OVR::Vector2i &p2,
		const OVR::Color &color,
		float lineWidth,
		int duration)
{
	typedef DrawAttributeList_Line2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.p1List.push_back(p1);
	drawList.p2List.push_back(p2);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(lineWidth);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddCross(const OVR::Vector2i &pos, 
		const OVR::Color &color,
		float size,
		int duration)
{
	typedef DrawAttributeList_Cross2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(size);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddString(const OVR::Vector2i &pos, 
								   const DrawAttributeListMixin_String::string_type &msg,
								   const OVR::Color &color,
								   float scale,
								   int duration)
{
	typedef DrawAttributeList_String2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(scale);
	drawList.textList.push_back(msg);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::AddCircle(const OVR::Vector2i &pos, float radius,
		const OVR::Color &color,
		int duration)
{
	typedef DrawAttributeList_Circle2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(radius);

	Field<ListType>(*this).runValidateOnce(duration);
}

}   // namespace kuuko