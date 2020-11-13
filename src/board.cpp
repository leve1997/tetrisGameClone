#include "board.h"


// Vaš kod dolazi ovdje

Board::Board() : mGrid(sf::Lines), mUniformDist(0,10)
{
	size_t i = 0;
	mGrid.resize(200);
	
	while (i < 42) {
		mGrid[i].position = sf::Vector2f(50, 50 + 10 * i);
		mGrid[i + 1].position = sf::Vector2f(250, 50 + 10 * i);
		i = i + 2;
	}

	i = 0;
	 
	while (i < 22) {
		mGrid[i + 42] = sf::Vector2f(50 + 10 * i, 50);
		mGrid[i + 1 + 42] = sf::Vector2f(50 + 10 * i, 450);
		i = i + 2;
	}

	mOriginX = 50;
	mOriginY = 50;
	mDx = 10;
	mDy = 10;
	
	for (i = 0; i < mRows; i++) {
		for (size_t j = 0; j < mCols; j++) {
			mGridContent[i][j] = -1;
		}
	}
}

void Board::update(int Operacija)
{
	bool pom = true;
	if (Operacija == 1) movePiece(-1, 0);
	if (Operacija == 2) movePiece(1, 0);
	if (Operacija == 3) rotatePiece();
	if (Operacija == 4) QuickDrop();
	if (Operacija == 5) pom = movePiece(0, 1);

	if (pom == false) {
		mNoLinesCompleted = clearLines(); 
		spawnPiece();
	}
}

void Board::spawnPiece()
{
	if (!mGameOver) {
		mPiece.mRotacija = mUniformDist(mRNE) % 4;
		int pom = mUniformDist(mRNE) % 7;
		Tetrimino::Types oblik;
		switch (pom) {
		case 0: oblik = Tetrimino::Types::O; break;
		case 1: oblik = Tetrimino::Types::I; break;
		case 2:	oblik = Tetrimino::Types::S; break;
		case 3: oblik = Tetrimino::Types::Z; break;
		case 4: oblik = Tetrimino::Types::L; break;
		case 5: oblik = Tetrimino::Types::J; break;
		case 6: oblik = Tetrimino::Types::T; break;
		}

		mPiece.mType = oblik;

		for (int i = 0; i < 4; i++) {
			for (int j = 3; j < 7; j++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i][j - 3] != 0) {
					if (mGridContent[i][j] != -1) mGameOver = true;
					mGridContent[i][j] = oblik;
				}
			}
		}
		mPiece.mXposition = 50;
		mPiece.mYposition = 110;

	}
}

bool Board::isGameOver() const
{
	return mGameOver;
}

int Board::getNoLinesCompleted() const
{
	return mNoLinesCompleted;
}

void Board::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mGrid, states);

	for (int i = 0; i < mRows; i++) {
		for (int j = 0; j < mCols; j++) {
			if (mGridContent[i][j] != -1) {
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(19, 19));
				rect.setPosition(50 + 20*j, 50 + 20*i);
				rect.setFillColor(Tetrimino::mColors[mGridContent[i][j]]);
				target.draw(rect, states);
			}
			
		}
	}
}

// for rotation
bool Board::checkSpaceForRotation()
{
	int indx = (mPiece.mXposition - 50) / 20;
	int indy = (mPiece.mYposition - 50) / 20;
	
	// lijeva granica
	if (mPiece.mYposition == 30)
		for (int i = 0; i < 4; i++)
			if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija + 1][i][0] != 0) return false;

	if (mPiece.mYposition == 10)
		for (int i = 0; i < 4; i++)
			if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija + 1][i][1] != 0) return false;

	// desna granica
	if (mPiece.mYposition == 190)
		for (int i = 0; i < 4; i++)
			if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija + 1][i][3] != 0) return false;

	if (mPiece.mYposition == 210)
		for (int i = 0; i < 4; i++)
			if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija + 1][i][2] != 0) return false;

	// ako vec je neki piece na tom mjestu
	removePiece();
	for (int i = indx; i < indx + 4; i++) {
		for (int j = indy; i < indy + 4; i++) {
			if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija + 1][i - indx][j - indy] != 0
				&& mGridContent[i][j] != -1) {
				placePiece(); // mozda ce trebati rucno (uvesti kopiju polja)
				return false;
			}
		}
	}
	return true;
}

