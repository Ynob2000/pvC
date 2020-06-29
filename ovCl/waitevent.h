#ifndef WAITEVENT_H
#define WAITEVENT_H

#include <QObject>
#include <QThread>
#include <windows.h>
#include <windows.h>
#include <SetupAPI.h>


class waitEvent : public QThread
{
    Q_OBJECT

public:
    waitEvent(uint16_t vector, HANDLE  event);
    HANDLE event;
    uint16_t vector;

signals:
  void Event(uint16_t vector, HANDLE  event);

private:
  void run();

};

#endif // WAITEVENT_H
