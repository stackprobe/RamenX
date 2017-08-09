/*
	スクリーンサイズは 800 x 600 とする。

	float, double をゼロクリアした値 (な初期値) は 0.0 とする。
	IEEE754って全CPUが準拠してるかどうか知らないけど、してると信じる。
*/

#include "Stdinc.h"

singleton(EnemyLv1PictureGroup, PictureGroup_t, MakePictureGroup(SIF_EnemyLv1, NULL))
singleton(EnemyLv2PictureGroup, PictureGroup_t, MakePictureGroup(SIF_EnemyLv2, NULL))
singleton(EnemyLv3PictureGroup, PictureGroup_t, MakePictureGroup(SIF_EnemyLv3, NULL))

singleton(Kuma00PicGrp, PictureGroup_t, MakePictureGroup(SIF_Kuma00, NULL))
singleton(Kuma01PicGrp, PictureGroup_t, MakePictureGroup(SIF_Kuma01, NULL))
singleton(Kuma02PicGrp, PictureGroup_t, MakePictureGroup(SIF_Kuma02, NULL))
singleton(Kuma03PicGrp, PictureGroup_t, MakePictureGroup(SIF_Kuma03, NULL))

static double SMWATarget = 0.0;
static double SMWA = 0.0;

static void DrawMenuWall(void)
{
	m_countApprox(SMWA, SMWATarget, 0.95, 0.001);

	if(SMWA <= 0.0)
	{
		SimpleDrawPic(0, 0, Pic(P_TITLEWALL));
	}
	else if(1.0 <= SMWA)
	{
		SimpleDrawPic(0, 0, Pic(P_SUBMENUWALL));
	}
	else
	{
		SimpleDrawPic(0, 0, Pic(P_TITLEWALL));
		SetAlpha(SMWA);
		SimpleDrawPic(0, 0, Pic(P_SUBMENUWALL));
		ResetBlend();
	}
}
static void Logo(void)
{
#if 0
	if(Sd.FullScreenMode)
	{
		forscene(90)
		{
			SetBright(0.0, 0.0, 0.0);
			DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
			ResetBright();

			GTEndFrame();
		}
	}
#endif
	GetHandle(Mus_Res(), MUSIC_TITLE);

	forscene(60)
	{
		SetBright(0.0, 0.0, 0.0);
		DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
		ResetBright();

		DrawPic(400.0, 300.0, Pic(P_LOGO));

		SetAlpha(1.0 - fs_rate);
		SetBright(0.0, 0.0, 0.0);
		DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
		ResetBlend();
		ResetBright();

		GTEndFrame();
	}
	int bfrTch = GetNowCount();

	forscene(90)
	{
		if(bfrTch + fs_max * 17 < GetNowCount())
		{
			break;
		}
		if(fs_count == 1)
		{
			TouchResources();
		}
		SetBright(0.0, 0.0, 0.0);
		DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
		ResetBright();

		DrawPic(400.0, 300.0, Pic(P_LOGO));

		GTEndFrame();
	}
	forscene(60)
	{
		if(fs_count == fs_max - 30)
//		if(fs_count == fs_max - 30 && Sd.FirstBootEnded)
		{
			MusicPlay(MUSIC_TITLE);
		}

		SetBright(0.0, 0.0, 0.0);
		DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
		ResetBright();

		DrawPic(400.0, 300.0, Pic(P_LOGO));

		SetAlpha(fs_rate);
		SetBright(0.0, 0.0, 0.0);
		DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
		ResetBlend();
		ResetBright();

		GTEndFrame();
	}
}

typedef struct ImagePos_st
{
	int PicResno;
	double X;
	double Y;
	double A;
	struct {
		double X;
		double Y;
		double A;
	}
	CurrPos;
}
ImagePos_t;

