#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include "settings.h"

class LedManager
{
public:
    static LedManager* init();
    static LedManager* getInstance();
    ~LedManager();

    void updateLeds();

    void setLedCounts(quint16 top, quint16 bottom, quint16 left, quint16 right);
    void setColor(QColor color);

protected:
    LedManager();

private:
    static LedManager* instance;

    Settings* settings;

    bool enabled;

    struct LedColor
    {
        quint8 r;
        quint8 g;
        quint8 b;
    };

    quint16 ledCount;
    LedColor* ledColors;

    quint16 topLedCount;
    quint16 bottomLedCount;
    quint16 leftLedCount;
    quint16 rightLedCount;

    float rCorrection;
    float gCorrection;
    float bCorrection;
};

#endif // LEDMANAGER_H
