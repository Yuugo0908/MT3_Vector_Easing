#include "DxLib.h"
#include "Vector3.h"

const char TITLE[] = "LE2B_12_�N�����}���E�S";

const int WIN_WIDTH = 1024; //�E�B���h�E����
const int WIN_HEIGHT = 576;//�E�B���h�E�c��

void DrawAxis3D(const float length);//x, y, z���̕`��

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//�E�B���h�E���[�h�ɐݒ�
	//�E�B���h�E�T�C�Y���蓮�ł͕ύX�ł����A���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// �^�C�g����ύX
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�i���j�^�[�̉𑜓x�ɍ��킹��j
	SetWindowSizeExtendRate(1.0);				//��ʃT�C�Y��ݒ�i�𑜓x�Ƃ̔䗦�Őݒ�j
	SetBackgroundColor(0x00, 0x00, 0x40);		// ��ʂ̔w�i�F��ݒ肷��

	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1) { return -1; }

	//�i�_�u���o�b�t�@�j�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);		//Z�o�b�t�@��L���ɂ���
	SetWriteZBuffer3D(TRUE);	//Z�o�b�t�@�ւ̏������݂�L���ɂ���

	SetCameraNearFar(1.0f, 1000.0f);	//�J�����̗L���͈͂̐ݒ�
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);	//��ʂ̒��S���J�����̒��S�ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(
		//Vector3(0.0f, 0.0f, -120.0f),	//�J�����̈ʒu
		Vector3(-20.0f, 20.0f, -120.0f),	//�J�����̈ʒu
		Vector3(0.0f, 0.0f, 0.0f),		//�J�����̒����_
		Vector3(0.0f, 1.0f, 0.0f)		//�J�����̏�̌���
	);
	//���Ԍv���ɕK�v�ȃf�[�^
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//��ԂŎg���f�[�^
	//start �� end ��5�b�Ŋ���������
	Vector3 start(-100.0f, 0, 0);	//�X�^�[�g�n�_
	Vector3 end(+100.0f, 0, 0);		//�G���h�n�_
	float maxTime = 3.0f;			//�S�̎��ԁi�b�j
	float timeRate;					//�������Ԃ��i�񂾂��i���j

	int Mode = 1;

	//���̈ʒu
	Vector3 position;

	//���s�O�ɁA�J�E���^�l���擾
	startCount = GetNowHiPerformanceCount(); //long long int�^�@64bit int

	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	char keys[256] = { 0 }; //�ŐV�̃L�[�{�[�h���p
	char oldkeys[256] = { 0 };//1���[�v�i�t���[���j�O�̃L�[�{�[�h���

	// �Q�[�����[�v
	while (1)
	{
		//�ŐV�̃L�[�{�[�h��񂾂������̂͂P�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; ++i) {
			oldkeys[i] = keys[i];
		}
		//�ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);


		//�o�ߎ��Ԃ̌v�Z
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		//�X�V����
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
		//�`�揈��
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		//�f�o�b�O�p
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position(%5.1f, %5.1f, %5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");
		DrawFormatString(0, 60, GetColor(255, 255, 255), "Mode : %d", Mode);

		//---------  �����܂łɃv���O�������L�q  ---------//
		ScreenFlip();//�i�_�u���o�b�t�@�j����

		//��ʃN���A
		ClearDrawScreen();

		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}
		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	//Dx���C�u�����I������
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