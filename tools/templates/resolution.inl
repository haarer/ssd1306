
static const PROGMEM uint8_t s_~CONTROLLER~_lcd~RESOLUTION~_initData[] =
{
#ifdef SDL_EMULATION
    // TODO: Add SDL_LCD_~CONTROLLER~ enum value to enum list in tools/sdl/sdl_core.h
    SDL_LCD_~CONTROLLER~, 0x00,
    // TODO: Add initialization argument for SDL emulator (display specific)
    0x00, 0x00,
#endif
    // Place your data here in the following format
    // ...
    // ID_CMD, N_ARGS, ARG1, ARG2, ..., ARGN,    // This is for regular LCD commands
    // ...
    // ID_CMD, CMD_DELAY, DELAY_IN_MS,           // This is for LCD command without args, requiring delay
    // ...
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
    // TODO: Give LCD some time to initialize. Refer to ~CONTROLLER~ datasheet
    lcd_delay(100);
    _configureSpiDisplay<I>(this->m_intf,
                            s_~CONTROLLER~_lcd~RESOLUTION~_initData,
                            sizeof(s_~CONTROLLER~_lcd~RESOLUTION~_initData));
}

template <class I>
void Display~CONTROLLER~_~RESOLUTION~<I>::end()
{
}
