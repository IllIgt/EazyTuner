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

    destinationFile.setFileName("/tmp/test.raw");
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

    audio = new QAudioInput(format, this);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    QTimer::singleShot(3000, this, SLOT(stopRecording()));
    audio->start(&destinationFile);
    // Records audio for 3000ms
}



void Recorder::stopRecording()
{
    audio->stop();
    destinationFile.close();
    delete audio;
}

void Recorder::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::StoppedState:
            if (audio->error() != QAudio::NoError) {
                // Error handling
            } else {
                // Finished recording
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

QStringListModel *Recorder::devices() const {
    return mDevices.get();
}

