#pragma once
#include <SFML/Graphics.hpp>
#include "board.h"
#include "score.h"

class Game{
	public:
            Game();
	    void run();
	private:
            sf::RenderWindow mWindow;
            Board mBoard;
            Score mScore;

            // Dodatne varijable dolaze ovdje... KOJA JE TIPKA STISNUTA ITD
			bool mUpPressed = false;
			bool mLeftPressed = false;
			bool mRightPressed = false;
			bool mSpacePressed = false;
			bool mDownPressed = false;

	    void processEvents();
            void update(sf::Time dt);
            void render();
};




