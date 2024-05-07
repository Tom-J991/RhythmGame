#include "ScoreTextPool.h"

void ScoreTextPool::CreateScoreText(Vector2 position, NoteScore score, float lifeSpan)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (m_text[i].IsDirty())
		{
			m_text[i].Init(position, score, lifeSpan);
			return;
		}
	}
}

void ScoreTextPool::Update(double dt)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_text[i].Update(dt);
	}
}

void ScoreTextPool::Draw()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_text[i].Draw();
	}
}
