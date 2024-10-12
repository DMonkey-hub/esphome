This work is based on the driver developed by @buglloc for the T-Display_S3_Long display...<br>
https://github.com/buglloc/esphome-components/tree/main<br>
for the JC3248W535EN device there were problems with updating only the changed portion of the screen and the touchscreen.
This repository fixes those problems in the driver and provides a working example.

Point to this driver using the following section in your yaml file

external_components:
  - source: github://dmonkey-hub/esphome
    components: [ axs15231 ] 

If there are problems compiling please do a 'Clean Build Files' from the ESPHome page

good luck
