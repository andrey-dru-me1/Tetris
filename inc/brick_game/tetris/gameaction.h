#ifndef GAMEACTION_H_497564
#define GAMEACTION_H_497564

typedef void (*gameaction_t)();

gameaction_t gameaction_launchfig;
gameaction_t gameaction_shiftfig;
gameaction_t gameaction_moveright;
gameaction_t gameaction_moveleft;
gameaction_t gameaction_rotatefig;
gameaction_t gameaction_initgame;
gameaction_t gameaction_endgame;

#endif
