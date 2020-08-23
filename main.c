#include <stdio.h>
#include <windows.h>
/******************************************************************************
Util
******************************************************************************/
#define dbg 1
typedef unsigned int ui;
typedef unsigned char bool,byte;
/******************************************************************************
Fwd Decl / Limits
******************************************************************************/
struct world;
typedef void (*systick)(ui ent);
#define TICKMS 32
#define MAXENTS 1024
#define MAXSYS 16
#define MAXCMP 32
/******************************************************************************
Component IDs
******************************************************************************/
#define CMP_SIZ 1 << 0
#define CMP_POS 1 << 1
#define CMP_COL 1 << 2
#define CMP_VEL 1 << 3
#define CMP_HP  1 << 4
/******************************************************************************
Masks
******************************************************************************/
#define COLLISION_MASK CMP_SIZ|CMP_POS|CMP_VEL
#define RENDER_MASK COLLISION_MASK|CMP_COL
#define DAMAGE_MASK CMP_HP
#define PLAYER_MASK RENDER_MASK|DAMAGE_MASK
/******************************************************************************
Structs
******************************************************************************/
typedef struct system_t
{
	ui mask;
	systick tick;
} system_t;
typedef struct world 
{
	ui entities[MAXENTS];
	system_t systems[MAXSYS];
	double size[MAXENTS][2]; 
	double pos[MAXENTS][2];
	byte col[MAXENTS][3];
	double vel[MAXENTS][2];
	ui hp[MAXENTS];
} world;
/******************************************************************************
Lib Methods
******************************************************************************/
void sys_reg(world *w,ui mask,systick tick)
{
	ui i = 0;
	for(;i<MAXSYS;i++) if(w->systems[i].mask == 0) break;
	w->systems[i].mask = mask;
	w->systems[i].tick = tick;
	#ifdef dbg
	printf("sys_reg: %d\n",i);
	#endif
}
ui ent_reg(world *w)
{
	ui i = 0;
	for(;i<MAXENTS;i++)
	{
		if(w->entities[i] == 0)
		{
			#ifdef dbg
			printf("ent_reg: %d\n",i);
			#endif
			return i;
		}
	}
	return i;
}
bool cmp_has(world *w,ui ent,ui cmp)
{
	return (w->entities[ent] & cmp) == cmp;
}
void cmp_mgr(world *w,ui ent,ui cmp)
{
	ui or  = w->entities[ent]|cmp;
	ui xor = w->entities[ent]^cmp;
	ui res = cmp_has(w,ent,cmp) ? xor:or;
	#ifdef dbg
	printf("cmp_mgr: ent:%d, cmp:%d added:%d\n",ent,cmp,res);
	#endif
	w->entities[ent] = res;
}
void world_tick(world *w)
{
	for(ui i=0;i<MAXENTS;i++)
	{
		if(w->entities[i] == 0) continue;
		ui entmask = w->entities[i];
		for(ui j=0;j<MAXSYS;j++)
		{
			ui sysmask = w->systems[j].mask;
			if(sysmask == 0) continue;
			if((entmask&sysmask) != sysmask) continue;
			w->systems[j].tick(i);
		}
	}
}
/******************************************************************************
Implementation / Usage
******************************************************************************/
/**************************************
Global
**************************************/
world w;
/**************************************
Player
**************************************/
void player_init(world *w,ui ent)
{
	w->size[ent][0] = 32;
	w->size[ent][1] = 32;
	w->col[ent][0] = 255;
	w->col[ent][1] = 255;
	w->col[ent][2] = 255;
	w->hp[ent] = 100;
	w->pos[ent][0] = 0;
	w->pos[ent][1] = 0;
	w->vel[ent][0] = 0;
	w->vel[ent][1] = 0;
	cmp_mgr(w,ent,PLAYER_MASK);
}
/**************************************
Systems
**************************************/
void game_loop() // just an example, not really practical
{
	while(1)
	{
		world_tick(&w);
		Sleep(TICKMS);
	}
}

void sys_collide(ui ent)
{
	printf("sys_collide: %d\n",ent);
}
void sys_render(ui ent)
{
	printf("sys_render: %d\n",ent);
}
void sys_damage(ui ent)
{
	printf("sys_damage: %d\n",ent);	
}
int main(int argc, char **argv)
{
	// register systems
	sys_reg(&w,COLLISION_MASK,sys_collide);
	sys_reg(&w,RENDER_MASK,sys_render);
	sys_reg(&w,DAMAGE_MASK,sys_damage);
	// create a player ent
	player_init(&w,ent_reg(&w));
	// start game loop
	game_loop();
	return 0;
}
