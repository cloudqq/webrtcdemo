#include <webrtc/voice_engine/include/voe_base.h>
#include <webrtc/voice_engine/include/voe_errors.h>
#include <webrtc/voice_engine/include/voe_file.h>
#include <webrtc/voice_engine/include/voe_hardware.h>

#include<windows.h>

webrtc::VoiceEngine *pVoeEngine = NULL;
webrtc::VoEBase *pVoeBase = NULL;
webrtc::VoEFile *pVoeFile = NULL;
webrtc::VoEHardware *pDevice = NULL;
int audio_channel;

void do_common()
{
	int rc = 0;

	pVoeEngine = webrtc::VoiceEngine::Create();
	if (!pVoeEngine) {
		printf("Create voice engine failed.\n");
		return;
	}
	pVoeBase = webrtc::VoEBase::GetInterface(pVoeEngine);
	if (!pVoeEngine) {
		printf("Create voice base failed.\n");
		return;
	}
	pVoeFile = webrtc::VoEFile::GetInterface(pVoeEngine);
	if (!pVoeEngine) {
		printf("Create voice file failed.\n");
		return;
	}

	pDevice = webrtc::VoEHardware::GetInterface(pVoeEngine);
	if (!pDevice) {
		printf("Create device file failed.\n");
		return;
	}

	pVoeBase->Init();
	pDevice->SetRecordingDevice(0);
	pDevice->SetPlayoutDevice(0);

	audio_channel = pVoeBase->CreateChannel();
	pVoeBase->StartPlayout(audio_channel);

	char version[1024];
	rc = pVoeBase->GetVersion(version);
	if (rc != 0) {
		printf("Get version failed.\n");
		return;
	}

	printf("The webrtc version(%s)\n", version);
}
void recording()
{
	pVoeFile->StartRecordingMicrophone("RecordingMicrophone.pcm");

	// Recording 1 minutes;
	Sleep(60*1000);

	pVoeFile->StopRecordingMicrophone();
}

void playout()
{
	pVoeFile->StartPlayingFileLocally(audio_channel, "RecordingMicrophone.pcm");

	// Recording 1 minutes;
	Sleep(60 * 1000);
	pVoeFile->StopPlayingFileLocally(audio_channel);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: recordplayout 1(record) or 2(playout).\n");
		return -1;
	}
	do_common();

	if (strcmp(argv[1], "1") == 0) {
		recording();
	} else {
		playout();
	}

	return 0;
}