#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "serial.h"
#include "dac8760.h"
#include <QThread>
#include "widget.h"
#include "tcp.h"
class mythread : public QThread
{
    Q_OBJECT
signals:

public:
  //  mythread()

private:

};

#endif // MYTHREAD_H
