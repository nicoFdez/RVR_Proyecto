#include "SDLTexturesManager.h"
#include <SDL_image.h>
#include <assert.h>

SDLTexturesManager::SDLTexturesManager() :
		initialized_(false) {
}

SDLTexturesManager::~SDLTexturesManager() {

	if (!initialized_)
		return;

	// free all textures
	for (const auto &pair : textures_) {
		if (pair.second != nullptr)
			delete pair.second;
	}

	IMG_Quit();

}

bool SDLTexturesManager::init() {
	if (initialized_)
		return false;

	// SDL Image
	int imgInit_ret = IMG_Init(
			IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	assert(imgInit_ret != 0);

	initialized_ = true;

	return true;
}

bool SDLTexturesManager::loadFromImg(std::size_t tag,
		SDL_Renderer *renderer, const string &fileName) {

	if (!initialized_)
		return false;

	Texture *texture = new Texture(renderer, fileName);
	if (texture->isReady()) {
		storeTexture(tag, texture);
		return true;
	}

	// if we get here something went wrong
	return false;
}

bool SDLTexturesManager::loadFromText(std::size_t tag,
		SDL_Renderer *renderer, const string &text, const Font *font,
		const SDL_Color &color) {

	if (!initialized_)
		return false;

	Texture *texture = new Texture(renderer, text, font, color);
	if (texture->isReady()) {
		storeTexture(tag, texture);
		return true;
	}

	// if we get here something went wrong
	return false;

}

void SDLTexturesManager::storeTexture(std::size_t tag, Texture *texture) {
	Texture *curr = textures_[tag];
	if (curr != nullptr)
		delete curr;
	textures_[tag] = texture;
}
