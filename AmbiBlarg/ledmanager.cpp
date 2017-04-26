#include "ledmanager.h"

#include "arduinocommunication.h"

LedManager* LedManager::instance = NULL;

LedManager* LedManager::init()
{
    if (instance != NULL)
        throw new std::runtime_error("LedManager already inited.");

    instance = new LedManager();
    return instance;
}

LedManager* LedManager::getInstance()
{
    if (instance == NULL)
        throw new std::runtime_error("LedManager not inited.");

    return instance;
}

LedManager::LedManager()
{
    settings = Settings::getInstance();

    ledCount = 0;

    setLedCounts(settings->get("ledCount.top", 38).toUInt(),
                 settings->get("ledCount.bottom", 2).toUInt(),
                 settings->get("ledCount.left", 20).toUInt(),
                 settings->get("ledCount.right", 20).toUInt());

    rCorrection = settings->get("correction.r", 1).toFloat();
    gCorrection = settings->get("correction.r", 1).toFloat();
    bCorrection = settings->get("correction.r", 1).toFloat();
}

LedManager::~LedManager()
{
    setColor(Qt::black);
    updateLeds();

    if (ledCount > 0)
        delete[] ledColors;
}


void LedManager::updateLeds()
{
    ArduinoCommunication* communication = ArduinoCommunication::getInstance();

    communication->writeStr(MAGIC);
    communication->write16(ledCount);

    for (int i = 0; i < ledCount; i++)
    {
        communication->write8(ledColors[i].r * rCorrection);
        communication->write8(ledColors[i].g * gCorrection);
        communication->write8(ledColors[i].b * bCorrection);
    }
}

void LedManager::setLedCounts(quint16 top, quint16 bottom, quint16 left, quint16 right)
{
    topLedCount = top;
    bottomLedCount = bottom;
    leftLedCount = left;
    rightLedCount = right;

    quint16 newLedCount = topLedCount + bottomLedCount + leftLedCount + rightLedCount;

    if (newLedCount == ledCount)
        return;

    ledCount = newLedCount;

    ledColors = new LedColor[ledCount];
}

void LedManager::setColor(QColor color)
{
    quint8 r = color.red();
    quint8 g = color.green();
    quint8 b = color.blue();

    for (int i = 0; i < ledCount; i++)
    {
        ledColors[i].r = r;
        ledColors[i].g = g;
        ledColors[i].b = b;
    }
}
