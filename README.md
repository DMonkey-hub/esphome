This work is based on the driver developed by @buglloc for the T-Display_S3_Long display...<br>
https://github.com/buglloc/esphome-components/tree/main<br>
for the JC3248W535EN device there were problems with updating only the changed portion of the screen, rotation and the touchscreen.
This repository fixes those problems in the driver and provides a working example.

Point to this driver using the following section in your yaml file

<pre>
external_components:
  - source: github://dmonkey-hub/esphome
    components: [ axs15231, touchscreen ] 
</pre>

If there are problems compiling please do a 'Clean Build Files' from the ESPHome page

good luck
<br>
<br>
The image below shows the example included.
The buttons 10-100 resets the Test meter to that value. Touching the meter resets it to zero. Touching the clock displays a simple message box.
That's it. Enjoy. <br><br>
<img src="LVGL%20example.jpg" alt="Example image"><br><br>
Tested using...
Esphome: 2024.10.2
Core: 2024.10.4
Supervisor: 2024.10.3
Operating System: 13.2
Frontend: 20241002.4

