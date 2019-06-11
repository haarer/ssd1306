
template <class I>
void Interface~CONTROLLER~<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
    // TODO: place code here
    lcduint_t rx = w ? (x + w - 1) : (m_base.width() - 1);
    this->start();
    spiDataMode(0);
    this->send(0x22);
    spiDataMode(1);  // According to datasheet all args must be passed in data mode
    this->send(x);
    this->send(rx < m_base.width() ? rx : (m_base.width() - 1));
    spiDataMode(0);
    this->send(0x72);
    spiDataMode(1);  // According to datasheet all args must be passed in data mode
    this->send(y);
    this->send(m_base.height() - 1);
    spiDataMode(0);
    this->send(0x55);
    spiDataMode(1);
}

template <class I>
void Interface~CONTROLLER~<I>::nextBlock()
{
}

template <class I>
void Interface~CONTROLLER~<I>::endBlock()
{
    this->stop();
}

template <class I>
void Interface~CONTROLLER~<I>::spiDataMode(uint8_t mode)
{
    if ( m_dc >= 0 )
    {
        lcd_gpioWrite( m_dc, mode ? LCD_HIGH : LCD_LOW );
    }
}

