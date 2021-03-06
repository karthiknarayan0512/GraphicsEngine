#pragma once

namespace eae6320
{
	namespace Audio
	{
		void Initialize();
		void IncreaseVolume();
		bool AddAudioFile(const char* i_AudioPath, bool bLoop = false, float i_InitialVolume = 1.0f);
		void PlayAudio(int i_Index);
		void StopAudio(int i_Index);

		void StopAll();

		void SetVolume(float currentDistance, float maxDistance, float minDistance);
	}
}
