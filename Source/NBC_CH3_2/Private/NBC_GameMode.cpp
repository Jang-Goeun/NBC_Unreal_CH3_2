#include "NBC_GameMode.h"
#include "NBC_Player.h"

ANBC_GameMode::ANBC_GameMode()
{
	// DefaultPawn 설정
	DefaultPawnClass = ANBC_Player::StaticClass();
}
