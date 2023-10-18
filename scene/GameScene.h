#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include"Enemy.h"
#include"Ground.h"
#include"Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// 3Dモデル
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	bool isDebgCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	Model* enemyModel_ = nullptr;
	Enemy* enemy_ = nullptr;


		#pragma region 天球のメンバ変数
	// テクスチャハンドル
	uint32_t skydomeTextureHandle_ = 0u;

	// 3Dモデル
	std::unique_ptr<Model> skydomeModel_;

	// 天球ポインタ
	std::unique_ptr<Skydome> skydome_;

#pragma endregion

#pragma region 地面
	//  テクスチャハンドル
	uint32_t GroundTextureHandle_ = 0u;

	// 3Dモデル
	std::unique_ptr<Model> gronudModel_;

	// 地面ポインタ
	std::unique_ptr<Ground> ground_;

#pragma endregion
};