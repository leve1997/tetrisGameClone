#include "game.h"

Game::Game() : mWindow(sf::VideoMode(500,600), "Tetris")
{
    // Vaš kod dolazi ovdje (ako bude potrebe).   
}

void Game::run(){
	mBoard.spawnPiece();
	sf::Clock clock;
	sf::Time p_vrijeme = sf::Time::Zero;
	sf::Time dt = sf::seconds(4.0/60.f);
	sf::Time mVrijeme = sf::Time::Zero;

    while(mWindow.isOpen()){
        processEvents();

		mVrijeme += clock.getElapsedTime();
		if (mVrijeme >= sf::seconds(0.4f)) {
			mBoard.update(5);
			mVrijeme = sf::Time::Zero;
		}

		p_vrijeme += clock.restart();
		
		while (p_vrijeme >= dt) {
			p_vrijeme -= dt;
			if (mDownPressed) mBoard.update(5);
			update(dt);
		}

        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            mWindow.close();
            break;

        case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Left) mLeftPressed = true;
			if (event.key.code == sf::Keyboard::Right) mRightPressed = true;
			if (event.key.code == sf::Keyboard::Up) mUpPressed = true;
			if (event.key.code == sf::Keyboard::Space) mSpacePressed = true;
			if (event.key.code == sf::Keyboard::Down) mDownPressed = true;
	    break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Left) mLeftPressed = false;
			if (event.key.code == sf::Keyboard::Right) mRightPressed = false;
			if (event.key.code == sf::Keyboard::Up) mUpPressed = false;
			if (event.key.code == sf::Keyboard::Space) mSpacePressed = false;
			if (event.key.code == sf::Keyboard::Down) mDownPressed = false;
			break;
        case sf::Event::Resized:
            // Spriječi deformaciju objekata prilikom promjene veličine prozora
            sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);
            mWindow.setView(sf::View(viewRect));
            break;
        }
    }
}

void Game::update(sf::Time dt)
{
   // Updatiranje scene. Vaš kod dolazi ovdje    za board i score
	if (mLeftPressed) mBoard.update(1);
	if (mRightPressed) mBoard.update(2);
	if (mUpPressed) mBoard.update(3);
	if (mSpacePressed) mBoard.update(4);

	mScore.addNoLinesCompleted(mBoard.getNoLinesCompleted());
	mBoard.clearNoLinesCompleted();
	mScore.update();

	if (mBoard.isGameOver()) mScore.setGameOver();
	//if (mDownPressed) mBoard.update(5);
}

//iscrtavanje
void Game::render()
{
     mWindow.clear();


     mWindow.draw(mBoard);
     mWindow.draw(mScore);

     mWindow.display();
}
