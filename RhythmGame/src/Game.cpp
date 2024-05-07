#include "Game.h"

#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TARGET_FPS 60

void Game::Run()
{
	InitAudioDevice();
	SetMasterVolume(0.5f);

	LoadAssets();

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rhythm Game");
	SetTargetFPS(TARGET_FPS);

	Init();

	double startTime = GetTime();
	double deltaTime = 1.0 / TARGET_FPS;

	m_isRunning = true;
	while (m_isRunning)
	{
		double currentTime = GetTime();
		deltaTime = currentTime - startTime;
		startTime = currentTime;

		Update(deltaTime);

		BeginDrawing();
		Draw();
		EndDrawing();

		m_isRunning = !WindowShouldClose();
	}

	Shutdown();
}

void Game::LoadAssets()
{
	m_loadedSounds["SFX_METRONOME"] = LoadSound("./assets/sfx/metronome.wav");
	m_loadedMusic["BGM_BRAZIL"] = LoadMusicStream("./assets/bgm/brazil.ogg");
	m_loadedMusic["BGM_FIRE"] = LoadMusicStream("./assets/bgm/fire.ogg");

	m_beatMaps["BRAZIL"] =
	{
		m_loadedMusic["BGM_BRAZIL"], 128.0f,
		{ // Notes
			1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
		}
	};

	m_beatMaps["FIRE"] =
	{
		m_loadedMusic["BGM_FIRE"], 200.0f,
		{ // Notes
			1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
		}
	};
}

void Game::Init()
{
	m_currentBeatMap = m_beatMaps["FIRE"];
	m_secondsPerBeat = 60.0f / m_currentBeatMap.bpm;

	PlayMusicStream(m_currentBeatMap.music);
	m_songTimeNormalized = 0.0f;
	m_songPlaying = true;

	m_timeSinceStart = GetMusicTimePlayed(m_currentBeatMap.music);
}

void Game::Shutdown()
{
	if (IsMusicStreamPlaying(m_currentBeatMap.music))
		StopMusicStream(m_currentBeatMap.music);

	UnloadMusicStream(m_loadedMusic["BGM_BRAZIL"]);
	UnloadMusicStream(m_loadedMusic["BGM_FIRE"]);
	UnloadSound(m_loadedSounds["SFX_METRONOME"]);

	CloseAudioDevice();
	CloseWindow();
}

void Game::Update(double dt)
{
	UpdateMusicStream(m_currentBeatMap.music);

	m_notePool.Update(dt, m_songPositionInBeats, (float)m_beatsInAdvance);

	m_songTimeNormalized = GetMusicTimePlayed(m_currentBeatMap.music) / GetMusicTimeLength(m_currentBeatMap.music);
	if (m_songTimeNormalized > 1.0f)
	{
		m_songTimeNormalized = 1.0f;
		m_songPlaying = false;
		return;
	}

	m_songPositionInSeconds = (GetMusicTimePlayed(m_currentBeatMap.music) - m_timeSinceStart);
	m_songPositionInBeats = m_songPositionInSeconds / m_secondsPerBeat;

	if (m_nextNoteIndex < m_currentBeatMap.notes.size() &&
		m_currentBeatMap.notes[m_nextNoteIndex] < m_songPositionInBeats + m_beatsInAdvance)
	{
		//m_notePool.CreateNote(m_currentBeatMap.notes[m_nextNoteIndex], { 320.0f, -32.0f }, { 320.0f, 240.0f }, 16.0f);

		m_nextNoteIndex++;
	}

	// Callback on each beat.
	static int lastBeat = -1;
	if (lastBeat != (int)m_songPositionInBeats)
	{
		OnBeat(lastBeat + 5);
		lastBeat = (int)m_songPositionInBeats;
	}
}

void Game::OnBeat(float beat)
{
	m_notePool.CreateNote(beat, {320.0f-8, -16.0f}, {320.0f-8, 240.0f-8}, 16.0f);

	PlaySound(m_loadedSounds["SFX_METRONOME"]);
}

void Game::Draw()
{
	ClearBackground(RAYWHITE);

	DrawRectangle(320.0f - 12, 240.0f - 12, 24.0f, 24.0f, LIGHTGRAY);

	m_notePool.Draw();

	DrawRectangle(64, 480-64-16, (int)(m_songTimeNormalized * 512.0f), 16, MAROON);
}
