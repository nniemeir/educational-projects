#include "../../include/audio.h"

audio::audio() {
  music = new QMediaPlayer();

  audioOutput = new QAudioOutput();
}

void audio::play(QString filePath, int isBackground) {
  if (music->playbackState() == QMediaPlayer::PlayingState) {
    music->stop();
  }
  music->setAudioOutput(audioOutput);
  music->setSource(QUrl(filePath));
  audioOutput->setVolume(1);
  if (isBackground == 1) {
    music->setLoops(QMediaPlayer::Infinite);
  }
  music->play();
}

audio ambiencePlayer;
audio musicPlayer;
audio sfxPlayer;
