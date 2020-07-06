#include "Recorder.h"
#include <QtDebug>
#include <QTimer>


Recorder::Recorder() {
    mDevices = std::make_unique<QStringListModel>();
    QList devicesInfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if (devicesInfo.length() == 0){
        qWarning() << "Нет доступных устройств ввода";
    }
    QStringList devices;
    foreach (QAudioDeviceInfo device, devicesInfo) {
        qWarning() << device.deviceName();
        devices.append(device.deviceName());
    }
    mDevices->setStringList(devices);
}

Recorder::~Recorder() {

}

void Recorder::startRecord() {

    destinationFile.open( QIODevice::WriteOnly | QIODevice::Truncate );

    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }

    iAudio = new QAudioInput(format, this);
    connect(iAudio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleRecordStateChanged(QAudio::State)));

    QTimer::singleShot(10000, this, SLOT(stopRecording()));
    iAudio->start(&destinationFile);
    // Records audio for 3000ms
}



void Recorder::stopRecording()
{
    iAudio->stop();
    destinationFile.close();
    delete iAudio;
    startPlaying();
}

void Recorder::handlePlayStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            oAudio->stop();
            destinationFile.close();
            delete oAudio;
            break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (oAudio->error() != QAudio::NoError) {
                // Error handling
            }
            break;

        default:
            // ... other cases as appropriate
            break;
    }

}

void Recorder::handleRecordStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::StoppedState:
            if (iAudio->error() != QAudio::NoError) {
                // Error handling
            } else {

            }
            break;

        case QAudio::ActiveState:
            // Started recording - read from IO device
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}

void Recorder::startPlaying()
{
    destinationFile.open(QIODevice::ReadOnly);

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    oAudio = new QAudioOutput(format, this);
    connect(oAudio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handlePlayStateChanged(QAudio::State)));
    oAudio->start(&destinationFile);
}

QStringListModel *Recorder::devices() const {
    return mDevices.get();
}

