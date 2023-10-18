#include "Enemy.h"
#include "Function.h"
#include <cassert>
#include <ImguiManager.h>


Enemy::~Enemy()
{
	//弾の解放処理
	//複数出たのでfor文で解放しよう
	for (EnemyBullet* bullet : bullets_) 
	{
		delete bullet;
	}
}




    void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) 
    {
	// NULLチェック
	assert(model);

	phase_ = Phase::Approach;
	


	model_ = model;
	// 敵
	textureHandle_ = TextureManager::Load("Player6.png");
	// ワールド変数の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	//Fire();
	PhaseInitialize();
	

    }



	// 座標を移動させる（1フレーム分の移動量を足しこむ）
   // worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
  //  const float EnemykRotSpeed = 0.80f;
    const float EnemykRotSpeed = 2.0f;
	
	void Enemy::ApproachUpdate()
	{
	
		// 指定の位置に到達したら離脱する
	worldTransform_.translation_.x += EnemykRotSpeed;
		if (worldTransform_.translation_.x > 80.0f) 
	
		{
		
			phase_ = Phase::Leave;
	
		}
    }


	 void Enemy::PhaseInitialize() 
	 { 
		 FireTimer = kFireInterval;
	 }



	

	
	 
    void Enemy::LeaveUpdate()
	{
	    // 移動(ベクトルを加算)
	    // 移動（ベクトル加算）
	  worldTransform_.translation_.x -= EnemykRotSpeed;
	   
	  if (worldTransform_.translation_.x <-80.0f) 
	  {
		    phase_ = Phase::Approach;
	   }
     }
   

   



void Enemy::Update() 
{
	////敵の動き
	switch (phase_) 
	{

	case Phase::Approach:
	default:
		    ApproachUpdate();


		break;

	case Phase::Leave:
		LeaveUpdate();
		break;
	}

		FireTimer--;
	if (FireTimer <= 0) {
		Fire();
		FireTimer = kFireInterval;
	}

	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	// 弾の寿命
	bullets_.remove_if([](EnemyBullet* bullet)
		{
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	//  ワールドトランスの更新
	worldTransform_.UpdeateMatrix();
	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	
	// 画面に座標を出す
	ImGui::Begin("Enemy");

	ImGui::InputFloat3("EnemyPosition", &worldTransform_.translation_.x);
	ImGui::SliderFloat3("EnemySlide", &worldTransform_.translation_.x, -20.0f, 30.0f);

	ImGui::End();

}





    void Enemy::Draw(ViewProjection& viewProjection_) 
    {

	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	for (EnemyBullet* bullet : bullets_) 
	{

		bullet->Draw(viewProjection_);
	}
	
    }


	void Enemy::Fire()
	{
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// intealize
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);
    }

    void Enemy::OnCollsion() {}

    Vector3 Enemy::GetWorldPosition() 
	{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
	}
