#ifndef AUDIO_H
#define AUDIO_H
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QUrl>
class audio {
public:
  audio();
  void play(QString filePath, int isBackground);

private:
  QMediaPlayer *music;
  QAudioOutput *audioOutput;
};

extern audio ambiencePlayer;
extern audio musicPlayer;
extern audio sfxPlayer;

#endif // AUDIO_H
