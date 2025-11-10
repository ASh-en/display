#include "dac8760.h"
#include <wiringPi.h>
#include <QDebug>
dac8760::dac8760(QObject *parent) : QObject(parent)
{
    wiringPiSetup();
    pinMode(SDO_PIN, INPUT);
    pinMode(SCLK_PIN, OUTPUT);
    pinMode(DIN_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLR_PIN, OUTPUT);

    digitalWrite(SCLK_PIN, LOW);
    digitalWrite(DIN_PIN, LOW);
    digitalWrite(LATCH_PIN, LOW);
    digitalWrite(CLR_PIN, LOW);

    DAC8760_Write(0x56, 0x0001);  // 软件复位

    uint16_t Ctrl_Sreg = 0x3005;  // 选择外部电流设置电阻，使能输出，电压输出±10V Range
    DAC8760_Write(0x55, Ctrl_Sreg);  // 写入控制寄存器

    uint16_t Conf_reg = 0;
    Conf_reg|=0x10;
    DAC8760_Write(0x57, Conf_reg);
}
void dac8760::DAC8760_Write(uint8_t Addr, uint16_t Data) {
    digitalWrite(LATCH_PIN, LOW);
    uint32_t Dtemp = 0;
    Dtemp += Addr;
    Dtemp <<= 16;
    Dtemp += Data;

    for (uint8_t i = 0; i < 24; i++) {
        if (Dtemp & 0x800000) {
            digitalWrite(DIN_PIN, HIGH);
        } else {
            digitalWrite(DIN_PIN, LOW);
        }
        digitalWrite(SCLK_PIN, HIGH);
        digitalWrite(SCLK_PIN, LOW);
        Dtemp <<= 1;
    }
    digitalWrite(LATCH_PIN, HIGH);
}
uint32_t dac8760::DAC8760_Read(uint8_t Addr) {
    uint32_t Rdata = 0;
    DAC8760_Write(0x02, Addr);

    for (uint8_t i = 0; i < 24; i++) {
        Rdata <<= 1;
        digitalWrite(SCLK_PIN, HIGH);
        if (digitalRead(SDO_PIN)) {
            Rdata++;
        }
        digitalWrite(SCLK_PIN, LOW);
    }
    return Rdata;
}

void dac8760::Delay(uint32_t nCount) {
    for (; nCount != 0; nCount--);
}
void dac8760::elec_write(double value){
    double SetValue;
    SetValue = value;
    SetValue -=4;
    SetValue *= (65536/16);
    if (SetValue<0)SetValue=0;//防止下溢出
    if (SetValue>65535)SetValue=65535;//防止上溢出
    uint16_t DAC_data = (uint16_t)SetValue;
    DAC8760_Write(0x01,DAC_data);//写入数据寄存器
}
void dac8760::send_thick(double thickness){
    elec_write(static_cast<float>(thickness));
    qDebug()<<"hello";
}
