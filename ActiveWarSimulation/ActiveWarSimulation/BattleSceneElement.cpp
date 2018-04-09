#include"BattleSceneElement.h"

//---------------BattleSceneElement-----------------
BattleSceneElement::BattleSceneElement(SceneKind::Kind sceneKind)
	:m_nextScene(nullptr),m_sceneKind(sceneKind){}

BattleSceneElement::~BattleSceneElement(){}
