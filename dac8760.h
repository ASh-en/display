#ifndef DAC8760_H
#define DAC8760_H

#include <QObject>
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>

#define SDO_PIN 28
#define SCLK_PIN 25
#define DIN_PIN 29
#define LATCH_PIN 27
#define CLR_PIN 24

class dac8760 : public QObject
{
    Q_OBJECT
public:
    explicit dac8760(QObject *parent = nullptr);
    void DAC8760_Write(uint8_t Addr, uint16_t Data);
    uint32_t DAC8760_Read(uint8_t Addr);
    void Delay(uint32_t nCount);
    void elec_write(float value);

public slots:
    void send_thick(double thickness);

};

#endif // DAC8760_H
