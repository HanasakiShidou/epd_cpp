#include "../e-paper.hpp"
#include "../../Hardware/hardwareapi.hpp"
#include <memory>

class EPaper_2in9v2 : public EPaper_API
{
    public:
    EPaper_2in9v2() = delete;
    EPaper_2in9v2(std::unique_ptr<HardwareAPI> api);

    void Reset() override;
    void Init() override;
    void Clear() override;
    void Display(uint8_t *image) override;
    void DisplayBase(uint8_t *image) override;
    void DisplayPartial(uint8_t *image) override;
    void Sleep() override;

    private:
    void SendCommand(uint8_t reg);
    void SendData(uint8_t data);
    void ReadBusy();
    void Lut(uint8_t *lut);
    void TurnOnDisplay();
    void LutByHost(uint8_t *lut);
    void TurnOnDisplayPartial();
    void SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
    void SetCursor(uint16_t Xstart, uint16_t Ystart);

    std::unique_ptr<HardwareAPI> api_;
};

