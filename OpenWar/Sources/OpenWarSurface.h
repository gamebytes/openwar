// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef OpenWarSurface_H
#define OpenWarSurface_H

#include "Surface.h"
#include "EditorModel.h"

class BattleGesture;
class BattleModel;
class BattleRendering;
class BattleView;
class ButtonGesture;
class ButtonItem;
class ButtonRendering;
class ButtonView;
class EditorGesture;
class SimulationRules;
class SimulationState;
class SmoothTerrainRendering;
class TerrainGesture;


class OpenWarSurface : public Surface
{
public: // TODO: just testing
	enum class Mode { None, Editing, Playing };
	Mode _mode;
	SimulationState* _simulationState;
	SimulationRules* _simulationRules;

	renderers* _renderers;
	BattleRendering* _battleRendering;
	ButtonRendering* _buttonRendering;

	BattleModel* _battleModel;
	EditorModel* _editorModel;

	SmoothTerrainRendering* _terrainRendering;
	BattleView* _battleView;
	ButtonView* _buttonsTopLeft;
	ButtonView* _buttonsTopRight;

	TerrainGesture* _terrainGesture;
	BattleGesture* _battleGesture;
	EditorGesture* _editorGesture;
	ButtonGesture* _buttonGesture;

	ButtonItem* _buttonItemHand;
	ButtonItem* _buttonItemPaint;
	ButtonItem* _buttonItemErase;
	ButtonItem* _buttonItemSmear;
	ButtonItem* _buttonItemHills;
	ButtonItem* _buttonItemWater;
	ButtonItem* _buttonItemTrees;

public:
	OpenWarSurface(glm::vec2 size, float pixelDensity);
	virtual ~OpenWarSurface();

	void Reset(SimulationState* simulationState);

	virtual void ScreenSizeChanged();
	virtual void Update(double secondsSinceLastUpdate);
	virtual void Render();

private:
	void UpdateSoundPlayer();

	void ClickedPlay();
	void ClickedPause();
	void ClickedRewind();

	void SetEditorMode(EditorMode editorMode);
	void SetEditorFeature(EditorFeature editorFeature);

	void UpdateButtonsAndGestures();
};


#endif