static ImagePos_t MakeImagePos(int pic_resno, double x, double y, double a, double dest_a = 1.0)
{
	ImagePos_t i;

	i.PicResno = pic_resno;
	i.X = x;
	i.Y = y;
	i.A = dest_a;
	i.CurrPos.X = x;
	i.CurrPos.Y = y;
	i.CurrPos.A = a;

	return i;
}
static void ActImage(ImagePos_t *i)
{
	m_countApprox(i->CurrPos.X, i->X, 0.90, 0.0);
	m_countApprox(i->CurrPos.Y, i->Y, 0.90, 0.0);
	m_countApprox(i->CurrPos.A, i->A, 0.95, 0.0);

	SetAlpha(i->CurrPos.A);
	SimpleDrawPic(i->CurrPos.X, i->CurrPos.Y, Pic(i->PicResno));
	ResetBlend();
}

static void WindowSizeConfig(void)
{
	GTFreezeInput();

	for(; ; )
	{
		int changed = 0;

		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			Sd.ScreenZoom -= 0.1;
			changed = 1;
		}
		else if(GTGetInput(INP_DIR_4, INP_HIT))
		{
			Sd.ScreenZoom -= 0.01;
			changed = 1;
		}
		else if(GTGetInput(INP_DIR_6, INP_HIT))
		{
			Sd.ScreenZoom += 0.01;
			changed = 1;
		}
		else if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			Sd.ScreenZoom += 0.1;
			changed = 1;
		}
		else if(GTGetInput(INP_A, INP_DOWN))
		{
			break;
		}
		else if(GTGetInput(INP_B, INP_DOWN))
		{
			Sd.ScreenZoom = 1.0;
			changed = 1;
		}
		if(changed)
		{
			Sd.ScreenZoom = m_d2i(Sd.ScreenZoom * 100.0) / 100.0;
			m_range(Sd.ScreenZoom, 0.1, 10.0);
			ApplyWindowSize();
		}
		DrawMenuWall();
		GTEarlyCurtain();

		PE_Border(GetColor(0, 0, 192));
		SetPrint(50, 50, 40);
		Print("ウィンドウのサイズ");
		PrintRet();
		PrintRet();
		Print_x(xcout("　× %.2f　(0.10～10.00)", Sd.ScreenZoom));
		PrintRet();
		PrintRet();
		Print_x(xcout("　★ 左(下):縮小, 右(上):拡大, キャンセル:等倍, 決定:戻る"));
		/*
		PrintRet();
		Print_x(xcout("　★ 拡大・縮小すると画面が粗くなることがあります。"));
		*/

		GTEndFrame();
	}
	clsDx();
}
static void PadConfig2(int **pPadBtns, char **btnNames, int padBtnNum)
{
	void *backup = memClone(&Sd.PadBtn, sizeof(Sd.PadBtn));
	int frmcnt = 0;
	int btnndx = 0;

	while(btnndx < padBtnNum)
	{
		if(frmcnt)
		{
			if(GetKeyDown(KEY_INPUT_SPACE))
			{
				memcpy(&Sd.PadBtn, backup, sizeof(Sd.PadBtn)); // 復元
				break;
			}
			for(int padno = 0; padno < PAD_MAX; padno++)
			{
				if(GetPadDown(padno))
				{
					GTPrimaryPadno = padno;
					break;
				}
			}
			int status = GetPadDown(GTPrimaryPadno);

			if(status)
			{
				int btnno;

				for(btnno = 0; btnno < 32; btnno++)
				{
					if((uint)status & (1u << btnno))
					{
						break;
					}
				}
				*pPadBtns[btnndx] = btnno;
				btnndx++;
			}
		}
		DrawMenuWall();
		GTEarlyCurtain();

		PE_Border(GetColor(128, 64, 64));
		SetPrint(50, 40, 35);
		Print("パッドのボタン設定");
		PrintRet();
		PrintRet();

		for(int index = 0; index < padBtnNum; index++)
		{
			Print_x(xcout("　[ %s ] ボタン", btnNames[index]));

			if(index < btnndx)
			{
				Print_x(xcout("　%d", *pPadBtns[index]));
			}
			else if(index == btnndx)
			{
				Print("　<- いまここ");
			}
			PrintRet();
		}
		PrintRet();
		Print("★ スペースキーを押すとキャンセルします。(それまでの入力は破棄します)");
		PrintRet();
		Print("★ 左右ボタンを二連打でもダッシュできます。");
		PE_Reset();

		GTEndFrame();

		frmcnt++;
	}
	clsDx();

	memFree(backup);
}
static void PadConfig(void)
{
	int *pPadBtns[] =
	{
		&Sd.PadBtn.Dir2,
		&Sd.PadBtn.Dir4,
		&Sd.PadBtn.Dir6,
		&Sd.PadBtn.Dir8,
		&Sd.PadBtn.A,
		&Sd.PadBtn.B,
		&Sd.PadBtn.C,
		&Sd.PadBtn.D,
		&Sd.PadBtn.E,
//		&Sd.PadBtn.F,
//		&Sd.PadBtn.L,
//		&Sd.PadBtn.R, // 廃止
		&Sd.PadBtn.Pause,
	};
	char *btnNames[lengthof(pPadBtns)] =
	{
		"下",
		"左",
		"右",
		"上",
		"ジャンプ／決定",
		"ダッシュ／飛翔／キャンセル",
		"弱攻撃",
		"強攻撃",
		"ボム",
//		"低速移動", // 廃止
		"ポーズ",
	};
	int padBtnNum = lengthof(pPadBtns);

#if 0 // 廃止
	if(Sd_PlayChara != PLAY_CHARA_02)
	{
		pPadBtns[9] = pPadBtns[10];
		btnNames[9] = btnNames[10];
		padBtnNum = 10;
	}
#endif
	PadConfig2(pPadBtns, btnNames, padBtnNum);
}
static void VolumeConfig(int bgmMode)
{
	double *p_volume = bgmMode ? &Sd.MusicMasterVolume : &Sd.SEMasterVolume;

	GTFreezeInput();

	for(; ; )
	{
		if(!bgmMode && LFrmCnt % 80 == 0)
		{
			switch(rnd(3))
			{
			case 0: SEPlay(SE_KAIFUKU); break;
			case 1: SEPlay(SE_ADD_BOMB); break;
			case 2: SEPlay(SE_ADD_ZANKI); break;

			default:
				error();
			}
		}
		int changed = 0;

		if(GTGetInput(INP_DIR_4, INP_HIT))
		{
			*p_volume -= 0.01;
			changed = 1;
		}
		else if(GTGetInput(INP_DIR_6, INP_HIT))
		{
			*p_volume += 0.01;
			changed = 1;
		}
		else if(GTGetInput(INP_A, INP_DOWN) || GTGetInput(INP_B, INP_DOWN))
		{
			break;
		}
		if(changed)
		{
			m_range(*p_volume, 0.0, 2.0);

			if(bgmMode) UpdateMusicVolume();
			else        UpdateSEVolume();
		}
		DrawMenuWall();
		GTEarlyCurtain();

		PE_Border(GetColor(64, 128, 64));
		SetPrint(50, 50, 40);
		Print_x(xcout("%s音量", bgmMode ? "ＢＧＭ" : "ＳＥ"));
		PrintRet();
		PrintRet();
		Print_x(xcout("　%.2f　(0.00～2.00)", *p_volume));
		PrintRet();
		PrintRet();
		Print("★ 左:音量下げ, 右:音量上げ, キャンセル:戻る, 決定:戻る");
		PrintRet();
		Print("★ デフォルト＝0.95");
		PE_Reset();

		GTEndFrame();
	}
	clsDx();
}
static void KumaSelectMenu(void)
{
	int uiSdPc;

	switch(Sd_PlayChara)
	{
	case 0: uiSdPc = 2; break;
	case 1: uiSdPc = 1; break;
	case 2: uiSdPc = 0; break;
	case 3: uiSdPc = 3; break;

	default:
		error();
	}

	GTFreezeInput();
	GTSetCurtain(0.0, 60);

	int selectMax = PLAY_CHARA_NUM;
	int selectIndex = uiSdPc;
	int oldSelectedIndex = uiSdPc;

	for(; ; )
	{
		if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex--;
		}
		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex++;
		}
		m_range(selectIndex, 0, selectMax - 1);

		if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == oldSelectedIndex)
			{
				break;
			}
			selectIndex = oldSelectedIndex;
		}
		if(GTGetInput(INP_A, INP_DOWN))
		{
			break;
		}
		SimpleDrawPic(0, 0, Pic(P_KUMASELECTWALL));

		int curX = 80;
		int curY;

		switch(selectIndex)
		{
		case 0: curY = 250; break;
		case 1: curY = 350; break;
		case 2: curY = 430; break;
		case 3: curY = 512; break;

		default:
			error();
		}
		DrawBegin(curX, curY, Pic(P_CURSOR));
		DrawZoom(1.5);
		DrawRotate(LFrmCnt / 10.0);
		DrawEnd();

		GTEndFrame();
	}
	uiSdPc = selectIndex;

	switch(uiSdPc)
	{
	case 0: Sd_PlayChara = PLAY_CHARA_02; break;
	case 1: Sd_PlayChara = PLAY_CHARA_01; break;
	case 2: Sd_PlayChara = PLAY_CHARA_00; break;
	case 3: Sd_PlayChara = PLAY_CHARA_03; break;
	}

	GTWaitFadeout(P_KUMASELECTWALL, -1.0, 30);
}
void Pub_KumaSelectMenu(void)
{
	forscene(60)
	{
		GTEndFrame();
	}
	GTActWhiteLevel = -1.0;
	KumaSelectMenu();
	GTActWhiteLevel = 0.0;
}
static void OtherConfig(void)
{
	const int selectMax = 4;
	int selectIndex = 0;

restartLoop:
	GTFreezeInput();
	GTSetCurtain(0.0, 60);
	SMWATarget = 1.0;
	LFrmCnt= 0;

	for(; ; )
	{
		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex++;
		}
		if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex--;
		}
		m_range(selectIndex, 0, selectMax - 1);

		if(GTGetInput(INP_A, INP_DOWN))
		{
			switch(selectIndex)
			{
			case 0:
				GTWaitFadeout(P_SUBMENUWALL, -1.0, 30);
				KumaSelectMenu();
				goto restartLoop;

			case 1:
				{
					forscene(60)
					{
						switch(fs_count)
						{
						case 0:
						case 10:
						case 20:
							SEPlay(SE_DEATH_BOSS_0001);
						}

						SetBright(0, 0, 0);
						DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));
						ResetBright();

						DrawBegin(400, 300, Pic(P_BOMB));
						DrawZoom(0.5 + rnd(2000) / 1000.0);
//						DrawRotate(fs_rate);
						DrawEnd();

						GTEndFrame();
					}

#if 1 // ぜんぶ
					for(int playChara = 0; playChara < PLAY_CHARA_NUM; playChara++)
						*Get_P_ClearStageIndex(playChara) = STAGE_NUM;
#else // カレントしろくまのみ
					*Get_P_ClearStageIndex() = STAGE_NUM;
#endif;
				}
				goto endMenu;
				//break;

			case 2:
				Sd.SysTuneEnabled = !Sd.SysTuneEnabled;
				break;

			case 3:
				goto endMenu;

			default:
				error();
			}
		}
		if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
		}

		DrawMenuWall();
		GTEarlyCurtain();

		PE_Border(GetColor(128, 96, 128));
		SetPrint(50, 50, 40);
		Print("その他の設定");
		PrintRet();
		PrintRet();
		int c = 0;
		Print_x(xcout("%c しろくまの変更", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("%c 全ステージ開放", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("%c ポーズ時にシステムの調整 ( 現在は : %s )", selectIndex == c++ ? '>' : ' ', Sd.SysTuneEnabled ? "オン" : "オフ"));
		PrintRet();
		Print_x(xcout("%c 戻る", selectIndex == c++ ? '>' : ' '));

		GTEndFrame();
	}
endMenu:;
}
static void Configuration(void)
{
	const int selectMax = 8;
	int selectIndex = 0;

restartLoop:
	GTFreezeInput();
	GTSetCurtain(0.0, 60);
	SMWATarget = 1.0;
	LFrmCnt= 0;

	ImagePos_t ipTitle = MakeImagePos(P_TITLECONFIG, 10.0, 10.0, 0.0);
	ImagePos_t ipItems[selectMax];

	memset(ipItems, 0x00, sizeof(ipItems));

	for(int index = 0; index < selectMax; index++)
	{
		ipItems[index].PicResno = P_TITLECONFIGITEM_00 + index;
		ipItems[index].X = 15.0;
		ipItems[index].Y = 120.0 + index * 60.0;
		ipItems[index].A = 1.0;
	}
	ImagePos_t *ipList[] =
	{
		&ipTitle,
		&ipItems[0],
		&ipItems[1],
		&ipItems[2],
		&ipItems[3],
		&ipItems[4],
		&ipItems[5],
		&ipItems[6],
		&ipItems[7],
	};
	ImagePos_t ipFullScreen = MakeImagePos(P_TITLECONFIGSCREEN_FULLSCREEN, 420.0, ipItems[0].Y + 10.0, 0.0);
	ImagePos_t ipWindow     = MakeImagePos(P_TITLECONFIGSCREEN_WINDOW,     570.0, ipItems[0].Y + 10.0, 0.0);

	ImagePos_t ipDispMouseOn  = MakeImagePos(P_TITLECONFIGDISPMOUSE_ON,  420.0, ipItems[2].Y + 10.0, 0.0);
	ImagePos_t ipDispMouseOff = MakeImagePos(P_TITLECONFIGDISPMOUSE_OFF, 570.0, ipItems[2].Y + 10.0, 0.0);

	ImagePos_t *ipOptList[] =
	{
		&ipFullScreen,
		&ipWindow,
		&ipDispMouseOn,
		&ipDispMouseOff,
	};
	for(int index = 0; index < lengthof(ipList); index++)
	{
		ipList[index]->CurrPos.X = -10.0 - index * 50.0;
		ipList[index]->CurrPos.Y = ipList[index]->Y;
	}
	for(int index = 0; index < lengthof(ipOptList); index++)
	{
		ipOptList[index]->CurrPos.X = 800.0 + index * 50.0;
	}

	for(; ; )
	{
		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex = m_min(selectIndex + 1, selectMax - 1);
//			selectIndex = (selectIndex + 1) % selectMax;
		}
		else if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex = m_max(selectIndex - 1, 0);
//			selectIndex = (selectIndex + selectMax - 1) % selectMax;
		}
		else if(GTGetInput(INP_A, INP_DOWN))
		{
			switch(selectIndex)
			{
			case 0:
				forscene(10)
				{
					if(!WindowIsActive)
					{
						goto endItemAction;
					}
					GTDrawBlackWall();
					GTEndFrame();
				}
				Sd.FullScreenMode = Sd.FullScreenMode ? 0 : 1;
				ApplyScreenMode();

				forscene(90)
				{
					GTDrawBlackWall();
					GTEndFrame();
				}
				break;

			case 1:
				WindowSizeConfig();
				break;

			case 2:
				Sd.DispMouseCursorMode = Sd.DispMouseCursorMode ? 0 : 1;
				ApplyDispMouseCursorMode();
				break;

			case 3:
				PadConfig();
				goto restartLoop;

			case 4:
				VolumeConfig(1);
				goto restartLoop;

			case 5:
				ReleaseSoundEffects(); // SEボリュームアップデートを少なくするように
				VolumeConfig(0);
				goto restartLoop;

			case 6:
				OtherConfig();
				goto restartLoop;

			case 7:
				goto endMenu;

			default:
				error();
			}
endItemAction:;
		}
		else if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
		}
		for(int index = 0; index < selectMax; index++)
		{
			ipItems[index].X = index == selectIndex ? 40.0 : 15.0;
			ipItems[index].A = index == selectIndex ? 1.0 : 0.3;
		}
		ipFullScreen.A   =  Sd.FullScreenMode      ? 1.0 : 0.3;
		ipWindow.A       = !Sd.FullScreenMode      ? 1.0 : 0.3;
		ipDispMouseOn.A  =  Sd.DispMouseCursorMode ? 1.0 : 0.3;
		ipDispMouseOff.A = !Sd.DispMouseCursorMode ? 1.0 : 0.3;

		DrawMenuWall();
		GTEarlyCurtain();

		for(int index = 0; index < lengthof(ipList); index++)
		{
			ActImage(ipList[index]);
		}
		for(int index = 0; index < lengthof(ipOptList); index++)
		{
			ActImage(ipOptList[index]);
		}
		GTEndFrame();
	}
