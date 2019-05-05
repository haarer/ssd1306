
static const PROGMEM uint8_t s_~CONTROLLER~_lcd~RESOLUTION~_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_~CONTROLLER~,
    0x00,
#endif
    // Place your data here
};

////////////////////////////////////////////////////////////////////////////////
//             ~CONTROLLER~ basic ~BITS~-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void Display~CONTROLLER~_~RESOLUTION~<I>::begin()
{
    ssd1306_resetController2( this->m_rstPin, 20 );
    this->m_w = ~WIDTH~;
    this->m_h = ~HEIGHT~;
    this->m_intf.start();
    this->m_intf.spiDataMode(0);
    for( uint8_t i=0; i<sizeof(s_~CONTROLLER~_lcd~RESOLUTION~_initData); i++)
    {
        uint8_t data = pgm_read_byte(&s_~CONTROLLER~_lcd~RESOLUTION~_initData[i]);
        if (data == CMD_ARG)
        {
            data = pgm_read_byte(&s_~CONTROLLER~_lcd~RESOLUTION~_initData[++i]);
            this->m_intf.spiDataMode(1);
            this->m_intf.send(data);
            this->m_intf.spiDataMode(0);
        }
        else
        {
            this->m_intf.send(data);
        }
    }
    this->m_intf.stop();
}

template <class I>
void Display~CONTROLLER~_~RESOLUTION~<I>::end()
{
}
