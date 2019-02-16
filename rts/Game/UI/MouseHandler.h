/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <string>
#include <vector>

#include "Rendering/GL/RenderDataBufferFwd.hpp"
#include "System/float3.h"
#include "System/type2.h"
#include "System/UnorderedMap.hpp"
#include "MouseCursor.h"

static const int NUM_BUTTONS = 10;

class CInputReceiver;
class CCameraController;
class CUnit;

class CMouseHandler
{
public:
	CMouseHandler();
	~CMouseHandler();

	static CMouseHandler* GetOrReloadInstance();

	void ChangeCursor(const std::string& cmdName, const float scale = 1.0f);
	void ReloadCursors();

	void Update();
	void UpdateCursors();
	std::string GetCurrentTooltip();

	void HideMouse();
	void ShowMouse();
	void ToggleMiddleClickScroll(); /// lock+hide
	void WarpMouse(int x, int y);

	void DrawSelectionBox(); /// draw mousebox (selection box)
	void DrawCursor();

	void MouseRelease(int x, int y, int button);
	void MousePress(int x, int y, int button);
	void MouseMove(int x, int y, int dx, int dy);
	void MouseWheel(float delta);

	bool AssignMouseCursor(const std::string& cmdName,
	                       const std::string& fileName,
	                       CMouseCursor::HotSpot hotSpot,
	                       bool overwrite);
	bool ReplaceMouseCursor(const std::string& oldName,
	                        const std::string& newName,
	                        CMouseCursor::HotSpot hotSpot);

	const CMouseCursor* FindCursor(const std::string& cursorName) const {
		const auto it = cursorCommandMap.find(cursorName);

		if (it != cursorCommandMap.end())
			return &loadedCursors[it->second];

		return nullptr;
	}

	const std::string& GetCurrentCursor() const { return newCursor; }
	const float&  GetCurrentCursorScale() const { return cursorScale; }

	void ToggleHwCursor(const bool& enable);

	/// @see ConfigHandler::ConfigNotifyCallback
	void ConfigNotify(const std::string& key, const std::string& value);

private:
	void SetCursor(const std::string& cmdName, const bool forceRebind = false);

	static void GetSelectionBoxCoeff(const float3& pos1, const float3& dir1, const float3& pos2, const float3& dir2, float2& topright, float2& btmleft);

	void DrawScrollCursor(GL::RenderDataBufferC* buffer);
	void DrawFPSCursor(GL::RenderDataBufferC* buffer);


public:
	int lastx;
	int lasty;
	int activeButtonIdx;
	int activeCursorIdx;

	bool locked;
	/// Stores if the mouse was locked or not before going into direct control,
	/// so we can restore it when we return to normal.
	bool wasLocked;
	bool offscreen;

private:
	bool hide;
	bool hwHide;
	bool hardwareCursor;
	bool invertMouse;

	float cursorScale;
	float dragScrollThreshold;

	float scrollx;
	float scrolly;

public:
	float doubleClickTime;
	float scrollWheelSpeed;
	int dragSelectionThreshold;

	/// locked mouse indicator size
	float crossSize;
	float crossAlpha;
	float crossMoveScale;


	struct ButtonPressEvt {
		bool pressed;
		bool chorded;
		int x;
		int y;
		float3 camPos;
		float3 dir;
		float time;
		float lastRelease;
		int movement;
	};

	ButtonPressEvt buttons[NUM_BUTTONS + 1]; /// One-bottomed.
	float3 dir;

private:
	std::string newCursor; /// cursor changes are delayed
	std::string cursorText; /// current cursor name

	std::vector<CMouseCursor> loadedCursors;
	spring::unordered_map<std::string, size_t> cursorFileMap;
	spring::unordered_map<std::string, size_t> cursorCommandMap;

	const CUnit* lastClicked;
};

extern CMouseHandler* mouse;

#endif /* MOUSEHANDLER_H */
