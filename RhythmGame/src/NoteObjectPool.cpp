#include "NoteObjectPool.h"

#include <vector>
#include <algorithm>
#include <iterator>

void NoteObjectPool::InitNoteCallbacks(const NoteHitCallback &hitCallback, const NoteMissCallback &missCallback)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_notes[i].SetNoteHitCallback(hitCallback);
		m_notes[i].SetNoteMissCallback(missCallback);
	}
}

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

NoteObject *NoteObjectPool::FindNearestNote()
{
	// TODO: Optimize this, two loops isn't very good.
	std::vector<float> yPositions;
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (m_notes[i].IsDirty())
			continue;
		yPositions.push_back(m_notes[i].GetPosition().y);
	}
	float maxY = *std::max_element(yPositions.begin(), yPositions.end());

	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (m_notes[i].IsDirty())
			continue;
		if (m_notes[i].GetPosition().y >= maxY - 0.001f && m_notes[i].GetPosition().y <= maxY + 0.001f) // Should be the lowest
		{
			return &m_notes[i];
		}
	}

	return nullptr; // Couldn't find it.
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
