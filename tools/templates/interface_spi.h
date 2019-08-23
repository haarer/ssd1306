
/**
 * Class implements interface functions to ~CONTROLLER~ displays
 */
template <class I>
class Interface~CONTROLLER~: public I
{
public:
    /**
     * Creates instance of interface to LCD display.
     *
     * @param base Reference to base class, which represents Display
     * @param dc Data/command control pin number, for i2c communication should be -1
     * @param data variable argument list, accepted by platform interface (PlatformI2c, PlatformSpi)
     */
    template <typename... Args>
    Interface~CONTROLLER~(NanoDisplayBase<Interface~CONTROLLER~<I>> &base, int8_t dc, Args&&... data)
        : I(data...)
        , m_dc( dc )
        , m_base(base)
    {
    }

~FUNCS_DECL~
private:
    const int8_t m_dc = -1; ///< data/command pin for SPI, -1 for i2c
    NanoDisplayBase<Interface~CONTROLLER~<I>> &m_base; ///< basic lcd display support interface
};

