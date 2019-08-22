
template <class I>
void Interface~CONTROLLER~<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
~SET_BLOCK~
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

template <class I>
void Interface~CONTROLLER~<I>::commandStart()
{
    this->start();
    if (m_dc >= 0)
        spiDataMode(0);
    else
        this->send(0x00);
}
