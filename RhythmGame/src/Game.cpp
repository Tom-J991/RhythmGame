#include "Game.h"

#include <iostream>

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
	m_loadedMusic["BGM_REMINDME"] = LoadMusicStream("./assets/bgm/remindme.ogg");

	m_beatMaps["BRAZIL"] =
	{
		m_loadedMusic["BGM_BRAZIL"], 128.0f,
		{ // Notes (Unused, right now)
			1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
		}
	};

	m_beatMaps["FIRE"] =
	{
		m_loadedMusic["BGM_FIRE"], 200.0f,
		{ // Notes (Unused, right now)
			1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
		}
	};

	m_beatMaps["REMINDME"] =
	{
		m_loadedMusic["BGM_REMINDME"], 122.45f,
		{ // Notes (Unused, right now)
			1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
		}
	};
}

void Game::Init()
{
	m_notePool.InitNoteCallbacks([this](const float d) { OnNoteHit(d); }, [this]() { OnNoteMiss(); });

	m_currentBeatMap = m_beatMaps["BRAZIL"];
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
	UnloadMusicStream(m_loadedMusic["BGM_REMINDME"]);
	UnloadSound(m_loadedSounds["SFX_METRONOME"]);

	CloseAudioDevice();
	CloseWindow();
}

void Game::Update(double dt)
{
	UpdateMusicStream(m_currentBeatMap.music);

	m_notePool.Update(dt, m_songPositionInBeats, (float)m_beatsInAdvance);
	m_scoreTextPool.Update(dt);

	if (IsKeyPressed(KEY_Z))
	{
		NoteObject *note = m_notePool.FindNearestNote();
		if (note != nullptr)
			note->DoHit();
	}

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
		//m_notePool.CreateNote(m_currentBeatMap.notes[m_nextNoteIndex], { SCREEN_WIDTH_HALF, -32.0f }, { SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF }, 16.0f);

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
	m_notePool.CreateNote(beat, {SCREEN_WIDTH_HALF-8, -16.0f}, {SCREEN_WIDTH_HALF-8, SCREEN_HEIGHT_HALF-8}, 16.0f);

	PlaySound(m_loadedSounds["SFX_METRONOME"]);
}

void Game::Draw()
{
	ClearBackground(RAYWHITE);

	DrawRectangle(SCREEN_WIDTH_HALF - (HITBOX_SIZE/2.0f), SCREEN_HEIGHT_HALF - (HITBOX_SIZE/2.0f), HITBOX_SIZE, HITBOX_SIZE, LIGHTGRAY);
	DrawRectangle(SCREEN_WIDTH_HALF - (HITBOX_SIZE/4.0f), SCREEN_HEIGHT_HALF - (HITBOX_SIZE/4.0f), (int)(HITBOX_SIZE/2.0f), (int)(HITBOX_SIZE/2.0f), GRAY);
	DrawRectangle(SCREEN_WIDTH_HALF - (HITBOX_SIZE/8.0f), SCREEN_HEIGHT_HALF - (HITBOX_SIZE/8.0f), (int)(HITBOX_SIZE/4.0f), (int)(HITBOX_SIZE/4.0f), DARKGRAY);

	m_notePool.Draw();
	m_scoreTextPool.Draw();

	DrawRectangle(64, SCREEN_HEIGHT-64-16, (int)(m_songTimeNormalized * 512.0f), 16, MAROON);
}

void Game::OnNoteHit(const float distance)
{
	NoteScore score = NoteScore::SCORE_TERRIBLE;

	float halfSize = HITBOX_SIZE / 2.0f;

	if (distance > halfSize) // Miss.
	{
		OnNoteMiss();
		return;
	}

	// Within light gray box.
	if (distance <= (halfSize/4.0f))
		score = NoteScore::SCORE_PERFECT;
	else if (distance <= (halfSize/2.0f))
		score = NoteScore::SCORE_GOOD;
	else if (distance <= halfSize)
		score = NoteScore::SCORE_OKAY;

	m_scoreTextPool.CreateScoreText({ SCREEN_WIDTH_HALF-HITBOX_SIZE, SCREEN_HEIGHT_HALF-HITBOX_SIZE }, score, 1.0f);
}

void Game::OnNoteMiss()
{
	m_scoreTextPool.CreateScoreText({ SCREEN_WIDTH_HALF-HITBOX_SIZE, SCREEN_HEIGHT_HALF-HITBOX_SIZE }, NoteScore::SCORE_TERRIBLE, 1.0f);
}
