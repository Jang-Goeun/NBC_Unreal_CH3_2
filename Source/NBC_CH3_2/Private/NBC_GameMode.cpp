#include "NBC_GameMode.h"
#include "NBC_Player.h"
#include "NBC_PlayerController.h"

ANBC_GameMode::ANBC_GameMode()
{
	// DefaultPawn 설정
	DefaultPawnClass = ANBC_Player::StaticClass();

	// 플레이어 컨트롤러 설정
	PlayerControllerClass = ANBC_PlayerController::StaticClass();
}
