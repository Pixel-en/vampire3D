#pragma once
#include "Engine/GameObject.h"
#include "WeaponObject.h"
#include "ArmorObject.h"
#include <vector>
#include <map>

//プレイヤー
class Player :public GameObject
{
private:
	struct Status
	{
		//ゲーム中にパワーアップが取得できるもの
		int hp_;						//体力(実数値)
		int maxHp_;						//最大体力(実数値)
		float speed_;					//移動速度(実数値)
		float strength_;				//攻撃力(割合)
		float critical_;					//クリティカル率(割合)
		float collectionRange_;			//回収範囲(割合)
		float haste_;					//攻撃頻度(割合)

		//できないもの
		float criticalBoost_;			//クリティカル攻撃倍率(割合)
		float area_;					//攻撃エリア(割合)
		int resist_;					//防御(実数値)
		float ExpBoost_;				//経験値ブースト(割合)
		unsigned int level_;			//現在のレベル(実数値)
		unsigned int currentExp_;		//今の経験値量(実数値)
		unsigned int nextLvExp_;		//次のレベルまでの必要経験値量(実数値)
		unsigned long long totalExp_;	//取得した経験値量(実数値)
	};

	//前のポジション
	XMFLOAT3 prePos_;
	//移動ベクトル
	XMVECTOR MoveVec_;

	Status status_;
	//元のステータス
	Status Basestatus_;

	Status Booststatus_;

	void WeaponCSVLoad();
	void PlayerStatusLoad();

	std::map<std::string, WeaponObject::Status> WeaponState_;

	bool die_;
	int hSEDead_;
	int hSEEXP_;

public:
	std::vector<WeaponObject*> MyWeaponList_;
	std::vector<ArmorObject*> MyArmorList_;

	//ナイフ用
	XMFLOAT3 LookPos_;
	XMFLOAT3 LookTarget_;

	//武器のステータスを書き込む
	//引数：name　武器の名前
	//引数:_state　書き込むステータス
	bool WeaponStateWrite(std::string name, WeaponObject::Status& _state);

	Status GetStatus() { return status_; }
	Status GetBaseStatus() { return Basestatus_; }
	Status GetBoostStatus() { return Booststatus_; }
	void SetBoostStatusResist(int _resist) { Booststatus_.resist_ = _resist; }
	void SetBoostStatusCriticalBoost(float _criticalBoost) { Booststatus_.criticalBoost_ = _criticalBoost; }
	void SetBoostStatusArea(float _area) { Booststatus_.area_ = _area; }
	void SetBoostStatusExpBoost(float _expBoost) { Booststatus_.ExpBoost_ = _expBoost; }
	/// <summary>
	/// trueなら乗算、falseなら除算をブーストに行う
	/// </summary>
	void MultDivBoostStatusSpeed(float _speed, bool _isMult) {
		if (_isMult)Booststatus_.speed_ *= _speed;
		else Booststatus_.speed_ /= _speed;
	}
	/// <summary>
	/// trueなら乗算、falseなら除算をブーストに行う
	/// </summary>
	void MultDivBoostStatusStrength(float _strength, bool _isMult) {
		if (_isMult) Booststatus_.strength_ *= _strength;
		else Booststatus_.strength_ /= _strength;
	}

	/// <summary>
	/// trueなら乗算、falseなら除算をブーストに行う
	/// </summary>
	void MultDivBoostStatusCritical(float _critical, bool _isMult) {
		if (_isMult) Booststatus_.critical_ = Booststatus_.critical_ * _critical;
		else Booststatus_.critical_ = Booststatus_.critical_ / _critical;
	}

	//trueで加算,falseで除算
	void AddSubBoostStatusHaste(float _haste, bool _isMult) {
		
		float val = fmodf(_haste, 1.0f);

		if (_isMult) Booststatus_.haste_ += val;
		else Booststatus_.haste_ -= val;
	}
	void MultDivBoostStatusCollectionRange(float _collectionRange, bool _isMult) {
		if (_isMult) Booststatus_.collectionRange_ *= _collectionRange;
		else Booststatus_.collectionRange_ /= _collectionRange;
	}

	void AddStatusMaxHp(int _maxHp) {
		Booststatus_.maxHp_ += _maxHp;
	}

	void HealingHp(int _hp);

	//ステータスの更新
	//最大HPだけ足し算
	void StatusUpdate();

	//今動いているかどうかを返す
	bool isMove();

private:

	bool onGround_;	//地面についているか
	float gravity;	//重力

	int hModel_;			//モデルハンドル
	float lookHeight_;	//視点の高さ
	int hImage_;	//画像のハンドル(クロスヘア)
	Transform crossTrans;	//クロスヘア用のトランス

	bool isDamege_;
	float InvincibleTimer_;

	//キャラクターの移動
	void Move();
	bool moveNow_;

public:
	Player(GameObject* parent);

	~Player();

	//初期化
	void Initialize() override;

	void SuperUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;

	void OnCollisionsList(GameObject* pTarget, std::list<Collider*>::iterator MyItr, std::list<Collider*> list) override;

	/// <summary>
	/// 経験値を取得
	/// </summary>	
	/// <param name="_exp">取得した経験値</param>
	void AcquisitionEXP(int _exp);
};