endMenu:;
}
static void ShowResult(void)
{
	GTFreezeInput();
	GTSetCurtain(0.0, 60);
	SMWATarget = 1.0;

	for(; ; )
	{
		if(GTGetInput(INP_A, INP_DOWN) ||
			GTGetInput(INP_B, INP_DOWN)
			)
			break;

		DrawMenuWall();
		GTEarlyCurtain();

		switch(Sd_PlayChara)
		{
		case PLAY_CHARA_00: PE_Border(GetColor(  0,   0,   0)); break; // normal
		case PLAY_CHARA_01: PE_Border(GetColor( 64, 128, 255)); break; // blue
		case PLAY_CHARA_02: PE_Border(GetColor(128, 128,  32)); break; // yellow
		case PLAY_CHARA_03: PE_Border(GetColor(192,  32,  32)); break; // red

		default:
			error();
		}

		SetPrint(16, 16, 30);
		Print("クリアタイム");
		PrintRet();
		Print("                                                                         NO-DAMAGE"); PrintRet();
		Print("                                                      WIPED-OUT          WIPED-OUT"); PrintRet();
		Print("STAGE    　   　 NORMAL 　　　      NO-BOMB　        　NO-BOMB            NO-BOMB"); PrintRet();
		Print("---------------------------------------------------------------------------------"); PrintRet();

		for(int stageIndex = 0; stageIndex < STAGE_NUM; stageIndex++)
		{
			char *stamp = strx(GmFrameToStamp(GetBestTimeFrmCnt(stageIndex)));
			char *nb_stamp = strx(GmFrameToStamp(GetBestTimeFrmCnt_NB(stageIndex)));
			char *nb_wo_stamp = strx(GmFrameToStamp(GetBestTimeFrmCnt_NBWipedOut(stageIndex)));
			char *nb_wo_nd_stamp = strx(GmFrameToStamp(GetBestTimeFrmCnt_NBWipedOut_NoDamage(stageIndex)));

			Print_x(xcout(
				"   %02d"
				"           %s"
				"           %s"
				"           %s"
				"           %s"
				,stageIndex + 1
				,stamp
				,nb_stamp
				,nb_wo_stamp
				,nb_wo_nd_stamp
				));
			PrintRet();

			memFree(stamp);
			memFree(nb_stamp);
			memFree(nb_wo_stamp);
			memFree(nb_wo_nd_stamp);
		}
		PrintRet();
		PrintRet();
		Print("★ 決定又はキャンセルボタンを押して下さい。");
		PE_Reset();

		GTEndFrame();
	}
	clsDx();
}
static void ApStageHome_Prep(void)
{
	clsDx(); // 2bs?
	MusicFadeout(30);
	GTWaitFadeout(P_TITLEWALL, -1.0, 30);
}
static void ApStageHome_After(void)
{
	clsDx(); // 2bs?
	MusicPlay(MUSIC_TITLE);
	GTActWhiteLevel = -1.0;
}
static void ContinueMenu(void)
{
#if 0 // haishi
	if(!*Get_P_ClearStageIndex()) // 以前からのゲームスタートと同じ振舞い。
	{
		ApStageHome_Prep();
		ApStageHome();
		ApStageHome_After();
		return;
	}
#endif

restartLoop:
	int sidx_max = *Get_P_ClearStageIndex(); // ステージ項目indexの最大値
	m_range(sidx_max, 0, STAGE_NUM - 1);

	int selectMax = sidx_max + 2; // メニュー項目数
	int selectIndex = sidx_max;

	GTFreezeInput();
	GTSetCurtain(-0.5, 60);
//	SMWATarget = 1.0; // ここではしない！

	for(; ; )
	{
		if(GTGetInput(INP_DIR_8, INP_DOWN))
		{
			selectIndex && selectIndex--;
		}
		if(GTGetInput(INP_DIR_2, INP_DOWN))
		{
			selectIndex < selectMax - 1 && selectIndex++;
		}
		if(GTGetInput(INP_A, INP_DOWN))
		{
			if(selectIndex <= sidx_max)
			{
				ApStageHome_Prep();
				ApStageHome_Continue(selectIndex);
				ApStageHome_After();
				goto restartLoop;
			}
			else
				goto endMenu;
		}
		if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
		}
		DrawMenuWall();
		GTEarlyCurtain();

		int sicnt = 0;

		PE_Border(GetColor(96, 96, 128));
		SetPrint(50, 50, 40);
		Print("コンテニュー");
		PrintRet();
		PrintRet();

		for(int sidx = 0; sidx <= sidx_max; sidx++)
		{
			Print_x(xcout("　%c ", selectIndex == sicnt++ ? '>' : ' '));

			int no = sidx + 1;

			switch(no)
			{
			case 5:
				Print("ボ　ス");
				break;

			default:
				Print_x(xcout("%s　面", c_toString(no)));
				break;
			}
			PrintRet();
		}
		Print_x(xcout("　%c 戻る", selectIndex == sicnt++ ? '>' : ' '));
		PE_Reset();

		GTEndFrame();
	}
