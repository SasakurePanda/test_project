#pragma once
#include<vector>
#include "Object.h"
#include "input.h"
#include "Sound.h"

#define STAGE_X		(10)//���̃}�X��
#define STAGE_Y		(20)//�c�̃}�X��
#define BLOCK_SIZE	(22)//��}�X�̃T�C�Y
#define FRAME_LATE  (60)//�t���[�����[�g
#define NO_HOLD     (7) //�z�[���h�����Ă��Ȃ��Ƃ��̓Y����

enum TETROMINOTYPE
{
	TetrominoType_I,		//�܂������̌^
	TetrominoType_O,		//�l�p
	TetrominoType_S,		//S��
	TetrominoType_Z,		//�tS��
	TetrominoType_J,		//J��
	TetrominoType_L,		//L��
	TetrominoType_T,		//T��
	TetrominoTypeSize,		//�e�g���~�m�̐�
};

enum SCENE
{
	SCENE_TITLE,    //�^�C�g���V�[��
	SCENE_GAME,     //�Q�[���V�[��
};

enum STATE
{
	STATE_NEWTETROMINO,  //�V�����e�g���~�m���o�Ă��Ă���X�e�[�g
	STATE_NOWDROP,		 //�e�g���~�m�������Ă��Ă���X�e�[�g
	STATE_DELETE,		 //�e�g���~�m�������ď�����X�e�[�g
	STATE_GAMEOVER		 //�Q�[���I�[�o�[�̃X�e�[�g
};

enum Tetromino_Status
{
	BLANK,		//�󔒂̏ꏊ
	FALLED,		//�����������e�g���~�m
	FALLING,	//�����Ă���e�g���~�m
	FILLED		//��񑵂����e�g���~�m
};

class Game
{
private:
	Input input;
	Sound sound;

	Object title;			//�^�C�g���I�u�W�F�N�g

	Object player;			//�v���C���[�I�u�W�F�N�g
	Object background;		//�w�i�I�u�W�F�N�g

	Object speed;

	Object scoreboard;		//�X�R�A�I�u�W�F�N�g

	Object blocks_next;
	//Object blocks_next2;
	//Object blocks_next3;
	Object blocks_hold;
	Object blocks[STAGE_X][STAGE_Y];		//�u���b�N�I�u�W�F�N�g
	int data[STAGE_X][STAGE_Y + 3] = {};	//�f�[�^�z��

	int count = 0;		//���������̃J�E���g
	int count_set = 30; //���������̑��������߂�
	int state = 0;		//�Q�[���̏��(�O����������̂��Ȃ���ԂP������)
	int S_state = 0;
	int S_xy[4][4] = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };//
	int score = 0;		//���_
	int scene = 0;		//�V�[��(�O�F�^�C�g���A�P�F�Q�[���{�ҁA�Q�F�Q�[�����U���g)
	bool hold_bool = true;
	int hold= 8;

	int Tetromino_loop_count = 0;
	int TetrominoType;		//�e�g���~�m�̎��(0�`6)
	std::vector<int> TetrominoType_array;
	int TetrominoType_next;	//���̃e�g���~�m�̎��(0�`6)
	int TetrominoType_next3[3];
	int TetrominoDriection;	//�e�g���~�m�̌���(0�`3)

	int TetrominoData[7][4][4][4] ={
		{//	����
			{//0
				{0,0,0,0},
				{0,0,0,0},
				{2,2,2,2},
				{0,0,0,0},
			},
			{//1
				{0,0,2,0},
				{0,0,2,0},
				{0,0,2,0},
				{0,0,2,0},
			},
			{//2
				{0,0,0,0},
				{2,2,2,2},
				{0,0,0,0},
				{0,0,0,0},
			},
			{//3
				{0,2,0,0},
				{0,2,0,0},
				{0,2,0,0},
				{0,2,0,0},
			},

		},
		{	//�l�p
			{//0
				{0,0,0,0},
				{0,2,2,0},
				{0,2,2,0},
				{0,0,0,0},
			},
			{//1
				{0,0,0,0},
				{0,2,2,0},
				{0,2,2,0},
				{0,0,0,0},
			},
			{//2
				{0,0,0,0},
				{0,2,2,0},
				{0,2,2,0},
				{0,0,0,0},
			},
			{//3
				{0,0,0,0},
				{0,2,2,0},
				{0,2,2,0},
				{0,0,0,0},
			},
		},
		{	//S��
			{//0
				{0,0,0,0},
				{0,0,0,0},
				{2,2,0,0},
				{0,2,2,0},
			},
			{//1
				{0,0,0,0},
				{0,0,2,0},
				{0,2,2,0},
				{0,2,0,0},
			},
			{//2
				{0,0,0,0},
				{2,2,0,0},
				{0,2,2,0},
				{0,0,0,0},
			},
			{//3
				{0,0,0,0},
				{0,2,0,0},
				{2,2,0,0},
				{2,0,0,0},
			},
		},
		{	//�tS��
			{//0
				{0,0,0,0},
				{0,0,0,0},
				{0,2,2,0},
				{2,2,0,0},
			},
			{//1
				{0,0,0,0},
				{0,2,0,0},
				{0,2,2,0},
				{0,0,2,0},
			},
			{//2
				{0,0,0,0},
				{0,2,2,0},
				{2,2,0,0},
				{0,0,0,0},
			},
			{//3
				{0,0,0,0},
				{2,0,0,0},
				{2,2,0,0},
				{0,2,0,0},
			},
		}, 
		{	//�tL��
			{//0
				{0,0,0,0},
				{0,0,0,0},
				{2,2,2,0},
				{2,0,0,0},
			},
			{//1
				{0,0,0,0},
				{0,2,0,0},
				{0,2,0,0},
				{0,2,2,0},
			},
			{//2
				{0,0,0,0},
				{0,0,2,0},
				{2,2,2,0},
				{0,0,0,0},
			},
			{//3
				{0,0,0,0},
				{2,2,0,0},
				{0,2,0,0},
				{0,2,0,0},
			},
		},
		{	//L��
			{//0
				{0,0,0,0},
				{0,0,0,0},
				{2,2,2,0},
				{0,0,2,0},
			},
			{//1
				{0,0,0,0},
				{0,2,2,0},
				{0,2,0,0},
				{0,2,0,0},
			},
			{//2
				{0,0,0,0},
				{2,0,0,0},
				{2,2,2,0},
				{0,0,0,0},
			},
			{//3
				{0,0,0,0},
				{0,2,0,0},
				{0,2,0,0},
				{2,2,0,0},
			},
		},
		{	//T��
			{//0
				{0,0,0,0},
				{0,0,0,0},
				{2,2,2,0},
				{0,2,0,0},
			},
			{//1
				{0,0,0,0},
				{0,2,0,0},
				{0,2,2,0},
				{0,2,0,0},
			},
			{//2
				{0,0,0,0},
				{0,2,0,0},
				{2,2,2,0},
				{0,0,0,0},
			},
			{//3
				{0,0,0,0},
				{0,2,0,0},
				{2,2,0,0},
				{0,2,0,0},
			},
		}

	};

public:
	void Init(HWND hWnd);
	void Update(void);
	void Draw();
	void Uninit();

};

	