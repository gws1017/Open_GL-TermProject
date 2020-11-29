#include "Header/PokemonManager.h"
#include "Header/Pokemon.h"
#include "Header/Sound.h"

extern Sound sound;

Pokemon_Manager::Pokemon_Manager()
{

}
Pokemon_Manager::~Pokemon_Manager()
{

}

void Pokemon_Manager::Create(const char* name ,GLuint s_program)
{
	Pokemon p(name);
	p.Init(s_program);
	Position(&p);
	vpm.push_back(p);
	FMOD_Channel_Stop(sound.Channel[1]);

	//울음소리 재생
	if (strcmp(name, "balchang") == 0)
	{
		FMOD_System_PlaySound(sound.soundSystem, sound.effectSound[4], NULL, 0, &sound.Channel[1]);
	}
	else if (strcmp(name, "picachu") == 0)
	{
		FMOD_System_PlaySound(sound.soundSystem, sound.effectSound[0], NULL, 0, &sound.Channel[1]);
	}
	else if (strcmp(name, "pang") == 0)
	{
		FMOD_System_PlaySound(sound.soundSystem, sound.effectSound[1], NULL, 0, &sound.Channel[1]);
	}
	else if (strcmp(name, "gong") == 0)
	{
		FMOD_System_PlaySound(sound.soundSystem, sound.effectSound[2], NULL, 0, &sound.Channel[1]);
	}
	else if (strcmp(name, "coil") == 0)
	{
		FMOD_System_PlaySound(sound.soundSystem, sound.effectSound[3], NULL, 0, &sound.Channel[1]);
	}
}


void Pokemon_Manager::Draw(const unsigned int modelLocation)
{
	if (!vpm.empty())
	{
		for (int i = 0; i < vpm.size(); i++)
		{
			vpm.at(i).Draw(modelLocation);
		}
	}
	
}

void Pokemon_Manager::Act()
{
	if (!vpm.empty())
	{
		for (int i = 0; i < vpm.size(); i++)
		{
			switch (vpm.at(i).Get_Act_Type())
			{
			case 0:
				vpm.at(i).Jump();
				break;
			case 1:
				vpm.at(i).Jump2();
				break;
			case 2:
				vpm.at(i).Swing();
				break;
			case 3:
				vpm.at(i).Float(0.0f);
				break;
			case 4:
				vpm.at(i).Tornado();
				break;
			case 5:
				vpm.at(i).Rotate();
				break;
			}
			
		}

	}

}

void Pokemon_Manager::Position(Pokemon* p)
{
	uniform_int_distribution<int> uidX(-100, 100), uidZ(5, 100);
	random_device rd;
	mt19937 mt(rd());

	float h = p->Get_p_height();
	float x, z; // X 범위 -9~ 9 Z 범위 1 ~ 20 X -2 ~ 2 제외 Z 범위 8~ 12 제외
	float tx, tz, th; // 임시 변수
	bool a1 = false, a2 = false,t1 = false,t2 = false , t3 = false , t4 = false; 


	float l, b, t, r;
	

	float tl, tb, tr, tt;
	

	while (1)
	{
		x = uidX(mt) * 0.09; // -9~9로변환
		z = uidZ(mt) * 0.2;  // 1~20으로 변환

		l = x - h;
		b = z - h;
		r = x + h;
		t = z + h;

		//냄비 안에 생성 되는거 피하기
		if (x >= -2.0f && x <= 2.0f)
		{
			if (z >= 8.0f && z <= 12.0f) a1 = false;
			else a1 = true;
		}
		else a1 = true;

		//다른 포켓몬과 위치 겹치지 않도록
		for (int i = 0; i < vpm.size(); i++)
		{
			tx = vpm[i].Get_PosX();
			tz = vpm[i].Get_PosZ();
			th = vpm[i].Get_p_height();

			tl = tx - th;
			tb = tz - th;
			tr = tx + th;
			tt = tz + th;

			if (l <= tr && b <= tt)t1 = true;

			if (r >= tl && b <= tt)t2 = true;		
				
			if (r >= tl && t >= tb)t3 = true;

			if (l <= tr && t >= tb)t4 = true;
			
			if (t1 && t2 && t3 && t4)
			{
				a2 = false;
				
				continue;
			}
			else a2 = true;

			t1 = false;
			t2 = false;
			t3 = false;
			t4 = false;
		}

		if (vpm.size() == 0) a2 = true;

		if (a1 && a2) break;
	}
	
	p->Set_Pos(x,0.0f,z);

	p->Update_Pos();

}