bool Board::checkSpaceForMoving(int x, int y)
{
	int indx = (mPiece.mXposition - 50) / 20;
	int indy = (mPiece.mYposition - 50) / 20;

	// lijeva strana
	if (x < 0) {
		if (mPiece.mYposition == 50)
			for (int i = 0; i < 4; i++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i][0] != 0) return false;
			}
		
		if (mPiece.mYposition == 30) 
			for (int i = 0; i < 4; i++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i][1] != 0) return false;
			}
		
		if (mPiece.mYposition < 30) return false;

		// + ako je lijevo vec zauzeto
		removePiece();
		for (int i = indx; i < indx + 4; i++) {
			for (int j = indy; j < indy + 4; j++) {
				if (j == 0) continue;
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i - indx][j - indy] != 0
					&& mGridContent[i][j - 1] != -1) {
					placePiece();
					return false;
				}
			}
		}
	}

	// desna strana
	if (x > 0) {
		if (mPiece.mYposition == 170)
			for (int i = 0; i < 4; i++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i][3] != 0) return false;
			}

		if (mPiece.mYposition == 190) 
			for (int i = 0; i < 4; i++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i][2] != 0) return false;
			}

		if (mPiece.mYposition > 190) return false;

		// + ako je desno vec zauzeto
		removePiece();
		for (int i = indx; i < indx + 4; i++) {
			for (int j = indy; j < indy + 4; j++) {
				if (j == 0) continue;
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i - indx][j - indy] != 0
					&& mGridContent[i][j + 1] != -1) {
					placePiece();
					return false;
				}
			}
		}
	}

	// dolje
	if (y != 0) {
		if (mPiece.mXposition == 370) 
			for (int i = 0; i < 4; i++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][3][i] != 0) return false; 
				// dosli smo na dno ploce
			}

		if (mPiece.mXposition == 390) 
			for (int i = 0; i < 4; i++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][2][i] != 0) return false;
				// dosli smo na dno ploce
			}
		if (mPiece.mXposition> 390) return false;

		// + ako ima nesto ispod
		
		removePiece();
		for (int i = indx; i < indx + 4; i++) {
			for (int j = indy; j < indy + 4; j++) {
				if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i - indx][j - indy] != 0
					&& mGridContent[i+1][j] != -1) {
					placePiece();
					return false;
				}
			}
		}
		
	}
	return true;
}

void Board::placePiece()
{
	int indx = (mPiece.mXposition - 50) / 20;
	int indy = (mPiece.mYposition - 50) / 20;

	for (int i = indx; i < indx + 4; i++) {
		for (int j = indy; j < indy + 4; j++) {
			if (Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i - indx][j - indy] != 0)
				mGridContent[i][j] = mPiece.mType;
		}
	}
}

void Board::removePiece()
{
	int indx = (mPiece.mXposition - 50) / 20;
	int indy = (mPiece.mYposition - 50) / 20;

	for (int i = indx; i < indx + 4; i++) {
		for (int j = indy; j < indy + 4; j++) {
			if (mGridContent[i][j] != -1 && Tetrimino::mDijelovi[mPiece.mType][mPiece.mRotacija][i - indx][j - indy] != 0)
				mGridContent[i][j] = -1;
		}
	}
}

bool Board::movePiece(int x, int y)
{
	if (!checkSpaceForMoving(x,y))	return false;
	if (x < 0) mPiece.mYposition -= 20;
	else if (x > 0) mPiece.mYposition += 20;
	else;

	if (y != 0) mPiece.mXposition += 20;

	placePiece();
	return true;
}

bool Board::rotatePiece()
{
	if (!checkSpaceForRotation()) return false;
	mPiece.rotate();
	placePiece();
	return true;
}

bool Board::isLineFull(int y) const
{
	for (int i = 0; i < 10; i++) {
		if (mGridContent[y][i] == -1) return false;
	}
	return true;
}

int Board::clearLines()
{
	int brojac = 0, j;

	for (int i = 19; i >= 0; i--) {
		if (isLineFull(i)) {
			brojac++;
			j = i - 1;
			while (j > 0) {
				for (int k = 0; k < 10; k++)
					mGridContent[j + 1][k] = mGridContent[j][k];
				j--;
			}
			i++;
		}
	}
	return brojac;
}

void Board::QuickDrop()
{
	while (movePiece(0, 1)) {}
}