endMenu:
	clsDx(); // 2bs?
}
void ApHome(void)
{
//	S_SoftImageFilterList() default: NULL
	S_SoftImageFilterList()->SetElement(P_BOMB, SIF_DarkToTrans);
//	for(int p_ndx = 0; p_ndx < 18; p_ndx++) S_SoftImageFilterList()->SetElement(P_CRASH_00 + p_ndx, SIF_DarkToTrans); // 廃止
//	for(int p_ndx = 0; p_ndx < 12; p_ndx++) S_SoftImageFilterList()->SetElement(P_PLAYER_CRASH_00 + p_ndx, SIF_DarkToTrans); // 廃止
//	for(int p_ndx = 0; p_ndx < 15; p_ndx++) S_SoftImageFilterList()->SetElement(P_BUBBLE_00 + p_ndx, SIF_DarkToTrans); // 廃止
	S_SoftImageFilterList()->SetElement(P_BUBBLE_BACK, SIF_DarkToTrans);
	S_SoftImageFilterList()->SetElement(P_UDONGE_ATTACKBACK, SIF_DarkToTrans);
	for(int p_ndx = 0; p_ndx < 10; p_ndx++) S_SoftImageFilterList()->SetElement(P_MARISA_MASTERSPARK_00 + p_ndx, SIF_DarkToTrans);
//	for(int p_ndx = 0; p_ndx < 8; p_ndx++) S_SoftImageFilterList()->SetElement(P_MARISA_MASTERSPARK_SUB_00 + p_ndx, SIF_DarkToTrans); // 廃止
//	S_SoftImageFilterList()->SetElement(P_MIKO_A, SIF_CheckMikoSize_A); // test
//	S_SoftImageFilterList()->SetElement(P_MIKO_B, SIF_CheckMikoSize_B); // test
	for(int p_ndx = 0; p_ndx < 3; p_ndx++) S_SoftImageFilterList()->SetElement(P_MAPITEMTOTTA_00 + p_ndx, SIF_DarkToTrans);

//	S_GraphicHandleFilterList() default: NULL

//	S_EnableTransList() default: 1
	S_EnableTransList()->SetElement(P_WHITEBOX, 0);
	S_EnableTransList()->SetElement(P_TITLEWALL, 0);
	/*
	S_EnableTransList()->SetElement(P_FIELDWALL_101, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_102, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_103, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_104, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_105, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_106, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_201, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_202, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_203, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_301, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_302, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_303, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_401, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_402, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_403, 0);
	S_EnableTransList()->SetElement(P_FIELDWALL_501, 0);
	*/
	S_EnableTransList()->SetElement(P_PAUSEWALL, 0);
	S_EnableTransList()->SetElement(P_SUBMENUWALL, 0);
	S_EnableTransList()->SetElement(P_P_CUBE, 0);
	S_EnableTransList()->SetElement(P_R_CUBE, 0);
	S_EnableTransList()->SetElement(P_G_CUBE, 0);
	S_EnableTransList()->SetElement(P_B_CUBE, 0);
	S_EnableTransList()->SetElement(P_Y_CUBE, 0);
	S_EnableTransList()->SetElement(P_W_CUBE, 0);

//	S_MusicVolumeList() default: 0.5
//	S_MusicVolumeList()->SetElement(MUSIC_BOSS, 0.4); // old
	S_MusicVolumeList()->SetElement(MUSIC_BOSS, 0.6);

//	S_SEVolumeList() default: 0.5
//	S_SEVolumeList()->SetElement(SE_HIT_PLAYER, 0.15);
//	S_SEVolumeList()->SetElement(SE_HIT_ENEMY, 0.2);
	S_SEVolumeList()->SetElement(SE_DEATH_ENEMY, 0.4);
//	S_SEVolumeList()->SetElement(SE_DEATH_BOSS, 0.75);
//	S_SEVolumeList()->SetElement(SE_FIRE07_GREZE, 0.1);
//	S_SEVolumeList()->SetElement(SE_NEWRECORD, 0.8);

	// < 初期値など

	if(Sd.ScreenZoom != 1.0)
	{
		ApplyWindowSize();

		GTDrawBlackWall();
		GTEndFrame();
	}
	if(!Sd.FirstBootEnded)
	{
		Pub_KumaSelectMenu();
		Sd.FirstBootEnded = 1;
	}
	Logo();

	const int selectMax = 5;
	int selectIndex = 0;

	ImagePos_t ipTitle = MakeImagePos(P_TITLE, 0.0, 5.0, 0.0);
	ImagePos_t ipItems[selectMax];

	ipTitle.X = 5.0;

	memset(ipItems, 0x00, sizeof(ipItems));

	for(int index = 0; index < selectMax; index++)
	{
		ipItems[index].PicResno = P_TITLEITEM_00 + index;
		ipItems[index].Y = 220.0 + 60.0 * index;
	}
	ImagePos_t *ipList[] =
	{
		&ipTitle,
		&ipItems[0],
		&ipItems[1],
		&ipItems[2],
		&ipItems[3],
		&ipItems[4],
	};

restart:
	MusicPlay(MUSIC_TITLE);
	GTActWhiteLevel = -1.0;

restartLoop:
	GTFreezeInput();
	GTSetCurtain(0.0, 60);
	SMWATarget = 0.0;
	LFrmCnt = 0;

	for(; ; )
	{
		clsDx(); // 2bs?

		/*
			基本的に、フレームは (処理 -> 描画) の順で、
		*/

		// ---- 処理 ----

		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex < selectMax - 1 && selectIndex++;
		}
		else if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex && selectIndex--;
		}
		else if(GTGetInput(INP_A, INP_DOWN))
		{
			switch(selectIndex)
			{
			case 0:
				ApStageHome_Prep();
				ApStageHome();
				goto restart; // <- ApStageHome_After()と同等

			case 1:
				ContinueMenu();
				goto restartLoop;

			case 2:
				ShowResult();
				goto restartLoop;

			case 3:
				Configuration();
				goto restartLoop;

			case 4:
				goto endMenu;

			default:
				error();
			}
		}
		else if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
		}
		for(int index = 0; index < selectMax; index++)
		{
			ipItems[index].X = index == selectIndex ? 50.0 : 25.0;
			ipItems[index].A = index == selectIndex ? 1.0 : 0.3;
		}
		if(!LFrmCnt)
		{
			for(int index = 0; index < lengthof(ipList); index++)
			{
				ipList[index]->CurrPos.X = ipList[index]->X + 15.0;
				ipList[index]->CurrPos.Y = ipList[index]->Y + 15.0;
				ipList[index]->CurrPos.A = 0.0;
			}
		}

		// ---- 描画 ----

//		GTDrawBlackWall();
		DrawMenuWall();
		GTEarlyCurtain();

		int actImageMax = m_min(LFrmCnt / 5, lengthof(ipList));

		for(int index = 0; index < actImageMax; index++)
		{
			ActImage(ipList[index]);
		}

		GTEndFrame();
	}
endMenu:
	MusicFadeout(30);
	GTWaitFadeout(P_TITLEWALL, -1.0, 40);
}
