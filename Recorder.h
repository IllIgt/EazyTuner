#ifndef RECORDER_H
#define RECORDER_H
#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QStringListModel>
#include <memory>

class Recorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QStringListModel *devices READ devices CONSTANT)
public:
    Recorder();
    ~Recorder();
    QBuffer destinationFile;
    QIODevice *audioBuffer;
    QAudioOutput* oAudio;
    QAudioInput* iAudio;
    QStringListModel *devices() const;

public slots:
    void startRecord();
    void stopRecording();
    void handleRecordStateChanged(QAudio::State newStat);
    void handlePlayStateChanged(QAudio::State newState);
    void startPlaying();
private:
    std::unique_ptr<QStringListModel> mDevices;
};

#endif // RECORDER_H
