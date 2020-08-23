/******************************************************************************
Util
******************************************************************************/
#define true 1
#define false 0
typedef unsigned int ui;
typedef unsigned char bool,byte;
/******************************************************************************
Fwd Decl / Limits
******************************************************************************/
struct world;
typedef void (*sysfunc)(bool *);
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
typedef struct system
{
	ui mask;
	bool entities[MAXENTS];
	sysfunc func;
} system;

typedef struct world // world struct manages literally everything. if it goes out of scope, rip.
{
	ui entities[MAXENTS];
	system systems[MAXSYS];
	// start components
	double size[MAXENTS][2]; // components are literally just arrays here, not structs. optionally make structs and pretty members, but this is way cleaner.
	double pos[MAXENTS][2];
	byte col[MAXENTS][3];
	double vel[MAXENTS][2];
	ui hp[MAXENTS];
	// end components
} world;
/******************************************************************************
Lib Methods
******************************************************************************/
void sys_reg(world *w,ui mask,sysfunc func)
{
	ui id = 0;
	for(;w->systems[id].mask != 0 && id < MAXSYS;id++)
	w->systems[id].mask = mask;
	w->systems[id].func = func;
}
void sys_match(world *w,ui ent)
{
	ui entmask = w->entities[ent];
	for(ui i=0;i < MAXSYS;i++)
	{
		ui sysmask = w->systems[i].mask;
		w->systems[i].entities[ent] = entmask == sysmask;
	}
}
ui ent_reg(world *w)
{
	ui id = 0;
	for(;w->entities[id] != 0 && id < MAXENTS;id++)
	return id;
}
bool cmp_has(world *w,ui ent,ui cmp)
{
	return (w->entities[ent] & cmp) == cmp;
}
void cmp_mgr(world *w,ui ent,ui cmp) // toggles adding / removing one or multiple components. can be a mask for all it cares.
{
	ui or  = w->entities[ent]|cmp;
	ui xor = w->entities[ent]^cmp;
	w->entities[ent] = cmp_has(w,ent,cmp) ? xor:or;
	sys_match(w,ent);
}
/******************************************************************************
Implementation / Usage
******************************************************************************/
/**************************************
Global
**************************************/
world w;
/**************************************
Base Entity Methods
**************************************/
void ent_setsize(world *_w,ui ent,double w,double h)
{
	_w->size[ent][0] = w;
	_w->size[ent][1] = h;
}
double *ent_getsize(world *w,ui ent)
{
	return w->size[ent];
}
void ent_setpos(world *w,ui ent,double x,double y)
{
	w->pos[ent][0] = x;
	w->pos[ent][1] = y;
}
double *ent_getpos(world *w,ui ent)
{
	return w->pos[ent];
}
void ent_setcol(world *w,ui ent,byte r,byte g,byte b)
{
	w->col[ent][0] = r;
	w->col[ent][1] = g;
	w->col[ent][2] = b;
}
byte *ent_getcol(world *w,ui ent)
{
	return w->col[ent];
}
void ent_setvel(world *w,ui ent,double vx,double vy)
{
	w->vel[ent][0] = vx;
	w->vel[ent][1] = vy;
}
double *ent_getvel(world *w,ui ent)
{
	return w->vel[ent];
}
/**************************************
Niche Methods
**************************************/
void ent_sethp(world *w,ui ent,ui hp)
{
	w->hp[ent] = hp;
}
ui ent_gethp(world *w,ui ent)
{
	return w->hp[ent];
}
/**************************************
Objects / Init Methods? w/e you want, really. just wraps calls.
**************************************/
void player_init(world *w,ui ent)
{
	ent_setsize(w,ent,32.0,32.0);
	ent_setpos(w,ent,256.0,256.0);
	ent_setvel(w,ent,0.0,0.0);
	ent_setcol(w,ent,255,255,255); // pasty ass
	ent_sethp(w,ent,100);
	cmp_mgr(w,ent,PLAYER_MASK); // add all of these, O(1) who cares.
}
/**************************************
Systems
**************************************/
void sys_collide()
{
	for(ui i=0;i<MAXENTS;i++)
	{

	}
}
void sys_render()
{
	for(ui i=0;i<MAXENTS;i++)
	{

	}
}
void sys_damage()
{
	for(ui i=0;i<MAXENTS;i++)
	{

	}	
}
int main(int argc, char **argv)
{
	// register systems
	sys_reg(&w,COLLISION_MASK,sys_collide);
	sys_reg(&w,RENDER_MASK,sys_render);
	sys_reg(&w,DAMAGE_MASK,sys_damage);
	// create a player ent
	player_init(&w,ent_reg(&w));
	// literally just make a game loop and loop systems and call their callbacks.
	return 0;
}