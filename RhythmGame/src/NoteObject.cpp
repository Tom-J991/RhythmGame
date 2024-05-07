#include "NoteObject.h"

#include <raymath.h>

void NoteObject::Init(float beat, Vector2 startPos, Vector2 endPos, float size)
{
	m_beat = beat;

	m_startPos = startPos;
	m_endPos = endPos;

	m_position = startPos;
	m_size = size;

	m_dirty = false;
}

void NoteObject::Destroy()
{
	m_dirty = true;
}

void NoteObject::Update(double dt, const float songPosInBeats, const float beatsInAdvance)
{
	if (m_dirty)
		return;

	m_position = Vector2Lerp(m_startPos, m_endPos, (beatsInAdvance - (m_beat - songPosInBeats)) / beatsInAdvance);

	if (Vector2Distance(m_position, m_endPos) <= 4.0f)
		Destroy();
}

void NoteObject::Draw()
{
	if (m_dirty)
		return;

	DrawRectangle((int)m_position.x, (int)m_position.y, (int)m_size, (int)m_size, DARKGRAY);
}
