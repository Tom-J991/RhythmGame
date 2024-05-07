#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <list>

#include <raylib.h>

#include "NoteObject.h"
#include "NoteObjectPool.h"

struct BeatMap
{
	Music music;
	float bpm;

	std::vector<float> notes;

};

class Game
{
public:
	Game() = default;
	~Game() = default;

	void Run();

	bool IsRunning() { return m_isRunning; }

private:
	void LoadAssets();
	void Init();
	void Shutdown();
	void Update(double dt);
	void OnBeat(float beat);
	void Draw();

private:
	bool m_isRunning = false;

	std::unordered_map<std::string, Sound> m_loadedSounds;
	std::unordered_map<std::string, Music> m_loadedMusic;

	std::unordered_map<std::string, BeatMap> m_beatMaps;

	NoteObjectPool m_notePool;

	BeatMap m_currentBeatMap;

	const int m_beatsInAdvance = 4;

	bool m_songPlaying = false;

	float m_secondsPerBeat;

	float m_songPositionInSeconds;
	float m_songPositionInBeats;

	float m_songTimeNormalized;
	float m_timeSinceStart;

	int m_nextNoteIndex = 0;

};
