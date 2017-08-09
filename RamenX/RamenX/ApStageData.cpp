#include "Stdinc.h"

void ApLoadStage_01(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(P_FIELDWALL_01);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeAlice(100.0, -1500.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(300.0, -1550.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(500.0, -1550.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(700.0, -1500.0, 0));

	// 1000

	GmDc.EnemyList->AddElement(MakeDoll(1000.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(1100.0, 300.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(1200.0, 400.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(1499.0,   0.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(1499.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(1499.0, 400.0, 0));

	// 1500

	GmDc.EnemyList->AddElement(MakeDoll(1510.0, 150.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(1530.0, 350.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(1550.0, 250.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(1570.0, 450.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(1999.0, -200.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(1999.0, -100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(1999.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(1999.0, 300.0, 0));

	// 2000

	GmDc.EnemyList->AddElement(MakeCube_P(2250.0, 450.0));
	GmDc.EnemyList->AddElement(MakeCube_R(2250.0, 450.0));

	GmDc.EnemyList->AddElement(MakeDoll(2100.0, 50.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(2400.0, 50.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(2499.0, 50.0, 0));

	// 2500, 3000

	GmDc.EnemyList->AddElement(MakeCube_P(2990.0, 150.0));
	GmDc.EnemyList->AddElement(MakeCube_P(2990.0, 250.0));
	GmDc.EnemyList->AddElement(MakeCube_P(2990.0, 350.0));

	GmDc.EnemyList->AddElement(MakeCube_R(2750.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_R(3250.0, 300.0));

	GmDc.EnemyList->AddElement(MakeSuica(2510.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(2990.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(3490.0, 0));

	GmDc.EnemyList->AddElement(MakeMarisa(3490.0, 0));

	// 3500

	GmDc.EnemyList->AddElement(MakeDoll(3501.0, 125.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(3601.0, 175.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(3701.0, 325.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(3801.0, 375.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(3901.0, 425.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(3999.0, 475.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(3999.0, 110.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, 220.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, 330.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, 440.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(3750.0, -3200.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3750.0, -3400.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3750.0, -3600.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3750.0, -3800.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3750.0, -4000.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(3999.0, -3100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, -3300.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, -3500.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, -3700.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(3999.0, -3900.0, 0));

	// 4000

	GmDc.EnemyList->AddElement(MakeUdonge(4001, 350.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(4050, 225.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(4100, 100.0, 0));

	GmDc.EnemyList->AddElement(MakeCube_P(4000.0, 330.0));
	GmDc.EnemyList->AddElement(MakeCube_P(4250.0, 330.0));
	GmDc.EnemyList->AddElement(MakeCube_P(4250.0,  80.0));
	GmDc.EnemyList->AddElement(MakeCube_P(4500.0, 330.0));

	GmDc.EnemyList->AddElement(MakeDoll(4470.0,  35.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(4480.0,  90.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(4490.0, 145.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(4499.0, 200.0, 0));

	GmDc.EnemyList->AddElement(MakeMarisa(4499.0, 0));

	// 4500

	GmDc.EnemyList->AddElement(MakeCube_R(4500.0, 230.0));
	GmDc.EnemyList->AddElement(MakeCube_R(4750.0, 230.0));
	GmDc.EnemyList->AddElement(MakeCube_R(4825.0, 180.0));
	GmDc.EnemyList->AddElement(MakeCube_R(4875.0, 230.0));
	GmDc.EnemyList->AddElement(MakeCube_R(4900.0, 180.0));
	GmDc.EnemyList->AddElement(MakeCube_R(5000.0, 230.0));

	GmDc.EnemyList->AddElement(MakeUdonge(4999.0, 450.0, 0));

	// 5000

	GmDc.EnemyList->AddElement(MakeUdonge(5300.0, 300.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(5499.0, 400.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(5300.0, 490.0, 0));

	GmDc.EnemyList->AddElement(MakeAya(5499.0, 200.0, 0));

	// 5500

	GmDc.EnemyList->AddElement(MakeMapItem(5340.0, 485.0, MIE_KAIFUKU));

	GmDc.EnemyList->AddElement(MakeSuica(5440.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(5520.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(5600.0, 0));

	GmDc.EnemyList->AddElement(MakeSuica(5850.0, 0));
	GmDc.EnemyList->AddElement(MakeBubble(5900.0, 100.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(5950.0, 0));

	// 6000

	for(int c = 0; c < 12; c++)
	{
		GmDc.EnemyList->AddElement(MakeAlice(6050.0, -7000.0 + c * 80.0, 0));
		GmDc.EnemyList->AddElement(MakeAlice(6250.0, -6000.0 + c * 80.0, 0));
		GmDc.EnemyList->AddElement(MakeAlice(6450.0, -6500.0 + c * 80.0, 0));
	}
	GmDc.EnemyList->AddElement(MakeCube_G(6050.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_G(6450.0, 150.0));

	// 6500

	GmDc.EnemyList->AddElement(MakeSuica(6550.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(6600.0, 500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(6650.0, 0));

	GmDc.EnemyList->AddElement(MakeDoll(6620.0, 300.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6660.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6700.0, 100.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6900.0, 480.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6940.0, 400.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6980.0, 320.0, 0));

	GmDc.EnemyList->AddElement(MakeSuica(6999.0 - 50.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(6999.0, 500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(6999.0 + 50.0, 0));

	// 7000

	GmDc.EnemyList->AddElement(MakeCube_R(7100.0, 100.0));
	GmDc.EnemyList->AddElement(MakeCube_R(7100.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_R(7400.0, 100.0));
	GmDc.EnemyList->AddElement(MakeCube_R(7400.0, 300.0));

	// 7500

	GmDc.EnemyList->AddElement(MakeSuica(7501.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(7901.0, 0));

	GmDc.EnemyList->AddElement(MakeMapItem(7900.0, 80.0, MIE_ADD_BOMB));

	GmDc.EnemyList->AddElement(MakeAlice(7501.0,  300.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7501.0,  200.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7501.0,  100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7501.0,    0.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7501.0, -100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7601.0, -100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7701.0, -100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7801.0, -100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7901.0, -100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7901.0, -200.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7901.0, -300.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7901.0, -400.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(7901.0, -500.0, 0));

	// 8000

	GmDc.EnemyList->AddElement(MakeMarisa(8001.0, 1));

	GmDc.EnemyList->AddElement(MakeSuica(8000.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(8100.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(8200.0, 0));

	// 8500

	GmDc.EnemyList->AddElement(MakeUdonge(8500.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(8500.0, 400.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(8700.0, 300.0, 0));

	GmDc.EnemyList->AddElement(MakeCube_B(8700.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_G(8700.0, 100.0));
	GmDc.EnemyList->AddElement(MakeCube_B(8999.0, 250.0));

	// 9000

	GmDc.EnemyList->AddElement(MakeMarisa(9001.0, 1));

	// x-max: 9100
}
void ApLoadStage_02(void)
{
	initrnd(2);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(P_FIELDWALL_02);

	// ---- Player ----

	GmDc.PlayerStartX = 3420.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0

	GmDc.EnemyList->AddElement(MakeMapItem(50.0, 50.0, MIE_ADD_BOMB));

	GmDc.EnemyList->AddElement(MakeCube_P(100.0, 110.0));
	GmDc.EnemyList->AddElement(MakeCube_P(100.0, 220.0));
	GmDc.EnemyList->AddElement(MakeCube_P(100.0, 330.0));

	GmDc.EnemyList->AddElement(MakeSuica( 60.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(140.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(220.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(300.0, 0));

	GmDc.EnemyList->AddElement(MakeDoll(250.0, 111.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(250.0, 222.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(250.0, 333.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(250.0, 444.0, 0));

	GmDc.EnemyList->AddElement(MakeDoll(350.0, 111.0 + 5.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(350.0, 222.0 + 5.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(350.0, 333.0 + 5.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(350.0, 444.0 + 5.0, 1));

	GmDc.EnemyList->AddElement(MakeDoll(450.0, 111.0 + 10.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(450.0, 222.0 + 10.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(450.0, 333.0 + 10.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(450.0, 444.0 + 10.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3000.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3200.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3300.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3400.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3500.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(490.0, -3600.0, 1));

	GmDc.EnemyList->AddElement(MakeAlice(499.0, -1000.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(480.0, -1100.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(460.0, -1200.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(440.0, -1300.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(420.0, -1400.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(400.0, -1500.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(380.0, -1600.0, 1));

	// 500

	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 -   0.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 -  50.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 - 200.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 - 250.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 - 400.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 - 450.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 - 600.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(501.0, 500.0 - 650.0, 0));

	GmDc.EnemyList->AddElement(MakeBubble(600.0, -100.0, 1));
	GmDc.EnemyList->AddElement(MakeBubble(600.0,  200.0, 1));

	for(int c = 0; c < 5; c++)
	{
		GmDc.EnemyList->AddElement(MakeCube_R(900.0, 200.0));
	}
	GmDc.EnemyList->AddElement(MakeSuica(900.0, 0));

	// 1000

	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -100.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -200.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -300.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -400.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -500.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -600.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1401.0, -700.0, 1));

	// 1500

	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -100.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -200.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -300.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -400.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -500.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -600.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(1501.0, -700.0, 1));

	GmDc.EnemyList->AddElement(MakeSuica(1500.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(1600.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(1700.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(1800.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(1900.0, 1));

	// 2000

	GmDc.EnemyList->AddElement(MakeAlice(2001.0, -500.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(2001.0, -700.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(2001.0, -900.0, 1));

	GmDc.EnemyList->AddElement(MakeAya(2001.0, 250.0, 1));

	GmDc.EnemyList->AddElement(MakeCube_P(2400.0, 100.0));

	GmDc.EnemyList->AddElement(MakeAlice(2400.0, -400.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(2400.0, -600.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(2400.0, -800.0, 1));

	GmDc.EnemyList->AddElement(MakeUdonge(2200.0, 100.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(2200.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(2400.0, 150.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(2400.0, 250.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(2200.0, 350.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(2050.0, 450.0, 0));

	GmDc.EnemyList->AddElement(MakeBubble(2400.0, -2000.0, 1));
	GmDc.EnemyList->AddElement(MakeBubble(2400.0, -2000.0, 0));

	// 2500

	// 3000

	GmDc.EnemyList->AddElement(MakeMarisa(3001.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(3001.0, 70.0, 0));

	// START

	// 3500

	GmDc.EnemyList->AddElement(MakeDoll(3840.0, 70.0, 0));
	GmDc.EnemyList->AddElement(MakeMarisa(3840.0, 1));

	// 4000

	GmDc.EnemyList->AddElement(MakeUdonge(4400.0, 100.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(4450.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(4470.0, 300.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(4480.0, 400.0, 0));

	GmDc.EnemyList->AddElement(MakeSuica(4250.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(4350.0, 0));

	// 4500

	GmDc.EnemyList->AddElement(MakeCube_Y(4750.0, 300.0));

	GmDc.EnemyList->AddElement(MakeDoll(4550.0, 250.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(4550.0, 300.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(4550.0, 350.0, 0));

	GmDc.EnemyList->AddElement(MakeMarisa(4550.0, 0));

	// 5000

//	GmDc.EnemyList->AddElement(MakeCube_B(5100.0, 300.0));

	GmDc.EnemyList->AddElement(MakeSuica(5100.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(5250.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(5400.0, 0));

	GmDc.EnemyList->AddElement(MakeAya(5400.0, 150.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(5400.0, 250.0, 0));

	// 5500

	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 290.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 340.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 390.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 440.0, 0));

	// 6000

	GmDc.EnemyList->AddElement(MakeUdonge(6100.0, 290.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(6100.0, 340.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(6100.0, 390.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(6100.0, 440.0, 1));

	GmDc.EnemyList->AddElement(MakeDoll(6150.0, 120.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6200.0, 160.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(6250.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(6300.0, 240.0, 1));

	// 6500

	GmDc.EnemyList->AddElement(MakeCube_W(6501.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_R(6501.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_R(6501.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_R(6501.0, 300.0));

	GmDc.EnemyList->AddElement(MakeAya(6600.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeMarisa(6800.0, 0));

	GmDc.EnemyList->AddElement(MakeMapItem(6840.0 - 50.0, 50.0, MIE_KAIFUKU));

	// x-max: 6840
}
void ApLoadStage_03(void)
{
	initrnd(3);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(P_FIELDWALL_03);
//	GmDc.SnowMode = 1; // îpé~

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0

	// 500

	// 1000

	for(int c = 0; c < 15; c++)
	{
		GmDc.EnemyList->AddElement(MakeCube_R(1100.0, 400.0));
		GmDc.EnemyList->AddElement(MakeCube_R(1300.0, 250.0));
		GmDc.EnemyList->AddElement(MakeCube_R(1500.0, 400.0));
	}
	GmDc.EnemyList->AddElement(MakeCube_P(1100.0, 400.0));
	GmDc.EnemyList->AddElement(MakeCube_W(1300.0, 250.0));
	GmDc.EnemyList->AddElement(MakeCube_P(1500.0, 400.0));

	// 1500

	GmDc.EnemyList->AddElement(MakeDoll(1800.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(1725.0, 275.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(1800.0, 350.0, 1));

	GmDc.EnemyList->AddElement(MakeSuica(1700.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(1850.0, 0));

	// 2000

	GmDc.EnemyList->AddElement(MakeCube_Y(2000.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_G(2250.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_B(2250.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_Y(2500.0, 300.0));

	// 2500

	// 3000

	GmDc.EnemyList->AddElement(MakeSuica(2900.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(2980.0, 1));

	for(int c = 0; c < 12; c++)
	{
		GmDc.EnemyList->AddElement(MakeCube_P(3080.0 + c * 60.0, 450.0));
	}
	GmDc.EnemyList->AddElement(MakeAya(3280.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(3480.0, 200.0, 0));

	// 3500

	GmDc.EnemyList->AddElement(MakeAya(3880.0, 200.0, 1));

	// 4000

	GmDc.EnemyList->AddElement(MakeMapItem(4150.0, 300.0, MIE_KAIFUKU));

	for(int xc = 0; xc < 3; xc++)
	for(int yc = 0; yc < 3; yc++)
	{
		GmDc.EnemyList->AddElement(MakeCube_P(4100.0 + xc * 50.0, 250.0 + yc * 50.0));
	}

	// 4500

	GmDc.EnemyList->AddElement(MakeAlice(4501.0, 100.0, 2));
	GmDc.EnemyList->AddElement(MakeAlice(4501.0, 200.0, 2));
	GmDc.EnemyList->AddElement(MakeAlice(4501.0, 300.0, 2));

	GmDc.EnemyList->AddElement(MakeAlice(4501.0, -700.0, 2));
	GmDc.EnemyList->AddElement(MakeAlice(4501.0, -600.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(4501.0, -500.0, 2));
	GmDc.EnemyList->AddElement(MakeAlice(4501.0, -400.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(4501.0, -300.0, 2));

	GmDc.EnemyList->AddElement(MakeAya(4750.0, 120.0, 2));

	// 5000

	GmDc.EnemyList->AddElement(MakeCube_W(5000.0, 100.0));
	GmDc.EnemyList->AddElement(MakeCube_W(5000.0, 400.0));
	GmDc.EnemyList->AddElement(MakeCube_R(5000.0, 250.0));
	GmDc.EnemyList->AddElement(MakeCube_R(5000.0, 250.0));
	GmDc.EnemyList->AddElement(MakeCube_R(5000.0, 250.0));

	GmDc.EnemyList->AddElement(MakeMarisa(5499.0, 2));

	// 5500

	GmDc.EnemyList->AddElement(MakeDoll(5501.0, 100.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(5501.0, 180.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(5501.0, 260.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(5420.0, 260.0, 0));

	GmDc.EnemyList->AddElement(MakeSuica(5500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(5600.0, 0));

	GmDc.EnemyList->AddElement(MakeSuica(5800.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(5900.0, 2));

	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 280.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(5900.0, 360.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5980.0, 360.0, 1));

	// 6000

	for(int c = 0; c < 12; c++)
	{
		GmDc.EnemyList->AddElement(MakeAlice(6001.0 + c * 40.0, 250.0 - c * 40.0, 2));
	}

	// 6500

	GmDc.EnemyList->AddElement(MakeSuica(6500.0, 2));

	GmDc.EnemyList->AddElement(MakeDoll(6501.0, 150.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(6550.0, 200.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(6600.0, 250.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(6650.0, 300.0, 2));

	GmDc.EnemyList->AddElement(MakeDoll(6701.0, 150.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(6750.0, 200.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(6800.0, 250.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(6850.0, 300.0, 2));

	GmDc.EnemyList->AddElement(MakeSuica(6700.0, 2));
	GmDc.EnemyList->AddElement(MakeSuica(6900.0, 0));

	// 7000

	GmDc.EnemyList->AddElement(MakeCube_Y(7200.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_W(7200.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_R(7200.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_R(7200.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_R(7200.0, 290.0));

	// 7500

	GmDc.EnemyList->AddElement(MakeSuica(6999.0, 0));

	// 8000

	GmDc.EnemyList->AddElement(MakeCube_Y(8250.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_Y(8250.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_W(8250.0, 290.0));
	GmDc.EnemyList->AddElement(MakeCube_R(8250.0, 290.0));

	GmDc.EnemyList->AddElement(MakeSuica(8000.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(8250.0, 2));
	GmDc.EnemyList->AddElement(MakeSuica(8500.0, 0));

	// 8500

	// 9000

	GmDc.EnemyList->AddElement(MakeUdonge(9001.0, 400.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(9400.0, 220.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(9330.0, 300.0, 2));
	GmDc.EnemyList->AddElement(MakeUdonge(9400.0, 380.0, 0));

	// 9500

	GmDc.EnemyList->AddElement(MakeDoll(9700.0, 100.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(9750.0, 150.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(9800.0, 200.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(9850.0, 250.0, 2));

	// 10000

	GmDc.EnemyList->AddElement(MakeUdonge(10001.0, 330.0, 2));
	GmDc.EnemyList->AddElement(MakeUdonge(10001.0, 430.0, 2));

	for(int c = 0; c < 10; c++)
	{
		GmDc.EnemyList->AddElement(MakeCube_R(10000.0 + c * 20.0, 200.0 + c * 40.0));
	}
	GmDc.EnemyList->AddElement(MakeCube_G(10100.0, 60.0));
	GmDc.EnemyList->AddElement(MakeCube_B(10450.0, 300.0));

	// 10500

	// 11000, 11500, 12000, 12500

	GmDc.EnemyList->AddElement(MakeSuica(10860.0, 1));

	GmDc.EnemyList->AddElement(MakeCube_G(11300.0,  60.0));
	GmDc.EnemyList->AddElement(MakeCube_G(11300.0, 480.0));
	GmDc.EnemyList->AddElement(MakeCube_G(12000.0,  60.0));
	GmDc.EnemyList->AddElement(MakeCube_G(12000.0, 480.0));

	GmDc.EnemyList->AddElement(MakeSuica(12440.0, 2));
	GmDc.EnemyList->AddElement(MakeSuica(12720.0, 2));
	GmDc.EnemyList->AddElement(MakeSuica(13000.0, 2));

	GmDc.EnemyList->AddElement(MakeBubble(12720.0, 200.0, 2));

	for(int c = 0; c < 6; c++)
	{
		GmDc.EnemyList->AddElement(MakeAlice(10860.0, 0.0 + c * 90.0, 0));
	}
	for(int c = 0; c < 12; c++)
	{
		GmDc.EnemyList->AddElement(MakeAlice(11300.0 + c * 90.0, -100.0, 0));
	}

	// 13000

	// 13500

	GmDc.EnemyList->AddElement(MakeMapItem(13500.0, 450.0, MIE_ADD_BOMB));

	GmDc.EnemyList->AddElement(MakeAya(13500.0, 150.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(13500.0, 250.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(13500.0, 350.0, 0));

	// 14000

	GmDc.EnemyList->AddElement(MakeSuica(14000.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(14080.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(14160.0, 1));
	GmDc.EnemyList->AddElement(MakeCube_P(14330.0, 50.0));
	GmDc.EnemyList->AddElement(MakeCube_P(14330.0, 450.0));

	// x-max: 14380
}
void ApLoadStage_04(void)
{
	initrnd(4);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(P_FIELDWALL_04);
//	GmDc.WallKasaneMode = 1; // îpé~

	// ---- Player ----

	GmDc.PlayerStartX = 80.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0

	// 500

	GmDc.EnemyList->AddElement(MakeSuica(500.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(750.0, 2));

	GmDc.EnemyList->AddElement(MakeUdonge(750.0, 180.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(750.0, 380.0, 1));

	// 1000, 1500

	GmDc.EnemyList->AddElement(MakeSuica(1000.0, 3));

	GmDc.EnemyList->AddElement(MakeSuica(1400.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(1500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(1600.0, 2));
	GmDc.EnemyList->AddElement(MakeSuica(1700.0, 2));

	// 2000

	GmDc.EnemyList->AddElement(MakeSuica(2100.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(2200.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(2300.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(2400.0, 3));

	// 2500, 3000

	for(int lv = 0; lv <= 2; lv++)
	{
		GmDc.EnemyList->AddElement(MakeAlice(2600.0, -300.0 - lv * 90.0, lv));
		GmDc.EnemyList->AddElement(MakeAlice(2800.0, -200.0 - lv * 90.0, lv));
		GmDc.EnemyList->AddElement(MakeAlice(3000.0, -100.0 - lv * 90.0, lv));
		GmDc.EnemyList->AddElement(MakeAlice(3200.0,    0.0 - lv * 90.0, lv));
	}
	GmDc.EnemyList->AddElement(MakeSuica(3000.0, 0)); // Alice 3 enc-bdr

	// 3500

	for(int c = 0; c < 6; c++)
	{
		GmDc.EnemyList->AddElement(MakeAlice(3501.0, -100.0 - c * 100.0, 3));
	}

	// 4000

	GmDc.EnemyList->AddElement(MakeSuica(4000.0, 1)); // Bubble 3 enc-bdr

	// 4500, 5000

	GmDc.EnemyList->AddElement(MakeBubble(4999.0, 300.0, 3));

	GmDc.EnemyList->AddElement(MakeDoll(5100.0, 100.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(5100.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(5100.0, 300.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(5100.0, 400.0, 0));

	GmDc.EnemyList->AddElement(MakeDoll(5300.0, 100.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5300.0, 200.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(5300.0, 300.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5300.0, 400.0, 2));

	GmDc.EnemyList->AddElement(MakeDoll(5499.0, 100.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(5499.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(5499.0, 300.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(5499.0, 400.0, 1));

	// 5500

	GmDc.EnemyList->AddElement(MakeSuica(5500.0, 0)); // Doll 3 enc-bdr

	GmDc.EnemyList->AddElement(MakeMapItem(5500.0, 400.0, MIE_ADD_BOMB));

	// 6000

	GmDc.EnemyList->AddElement(MakeDoll(6001.0, 300.0, 3));

	// 6500

	GmDc.EnemyList->AddElement(MakeSuica(6500.0, 0)); // Marisa 3 enc-bdr

	GmDc.EnemyList->AddElement(MakeMapItem(6500.0, 400.0, MIE_KAIFUKU));

	// 7000

	GmDc.EnemyList->AddElement(MakeUdonge(7001.0,  90.0, 2));
	GmDc.EnemyList->AddElement(MakeUdonge(7050.0, 200.0, 2));

	GmDc.EnemyList->AddElement(MakeMarisa(7001.0, 3));

	// 7500

	GmDc.EnemyList->AddElement(MakeSuica(7500.0, 0)); // Aya 3 enc-bdr

	GmDc.EnemyList->AddElement(MakeAya(7900.0, 100.0, 3));

	// 8000

	GmDc.EnemyList->AddElement(MakeSuica(8000.0, 0)); // Udonge 3 enc-bdr

	// 8500

	GmDc.EnemyList->AddElement(MakeUdonge(8501.0, 300.0, 3));
	GmDc.EnemyList->AddElement(MakeUdonge(8700.0, 300.0, 1));

	// 9000, 9500

	GmDc.EnemyList->AddElement(MakeSuica(9000.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(9100.0, 3));

	GmDc.EnemyList->AddElement(MakeCube_W(9300.0, 400.0));
	GmDc.EnemyList->AddElement(MakeCube_W(9500.0, 200.0));
	GmDc.EnemyList->AddElement(MakeCube_W(9700.0, 400.0));

	GmDc.EnemyList->AddElement(MakeCube_X(9500.0, 450.0));
//	GmDc.EnemyList->AddElement(MakeCube_X(9700.0, 150.0));

	GmDc.EnemyList->AddElement(MakeSuica( 9900.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(10000.0, 3));

	GmDc.EnemyList->AddElement(MakeMapItem(10350.0, 50.0, MIE_ADD_ZANKI));

	// x-max: 10400
}
void ApLoadStage_05_Boss(void)
{
	initrnd(5);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(P_FIELDWALL_05);

	// ---- Player ----

	GmDc.PlayerStartX = 450.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeMiko01(0.0, 0.0));

	// x-max: 900

	GmDc.BossStageMode = 1;
	Miko03Touch();
	Miko05Touch();
}
