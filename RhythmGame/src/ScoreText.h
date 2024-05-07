#pragma once

#include "Common.h"

#include <string>

#include <raylib.h>

enum class NoteScore : int
{
	SCORE_TERRIBLE = 0,
	SCORE_OKAY,
	SCORE_GOOD,
	SCORE_PERFECT,

	SCORE_COUNT
};

class ScoreText
{
public:
	ScoreText() = default;
	~ScoreText() = default;

	void Init(Vector2 position, NoteScore score, float lifeSpan = 4.0f);
	void Destroy();

	void Update(double dt);
	void Draw();

	void SetDirty(bool dirty = true) { m_dirty = dirty; }
	bool IsDirty() { return m_dirty; }

private:
	bool m_dirty = true;

	Vector2 m_position = { 0, 0 };
	float m_fontScale = 16.0f;

	const float m_floatSpeed = 12.0f;

	NoteScore m_score;

	float m_lifeSpan;
	float m_timeLeft = 0.0f;

};
