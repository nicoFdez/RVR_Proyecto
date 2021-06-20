#pragma once

#include <SDL.h>
#include <string>
#include <vector>

using namespace std;

/*
 * Clase que implementa la carga de los recursos
 */
class Resources {
public:

	//Texturas
	enum TextureId : std::size_t {
		// images
		PacManSprites,
		Red,
		Blue,
		// text
		PressEnterToStartANewGame,
		PressEnterToContinue,
		Player1Wins,
		Player2Wins
	};

	//Audio
	enum AudioId : std::size_t {
		// music
		Music,

		// sound effects
		Collision
	};

	//Fuentes
	enum FontId : std::size_t {
		ARIAL16, ARIAL24,
	};

	//Info de las fuentes
	struct FontInfo {
		FontId id;
		string fileName;
		int size;
	};

	//Info de las imagenes
	struct ImageInfo {
		TextureId id;
		string fileName;
	};

	//Info de los mensajes
	struct TextMsgInfo {
		TextureId id;
		string msg;
		SDL_Color color;
		FontId fontId;
	};

	//Info de la musica
	struct MusicInfo {
		AudioId id;
		string fileName;
	};

	//Info de los sonidos
	struct SoundInfo {
		AudioId id;
		string fileName;
	};

	static vector<FontInfo> fonts_; // initialized in .cpp
	static vector<ImageInfo> images_; // initialized in .cpp
	static vector<TextMsgInfo> messages_; // initialized in .cpp
	static vector<MusicInfo> musics_; // initialized in .cpp
	static vector<SoundInfo> sounds_; // initialized in .cpp
};
