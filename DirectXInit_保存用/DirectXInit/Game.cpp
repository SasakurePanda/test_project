#include "Game.h"
#include<time.h>
extern int Move_g_key[4];
int Count;			//秒数をカウントする用の変数
int BG_Count = 0;
int Count_Max;		//カウントの最大値を各画像で設定する変数
int Restart_happy = 1;	//二回アニメーションを繰り返す用の変数
int Restart_hurehure = 5;
int color = -1;

void TetrominoSpin();

void Game::Init(HWND hWnd)
{
	srand((unsigned)time(NULL));
	D3D_Create(hWnd);
	sound.Init();
	sound.Play(SOUND_LABEL_BGM000);//BGMを鳴らす

	
	player.Init(L"asset/speed.png", 3, 0);
	player.SetPos(0.0f, 0.0f, 0.0f);
	player.SetSize(100.0f, 100.0f, 0.0f);
	player.SetAngle(0.0f);
	//背景
	background.Init(L"asset/back_img_02.png");
	background.SetPos(0.0f, 0.0f, 0.0f);
	background.SetSize(640.0f, 480.0f, 0.0f);
	background.SetAngle(0.0f);

	//speed
	speed.Init(L"asset/speed.png",3,0);
	speed.SetPos(-220.0f, -80.0f, 0.0f);
	speed.SetSize(180.0f, 150.0f, 0.0f);
	speed.SetAngle(0.0f);

	blocks_next.Init(L"asset/block_next.png",7,1);
	blocks_next.SetPos(240.0f, 140.0f, 0.0f);
	blocks_next.SetSize(120.0f, 120.0f, 0.0f);
	blocks_next.SetAngle(0.0f);

	blocks_hold.Init(L"asset/block_hold.png", 8, 1);
	blocks_hold.SetPos(-220.0f, 70.0f, 0.0f);
	blocks_hold.SetSize(120.0f, 120.0f, 0.0f);
	blocks_hold.SetAngle(0.0f);

	for (int x = 0; x < STAGE_X; x++)//最初に二次元配列の全部に初期化として情報を入力する
	{
		for (int y = 0; y < STAGE_Y; y++)
		{
			blocks[x][y].Init(L"asset/block.png");//ブロックを初期化
			blocks[x][y].SetPos(BLOCK_SIZE * (x - STAGE_X / 2), BLOCK_SIZE * (y - STAGE_Y / 2), 0.0f);//位置を設定
			blocks[x][y].SetSize(BLOCK_SIZE, BLOCK_SIZE, 0.0f);//大きさを設定
		}
	}

	scoreboard.Init(L"asset/number.png", 10, 1);
	scoreboard.SetPos(-145.0f, 200.0f, 0.0f);
	scoreboard.SetSize(40.0f, 40.0f, 0.0f);
	score = 0;			//得点を初期化

	title.Init(L"asset/title_img_01.png");
	title.SetPos(0.0f, 0.0f, 0.0f);
	title.SetSize(640.0f, 480.0f, 0.0f);
	scene = 0;			//シーンを初期化
}

