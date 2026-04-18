here we have the building blocks for a fully functional wifi AC controller with a d1 mini lite arduino board reading from a bmp280 sensor and controlling the window AC unit using a srd-05vdc-sl-c relay. the relay is connected to a board specifically designed to interface with this d1 mini. Please create a robust, fault-tolerant application that

1. subscribes to mqtt messages from the server to set the temperature and
2. periodically publishes data on its own. We can include the temperature (fahrenheit), hummidity (percent like .36) and air pressure (psi). We should ideally serialize this as a json payload if possible and include units.

Key details:

- We must implement a minimum off-time protection to avoid damaging the compressor so that the ac never turns back on within 3 min of shutting off
- We should try to gracefully handle connection errors and other unknowns. Ask the user how they would like to handle any edge cases or gotchas that you can come up with
- Keep in mind that the arduino d4 pin is connected to the sensor scl. this might cause issues with trying to control the led correctly.
- Feel free to make suggestions that will make this application more fault-tolerant and reliable.
- The server running mqtt is also running home assistant, but I think as long as we are doing mqtt pub/subs we should be ok, flag any potential issues here.
- The current code is maining for testing/debugging. PLEASE refactor and don't feel you have to keep code around just cause. I want a clean, modular, well architected implementation that is easy to read and maintain. feel free to split into separate files if needed (platformio project).
