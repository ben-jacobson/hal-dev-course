#include <Arduino.h>

class car {
	public:
		car(int seats, String make, String model, bool manual_transmission) {
			_seats = seats; 
			_make = make;  
			_model = model; 
			_manual_transmission = manual_transmission;
			// intialize with a 0 miles odometer. 
		}

		void start_ignition(void) {
			// start the car
		}

		void brake(void) {

		}

	private:
		int _seats;
		String _make, _model;
		bool _manual_transmission;
};

void setup() {
	car family_car(4, "Toyota", "Family+", false);
	family_car.start_ignition();
}

void loop() {
	// do nothing for now
}
