// Ŭnicode please
#include "stdafx.h"
#include "debug_draw_manager.h"

using namespace Loki;
using namespace kuuko;
using OVR::Vector2i;
using OVR::Vector3f;
using OVR::Color;


class DebugDrawManagerTest : public ::testing::Test {
protected:
	virtual void SetUp()
	{	
	}
	virtual void TearDown()
	{
	}
};

TEST(swapVectorElement, test)
{
	DrawAttributeListMixin_3D vec;
	vec.depthEnableList.push_back(false);
	vec.depthEnableList.push_back(true);

	swapVectorElement(vec.depthEnableList, 0, 1);
	EXPECT_EQ(true, vec.depthEnableList[0]);
	EXPECT_EQ(false, vec.depthEnableList[1]);
}

TEST_F(DebugDrawManagerTest, updateAll) 
{
	DebugDrawManager mgr;
	EXPECT_EQ(0, mgr.Size());

	Color color;
	mgr.AddCross(Vector2i(0, 0), color, 1);	// without duration
	mgr.AddCross(Vector2i(0, 0), color, 1, 1000);	// with duration
	mgr.AddLine(Vector3f(0, 0, 0), Vector3f(0, 0, 0), color, 1);	// without duration
	mgr.AddLine(Vector3f(0, 0, 0), Vector3f(0, 0, 0), color, 1, 1000);	// with duration
	EXPECT_EQ(4, mgr.Size());

	mgr.UpdateAll(500);
	EXPECT_EQ(2, mgr.Size());

	mgr.UpdateAll(1000);
	EXPECT_EQ(0, mgr.Size());

	mgr.UpdateAll(1000);
	EXPECT_EQ(0, mgr.Size());
}

TEST_F(DebugDrawManagerTest, updateAll_complexDuration)
{
	DebugDrawManager mgr;
	EXPECT_EQ(0, mgr.Size());

	Color color;
	mgr.AddLine(Vector3f(0, 0, 0), Vector3f(0, 0, 0), color, 1, 1000);
	mgr.AddLine(Vector3f(0, 0, 0), Vector3f(0, 0, 0), color, 2, 3000);
	mgr.AddLine(Vector3f(0, 0, 0), Vector3f(0, 0, 0), color, 3, 2000);
	mgr.AddLine(Vector3f(0, 0, 0), Vector3f(0, 0, 0), color, 4, 4000);
	EXPECT_EQ(4, mgr.Size());

	const DrawAttributeList_Line3D &durationDrawList = Loki::Field<DrawAttributeList_Line3D>(mgr).durationDrawList;
	const std::vector<int> &durationList = Loki::Field<DrawAttributeList_Line3D>(mgr).durationList;

	mgr.UpdateAll(500);
	ASSERT_EQ(4, mgr.Size());
	ASSERT_EQ(4, durationDrawList.size());
	ASSERT_EQ(4, durationList.size());
	{
		auto &vec = static_cast<DrawAttributeListMixin_Scale<float>>(durationDrawList);
		ASSERT_EQ(4, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
		EXPECT_EQ(2, vec.scaleList[1]);
		EXPECT_EQ(3, vec.scaleList[2]);
		EXPECT_EQ(1, vec.scaleList[3]);
	}

	mgr.UpdateAll(1000);
	ASSERT_EQ(3, mgr.Size());
	ASSERT_EQ(3, durationDrawList.size());
	ASSERT_EQ(3, durationList.size());
	{
		auto &vec = static_cast<DrawAttributeListMixin_Scale<float>>(durationDrawList);
		ASSERT_EQ(3, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
		EXPECT_EQ(2, vec.scaleList[1]);
		EXPECT_EQ(3, vec.scaleList[2]);
	}

	mgr.UpdateAll(1000);
	ASSERT_EQ(2, mgr.Size());
	ASSERT_EQ(2, durationDrawList.size());
	ASSERT_EQ(2, durationList.size());
	{
		auto &vec = static_cast<DrawAttributeListMixin_Scale<float>>(durationDrawList);
		ASSERT_EQ(2, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
		EXPECT_EQ(2, vec.scaleList[1]);
	}

	mgr.UpdateAll(1000);
	ASSERT_EQ(1, mgr.Size());
	ASSERT_EQ(1, durationDrawList.size());
	ASSERT_EQ(1, durationList.size());
	{
		auto &vec = static_cast<DrawAttributeListMixin_Scale<float>>(durationDrawList);
		ASSERT_EQ(1, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
	}

	mgr.UpdateAll(1000);
	ASSERT_EQ(0, mgr.Size());
	ASSERT_EQ(0, durationDrawList.size());
	ASSERT_EQ(0, durationList.size());
	{
		auto &vec = static_cast<DrawAttributeListMixin_Scale<float>>(durationDrawList);
		ASSERT_EQ(0, vec.size());
	}
}


TEST_F(DebugDrawManagerTest, getCmdList)
{
	DebugDrawManager mgr;
	const auto &line3d = Loki::Field<DrawAttributeList_Line3D>(mgr);
	const auto &line2d = Loki::Field<DrawAttributeList_Line2D>(mgr);
}

