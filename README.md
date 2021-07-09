# BadlionLogger
BadlionLogger is a rudimentary PoC of a kernel driver used to monitor the activity of BadlionClient's kernelmode anti-cheat (`BadlionAnticheat.sys`) by applying IAT hooks on the image during the image load callback.
  
BadlionAnticheat is virtualized with [VMProtect](https://vmpsoft.com/), a popular VM packer used in many products.  
This project employs a blackbox approach to monitoring driver activity without devirtualization due to no integrity checking.

# Example
### DriverEntry
<img src="https://i.imgur.com/W1O7wPR.png"/>

## CreateProcess callback
<img src="https://i.imgur.com/WoJkeg1.png"/>

## Disclaimer
- BadlionLogger is a proof-of-concept and is not intended to be a reliable product
- This project contains numerous bad practices that should be considered 
