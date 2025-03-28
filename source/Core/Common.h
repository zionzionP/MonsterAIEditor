#pragma once
#include <codecvt>
#include <Siv3D.hpp>

namespace Common
{
	//Windowサイズ
	static constexpr int32 WindowWidth = 640;
	static constexpr int32 WindowHeight = 480;
	//エディターWindowサイズ
	static constexpr int32 EditorWindowWidth = 1600;
	static constexpr int32 EditorWindowHeight = 900;
	//Knight攻撃用ベクトル(0:下 1:左 2:右 3:上)
	static constexpr Vec2 attackVec2[4] = { Vec2(0, 30), Vec2(-30, 0),Vec2(30, 0), Vec2(0, -30) };

	//Knight移動用ベクトル
	static constexpr Vec2 moveVec2[4] = { Vec2(0, 1), Vec2(-1, 0),Vec2(1, 0), Vec2(0, -1) };

	//Knight、Enemy移動アニメーション用配列
	static constexpr int32 patterns[4] = { 1, 2, 1, 0 };
	//ノックバック距離
	static constexpr int32 KNOCKBACK_DISTANCE = 120;
	static constexpr int32 TACKLE_DISTANCE = 80;
	//プレイヤー最大HP
	static constexpr int32 PlayerMaxHP = 200;
	//ゴースト最大HP
	static constexpr int32 EnemyMaxHP = 60;
	//スケルトン最大HP
	static constexpr int32 SkeletonMaxHP = 100;
	//ナイトの攻撃力
	static constexpr int32 Knight_Attack = -20;
	//エネミーの攻撃力
	static constexpr int32 Enemy_Attack = -20;
	//エネミーの攻撃力
	static constexpr int32 Skeleton_Attack = -20;
	//エネミーの生成間隔
	static constexpr double generateTime = 1.0;
	//ナイトの無敵時間
	static constexpr double undamageableTimeKinght = 0.8;
	//1マスのサイズ
	static constexpr int CHIPSIZE = 32;
	//キャラクター半径
	static constexpr double charRadius = 16;

	static constexpr Vec2 aroundPoint[8] = { Vec2(0, -32), Vec2(-32, 0),Vec2(0, 32), Vec2(32, 0)
		                                    ,Vec2(-32, -32),Vec2(32, -32),Vec2(-32, 32),Vec2(32, 32) };
	//回復アイテムの回復量
	static constexpr int healPoint = 50;

	enum CollisionTag {
		NOTHING = 0,
		COLLISION_KNIGHT = 0x1,
		COLLISION_ENEMY = 0x2,
		COLLISION_ATTACKEFFECT = 0x4,
		COLLISION_WALL = 0x8,
		COLLISION_HEALITEM = 0x10,
	};

	//ピクセル座標->マス座標
	static Vec2 PixelPosToNavMeshPos(const Vec2& i_pos)
	{
		int x = i_pos.x / Common::CHIPSIZE;
		int y = i_pos.y / Common::CHIPSIZE;
		return Vec2{ x,y };
	}

	//マス座標->ピクセル座標
	static Vec2 NavMeshPosToPixelPos(const Vec2& i_pos)
	{
		int x = i_pos.x * Common::CHIPSIZE + Common::CHIPSIZE/2;
		int y = i_pos.y * Common::CHIPSIZE + Common::CHIPSIZE/2;
		return Vec2{ x,y };
	}
	//AABB上に外の点と一番近い点を求める
	static const Vec2 ClosestPointAABB(const Vec2& i_point, const RectF& i_hitbox)
	{
		double o_x = i_point.x;
		double o_y = i_point.y;
		Vec2 minPoint = i_hitbox.tl();
		Vec2 maxPoint = i_hitbox.br();
		if (i_point.x < minPoint.x)
		{
			o_x = minPoint.x;
		}
		if (i_point.x > maxPoint.x)
		{
			o_x = maxPoint.x;
		}
		if (i_point.y < minPoint.y)
		{
			o_y = minPoint.y;
		}
		if (i_point.y > maxPoint.y)
		{
			o_y = maxPoint.y;
		}
		return Vec2{ o_x,o_y };
	}

	static std::string doubleToString(double value)
	{
		std::string str = std::format("{}", value);

		//不要な0を削除
		if (str.find('.') != std::string::npos) {
			str.erase(str.find_last_not_of('0') + 1);
			if (str.back() == '.') {
				str.pop_back();
			}
		}
		return str;
	}	
}
