#pragma once

#include "Common.h"

#include "ScoreText.h"

#include <raylib.h>

class ScoreTextPool
{
public:
	ScoreTextPool() = default;
	~ScoreTextPool() = default;

	void CreateScoreText(Vector2 position, NoteScore score, float lifeSpan = 4.0f);

	void Update(double dt);
	void Draw();

private:
	static const size_t POOL_SIZE = 16;
	ScoreText m_text[POOL_SIZE];

};
