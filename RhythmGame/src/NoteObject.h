#pragma once

#include "Common.h"

#include <functional>

#include <raylib.h>

using NoteHitCallback = std::function<void(const float)>;
using NoteMissCallback = std::function<void()>;

class NoteObject
{
public:
	NoteObject() = default;
	~NoteObject() = default;

	void Init(float beat, Vector2 startPos, Vector2 endPos, float size = 16.0f);
	void Destroy();

	void Update(double dt, const float songPosInBeats, const float beatsInAdvance);
	void Draw();

	void DoHit();

	void SetNoteHitCallback(const NoteHitCallback &callback) { m_noteHitCallback = callback; }
	void SetNoteMissCallback(const NoteMissCallback &callback) { m_noteMissCallback = callback; }

	Vector2 GetPosition() { return m_position; }

	void SetDirty(bool dirty = true) { m_dirty = dirty; }
	bool IsDirty() { return m_dirty; }

private:
	bool m_dirty = true;

	float m_beat;

	Vector2 m_position = { 0, 0 };
	float m_size = 16.0f;

	Vector2 m_startPos;
	Vector2 m_endPos;

	NoteHitCallback m_noteHitCallback;
	NoteMissCallback m_noteMissCallback;

};
