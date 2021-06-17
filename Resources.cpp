#include "Resources.h"
#include "SDL_macros.h"

vector<Resources::FontInfo> Resources::fonts_{
	//
			{ ARIAL16, "resources/fonts/ARIAL.ttf", 16 }, //
			{ ARIAL24, "resources/fonts/ARIAL.ttf", 24 } //
};

vector<Resources::ImageInfo> Resources::images_{
	//
			{ PacManSprites, "resources/images/pacman_sprites.png" }, //
			{ Red, "resources/images/Red.png" }, //
			{ Blue, "resources/images/Blue.png" }  //
};

vector<Resources::TextMsgInfo> Resources::messages_{
	//
			{ PressEnterToStartANewGame, "Press ENTER to start a new game", { COLOR(0xaaffffff) }, ARIAL16 }, //
			{ PressEnterToContinue, "Press ENTER to continue", { COLOR(0xaaffbbff) }, ARIAL24 }, //
			{ Player1Wins, "Player1 wins!", { COLOR(0x0000ffff) }, ARIAL24 }, //
			{ Player2Wins, "Player2 wins!", { COLOR(0xff0000ff) }, ARIAL24 } //
};

vector<Resources::MusicInfo> Resources::musics_{
	//
			{ Music, "resources/sound/music.ogg" }, //
};

vector<Resources::SoundInfo> Resources::sounds_{
	//
			{ Collision, "resources/sound/collision.wav" }, //
};