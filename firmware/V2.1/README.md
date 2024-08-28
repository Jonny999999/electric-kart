Firmware for control board version V2.1 (`hardware/V2.1_board-ecar_isolated`).  
This firmware project is started from scratch and is no longer based on the code from V1.  

# Usage
### Button
Pushbutton at the side of emergency stop switch:
- 1x short press: change configuration - rotates through config presets (different pwm frequencies and fan thresholds)
- 1x long press: toggle between normal and slow mode - in slow mode max duty is limited to certain percentage

### Switch
Toggle switch next to emergency stop switch:
- turn Relay on/off (light)

### Led
- blinks at startup
- always on when controller is running