#pragma once

#include "NoteObject.h"

#include <raylib.h>

class NoteObjectPool
{
public:
	NoteObjectPool() = default;
	~NoteObjectPool() = default;

	void CreateNote(float beat, Vector2 startPos, Vector2 endPos, float size = 16.0f);

	void Update(double dt, const float songPosInBeats, const float beatsInAdvance);
	void Draw();

private:
	static const size_t POOL_SIZE = 16;
	NoteObject m_notes[POOL_SIZE];

};
