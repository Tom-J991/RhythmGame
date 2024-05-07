#include "NoteObjectPool.h"

void NoteObjectPool::CreateNote(float beat, Vector2 startPos, Vector2 endPos, float size)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (m_notes[i].IsDirty())
		{
			m_notes[i].Init(beat, startPos, endPos, size);
			return;
		}
	}
}

void NoteObjectPool::Update(double dt, const float songPosInBeats, const float beatsInAdvance)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_notes[i].Update(dt, songPosInBeats, beatsInAdvance);
	}
}

void NoteObjectPool::Draw()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_notes[i].Draw();
	}
}
