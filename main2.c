// This is really just a downscaled ECS of how to handle reusing masks
// as different groups on an 8 bit machine.
// With this setup, you could thread each set of systems and entities,
// where each set represents a layer such as rendering, networking, audio, etc.
// The only problem i havent figured out yet is how to resolve communication
// between systems across different sets. While you can simply check if an
// entity contains a mask from a totally different set and operate on it or
// not, it defeats the purpose of the library handling that for you. If systems
// can have multiple masks, thats a solution, but will take a performance hit.
#include <stdlib.h>
#include <stdio.h>
// def
typedef unsigned int ui;
typedef unsigned char uc;
// fwd decl
struct world;
struct system;
// func ptrs
typedef void (*sysexec)(ui ent);
// lib limits
#define ENT_MAX 1024
#define ENT_SPAN 2
#define CMP_MAX 3
#define SYS_MAX 16
// cmp limits
#define INV_MAX 32
// set 0
#define CMP_SIZ 1 << 0
#define CMP_POS 1 << 1
#define CMP_VEL 1 << 2
#define CMP_COL 1 << 3
#define CMP_HP  1 << 4
#define CMP_STA 1 << 5
#define CMP_INV 1 << 6
#define CMP_CUR 1 << 7
// set 1
#define CMP_COLL  1 << 0
#define CMP_JMP   1 << 1
#define CMP_AI    1 << 2
#define CMP_USE   1 << 3
#define CMP_HELM  1 << 4
#define CMP_CHEST 1 << 5
#define CMP_GLOVE 1 << 6
#define CMP_BOOT  1 << 7
// structs
typedef struct sys
{
	ui mask;
	sysexec exec;
} sys;

/*
what the world layout generally looks like:
world:
{
	entities:
	{
		{
			1024 masks
		},
		{
			1024 masks
		}
	},
	systems
	{
		{
			16 systems each w/ own mask
		},
		{
			16 systems each w/ own mask
		}
	}
}
*/

typedef struct world
{
	uc entities[ENT_SPAN][ENT_MAX];
	sys systems[ENT_SPAN][SYS_MAX];
	// set 0
	double siz[ENT_MAX][2];
	double pos[ENT_MAX][2];
	double vel[ENT_MAX][2];
	uc col[ENT_MAX][3];
	ui hp[ENT_MAX];
	ui sta[ENT_MAX];
	ui inv[ENT_MAX][INV_MAX];
	ui cur[ENT_MAX];
	// set 1
	uc coll[ENT_MAX]; // are we colliding
	uc jmp[ENT_MAX]; // are we jumping
	ui ai[ENT_MAX]; // point to ai ids
	ui use[ENT_MAX]; // point to last used / attempted use item
	ui helm[ENT_MAX]; // helmet ids per ent
	ui chest[ENT_MAX]; // chest ids per ent
	ui glove[ENT_MAX]; // glove ids per ent
	ui boot[ENT_MAX]; // boot ids per ent
} world;

void cmp_add(world *w,ui ent,ui set,ui cmp)
{
	ui setmask = w->entities[set][ent]|cmp;
	w->entities[set][ent] = setmask;
}
void cmp_rem(world *w,ui ent,ui set,ui cmp)
{
	ui setmask = w->entities[set][ent];
	w->entities[set][ent] = setmask^cmp;
}
void world_tick(world *w)
{
	for(uc i=0;i<ENT_SPAN;i++)
	{
		for(ui j=0;j<ENT_MAX;j++)
		{
			uc setmask = w->entities[i][j];
			if(setmask == 0) continue;
			for(uc k=0;k<SYS_MAX;k++)
			{
				ui sysmask = w->systems[i][k].mask;
				if(sysmask == 0 || (sysmask & setmask) != sysmask ) continue;
				w->systems[i][k].exec(j);
			}
		}		
	}
}

// src
int main(int argc, char **argv)
{
	printf("%d\n",sizeof(world));
	return 0;
}