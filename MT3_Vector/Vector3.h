#pragma once
#include <Dxlib.h>
class Vector3 : public VECTOR
{
public:
	//VECTOR構造体で定義済み
	//float x;	//x成分
	//float y;	//y成分
	//float z;	//z成分

public:
	//コンストラクタ
	Vector3();							//零ベクトルとして生成
	Vector3(float x, float y, float z);	//x成分, y成分を指定しての生成

	//メンバ関数
	float length() const;					//ノルム(長さ)を求める
	Vector3& normalize();					//正規化する
	float dot(const Vector3& v) const;		//内積を求める
	Vector3 cross(const Vector3& v)const;	//外積を求める

	// 単項演算子オーバーロード
	Vector3 operator +() const;
	Vector3 operator -() const;

	//代入演算子オーバーロード
	Vector3& operator +=(const Vector3& v);
	Vector3& operator -=(const Vector3& v);
	Vector3& operator *=(float s);
	Vector3& operator /=(float s);
};

//2項演算子オーバーロード
//※いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備している
Vector3 operator + (const Vector3& v1, const Vector3& v2);
Vector3 operator - (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Vector3& v, float s);
Vector3 operator * (float s, const Vector3& v);
Vector3 operator / (const Vector3& v, float s);

                     //スタート位置           終了位置             時間の割合
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeIn(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeOut(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInOut(const Vector3& start, const Vector3& end, const float t);

