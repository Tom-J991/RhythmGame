#include "ScoreText.h"

void ScoreText::Init(Vector2 position, NoteScore score, float lifeSpan)
{
	m_position = position;
	m_score = score;
	m_lifeSpan = lifeSpan;
	m_timeLeft = lifeSpan;

	m_dirty = false;
}

void ScoreText::Destroy()
{
	m_dirty = true;
}

void ScoreText::Update(double dt)
{
	if (m_dirty)
		return;

	m_timeLeft -= dt;

	float random = GetRandomValue(-4.0f, 4.0f);
	m_position.x += dt * m_floatSpeed * random;
	m_position.y -= dt * m_floatSpeed;

	if (m_timeLeft <= 0.0f)
		Destroy();
}

void ScoreText::Draw()
{
	if (m_dirty)
		return;

	Color color = BLACK;
	color.a = (m_timeLeft / m_lifeSpan) * 255;

	std::string text = "Default";
	switch (m_score)
	{
		case NoteScore::SCORE_TERRIBLE: { text = "TERRIBLE!"; } break;
		case NoteScore::SCORE_OKAY: { text = "OKAY."; } break;
		case NoteScore::SCORE_GOOD: { text = "GOOD!"; } break;
		case NoteScore::SCORE_PERFECT: { text = "PERFECT!"; } break;
	}

	int m = MeasureText(text.c_str(), (int)m_fontScale);
	DrawText(text.c_str(), (int)m_position.x - m, (int)m_position.y - (int)(m_fontScale/2.0f), (int)m_fontScale, color);
}
