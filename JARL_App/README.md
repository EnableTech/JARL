# JARLReactApp

## Current Arduino API
* Use: https://github.com/adafruit/Adafruit_nRF8001/tree/1.1.1
* Each button has code corresponding to it (`f`, `b`, `l`, `r`, `u`, `d`, `s`)
* On pressIn sends `code`+`1`
* On pressOut sends `code`+`0`
* Example: `f1`, `f0`, `s1`, `s0`, etc.
