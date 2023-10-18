#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	// モデルの開放
	delete model_;
	// 自キャラの解放
	//delete player_;
	delete debugCamera_;

	  /// delete enemy_;
	delete enemy_;
}

void GameScene::Initialize() {

	//敵に自分のアドレスを渡す

	//enemy_->SetPlayer(player_);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");

	
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);



	// 生成
	skydome_ = std::make_unique<Skydome>();

	// フォルダの名前を指定してね

	skydomeModel_.reset(Model::CreateFromOBJ("CelestialSphere", true));

	// テクスチャ読み込み
	// skydomeTextureHandle_ = TextureManager::Load("CelestialSphere/uvChecker.png");

	// 天球の初期化
	skydome_->Initialize(skydomeModel_.get(), skydomeTextureHandle_);




	// デバックカメラの生成
	debugCamera_ = new DebugCamera(720, 1280);

	// 敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_);


	// 軸方向表示の表示を有効化する
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//敵/////////////
const float kEnemySpeed = 1.0f;
	Vector3 velocity(kEnemySpeed, 0, 0);
	Vector3 enemyPosition(0, 1.0, 300);

	enemyModel_ = Model::Create();
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyPosition, velocity);
	/////////////////////////


	// 生成
	ground_ = std::make_unique<Ground>();

	// フォルダの名前を指定してね

	gronudModel_.reset(Model::CreateFromOBJ("Ground", true));

	// テクスチャ読み込み

	// 地面の初期化
	ground_->Initialize(gronudModel_.get(), GroundTextureHandle_);




}





void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	debugCamera_->Update();

	// 敵
	enemy_->Update();

	// 天球
	skydome_->Update();

	// 天球
	ground_->Update();

	Matrix4x4 cameraMatrix = {};
	cameraMatrix.m[0][0] = 1.0f;
	cameraMatrix.m[0][1] = 0.0f;
	cameraMatrix.m[0][2] = 0.0f;
	cameraMatrix.m[0][3] = 0.0f;

	cameraMatrix.m[1][0] = 0.0f;
	cameraMatrix.m[1][1] = 1.0f;
	cameraMatrix.m[1][2] = 0.0f;
	cameraMatrix.m[1][3] = 0.0f;

	cameraMatrix.m[2][0] = 0.0f;
	cameraMatrix.m[2][1] = 0.0f;
	cameraMatrix.m[2][2] = 1.0f;
	cameraMatrix.m[2][3] = 0.0f;

	cameraMatrix.m[2][0] = 1280.0f;
	cameraMatrix.m[2][1] = 720.0f;
	cameraMatrix.m[2][2] = 1.0f;
	cameraMatrix.m[2][3] = 1.0f;

	#ifdef _DEBUG
	// デバックの頭文字
	if (input_->TriggerKey(DIK_Q)) {
		isDebgCameraActive_ = true;
	}

#endif
	if (isDebgCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}


}

void GameScene::Draw() {

	;
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);


	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

	// 天球
	skydome_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 地面
	ground_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}