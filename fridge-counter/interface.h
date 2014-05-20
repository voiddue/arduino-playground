#include <inttypes.h>
class Interface
{
public:
	Interface(uint8_t increase, uint8_t decrease, uint8_t refill);
	void pollButtons();
	void resetInput();
	struct userInput
	{
		int taken;
		bool refilled;
	};
	userInput entered;
protected:
private:
	void init(uint8_t increase, uint8_t decrease, uint8_t refill);
	void debounce();
	uint8_t increase_pin;
	uint8_t decrease_pin;
	uint8_t refill_pin;
};