void Game::Update(void)
{
	static const int Tetromino_TwoLoop = 14;//七種で二巡するまでの回数
	input.Update();
	
	switch (scene)
	{
	case SCENE_TITLE://タイトル画面
		//キー入力で本編で
		if (input.GetKeyTrigger(VK_RETURN))//エンターキーを押されると
		{
			scene = SCENE_GAME;//シーン変化
			//data配列を初期化
			for (int x = 0; x < STAGE_X; x++)//全部の二次元配列を初期化
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					data[x][y] = 0;
				}
			}

			state = STATE_NEWTETROMINO;//ゲーム状態を初期化
			score = 0;//得点を初期化
			hold = NO_HOLD;
			hold_bool = true;
			blocks_hold.numU = NO_HOLD;
			TetrominoType_array = { //一巡目
									TetrominoType_I,
									TetrominoType_O,
									TetrominoType_S,
									TetrominoType_Z,
									TetrominoType_J,
									TetrominoType_L,
									TetrominoType_T,
									//二巡目
									TetrominoType_S,
									TetrominoType_O,
									TetrominoType_I,
									TetrominoType_Z,
									TetrominoType_L,
									TetrominoType_J,
									TetrominoType_T };//テトリミノのサイズを入れておくVector配列
			/*TetrominoType_next3[0] = rand() % 7;
			TetrominoType_next3[1] = rand() % 7;
			TetrominoType_next3[2] = rand() % 7;*/
			Tetromino_loop_count = 0;

		}
		break;
	case SCENE_GAME:
		//プレイヤ—のアニメーション
		if (Count >= FRAME_LATE)
		{
			Count = 0;
		}

		DirectX::XMFLOAT3 pos = player.GetPos();
		/*if (input.GetKeyPress(VK_W)) { pos.y += 1.0f; }
		if (input.GetKeyPress(VK_A)) { pos.x -= 1.0f; }
		if (input.GetKeyPress(VK_S)) { pos.y -= 1.0f; }
		if (input.GetKeyPress(VK_D)) { pos.x += 1.0f; }
		player.SetPos(pos.x, pos.y, pos.z);*/

		//ゲームオーバーならゲームの進行を止める
		if (state == STATE_GAMEOVER)
		{
			//キー入力でタイトル画面に戻る
			if (input.GetKeyTrigger(VK_RETURN))
			{
				scene = SCENE_TITLE;
			}
			return;
		}
		//新たにテトリミノを出現させる
		if (state == STATE_NEWTETROMINO)
		{
			state = STATE_NOWDROP;
			TetrominoType = TetrominoType_next3[0];
			Tetromino_loop_count++;

			/*TetrominoType = TetrominoType_next3[0];//0～7をランダムで取得
		
			TetrominoType_next3[0] = TetrominoType_next3[1];	  //nextで待機していた子を入れる
			TetrominoType_next3[1] = TetrominoType_next3[2];	  //nextで待機していた子を入れる
			TetrominoType_next3[2] = rand() % 7;	  //nextで待機していた子を入れる
			blocks_next.numU = TetrominoType_next;*/
			//TetrominoType = TetrominoType_array[Tetromino_loop_count];//0～7をランダムで取得
			if (Tetromino_loop_count >= Tetromino_TwoLoop)
			{
				TetrominoType_next3[0] = TetrominoType_array[0];	  //nextで待機していた子を入れる
			}
			else
			{
				TetrominoType_next3[0] = TetrominoType_array[Tetromino_loop_count];
			}

			if (Tetromino_loop_count == TetrominoTypeSize)//一巡目が終わったら
			{
				int tmp;
				int j = rand() % TetrominoTypeSize;
				for (int i = 0; i < TetrominoTypeSize; i++)//一巡目で使った配列部分を入れ替える
				{
					tmp = TetrominoType_array[i];
					TetrominoType_array[i] = TetrominoType_array[j];
					TetrominoType_array[j] = tmp;
					j = rand() % TetrominoTypeSize;
				}
			}

			if (Tetromino_loop_count == Tetromino_TwoLoop)//二巡目が終わったら
			{
				int tmp;
				int j = rand() % TetrominoTypeSize;
				for (int i = 7; i < 14; i++)//一巡目で使った配列部分を入れ替える
				{
					tmp = TetrominoType_array[i];
					TetrominoType_array[i] = TetrominoType_array[j + 7];
					TetrominoType_array[j + TetrominoTypeSize] = tmp;
					j = rand() % TetrominoTypeSize;
				}

				Tetromino_loop_count = 0;
			}
			//TetrominoType_next3[1] = TetrominoType_array[Tetromino_loop_count + 2];	  //nextで待機していた子を入れる
			//TetrominoType_next3[2] = TetrominoType_array[Tetromino_loop_count + 3];	  //nextで待機していた子を入れる
			//blocks_next.numU = TetrominoType_next;

			switch (TetrominoType)
			{
			case TetrominoType_I:
				data[4][19] = FALLING;
				data[5][19] = FALLING;
				data[6][19] = FALLING;
				data[7][19] = FALLING;
				color = 0;
				break;
			case TetrominoType_O:
				data[5][20] = FALLING;
				data[6][20] = FALLING;
				data[5][19] = FALLING;
				data[6][19] = FALLING;
				color = 1;
				break;
			case TetrominoType_S:
				data[4][19] = FALLING;
				data[5][19] = FALLING;
				data[5][20] = FALLING;
				data[6][20] = FALLING;
				color = 2;
				break;
			case TetrominoType_Z:
				data[4][20] = FALLING;
				data[5][20] = FALLING;
				data[5][19] = FALLING;
				data[6][19] = FALLING;
				color = 3;
				break;
			case TetrominoType_J:
				data[4][20] = FALLING;
				data[4][19] = FALLING;
				data[5][19] = FALLING;
				data[6][19] = FALLING;
				color = 4;
				break;
			case TetrominoType_L:
				data[4][19] = FALLING;
				data[5][19] = FALLING;
				data[6][19] = FALLING;
				data[6][20] = FALLING;
				color = 5;
				break;
			case TetrominoType_T:
				data[4][19] = FALLING;
				data[5][19] = FALLING;
				data[6][19] = FALLING;
				data[5][20] = FALLING;
				color = 6;
				break;
			}
			count = 0;
			TetrominoDriection = 0;
		}

		/*//回転
		if (input.GetKeyTrigger(VK_UP))//上キーを押すと回転するよ
		{
			//向きを変更する
			int newDirection = TetrominoDriection + 1;//向きの指定変数の変更
			if (newDirection > 3)//回り切ったら戻る
			{
				newDirection = 0;
			}
			//基準点を調べる
			int baseX = -1, baseY = -1;//基準点を探す
			for (int x = 0; x < STAGE_X; x++)
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[x][y] == FALLING)
					{
						for (int tx = 0; tx < 4; tx++)
						{
							for (int ty = 0; ty < 4; ty++)
							{
								if (TetrominoData[TetrominoType][TetrominoDriection][ty][tx] == 2)
								{
									baseX = x - tx;
									baseY = y - ty;
									break;
								}
							}
							if (baseX >= 0) { break; }
						}
						break;
					}
				}
				if (baseX >= 0) { break; }
			}
			//回転が出来るかを調べる
			bool ngFg = false;
			for (int tx = 0; tx < 4; tx++)
			{
				for (int ty = 0; ty < 4; ty++)
				{
					if (TetrominoData[TetrominoType][newDirection][ty][tx] == 2 &&
						(data[baseX + tx][baseY + ty] == FALLED ||
							baseX + tx > STAGE_X - 1 ||
							baseY + ty > STAGE_Y - 1 ||
							baseX + tx < 0 ||
							baseY + ty < 0))
					{
						ngFg = true;
						break;

					}
				}
				if (ngFg == true)
				{
					break;
				}
			}
			//回転が可能なら
			if (ngFg == false)
			{
				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							data[x][y] = BLANK;
						}
					}
				}
				for (int tx = 0; tx < 4; tx++)
				{
					for (int ty = 0; ty < 4; ty++)
					{
						if (TetrominoData[TetrominoType][newDirection][ty][tx] == 2)
						{
							data[baseX + tx][baseY + ty] = FALLING;
						}
					}
				}
				TetrominoDriection = newDirection;//方向を更新
			}
		}*/
		//下移動
		if (input.GetKeyPress(VK_DOWN))
		{
			count = 31;
		}

		//ブロックを落とす処理
		count++;
		if (score < 1000)
		{
			count_set = 30;
		}
		else if (score < 2000)
		{
			count_set = 20;
		}
		else if (score > 3000)
		{
			count_set = 10;
		}
		if (count > count_set)
		{
			count = 0;

			//揃っていれば、その行より上の行を一列ずつずらす
			if (state == STATE_DELETE)
			{
				//一列揃っているかを確認
				int cnt = 0;
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[0][y] == FILLED)
					{
						for (int ty = y + 1; ty < STAGE_Y + 3; ty++)
						{
							for (int tx = 0; tx < STAGE_X; tx++)
							{
								data[tx][ty - 1] = data[tx][ty];
							}
						}
						y--;
						cnt++;
					}

				}
				sound.Play(SOUND_LABEL_SE001);
				//得点を加算する
				switch (cnt)
				{
				case 1: score += 100; break;//シングル(一列消し)
				case 2: score += 300; break;//ダブル　(二列消し)
				case 3: score += 500; break;//トリプル(三列消し)
				case 4: score += 800; break;//テトリス(四列消し)
				default:
					break;
				}

				state = STATE_NEWTETROMINO;

			}

			//落とすことが出来るかどうかを確かめる
			bool ngFg = false;//着地する何かしらの理由で止まる
			for (int x = 0; x < STAGE_X; x++)
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[x][y] == FALLING && (y == 0 || data[x][y - 1] == FALLED))
					{
						ngFg = true;
						break;
					}
				}
				if (ngFg == true)
				{
					break;
				}
			}

			//落とすことが不可能なら
			if (ngFg == true)
			{
				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							data[x][y] = FALLED;
						}
					}
				}
				state = STATE_NEWTETROMINO;

				//一列揃った行がないかをチェックする
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					bool completeFg = true;
					for (int x = 0; x < STAGE_X; x++)
					{
						if (data[x][y] == BLANK)
						{
							completeFg = false;
							break;
						}
					}

					//揃っていたら色を付ける
					if (completeFg == true)
					{
						for (int x = 0; x < STAGE_X; x++)
						{
							data[x][y] = FILLED;
						}

						//ゲーム状態を「２：揃った状態にする」に変更する
						state = STATE_DELETE;
					}
				}
				//ゲームオーバーになっていないかチェックする
				for (int x = 0; x < STAGE_X; x++)
				{
					if (data[x][STAGE_Y - 1] == FALLED)
					{
						//テトリミノの色を変える
						for (int tx = 0; tx < STAGE_X; tx++)
						{
							for (int ty = 0; ty < STAGE_Y + 3; ty++)
							{
								if (data[tx][ty] == FALLED)
								{
									data[tx][ty] = 4;
								}
							}
						}
						//ゲームの状態を「３：ゲームオーバー」に変更
						state = STATE_GAMEOVER;
						break;
					}

				}
			}
			else
			{
				int tmp[STAGE_X][STAGE_Y + 3] = {};
				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							tmp[x][y - 1] = FALLING;
						}
						else if (data[x][y] == FALLED)
						{
							tmp[x][y] = FALLED;
						}

					}
				}
				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						data[x][y] = tmp[x][y];
					}
				}

			}
		}
		//左移動
		if (input.GetKeyTrigger(VK_LEFT))
		{
			//左移動が可能になる
			bool ngFg = false;
			for (int x = 0; x < STAGE_X; x++)//ｘを０から１ずつ増やしSTAGE_Xより小さい間
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[x][y] == FALLING && (x == 0 || data[x - 1][y] == FALLED))
					{
						ngFg = true;
						break;
					}
				}
				if (ngFg == true) { break; }
			}

			//左移動が可能なら
			if (ngFg == false)
			{
				int tmp[STAGE_X][STAGE_Y + 3] = {};
				for (int x = 0; x < STAGE_X; x++)//ｘを０から１ずつ増やしSTAGE_Xより小さい間
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							tmp[x - 1][y] = FALLING;
						}
						else if (data[x][y] == FALLED)
						{
							tmp[x][y] = FALLED;
						}

					}
				}

				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						data[x][y] = tmp[x][y];
					}
				}
			}

		}
		//右移動
		if (input.GetKeyTrigger(VK_RIGHT))
		{
			bool ngFg = false;
			for (int x = 0; x < STAGE_X; x++)//ｘを０から１ずつ増やしSTAGE_Xより小さい間
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[x][y] == FALLING && (x == STAGE_X - 1 || data[x + 1][y] == FALLED))
					{
						ngFg = true;
						break;
					}
				}

				if (ngFg == true) { break; }
			}


			if (ngFg == false)
			{
				int tmp[STAGE_X][STAGE_Y + 3] = {};
				for (int x = 0; x < STAGE_X; x++)//ｘを０から１ずつ増やしSTAGE_Xより小さい間
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							tmp[x + 1][y] = FALLING;
						}
						else if (data[x][y] == FALLED)
						{
							tmp[x][y] = FALLED;
						}

					}
				}

				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						data[x][y] = tmp[x][y];
					}
				}
			}

		}

		if (input.GetKeyTrigger(VK_A))//holdに入れる
		{
			if(hold_bool == true)
			{
				//左移動が可能なら
				hold = TetrominoType;
				hold_bool = false;
				int tmp[STAGE_X][STAGE_Y + 3] = {};
				for (int x = 0; x < STAGE_X; x++)//ｘを０から１ずつ増やしSTAGE_Xより小さい間
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							tmp[x][y] = BLANK;
						}
						if (data[x][y] == FALLED)
						{
							tmp[x][y] = FALLED;
						}
					}
				}

				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						data[x][y] = tmp[x][y];
					}
				}
				state = STATE_NEWTETROMINO;
				blocks_hold.numU = hold;
			}
		}

		if (input.GetKeyTrigger(VK_S))//holdしたのもをだす
		{
			if (hold_bool == false)
			{
				//左移動が可能な
				hold_bool = true;
				int tmp[STAGE_X][STAGE_Y + 3] = {};
				for (int x = 0; x < STAGE_X; x++)//ｘを０から１ずつ増やしSTAGE_Xより小さい間
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						if (data[x][y] == FALLING)
						{
							tmp[x][y] = BLANK;
						}
						if (data[x][y] == FALLED)
						{
							tmp[x][y] = FALLED;
						}
					}
				}

				for (int x = 0; x < STAGE_X; x++)
				{
					for (int y = 0; y < STAGE_Y + 3; y++)
					{
						data[x][y] = tmp[x][y];
					}
				}

				TetrominoType_next3[0] = hold;
				hold = 7;
				blocks_hold.numU = hold;
				state = STATE_NEWTETROMINO;
			}
			
		}

		if (state == STATE_NOWDROP)
		{
			//int tmp[STAGE_X][STAGE_Y + 3] = {};
			int count_stop = 23;

			for (int x = 0; x < STAGE_X; x++)
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[x][y] == FALLING)//落下中のテトリミノがあったら
					{
						int i = 0;
						int j = 0;
						//落下し終わったテトリミノか地面に当たるまで何マスかを数える
						for (i = y; data[x][i-1] != 1 && i > 0; i--)
						{
							j++;//止まるまでのマス数を数える
						}

						if (j <= count_stop)//新しく数えたマス数が今登録されている止まるまでのマス数より少ないなら
						{
							count_stop = j;//入れ替える
						}
					}
				}
			}

			int i = 0;
			for (int x = 0; x < STAGE_X; x++)
			{
				for (int y = 0; y < STAGE_Y + 3; y++)
				{
					if (data[x][y] == FALLING)//落下中の手取り見のがあったら
					{
						S_xy[0][i] = x;					//Ｘはそのままで()
						S_xy[1][i] = y - count_stop;	//Ｙの場所から止まるまでのマス分下を記録する
						//   [0][1][2][3]
						//[0] X  X  X  X
						//[1] Y  Y  Y  Y

						i++;							//進める
					}

				}
			}
		}
		bool stop_color_flag = true;
		//色を付ける
		for (int x = 0; x < STAGE_X; x++)
		{
			for (int y = 0; y < STAGE_Y; y++)
			{
				switch (data[x][y])
				{
				case BLANK://空白
					for (int i = 0;i < 4;i++)
					{
						if (x == S_xy[0][i] && y == S_xy[1][i])			  //落下予定のマスなら
						{
							blocks[x][y].SetColor(1.0f, 0.4f, 0.2f, 0.4f);//色を付ける
							stop_color_flag = false;
							break;
						}

						if (i == 3)			//4回色を付けたら
						{
							stop_color_flag = true;
						}
					}
					if (stop_color_flag == true)
					{
						blocks[x][y].SetColor(1.0f, 1.0f, 1.0f, 0.2f);
					}
					
					break;
				case FALLED://着地しているテトリミノ
					blocks[x][y].SetColor(0.8f, 1.0, 0.5f, 1.0f);
					break;
				case FALLING://移動しているテトリミノ
					switch (color)
					{
					case 0:
						blocks[x][y].SetColor(1.0f, 0.6f, 1.0f, 1.0f);//I型
						break;
					case 1:
						blocks[x][y].SetColor(0.3f, 1.0f, 0.7f, 1.0f);
						break;
					case 2:
						blocks[x][y].SetColor(0.5f, 0.8f, 1.0f, 1.0f);
						break;
					case 3:
						blocks[x][y].SetColor(1.0f, 1.0f, 0.2f, 1.0f);
						break;
					case 4:
						blocks[x][y].SetColor(0.0f, 1.0f, 1.0f, 1.0f);
						break;
					case 5:
						blocks[x][y].SetColor(1.0f, 0.0f, 1.0f, 1.0f);
						break;
					case 6:
						blocks[x][y].SetColor(1.0f, 0.0f, 0.0f, 1.0f);
						break;
					default:
						break;
					}
					break;
				case FILLED://揃ったテトリミノ
					blocks[x][y].SetColor(0.4f, 0.4f, 1.0f, 1.0f);
					break;
				case 4://ゲームオーバー
					blocks[x][y].SetColor(1.0f, 0.2f, 0.2f, 1.0f);
					break;
				case 5://どこに落ちるかの予想テトリミノ
					blocks[x][y].SetColor(1.0f, 0.4f, 0.2f, 0.7f);
					break;
				}

			}
		}
		player.numU = Count / 20;
		Count++;
		break;

	/*case 2:
		while (score <= 0)
		{
			

		}*/
	}
}

