#include "score.h"
#include <cstdlib>
#include <iostream>
#include <string>

Score::Score()
{
    if(!mFont.loadFromFile("Gasalt-Regular.ttf")){
        std::cerr << "Ne mogu učitati font: Gasalt-Regular.ttf !" << std::endl;
        std::exit(1);
    }
   mGameOverText.setString("");
   mGameOverText.setFont(mFont);
   mGameOverText.setPosition(sf::Vector2f(280, 100));
   mScoreText.setString("Score : 0");
   mScoreText.setFont(mFont);
   mScoreText.setPosition(sf::Vector2f(300, 50));

}

void Score::addNoLinesCompleted(int n)
{
	
	mScore += n*10;
}

void Score::setGameOver()
{
  mGameOverText.setString("Game is over.");
}

void Score::update()
{
	mScoreText.setString("Score : " + std::to_string(mScore));
}

void Score::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mScoreText, states);
	target.draw(mGameOverText, states);
}
