#pragma once
#include<vector>
#include "Object.h"
#include "input.h"
#include "Sound.h"

#define STAGE_X		(10)//横のマス数
#define STAGE_Y		(20)//縦のマス数
#define BLOCK_SIZE	(22)//一マスのサイズ
#define FRAME_LATE  (60)//フレームレート
#define NO_HOLD     (7) //ホールドをしていないときの添え字

enum TETROMINOTYPE
{
	TetrominoType_I,		//まっすぐの型
	TetrominoType_O,		//四角
	TetrominoType_S,		//S字
	TetrominoType_Z,		//逆S字
	TetrominoType_J,		//J字
	TetrominoType_L,		//L字
	TetrominoType_T,		//T字
	TetrominoTypeSize,		//テトリミノの数
};

enum SCENE
{
	SCENE_TITLE,    //タイトルシーン
	SCENE_GAME,     //ゲームシーン
};

enum STATE
{
	STATE_NEWTETROMINO,  //新しいテトリミノが出てきているステート
	STATE_NOWDROP,		 //テトリミノが落ちてきているステート
	STATE_DELETE,		 //テトリミノが揃って消えるステート
	STATE_GAMEOVER		 //ゲームオーバーのステート
};

enum Tetromino_Status
{
	BLANK,		//空白の場所
	FALLED,		//落ちきったテトリミノ
	FALLING,	//落ちているテトリミノ
	FILLED		//一列揃ったテトリミノ
};

class Game
{
private:
	Input input;
	Sound sound;

	Object title;			//タイトルオブジェクト

	Object player;			//プレイヤーオブジェクト
	Object background;		//背景オブジェクト

	Object speed;

	Object scoreboard;		//スコアオブジェクト

	Object blocks_next;
	//Object blocks_next2;
	//Object blocks_next3;
	Object blocks_hold;
	Object blocks[STAGE_X][STAGE_Y];		//ブロックオブジェクト
	int data[STAGE_X][STAGE_Y + 3] = {};	//データ配列

	int count = 0;		//自動落下のカウント
	int count_set = 30; //自動落下の速さを決める
	int state = 0;		//ゲームの状態(０落下するものがない状態１落下中)
	int S_state = 0;
	int S_xy[4][4] = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };//
	int score = 0;		//得点
	int scene = 0;		//シーン(０：タイトル、１：ゲーム本編、２：ゲームリザルト)
	bool hold_bool = true;
	int hold= 8;

	int Tetromino_loop_count = 0;
	int TetrominoType;		//テトリミノの種類(0～6)
	std::vector<int> TetrominoType_array;
	int TetrominoType_next;	//次のテトリミノの種類(0～6)
	int TetrominoType_next3[3];
	int TetrominoDriection;	//テトリミノの向き(0～3)

	int TetrominoData[7][4][4][4] ={
		{//	直線
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
		{	//四角
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
		{	//S字
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
		{	//逆S字
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
		{	//逆L字
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
		{	//L字
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
		{	//T字
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

	