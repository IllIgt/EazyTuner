#ifndef RECORDER_H
#define RECORDER_H
#include <QObject>
#include <QAudioInput>
#include <QFile>
#include <QStringListModel>
#include <memory>

class Recorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QStringListModel *devices READ devices CONSTANT)
public:
    Recorder();
    ~Recorder();
    QFile destinationFile;
    QAudioInput* audio;
    QStringListModel *devices() const;

public slots:
    void startRecord();
    void stopRecording();
    void handleStateChanged(QAudio::State newStat);
private:
    std::unique_ptr<QStringListModel> mDevices;
};

#endif // RECORDER_H
