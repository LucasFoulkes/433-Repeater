#include <RCSwitch.h>
static const char *bin2tristate(const char *bin);
static char *dec2binWzerofill(unsigned long Dec, unsigned int bitLength);

RCSwitch mySwitch = RCSwitch();

void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int *raw, unsigned int protocol)
{

    const char *b = dec2binWzerofill(decimal, length);
    Serial.println(b);
    mySwitch.send(b);
}

static char *dec2binWzerofill(unsigned long Dec, unsigned int bitLength)
{
    static char bin[64];
    unsigned int i = 0;

    while (Dec > 0)
    {
        bin[32 + i++] = ((Dec & 1) > 0) ? '1' : '0';
        Dec = Dec >> 1;
    }

    for (unsigned int j = 0; j < bitLength; j++)
    {
        if (j >= bitLength - i)
        {
            bin[j] = bin[31 + i - (j - (bitLength - i))];
        }
        else
        {
            bin[j] = '0';
        }
    }
    bin[bitLength] = '\0';

    return bin;
}

void setup()
{
    Serial.begin(115200);
    mySwitch.enableReceive(0); // Receiver on interrupt 0 => that is pin #2
    mySwitch.enableTransmit(10);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    digitalWrite(LED_BUILTIN, LOW);
    if (mySwitch.available())
    {
        output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(), mySwitch.getReceivedProtocol());
        mySwitch.resetAvailable();
        digitalWrite(LED_BUILTIN, HIGH);
    }
}