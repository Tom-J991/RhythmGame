#pragma once

#include <raylib.h>

class NoteObject
{
public:
	NoteObject() = default;
	~NoteObject() = default;

	void Init(float beat, Vector2 startPos, Vector2 endPos, float size = 16.0f);
	void Destroy();

	void Update(double dt, const float songPosInBeats, const float beatsInAdvance);
	void Draw();

	void SetDirty(bool dirty = true) { m_dirty = dirty; }
	bool IsDirty() { return m_dirty; }

private:
	bool m_dirty = false;

	float m_beat;

	Vector2 m_position;
	float m_size;

	Vector2 m_startPos;
	Vector2 m_endPos;

};