void Game::Draw(void)
{
	D3D_StartRender();//最初の描画準備処理

	switch (scene)
	{
	case SCENE_TITLE://タイトル画面
		title.Draw();
		break;
	case SCENE_GAME://ゲーム本編
		background.Draw();
		player.Draw();

		blocks_next.numU = TetrominoType_next3[0];
		blocks_next.Draw();
		speed.numU = score / 1000;
		speed.Draw();

		/*blocks_next.SetPos(240.0f, 30.0f, 0.0f);
		blocks_next.numU = TetrominoType_next3[1];
		blocks_next.Draw();

		blocks_next.SetPos(240.0f, -100.0f, 0.0f);
		blocks_next.numU = TetrominoType_next3[2];
		blocks_next.Draw();

		blocks_next.SetPos(240.0f, 140.0f, 0.0f);
		*/
		blocks_hold.Draw();
		//ブロックを表示
		for (int x = 0; x < STAGE_X; x++)
		{
			for (int y = 0; y < STAGE_Y; y++)
			{
				blocks[x][y].Draw();
			}
		}
		DirectX::XMFLOAT3 pos = scoreboard.GetPos();
		DirectX::XMFLOAT3 size = scoreboard.GetSize();
		int keta = 0;
		do
		{
			scoreboard.numU = score % (int)pow(10, keta + 1) / (int)pow(10, keta);//一桁を切り出す
			scoreboard.SetPos(pos.x - size.x * keta, pos.y, pos.z);//位置を設定
			scoreboard.Draw();
			keta++;
		} while (score >= (int)pow(10, keta));
		scoreboard.SetPos(pos.x, pos.y, pos.z);//位置を元にもどす

		break;

	}
	D3D_FinishRender();//画面の切り替え;更新処理
}

void Game::Uninit(void)
{
	player.Uninit();//メモリ開放のやつ
	background.Uninit();//メモリ開放のやつ

	for (int x = 0; x < STAGE_X; x++)
	{
		for (int y = 0; y < STAGE_Y; y++)
		{
			blocks[x][y].Uninit();
		}
	}

	scoreboard.Uninit();//スコア終了
	sound.Uninit();
	title.Uninit();	//タイトルを終了
	D3D_Release();//最後の最後での解放
}

void TetrominoSpin()
{



}