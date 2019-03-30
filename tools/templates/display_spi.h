
/**
 * Class implements ~CONTROLLER~ ~RESOLUTION~ oled display in ~BITS~ bit mode over SPI
 */
class Display~CONTROLLER~_~RESOLUTION~_SPI: public Display~CONTROLLER~<Interface~CONTROLLER~<PlatformSpi>>
{
public:
    /**
     * @brief Inits ~RESOLUTION~ OLED display over spi (based on ~CONTROLLER~ controller): ~BITS~-bit mode.
     *
     * Inits ~RESOLUTION~ OLED display over spi (based on ~CONTROLLER~ controller): ~BITS~-bit mode
     * @param rstPin pin controlling LCD reset (-1 if not used)
     * @param config platform spi configuration. Please refer to SPlatformI2cConfig.
     */
    Display~CONTROLLER~_~RESOLUTION~_SPI( int8_t rstPin, const SPlatformSpiConfig &config = { -1, -1, -1, 0, -1, -1 } )
        : Display~CONTROLLER~_~RESOLUTION~(m_spi, rstPin)
        , m_spi( *this, config.dc,
                 SPlatformSpiConfig{ config.busId,
                                     config.cs,
                                     config.dc,
                                     config.frequency ?: 4400000,
                                     config.scl,
                                     config.sda } ) {}

    /**
     * Initializes ~CONTROLLER~ lcd in ~BITS~-bit mode
     */
    void begin() override;

    /**
     * Closes connection to display
     */
    void end() override;

private:
    Interface~CONTROLLER~<PlatformSpi> m_spi;
};

