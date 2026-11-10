#pragma once

//シーンの種類
enum Scene
{
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_GAME_CLEAR,
	SCENE_GAME_OVER,
};

enum SceneState
{
	SCENE_STATE_INIT,
	SCENE_STATE_LOAD,
	SCENE_STATE_START,
	SCENE_STATE_LOOP,
	SCENE_STATE_FIN,
};
//シーン管理の更新処理
void SceneManagerUpdate();

//各シーンの状態別処理
void InitScene();
void LoadScene();
void StartScene();
void StepScene();
void UpdateScene();
void DrawScene();
void FinScene();

//シーン切り替え
void ChangeScene(Scene scene);