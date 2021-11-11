#include "DxLib.h"
#include "Vector3.h"

const char TITLE[] = "LE2B_12_クリヤマユウゴ";

const int WIN_WIDTH = 1024; //ウィンドウ横幅
const int WIN_HEIGHT = 576;//ウィンドウ縦幅

void DrawAxis3D(const float length);//x, y, z軸の描画

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//ウィンドウモードに設定
	//ウィンドウサイズを手動では変更できず、かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// タイトルを変更
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//画面サイズの最大サイズ、カラービット数を設定（モニターの解像度に合わせる）
	SetWindowSizeExtendRate(1.0);				//画面サイズを設定（解像度との比率で設定）
	SetBackgroundColor(0x00, 0x00, 0x40);		// 画面の背景色を設定する

	//Dxライブラリの初期化
	if (DxLib_Init() == -1) { return -1; }

	//（ダブルバッファ）描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);		//Zバッファを有効にする
	SetWriteZBuffer3D(TRUE);	//Zバッファへの書き込みを有効にする

	SetCameraNearFar(1.0f, 1000.0f);	//カメラの有効範囲の設定
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);	//画面の中心をカメラの中心に合わせる
	SetCameraPositionAndTargetAndUpVec(
		//Vector3(0.0f, 0.0f, -120.0f),	//カメラの位置
		Vector3(-20.0f, 20.0f, -120.0f),	//カメラの位置
		Vector3(0.0f, 0.0f, 0.0f),		//カメラの注視点
		Vector3(0.0f, 1.0f, 0.0f)		//カメラの上の向き
	);
	//時間計測に必要なデータ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//補間で使うデータ
	//start → end を5秒で完了させる
	Vector3 start(-100.0f, 0, 0);	//スタート地点
	Vector3 end(+100.0f, 0, 0);		//エンド地点
	float maxTime = 3.0f;			//全体時間（秒）
	float timeRate;					//何％時間が進んだか（率）

	int Mode = 1;

	//球の位置
	Vector3 position;

	//実行前に、カウンタ値を取得
	startCount = GetNowHiPerformanceCount(); //long long int型　64bit int

	//ゲームループで使う変数の宣言
	char keys[256] = { 0 }; //最新のキーボード情報用
	char oldkeys[256] = { 0 };//1ループ（フレーム）前のキーボード情報

	// ゲームループ
	while (1)
	{
		//最新のキーボード情報だったものは１フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; ++i) {
			oldkeys[i] = keys[i];
		}
		//最新のキーボード情報を取得
		GetHitKeyStateAll(keys);


		//経過時間の計算
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		//更新処理
		if (CheckHitKey(KEY_INPUT_R)) {
			startCount = GetNowHiPerformanceCount();
		}

		if (CheckHitKey(KEY_INPUT_1)) {
			startCount = GetNowHiPerformanceCount();
			Mode = 1;
		}
		if (CheckHitKey(KEY_INPUT_2)) {
			startCount = GetNowHiPerformanceCount();
			Mode = 2;
		}
		if (CheckHitKey(KEY_INPUT_3)) {
			startCount = GetNowHiPerformanceCount();
			Mode = 3;
		}

		if (Mode == 1) {
			position = easeIn(start, end, timeRate);
		}
		if (Mode == 2) {
			position = easeOut(start, end, timeRate);
		}
		if (Mode == 3) {
			position = easeInOut(start, end, timeRate);
		}

		DrawAxis3D(500.0f);
		//描画処理
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		//デバッグ用
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position(%5.1f, %5.1f, %5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");
		DrawFormatString(0, 60, GetColor(255, 255, 255), "Mode : %d", Mode);

		//---------  ここまでにプログラムを記述  ---------//
		ScreenFlip();//（ダブルバッファ）裏面

		//画面クリア
		ClearDrawScreen();

		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}
		// ＥＳＣキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	//Dxライブラリ終了処理
	DxLib_End();

	return 0;
}

void DrawAxis3D(const float length) {
	const unsigned R = GetColor(255, 0, 0);
	const unsigned G = GetColor(0, 255, 0);
	const unsigned B = GetColor(0, 0, 255);

	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), R);
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), G);
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), B);
}